import logging

from connection import ClientConnection, DestinationConnection, Connection
from dns_resolving.dns_request import DNSRequest
from dns_resolving.dns_response import DNSResponse
from socks5 import ServerReply, ServerGreeting, ClientRequest, ClientGreeting
from .proxy_logger import ProxyLogger


class DefaultProxyLogger(ProxyLogger):
    __log_file_name: str = 'proxy.log'

    def __init__(self):
        logging.basicConfig(level=logging.INFO, filename=self.__log_file_name, filemode='w',
                            format='\n%(asctime)s %(levelname)s %(message)s')

    def log_proxy_ready(self) -> None:
        logging.info('Proxy is ready')

    def log_new_client(self, client: ClientConnection) -> None:
        logging.info(f'New client {client}')

    def log_new_destination(self, destination: DestinationConnection) -> None:
        logging.info(f'New destination {destination}')

    def log_client_greeting(self, client: ClientConnection, greeting: ClientGreeting) -> None:
        logging.info(f'Client {client} sent {greeting}')

    def log_client_request(self, client: ClientConnection, request: ClientRequest) -> None:
        logging.info(f'Client {client} sent {request}')

    def log_client_data(self, client: ClientConnection, data: bytes) -> None:
        logging.info(self.__add_dots_if_long(f'Client {client} sent {data[:20]}', data))

    def log_destination_data(self, destination: DestinationConnection, data: bytes) -> None:
        logging.info(self.__add_dots_if_long(f'Destination {destination} sent {data[:20]}', data))

    def log_connection_close(self, connection: Connection) -> None:
        logging.info(f'Connection {connection} is closed')

    def log_greeting_reply(self, client: ClientConnection, reply: ServerGreeting) -> None:
        logging.info(f'Proxy sent {reply} to client {client}')

    def log_request_reply(self, client: ClientConnection, reply: ServerReply) -> None:
        logging.info(f'Proxy sent {reply} to client {client}')

    def log_dns_request(self, dns_request: DNSRequest) -> None:
        logging.info(f'Proxy sent {dns_request}')

    def log_dns_response(self, dns_response: DNSResponse) -> None:
        logging.info(f'Proxy received {dns_response}')

    def log_malformed_client(self, client: ClientConnection) -> None:
        logging.error(f'Client {client} is malformed')

    def log_invalid_dns_message_id(self, message_id: int) -> None:
        logging.error(f'Proxy received DNS-message with unexpected message id {message_id}')

    def log_no_dns_servers(self) -> None:
        logging.critical('No DNS-servers found')

    def log_exception(self, exception: Exception, connection: Connection) -> None:
        logging.warning(f'Exception in connection {connection}')
        logging.exception(exception)

    def log_error(self, error: str) -> None:
        logging.error(error)

    @staticmethod
    def __add_dots_if_long(s: str, data: bytes) -> str:
        return f'{s} ...' if len(data) > 20 else s
