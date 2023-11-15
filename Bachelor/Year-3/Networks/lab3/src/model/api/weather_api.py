from aiohttp import ClientSession

from ..downloading.json_downloader import JSONDownloader


class WeatherAPI:
    __url: str = 'https://api.openweathermap.org/data/2.5/weather'
    __api_key: str

    def __init__(self, api_key: str) -> None:
        self.__api_key = api_key

    async def find(self, session: ClientSession, lat: float, lon: float, units: str, lang: str) -> dict:
        url = self.__create_url(lat=lat, lon=lon, units=units, lang=lang)
        return await JSONDownloader.download(session=session, url=url)

    def __create_url(self, lat: float, lon: float, units: str, lang: str) -> str:
        return f'{WeatherAPI.__url}?lat={lat}&lon={lon}&appid={self.__api_key}&units={units}&lang={lang}'
