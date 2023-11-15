from dataclasses import dataclass

from aiohttp import ClientSession

from ..api_wrappers import *


@dataclass
class Application:
    session: ClientSession
    location_api_wrapper: LocationAPIWrapper
    weather_api_wrapper: WeatherAPIWrapper
    place_api_wrapper: PlacesAPIWrapper
