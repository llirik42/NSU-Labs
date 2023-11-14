import ipaddress
import select
import socket
from typing import List, Optional, Dict

from connection import *
from dns_resolving import DNSResolver, get_dns_servers_ips
from dns_resolving.dns_request import DNSRequest
from dns_resolving.dns_response import DNSResponse
from proxy_logger.proxy_logger import ProxyLogger
from server import Server
from socks5 import *
from utils import SocketLike, network_bytes_to_int, int_to_network_bytes


class Proxy:
    __dns_resolver: DNSResolver
    __server: Server

    __inputs: List[SocketLike] = []
    __outputs: List[Connection] = []

    __requests: Dict[int, ClientRequest] = {}  # message_id -> ClientRequest

    __global_message_id: int = 0

    __port: int
    __localhost_bytes: bytes = socket.inet_aton('127.0.0.1')

    __logger: ProxyLogger

    def __init__(self, port: int, logger: ProxyLogger):
        self.__dns_resolver = self.__create_dns_resolver()
        self.__server = Server(port)
        self.__port = port
        self.__logger = logger

    def launch(self) -> None:
        self.__inputs.append(self.__server)
        self.__inputs.append(self.__dns_resolver)

        self.__logger.log_proxy_ready()

        while True:
            readable, writeable, exceptional = select.select(self.__inputs, self.__outputs, [])

            if len(readable) > 0:
                self.__handle_readable(readable[0])

            if len(writeable) > 0:
                self.__handle_writeable(writeable[0])

    def __handle_readable(self, r: SocketLike) -> None:
        if isinstance(r, DestinationConnection):
            self.__handle_destination(destination=r)
        elif isinstance(r, ClientConnection):
            self.__handle_client(r)
        elif isinstance(r, Server):
            self.__handle_server()
        else:
            self.__handle_dns_resolver()

    def __handle_writeable(self, w: Connection) -> None:
        if w.has_data_to_send:
            w.send()
        else:
            self.__outputs.remove(w)

    def __handle_destination(self, destination: DestinationConnection) -> None:
        try:
            recv_data: bytes = destination.recv()
        except OSError as e:
            self.__logger.log_exception(exception=e, connection=destination)
            self.__forget_and_close_destination_connection(destination)
            return

        if len(recv_data) == 0:
            self.__forget_and_close_destination_connection(destination)
        else:
            self.__handle_non_empty_destination_message(destination, recv_data)

    def __handle_client(self, client: ClientConnection) -> None:
        try:
            recv_data: bytes = client.recv()
        except OSError as e:
            self.__logger.log_exception(exception=e, connection=client)
            self.__forget_and_close_client_connection(client)
            return

        if len(recv_data) == 0:
            self.__forget_and_close_client_connection(client)
        else:
            self.__handle_non_empty_client_message(client=client, message=recv_data)

    def __handle_server(self) -> None:
        client_socket: socket.socket = self.__server.accept()
        client_socket.setblocking(False)
        client: ClientConnection = ClientConnection(client_socket)
        self.__inputs.append(client)
        self.__logger.log_new_client(client)

    def __handle_dns_resolver(self) -> None:
        response: DNSResponse = self.__dns_resolver.recv_response()
        self.__logger.log_dns_response(response)

        message_id: int = response.message_id
        client: Optional[ClientConnection] = self.__find_client_by_message_id(message_id)

        if client is None:
            self.__logger.log_invalid_dns_message_id(message_id)
        elif len(response.ips) == 0:
            client.push_and_send(create_error_server_reply(rep=CONNECTION_REFUSED).to_bytes())
            self.__forget_and_close_client_connection(client)
        else:
            request: ClientRequest = self.__requests.pop(message_id)
            self.__complete_reply_to_request(client=client, ip=str(response.ips[0]),
                                             port=network_bytes_to_int(request.dst_port))

    def __handle_non_empty_destination_message(self, destination: DestinationConnection, message: bytes) -> None:
        self.__logger.log_destination_data(destination=destination, data=message)

        client: ClientConnection = destination.get_client_connection()
        if client.push_and_send(message) == 0:
            self.__forget_and_close_destination_connection(destination)

    def __handle_non_empty_client_message(self, client: ClientConnection, message: bytes) -> None:
        if client.is_ready_for_greeting():
            client.push_data_to_recv(message)
            self.__handle_ready_for_greeting_client(client)
            return

        if client.is_ready_for_request():
            client.push_data_to_recv(message)
            self.__handle_ready_for_request_client(client)
            return

        if client.is_ready_for_sending_data():
            self.__handle_ready_for_communications_connection(client, message)
            return

        self.__logger.log_malformed_client(client)
        self.__forget_and_close_client_connection(client)

    def __handle_ready_for_greeting_client(self, client: ClientConnection) -> None:
        data_to_recv: bytes = client.get_data_to_recv()

        try:
            greeting: Optional[ClientGreeting] = parse_greeting(data_to_recv)
            if greeting is None:
                return
        except ProtocolException as e:
            self.__logger.log_exception(exception=e, connection=client)
            self.__forget_and_close_client_connection(client)
        else:
            self.__logger.log_client_greeting(client=client, greeting=greeting)
            client.clear_data_to_recv()
            self.__reply_to_greeting(greeting, client)

    def __handle_ready_for_request_client(self, client: ClientConnection) -> None:
        data_to_recv: bytes = client.get_data_to_recv()

        try:
            request: Optional[ClientRequest] = parse_client_request(data_to_recv)
            if request is None:
                return
        except ProtocolException as e:
            self.__logger.log_exception(exception=e, connection=client)
            self.__forget_and_close_client_connection(client)
        else:
            self.__logger.log_client_request(client=client, request=request)
            client.clear_data_to_recv()
            self.__reply_to_request(request, client)

    def __handle_ready_for_communications_connection(self, client: ClientConnection, message: bytes) -> None:
        destination: Optional[DestinationConnection] = client.get_destination_connection()

        if destination is None:
            self.__logger.log_malformed_client(client=client)
            self.__forget_and_close_client_connection(client)

        self.__logger.log_client_data(client=client, data=message)

        if destination.push_and_send(message) == 0:
            self.__forget_and_close_destination_connection(destination)

    def __reply_to_greeting(self, greeting: ClientGreeting, client: ClientConnection) -> None:
        error: bool = not is_client_greeting_valid(greeting)

        if error:
            server_greeting: ServerGreeting = create_error_server_greeting()
        else:
            server_greeting: ServerGreeting = create_success_server_greeting()
            client.set_ready_for_request_state()

        self.__logger.log_greeting_reply(client=client, reply=server_greeting)

        if client.push_and_send(server_greeting.to_bytes()) == 0:
            self.__forget_and_close_client_connection(client)

        if error:
            self.__forget_and_close_client_connection(client)

    def __reply_to_request(self, request: ClientRequest, client: ClientConnection) -> None:
        rep: bytes = validate_request(request)
        error: bool = not is_success_rep(rep)

        if error:
            reply: ServerReply = create_error_server_reply(rep)
            self.__logger.log_request_reply(client=client, reply=reply)
            client.push_and_send(reply.to_bytes())
            self.__forget_and_close_client_connection(client)
            return

        if is_domain_name_atyp(request.atyp):
            dns_request: DNSRequest = DNSRequest(domain_name=request.dst_addr.decode(),
                                                 message_id=self.__global_message_id)
            self.__dns_resolver.send_request(dns_request)
            self.__logger.log_dns_request(dns_request)

            client.set_message_id(self.__global_message_id)
            self.__requests.update([(self.__global_message_id, request)])
            self.__global_message_id += 1
        else:
            self.__complete_reply_to_request(
                client=client,
                ip=str(ipaddress.ip_address(request.dst_addr)),
                port=network_bytes_to_int(request.dst_port),
            )

    def __complete_reply_to_request(self, client: ClientConnection, ip: str, port: int) -> None:
        try:
            destination: DestinationConnection = IPv4DestinationConnection(ip=ip, port=port)
        except OSError as e:
            self.__logger.log_exception(exception=e, connection=client)
            reply: ServerReply = create_error_server_reply_from_exception(e)
            self.__logger.log_request_reply(client=client, reply=reply)

            client.push_and_send(reply.to_bytes())
            self.__forget_and_close_client_connection(client)
            return

        self.__logger.log_new_destination(destination)
        destination.set_client_connection(client)
        self.__inputs.append(destination)

        client.set_destination_connection(destination)
        client.set_ready_for_sending_data_state()
        self.__outputs.append(client)
        self.__outputs.append(destination)

        reply = create_success_server_reply(
            atyp=IPV4_ADDRESS_TYPE,
            bnd_addr=self.__localhost_bytes,
            bnd_port=int_to_network_bytes(self.__port)
        )

        self.__logger.log_request_reply(client=client, reply=reply)

        if client.push_and_send(reply.to_bytes()) == 0:
            self.__forget_and_close_client_connection(client)

    def __create_dns_resolver(self) -> DNSResolver:
        dns_servers_ips: List[str] = get_dns_servers_ips()
        if len(dns_servers_ips) == 0:
            self.__logger.log_no_dns_servers()
            exit(1)

        return DNSResolver(dns_server_ip=dns_servers_ips[0])

    def __find_client_by_message_id(self, message_id: int) -> Optional[ClientConnection]:
        try:
            return next(
                x for x in self.__inputs if isinstance(x, ClientConnection) and x.get_message_id() == message_id)
        except StopIteration:
            return None

    def __forget_and_close_destination_connection(self, destination: DestinationConnection) -> None:
        client: Connection = destination.get_client_connection()
        self.__forget_and_close_connection(client)
        self.__forget_and_close_connection(destination)

    def __forget_and_close_client_connection(self, client: ClientConnection) -> None:
        destination: Connection = client.get_destination_connection()
        self.__forget_and_close_connection(client)
        self.__forget_and_close_connection(destination)

    def __forget_and_close_connection(self, connection: Connection) -> None:
        if connection is None:
            return

        self.__logger.log_connection_close(connection)
        self.__try_to_remove(self.__inputs, connection)
        self.__try_to_remove(self.__outputs, connection)
        connection.close()

    @staticmethod
    def __try_to_remove(a: list, value) -> None:
        if value in a:
            a.remove(value)
