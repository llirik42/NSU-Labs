from typing import Dict

from aiohttp import ClientSession

from ..downloading import JSONDownloader


class LocationAPI:
    __url: str = 'https://graphhopper.com/api/1/geocode'
    __api_key: str

    def __init__(self, api_key: str) -> None:
        self.__api_key = api_key

    async def find(self, session: ClientSession, location_name: str, limit: int) -> dict:
        params = self.__create_params(location_name=location_name, limit=limit)
        return await JSONDownloader.download(session=session, url=LocationAPI.__url, params=params)

    def __create_params(self, location_name: str, limit: int) -> Dict[str, str]:
        return {
            'q': location_name,
            'limit': limit,
            'key': self.__api_key
        }
