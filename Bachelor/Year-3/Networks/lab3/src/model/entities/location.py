from dataclasses import dataclass
from typing import Optional


@dataclass
class Location:
    name: str

    lat: float
    lng: float

    min_lat: Optional[float]
    max_lat: Optional[float]
    min_lng: Optional[float]
    max_lng: Optional[float]
