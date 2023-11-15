from typing import List, Optional

from model.entities import Location
from view import View


class Controller:
    __view: View

    def __init__(self, view: View) -> None:
        self.__view = view

    def input_name_of_location(self) -> str:
        while True:
            self.__view.show_location_name_input()
            location_name: str = input()
            self.__view.show_empty_line()

            if not self.__is_white_space(location_name):
                break

        return location_name

    def input_number_of_locations(self) -> int:
        return self.__input_int(
            value_input=self.__view.show_number_of_locations_input,
            invalid_value_handle=self.__view.show_invalid_input,
            min_included=1,
        )

    def input_index_of_location(self, locations: List[Location]) -> int:
        self.__view.show_locations(locations)

        location_number: int = self.__input_int(
            value_input=self.__view.show_index_of_location_input,
            invalid_value_handle=self.__view.show_invalid_input,
            min_included=1,
            max_included=len(locations),
        )

        # Shifting result to [0, length(locations) - 1]
        return location_number - 1

    def input_number_of_places(self) -> int:
        return self.__input_int(
            value_input=self.__view.show_number_of_places_input,
            invalid_value_handle=self.__view.show_invalid_input,
            min_included=1,
        )

    def __input_int(self,
                    value_input,
                    invalid_value_handle,
                    min_included: Optional[int] = None,
                    max_included: Optional[int] = None) -> int:
        while True:
            try:
                value_input()
                result: int = int(input())
            except ValueError:
                invalid_value_handle()
            else:
                min_match: bool = True if min_included is None else result >= min_included
                max_match: bool = True if max_included is None else result <= max_included

                if min_match and max_match:
                    self.__view.show_empty_line()
                    break

                invalid_value_handle()

        return result

    @staticmethod
    def __is_white_space(s: str) -> bool:
        return s.strip() == 0
