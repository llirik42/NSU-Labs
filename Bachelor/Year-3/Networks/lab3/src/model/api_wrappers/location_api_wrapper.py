from typing import List, Optional

from aiohttp import ClientSession

from ..api import LocationAPI
from ..entities import Location


class LocationAPIWrapper:
    __location_api: LocationAPI

    def __init__(self, api_key: str):
        self.__location_api = LocationAPI(api_key)

    async def find(self, session: ClientSession, location_name: str, limit: int) -> List[Location]:
        api_data: dict = await self.__location_api.find(session=session, location_name=location_name, limit=limit)
        return [LocationAPIWrapper.__extract_location(hit) for hit in api_data.get('hits')]

    @staticmethod
    def __extract_location(hit: dict) -> Location:
        point: dict[str, float] = hit.get('point')
        extent: Optional[List[float]] = hit.get('extent')
        name: str = hit.get('name')

        min_lat: Optional[float] = None if extent is None else extent[1]
        max_lat: Optional[float] = None if extent is None else extent[3]
        min_lng: Optional[float] = None if extent is None else extent[0]
        max_lng: Optional[float] = None if extent is None else extent[2]

        return Location(
            name=name,
            lat=point['lat'],
            lng=point['lng'],
            min_lat=min_lat,
            max_lat=max_lat,
            min_lng=min_lng,
            max_lng=max_lng
        )
