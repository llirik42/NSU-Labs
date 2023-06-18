import math
from typing import List, Optional, Tuple

import numpy


def align_string_to_width(string: str, width: int) -> str:
    """
    Добавляет пробелы к строке, пока длина получившейся строки не будет равна 'width'.

    :param string: исходная строка
    :param width: необходимая длина строки
    """

    return '{0: <{width}}'.format(string, width=width)


def calculate_s1(sampling: numpy.ndarray, expected_mean: float) -> float:
    """
    Вычисляет S1 для выборки. S1 - почти то же самое, что и выборочная дисперсия, но вместо выборочного среднего стоит
    математическое ожидание.

    :param sampling: выборка
    :param expected_mean: математическое ожидание
    """

    return math.sqrt(sum([(x - expected_mean) ** 2 for x in sampling]) / sampling.size)


def get_quantile(level: float, variable) -> float:
    """
    :param level: уровень квантиля из (0, 1)
    :param variable: случайная величина
    :return: квантиль уровня 'level' для случайной величины 'variable'
    """

    return variable.ppf(level)


def get_distribution_function_value(t: float, variable) -> float:
    """
    :param t: вещественное число
    :param variable: случайная величина
    :return: значение функции распределения для случайной величины 'variable' в точке 't'
    """

    return variable.cdf(t)


def calculate_unbiased_var(sampling: numpy.ndarray) -> float:
    """
    :param sampling: выборка
    :return: выборочная несмещённая дисперсия для выборки 'sampling'
    """

    return sampling.var() * sampling.size / (sampling.size - 1)


def read_sampling(input_file_name: str) -> numpy.ndarray:
    """
    :param input_file_name: имя файла с выборкой
    :return: numpy-массив, содержащий выборку
    """

    with open(input_file_name, 'r') as input_file:
        lines: List[str] = [line[:-1] for line in input_file.readlines()]  # line[:-1] - чтобы убрать '\n' в строках

        str_values: List[str] = [value for line in lines for value in line.split()]

        values_list: List[float] = [
            float(v) if v.count(',') == 0 else float(v.replace(',', '.', -1)) for v in str_values
        ]

        return numpy.array(values_list)


def get_kolmogorov_distribution_function_value(t: float) -> float:
    """
    Возвращает значение функции распределения для случайной величины, имеющей распределение Колмогорова, в точке 't'.

    :param t: точка, в которой нужно посчитать значение функции распределения
    """

    if t <= 0:
        return 0

    result = 0

    n = 100
    sign = (-1) ** n
    m = -2 * t * t

    for k in range(-n, n + 1):
        result += sign * math.exp(k * k * m)
        sign *= -1

    return result


def get_kolmogorov_quantile(level: float) -> float:
    """
    Ищет квантиль уровня 'level' для распределения Колмогорова.

    :param level: уровень квантиля
    """

    step = 0.5
    result = 1
    epsilon = 0.00001
    f_value = get_kolmogorov_distribution_function_value(result)

    while math.fabs(f_value - level) > epsilon:
        if f_value > level:
            result -= step
        else:
            result += step

        f_value = get_kolmogorov_distribution_function_value(result)
        step /= 2

    return result


def calculate_probability_for_range(array: numpy.ndarray, min_value: Optional[float] = None,
                                    max_value: Optional[float] = None) -> float:
    """
    Возвращает вероятность того что элемент массива 'element' удовлетворяет неравенству
    min_value <= element < max_value. Если min_value или max_value - None, то соответствующее неравенство не
    проверяется.

    :param array: массив чисел
    :param min_value: нижняя граница полуинтервала (включительно)
    :param max_value: верхняя граница полуинтервала (не включительно)
    :return: вероятность того что элемент массива лежит в полуинтервале [min_value, max_value)
    """

    def in_range(value: float) -> bool:
        """
        Возвращает выполняется ли предикат min_value <= value < max_value. Если min_value или max_value - None, то
        соответствующее неравенство не проверяется.

        :param value: проверяемое значение
        :return: результат попадания значения в интервал
        """

        if min_value is None and max_value is None:
            return True
        if min_value is None:
            return value < max_value
        if max_value is None:
            return min_value <= value
        return min_value <= value < max_value

    return len([i for i in array if in_range(i)]) / array.size


def get_empirical_distribution_function(sampling: numpy.ndarray, x_values: numpy.ndarray) -> numpy.ndarray:
    """
    :param x_values: точки, в которых нужно посчитать значение эмпирической функции распределения
    :param sampling: выборка, по которой считается эмпирическая функция распределения
    :return: значения эмпирической функции распределения в точках, взятых из 'x_values'
    """

    return numpy.array([calculate_probability_for_range(sampling, None, t) for t in x_values])


def get_density_function(sampling: numpy.ndarray, a: float, b: float) -> Tuple[numpy.ndarray, numpy.ndarray]:
    """
    Возвращает кортеж из двух элементов, в котором первый - массив значений по оси 'OX', второй - массив значений
    эмпирической плотности в точках массива по оси 'OX'. Массив значений по оси 'OX' - подмножество отрезка [a, b].

    :param sampling: выборка
    :param a: начало рассматриваемого отрезка
    :param b: конец рассматриваемого отрезка
    """

    k = math.ceil(math.log2(sampling.size))  # Количество промежутков разбиения, вычисленное по форумле Стёрджисса
    points = numpy.linspace(a, b, k + 1)  # Концы промежутков разбиения
    density_list = [calculate_probability_for_range(sampling, points[i - 1], points[i]) for i in range(1, k + 1)]
    return points, numpy.array(density_list)
