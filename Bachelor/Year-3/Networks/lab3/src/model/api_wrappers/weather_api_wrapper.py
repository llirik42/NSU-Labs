from aiohttp import ClientSession

from ..api.weather_api import WeatherAPI
from ..entities import Weather, Location


class WeatherAPIWrapper:
    __weather_api: WeatherAPI

    def __init__(self, api_key: str) -> None:
        self.__weather_api = WeatherAPI(api_key)

    async def find(self, session: ClientSession, location: Location) -> Weather:
        api_data: dict = await self.__weather_api.find(
            session=session,
            lat=location.lat,
            lon=location.lng,
            units='metric',
            lang='en'
        )

        return Weather(
            description=api_data.get('weather')[0].get('description'),
            temp=api_data.get('main').get('temp'),
            feels_like=api_data.get('main').get('feels_like'),
            wind_speed=api_data.get('wind').get('speed')
        )
