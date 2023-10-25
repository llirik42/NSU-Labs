from abc import ABC, abstractmethod

from connection import ClientConnection, DestinationConnection, Connection
from dns_resolving.dns_request import DNSRequest
from dns_resolving.dns_response import DNSResponse
from socks5 import ClientGreeting, ClientRequest, ServerGreeting, ServerReply


class ProxyLogger(ABC):
    @abstractmethod
    def log_proxy_ready(self) -> None:
        pass

    @abstractmethod
    def log_new_client(self, client: ClientConnection) -> None:
        pass

    @abstractmethod
    def log_new_destination(self, destination: DestinationConnection) -> None:
        pass

    @abstractmethod
    def log_client_greeting(self, client: ClientConnection, greeting: ClientGreeting) -> None:
        pass

    @abstractmethod
    def log_client_request(self, client: ClientConnection, request: ClientRequest) -> None:
        pass

    @abstractmethod
    def log_client_data(self, client: ClientConnection, data: bytes) -> None:
        pass

    @abstractmethod
    def log_destination_data(self, destination: DestinationConnection, data: bytes) -> None:
        pass

    @abstractmethod
    def log_connection_close(self, connection: Connection) -> None:
        pass

    @abstractmethod
    def log_greeting_reply(self, client: ClientConnection, reply: ServerGreeting) -> None:
        pass

    @abstractmethod
    def log_request_reply(self, client: ClientConnection, reply: ServerReply) -> None:
        pass

    @abstractmethod
    def log_dns_request(self, dns_request: DNSRequest) -> None:
        pass

    @abstractmethod
    def log_dns_response(self, dns_response: DNSResponse) -> None:
        pass

    @abstractmethod
    def log_malformed_client(self, client: ClientConnection) -> None:
        pass

    @abstractmethod
    def log_invalid_dns_message_id(self, message_id: int) -> None:
        pass

    @abstractmethod
    def log_no_dns_servers(self) -> None:
        pass

    @abstractmethod
    def log_exception(self, exception: Exception, connection: Connection) -> None:
        pass

    @abstractmethod
    def log_error(self, error: str) -> None:
        pass
