from dataclasses import dataclass


@dataclass
class Weather:
    description: str
    temp: float
    feels_like: float
    wind_speed: float
