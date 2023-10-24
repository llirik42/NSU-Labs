import errno
import ipaddress
from abc import ABC, abstractmethod
from dataclasses import dataclass
from typing import Optional

from utils import get_byte, network_bytes_to_int, int_to_network_bytes
from .protocol_exception import ProtocolException

SUPPORTED_VERSION = b'\x05'
NO_AUTHENTICATION_REQUIRED = b'\x00'
SUCCEEDED = b'\x00'
RSV = b'\x00'
NETWORK_UNREACHABLE = b'\x03'
HOST_UNREACHABLE = b'\x03'
CONNECTION_REFUSED = b'\x05'
IPV4_ADDRESS_TYPE = b'\x01'
GENERAL_SOCKS_SERVER_FAILURE = b'\x01'


class Message(ABC):
    @abstractmethod
    def to_bytes(self) -> bytes:
        pass


@dataclass
class ClientGreeting(Message):
    ver: bytes
    nmethods: bytes
    methods: bytes

    def to_bytes(self) -> bytes:
        return self.ver + self.nmethods + self.methods

    def __str__(self) -> str:
        return (
            f'ClientGreeting('
            f'VER={network_bytes_to_int(self.ver)}, '
            f'NMETHODS={network_bytes_to_int(self.nmethods)}, '
            f'METHODS={[get_method_name(int_to_network_bytes(i)) for i in self.methods]}'
            f')'
        )


@dataclass
class ServerGreeting(Message):
    ver: bytes
    method: bytes

    def to_bytes(self) -> bytes:
        return self.ver + self.method

    def __str__(self) -> str:
        return (
            f'ServerGreeting('
            f'VER={network_bytes_to_int(self.ver)}, '
            f'METHOD={get_method_name(self.method)})'
        )


@dataclass
class ClientRequest(Message):
    ver: bytes
    cmd: bytes
    rsv: bytes
    atyp: bytes
    dst_addr: bytes
    dst_port: bytes

    def to_bytes(self) -> bytes:
        return self.ver + self.cmd + self.rsv + self.atyp + self.dst_addr + self.dst_port

    def __str__(self) -> str:
        return (
            f'ClientRequest('
            f'VER={network_bytes_to_int(self.ver)}, '
            f'CMD={get_command_name(self.cmd)}, '
            f'RSV={rsv(self.rsv)}, '
            f'ATYP={get_address_type(self.atyp)}, '
            f'DST.ADDR={get_address_repr(self.atyp, self.dst_addr)}, '
            f'DST.PORT={network_bytes_to_int(self.dst_port)})'
        )


@dataclass
class ServerReply(Message):
    ver: bytes
    rep: bytes
    rsv: bytes
    atyp: bytes
    bnd_addr: bytes
    bnd_port: bytes

    def to_bytes(self) -> bytes:
        return self.ver + self.rep + self.rsv + self.atyp + self.bnd_addr + self.bnd_port

    def __str__(self) -> str:
        return (
            f'ServerReply('
            f'VER={network_bytes_to_int(self.ver)}, '
            f'REP={get_reply(self.rep)}, '
            f'RSV={rsv(self.rsv)}, '
            f'ATYP={get_address_type(self.atyp)}, '
            f'BND.ADDR={get_address_repr(self.atyp, self.bnd_addr)}, '
            f'BND.PORT={network_bytes_to_int(self.bnd_port)})'
        )


def parse_greeting(data: bytes) -> Optional[ClientGreeting]:
    if len(data) < 3:
        return None

    ver: bytes = get_byte(data, 0)
    nmethods: bytes = get_byte(data, 1)
    nmethods_int: int = data[1]

    if len(data) < 2 + nmethods_int:
        return None
    elif len(data) == 2 + nmethods_int:
        return ClientGreeting(ver=ver, nmethods=nmethods, methods=data[2:])
    else:
        raise ProtocolException('Invalid format of client greeting')


def parse_client_request(data: bytes) -> Optional[ClientRequest]:
    if len(data) < 7:
        return None

    ver: bytes = get_byte(data, 0)
    cmd: bytes = get_byte(data, 1)
    rsv: bytes = get_byte(data, 2)
    atyp: bytes = get_byte(data, 3)

    if is_ipv4_atyp(atyp):
        if len(data) < 10:
            return None
        elif len(data) == 10:
            dst_addr: bytes = data[4:8]
        else:
            raise ProtocolException('Invalid format of client greeting')
    elif is_domain_name_atyp(atyp):
        domain_name_length = data[4]
        if len(data) < domain_name_length + 7:
            return None
        elif len(data) == domain_name_length + 7:
            dst_addr: bytes = data[5:5 + domain_name_length]
        else:
            raise ProtocolException('Invalid format of client greeting')
    elif is_ipv6_atyp(atyp):
        if len(data) < 22:
            return None
        elif len(data) == 22:
            dst_addr: bytes = data[4:20]
        else:
            raise ProtocolException('Invalid format of client greeting')
    else:
        raise ProtocolException("Invalid format of request: unknown atyp")

    dst_port: bytes = data[-2:]

    return ClientRequest(ver=ver, cmd=cmd, rsv=rsv, atyp=atyp, dst_addr=dst_addr, dst_port=dst_port)


def get_method_name(method: bytes) -> str:
    if method == NO_AUTHENTICATION_REQUIRED:
        return 'NO AUTHENTICATION REQUIRED'

    if method == b'\x01':
        return 'GSSAPI'

    if method == b'\x02':
        return 'USERNAME/PASSWORD'

    if b'\x03' <= method <= b'\x7f':
        return 'IANA ASSIGNED'

    if b'\x80' <= method <= b'\xfe':
        return 'RESERVED FOR PRIVATE METHODS'

    return 'NO ACCEPTABLE METHODS'


def get_command_name(command: bytes) -> str:
    if command == b'\x01':
        return 'CONNECT'

    if command == b'\x02':
        return 'BIND'

    if command == b'\x03':
        return 'UDP ASSOCIATE'

    return 'UNKNOWN COMMAND'


def get_reply(rep: bytes) -> str:
    if rep == b'\x00':
        return 'SUCCEEDED'

    if rep == b'\x01':
        return 'GENERAL SOCKS SERVER FAILURE'

    if rep == b'\x02':
        return 'CONNECTION NOT ALLOWED BY RULESET'

    if rep == b'\x03':
        return 'NETWORK UNREACHABLE'

    if rep == b'\x04':
        return 'HOST UNREACHABLE'

    if rep == b'\x05':
        return 'CONNECTION REFUSED'

    if rep == b'\x06':
        return 'TTL EXPIRED'

    if rep == b'\x07':
        return 'COMMAND NOT SUPPORTED'

    if rep == b'\x08':
        return 'ADDRESS TYPE NOT SUPPORTED'

    return 'UNASSIGNED'


def get_address_type(atyp: bytes) -> str:
    if is_ipv4_atyp(atyp):
        return 'IP V4 address'

    if is_domain_name_atyp(atyp):
        return 'DOMAINNAME'

    if is_ipv6_atyp(atyp):
        return 'IP V6 address'

    return 'UNKNOWN ATYP'


def get_address_repr(atyp: bytes, dest_address: bytes) -> str:
    if is_ipv4_atyp(atyp):
        return get_ipv4_dest_address(dest_address)

    if is_domain_name_atyp(atyp):
        return get_domain_name_dest_address(dest_address)

    if is_ipv6_atyp(atyp):
        return get_ipv6_dest_address(dest_address)


def get_ipv4_dest_address(dest_address: bytes) -> str:
    return str(ipaddress.IPv4Address(dest_address))


def get_domain_name_dest_address(dest_address: bytes) -> str:
    return dest_address.decode()


def get_ipv6_dest_address(dest_address: bytes) -> str:
    return str(ipaddress.IPv6Address(dest_address))


def rsv(data: bytes) -> bool:
    return data == RSV


def is_ipv4_atyp(atyp: bytes) -> bool:
    return atyp == b'\x01'


def is_domain_name_atyp(atyp: bytes) -> bool:
    return atyp == b'\x03'


def is_ipv6_atyp(atyp: bytes) -> bool:
    return atyp == b'\x04'


def is_success_rep(rep: bytes) -> bool:
    return rep == b'\x00'


def is_client_greeting_valid(client_greeting: ClientGreeting) -> bool:
    return client_greeting.ver == SUPPORTED_VERSION and (NO_AUTHENTICATION_REQUIRED in client_greeting.methods)


def validate_request(request: ClientRequest) -> bytes:
    if request.ver != SUPPORTED_VERSION or not rsv(request.rsv):
        return b'\x01'  # general SOCKS server failure

    if request.cmd != b'\x01':
        return b'\x07'  # Command not supported

    atyp: bytes = request.atyp
    if not is_ipv4_atyp(atyp) and not is_domain_name_atyp(atyp):
        return b'\x08'  # Address type not supported

    return SUCCEEDED


def create_success_server_greeting() -> ServerGreeting:
    return ServerGreeting(ver=SUPPORTED_VERSION, method=NO_AUTHENTICATION_REQUIRED)


def create_error_server_greeting() -> ServerGreeting:
    return ServerGreeting(ver=SUPPORTED_VERSION, method=b'\xff')


def create_success_server_reply(atyp: bytes, bnd_addr: bytes, bnd_port: bytes) -> ServerReply:
    return ServerReply(ver=SUPPORTED_VERSION, rep=SUCCEEDED, rsv=RSV, atyp=atyp, bnd_addr=bnd_addr, bnd_port=bnd_port)


def create_error_server_reply(rep: bytes) -> ServerReply:
    return ServerReply(ver=SUPPORTED_VERSION, rep=rep, rsv=b'\x00', atyp=b'\x00', bnd_addr=b'\x00', bnd_port=b'\x00')


def create_error_server_reply_from_exception(e: OSError) -> ServerReply:
    code_to_rep = {
        errno.ENETUNREACH: NETWORK_UNREACHABLE,
        errno.ECONNREFUSED: CONNECTION_REFUSED,
        errno.EHOSTUNREACH: HOST_UNREACHABLE
    }

    rep: bytes = code_to_rep.get(e.errno) or GENERAL_SOCKS_SERVER_FAILURE

    return create_error_server_reply(rep=rep)
