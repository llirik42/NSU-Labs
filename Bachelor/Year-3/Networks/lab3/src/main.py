import asyncio
import sys
from typing import List

from aiohttp import ClientSession

from controller import Controller
from model.api_wrappers import LocationAPIWrapper, WeatherAPIWrapper, PlacesAPIWrapper
from model.app import Application
from model.config_parsing import ConfigParser
from model.entities import Weather, Place, Location
from view import View


def create_locations_task(application: Application, location_name: str, locations_number: int):
    return asyncio.create_task(application.location_api_wrapper.find(
        session=application.session,
        location_name=location_name,
        limit=locations_number
    ))


def create_weather_task(application: Application, location: Location):
    return asyncio.create_task(application.weather_api_wrapper.find(
        session=application.session,
        location=location
    ))


def create_places_list_task(application: Application, location: Location, places_number: int):
    return asyncio.create_task(application.place_api_wrapper.find_places(
        session=application.session,
        location=location,
        limit=places_number
    ))


def create_place_description_task(application: Application, xid: str):
    return asyncio.create_task(application.place_api_wrapper.find_place_description(
        session=application.session,
        xid=xid,
    ))


async def gather_single_value(task):
    return (await asyncio.gather(task))[0]


async def gather_values_list(tasks: list) -> list:
    return await asyncio.gather(*tasks)


async def main():
    config_parser: ConfigParser = ConfigParser()

    # Parsing of api-keys
    try:
        location_api_key: str = config_parser.parse_location_api_key()
        weather_api_key: str = config_parser.parse_weather_api_key()
        places_api_key: str = config_parser.parse_places_api_key()
    except KeyError as e:
        print(e.args[0], file=sys.stderr)
        exit(1)

    view: View = View()
    controller: Controller = Controller(view)
    application: Application = Application(
        session=ClientSession(),
        location_api_wrapper=LocationAPIWrapper(location_api_key),
        weather_api_wrapper=WeatherAPIWrapper(weather_api_key),
        place_api_wrapper=PlacesAPIWrapper(places_api_key)
    )

    async with application.session:
        # Input of location name and number of locations
        location_name: str = controller.input_name_of_location()
        locations_number: int = controller.input_number_of_locations()

        # Finding locations
        locations_task = create_locations_task(
            application=application,
            location_name=location_name,
            locations_number=locations_number
        )
        locations: List[Location] = await gather_single_value(locations_task)

        # Input of location index and determination of target location
        location_index: int = controller.input_index_of_location(locations)
        location: Location = locations[location_index]

        # Creating task that finds weather in the location
        weather_task = create_weather_task(application=application, location=location)

        # Input of number of places and creating tasks of places
        places_number: int = controller.input_number_of_places()
        places_task = create_places_list_task(
            application=application,
            location=location,
            places_number=places_number
        )
        xids: List[str] = await gather_single_value(places_task)

        place_description_tasks = []
        for xid in xids:
            place_description_tasks.append(create_place_description_task(application=application, xid=xid))

        # Сбор итоговых данных
        weather: Weather = await gather_single_value(weather_task)
        places: List[Place] = await gather_values_list(place_description_tasks)

        view.show_weather(weather)
        view.show_places(places)


if __name__ == '__main__':
    asyncio.run(main())
