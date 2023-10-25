from dataclasses import dataclass


@dataclass
class DNSRequest:
    domain_name: str
    message_id: int
