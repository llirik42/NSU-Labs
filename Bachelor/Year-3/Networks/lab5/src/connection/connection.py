import socket

from utils import SocketLike


class Connection(SocketLike):
    __socket: socket.socket
    __data_to_send: bytes = b''
    __recv_count: int

    def __init__(self, client_socket: socket.socket, recv_count: int = 1024):
        self.__socket = client_socket
        self.__recv_count = recv_count

    @property
    def has_data_to_send(self) -> bool:
        return len(self.__data_to_send) > 0

    def send(self) -> int:
        try:
            sent_count: int = self.__socket.send(self.__data_to_send)
        except OSError:
            pass
        else:
            self.__data_to_send: bytes = self.__data_to_send[sent_count:]
            return sent_count

    def push_and_send(self, data: bytes) -> int:
        self.__push_data_to_send(data)
        return self.send()

    def recv(self) -> bytes:
        return self.__socket.recv(self.__recv_count)

    def fileno(self) -> int:
        return self.__socket.fileno()

    def close(self) -> None:
        self.__socket.close()

    def __push_data_to_send(self, data: bytes) -> None:
        self.__data_to_send = self.__data_to_send + data

    def __str__(self) -> str:
        try:
            return f'{self.__socket.getpeername()[0]}:{self.__socket.getpeername()[1]}'
        except OSError:
            return 'Malformed connection with no endpoint'
