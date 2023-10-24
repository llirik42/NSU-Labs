from enum import IntEnum, auto
from typing import Optional

from .connection import Connection


class ClientConnectionState(IntEnum):
    def _generate_next_value_(self, start, count, last_values) -> int:
        return count

    READY_FOR_GREETING = auto()
    READY_FOR_REQUEST = auto()
    READY_FOR_SENDING_DATA = auto()


class ClientConnection(Connection):
    __state: ClientConnectionState = ClientConnectionState.READY_FOR_GREETING
    __destination_connection: Optional[Connection] = None
    __data_to_recv: bytes = b''
    __message_id: int = 0

    def is_ready_for_greeting(self) -> bool:
        return self.__state == ClientConnectionState.READY_FOR_GREETING

    def is_ready_for_request(self) -> bool:
        return self.__state == ClientConnectionState.READY_FOR_REQUEST

    def is_ready_for_sending_data(self) -> bool:
        return self.__state == ClientConnectionState.READY_FOR_SENDING_DATA

    def push_data_to_recv(self, data: bytes) -> None:
        self.__data_to_recv = self.__data_to_recv + data

    def get_data_to_recv(self) -> bytes:
        return self.__data_to_recv

    def clear_data_to_recv(self) -> None:
        self.__data_to_recv = b''

    def set_ready_for_request_state(self) -> None:
        self.__state = ClientConnectionState.READY_FOR_REQUEST

    def set_ready_for_sending_data_state(self) -> None:
        self.__state = ClientConnectionState.READY_FOR_SENDING_DATA

    def get_destination_connection(self) -> Optional[Connection]:
        return self.__destination_connection

    def set_destination_connection(self, new_destination_connection: Connection) -> None:
        self.__destination_connection = new_destination_connection

    def get_message_id(self) -> int:
        return self.__message_id

    def set_message_id(self, value: int) -> None:
        self.__message_id = value
