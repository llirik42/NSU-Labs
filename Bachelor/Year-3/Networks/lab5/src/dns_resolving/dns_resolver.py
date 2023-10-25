import socket

from dns_messages import DnsMessage, OPCODE, RRType, RRClass, Question
from dns_messages.dns_objects.rr_data import A

from utils import SocketLike
from .dns_request import DNSRequest
from .dns_response import DNSResponse

MAX_DNS_MESSAGE_LENGTH = 2048


class DNSResolver(SocketLike):
    __dns_port: int = 53
    __dns_server_socket: socket.socket

    def __init__(self, dns_server_ip: str):
        self.__dns_server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.__dns_server_socket.connect((dns_server_ip, self.__dns_port))

    def send_request(self, request: DNSRequest) -> None:
        message_bytes: bytes = self.__request_to_bytes(domain_name=request.domain_name, message_id=request.message_id)
        self.__dns_server_socket.send(message_bytes)

    def recv_response(self) -> DNSResponse:
        read_bytes: bytes = self.__dns_server_socket.recv(MAX_DNS_MESSAGE_LENGTH)
        parsed_message: DnsMessage = DnsMessage.from_bytes(read_bytes)

        return DNSResponse(
            message_id=parsed_message.message_id,
            ips=[answer.ip_address for answer in parsed_message.answers_RRs if isinstance(answer, A)]
        )

    def fileno(self) -> int:
        return self.__dns_server_socket.fileno()

    @staticmethod
    def __request_to_bytes(domain_name: str, message_id: int) -> bytes:
        message: DnsMessage = DnsMessage(message_id=message_id, qr=0, op_code=OPCODE.QUERY, rd=1)
        question: Question = Question(name=domain_name, rr_type=RRType.A, rr_class=RRClass.IN)
        message.questions.append(question)

        return message.to_bytes()
