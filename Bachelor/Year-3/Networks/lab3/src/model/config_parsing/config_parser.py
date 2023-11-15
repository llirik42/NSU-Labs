import os
from dotenv import load_dotenv


class ConfigParser:
    LOCATION_API_KEY_NAME: str = 'LOCATION_API_KEY'
    WEATHER_API_KEY_NAME: str = 'WEATHER_API_KEY'
    PLACES_API_KEY_NAME: str = 'PLACES_API_KEY'

    def __init__(self) -> None:
        load_dotenv()

    @staticmethod
    def parse_location_api_key() -> str:
        return ConfigParser.__parse_value(ConfigParser.LOCATION_API_KEY_NAME)

    @staticmethod
    def parse_weather_api_key() -> str:
        return ConfigParser.__parse_value(ConfigParser.WEATHER_API_KEY_NAME)

    @staticmethod
    def parse_places_api_key() -> str:
        return ConfigParser.__parse_value(ConfigParser.PLACES_API_KEY_NAME)

    @staticmethod
    def __parse_value(key: str) -> str:
        ret = os.getenv(key)

        if ret is None:
            raise KeyError(f'No {key} in .env')

        return ret
