import socket

from connection import DestinationConnection


class IPv4DestinationConnection(DestinationConnection):
    def __init__(self, ip: str, port: int):
        destination_socket: socket.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        destination_socket.connect((ip, port))
        destination_socket.setblocking(False)
        super().__init__(destination_socket)
