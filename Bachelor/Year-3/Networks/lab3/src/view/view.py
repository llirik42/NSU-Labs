from typing import Optional, List

from model.entities import Weather, Address, Place, Location


class View:
    @staticmethod
    def show_location_name_input() -> None:
        View.__show_input('location name')

    @staticmethod
    def show_number_of_locations_input() -> None:
        View.__show_input('number of locations to find')

    @staticmethod
    def show_index_of_location_input() -> None:
        View.__show_input('index of location')

    @staticmethod
    def show_number_of_places_input() -> None:
        View.__show_input('number of places')

    @staticmethod
    def show_locations(locations: List[Location]) -> None:
        locations_number: int = len(locations)
        location_str = 'location'

        tmp: str = location_str if locations_number == 1 else location_str + 's'

        print(f'Found {len(locations)} {tmp}')

        for i, l in enumerate(locations):
            print(f'    {i + 1}. {l.name} ({l.lat}, {l.lng})')
        print()

    @staticmethod
    def show_invalid_input() -> None:
        print('Invalid input. Try again!' + '\n')

    @staticmethod
    def show_empty_line() -> None:
        print()

    @staticmethod
    def show_weather(weather: Weather) -> None:
        print('WEATHER')
        print(f'    {weather.description}')
        print(f'    temperature {weather.temp} °C')
        print(f'    feels like {weather.feels_like} °C')
        print(f'    wind {weather.wind_speed} m/s')
        print()

    @staticmethod
    def show_places(places: List[Place]) -> None:
        print('INTERESTING PLACES')

        for p in places:
            print(f'   ')

            if p.url is None:
                print(f'    {p.name}')
            else:
                print(f'    {p.name} ({p.url})')

            if p.address is not None:
                print(f'        address: {View.__get_repr_of_address(p.address)}')
                print()

            if p.description is not None:
                print(f'        description: {p.description}')

    @staticmethod
    def __show_input(input_name: str) -> None:
        print(f'Input {input_name}', end=': ')

    @staticmethod
    def __get_repr_of_address(address: Address) -> str:
        elements: List[Optional[str]] = [
            address.country,
            address.state,
            address.county,
            address.city,
            address.city_district,
            address.road,
            address.house_number
        ]

        s: str = ''

        for i, e in enumerate(elements):
            if e is not None:
                s += f'{e}, '

        return s
