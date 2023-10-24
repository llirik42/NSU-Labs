from typing import Optional

from .client_connection import ClientConnection
from .connection import Connection


class DestinationConnection(Connection):
    __client_connection: Optional[ClientConnection] = None

    def get_client_connection(self) -> ClientConnection:
        return self.__client_connection

    def set_client_connection(self, new_client_connection: ClientConnection) -> None:
        self.__client_connection = new_client_connection
