import ipaddress
from dataclasses import dataclass
from typing import List


@dataclass
class DNSMessage:
    message_id: int
    ips: List[ipaddress.IPv4Address]
