from typing import List, Optional

from aiohttp import ClientSession

from ..api import PlacesAPI
from ..entities import Location, Address, Place


class PlacesAPIWrapper:
    __places_api: PlacesAPI

    def __init__(self, api_key: str) -> None:
        self.__places_api = PlacesAPI(api_key)

    # Returns list of xids (one xid - one place)
    async def find_places(self, session: ClientSession, location: Location, limit: int) -> List[str]:
        if (location.min_lng and location.max_lng and location.min_lat and location.max_lat) is None:
            return []

        api_data: dict = await self.__places_api.find_places(
            session=session,
            min_lon=location.min_lng,
            max_lon=location.max_lng,
            min_lat=location.min_lat,
            max_lat=location.max_lat,
            limit=limit
        )

        return [f.get('properties').get('xid') for f in api_data.get('features')]

    async def find_place_description(self, session: ClientSession, xid: str) -> Place:
        api_data: dict = await self.__places_api.find_place_description(
            session=session,
            lang='en',
            xid=xid
        )

        return self.__extract_place(api_data)

    @staticmethod
    def __extract_place(place_data: dict) -> Place:
        info: dict = place_data.get('info')
        description_from_info: Optional[str] = info.get('descr') if info is not None else None
        url_from_info: Optional[str] = info.get('url') if info is not None else None

        wiki_data: dict = place_data.get('wikipedia_extracts')
        description_from_wiki_data: Optional[str] = wiki_data.get('text') if wiki_data is not None else None

        name: str = place_data.get('name')
        url: str = place_data.get('url') or url_from_info
        description: str = description_from_info or description_from_wiki_data
        address: Optional[Address] = PlacesAPIWrapper.__extract_address(place_data)

        return Place(
            name=name,
            url=url,
            description=description,
            address=address
        )

    @staticmethod
    def __extract_address(place_data: dict) -> Optional[Address]:
        address: Optional[dict] = place_data.get('address')

        if address is None:
            return None

        return Address(
            country=address.get('country'),
            state=address.get('state'),
            county=address.get('county'),
            city=address.get('city'),
            city_district=address.get('city_district'),
            road=address.get('road'),
            house_number=address.get('house_number'),
        )
