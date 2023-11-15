from dataclasses import dataclass
from typing import Optional


@dataclass
class Address:
    country: Optional[str] = None
    state: Optional[str] = None
    county: Optional[str] = None
    city: Optional[str] = None
    city_district: Optional[str] = None
    road: Optional[str] = None
    house_number: Optional[str] = None
