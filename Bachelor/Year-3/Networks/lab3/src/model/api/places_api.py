from aiohttp import ClientSession

from ..downloading.json_downloader import JSONDownloader


class PlacesAPI:
    __api_key: str

    __interesting_places_url: str = 'https://api.opentripmap.com/0.1/ru/places/bbox'
    __place_description_url: str = 'https://api.opentripmap.com/0.1/ru/places/xid'

    def __init__(self, api_key: str) -> None:
        self.__api_key = api_key

    async def find_places(self,
                          session: ClientSession,
                          min_lon: float,
                          max_lon: float,
                          min_lat: float,
                          max_lat: float,
                          limit: int) -> dict:
        url: str = self.__create_interesting_places_url(
            lon_min=min_lon,
            lon_max=max_lon,
            lat_min=min_lat,
            lat_max=max_lat,
            limit=limit
        )

        return await JSONDownloader.download(session=session, url=url)

    async def find_place_description(self, session: ClientSession, lang: str, xid: str) -> dict:
        url: str = self.__create_place_description_url(
            lang=lang,
            xid=xid
        )

        return await JSONDownloader.download(session=session, url=url)

    def __create_interesting_places_url(self,
                                        lon_min: float,
                                        lon_max: float,
                                        lat_min: float,
                                        lat_max: float,
                                        limit: int) -> str:
        return (f'{self.__interesting_places_url}?'
                f'lon_min={lon_min}&'
                f'lat_min={lat_min}&'
                f'lon_max={lon_max}&'
                f'lat_max={lat_max}&k'
                f'inds=interesting_places&'
                f'limit={limit}&'
                f'format=geojson&'
                f'apikey={self.__api_key}')

    def __create_place_description_url(self, lang: str, xid: str) -> str:
        return (f'{self.__place_description_url}/{xid}?'
                f'apikey={self.__api_key}&'
                f'lang={lang}')
