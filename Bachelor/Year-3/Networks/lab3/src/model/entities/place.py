from dataclasses import dataclass
from typing import Optional

from .address import Address


@dataclass
class Place:
    name: str
    url: Optional[str]
    description: Optional[str]
    address: Optional[Address]
