import socket

from utils import SocketLike


class Server(SocketLike):
    __server_socket: socket.socket

    def __init__(self, port: int):
        self.__server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.__server_socket.bind(('localhost', port))
        self.__server_socket.listen()

    def fileno(self) -> int:
        return self.__server_socket.fileno()

    def accept(self) -> socket.socket:
        return self.__server_socket.accept()[0]
