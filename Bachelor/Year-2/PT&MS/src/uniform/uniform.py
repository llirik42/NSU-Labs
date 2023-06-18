from typing import List
import math

import numpy
import scipy
import matplotlib.pyplot

from utils import *
from uniform_config import *


def draw_distribution_functions(sampling: numpy.ndarray, max_distance_point: float) -> None:
    """
    Рисует график эмпирической функции распределения по выборке 'sampling'.

    :param sampling: выборка
    :param max_distance_point: точка, в которой достигается максимальная разница между теоретической и эмпирической функцяими распределения
    """

    x: numpy.ndarray = numpy.linspace(a, b, 10000)
    y: numpy.ndarray = get_empirical_distribution_function(sampling, x)

    _, ax = matplotlib.pyplot.subplots()
    ax.plot(x, y, linewidth=3, color='g', label='Эмпирическая функция распределения')
    ax.set_xticks(numpy.linspace(a, b, 21))
    ax.set_yticks(numpy.linspace(a, b, 21))
    ax.plot([a, b], [0, 1], linewidth=3, color='b', label='Теоретическая функция распределения')
    ax.plot([max_distance_point, max_distance_point], [0, 1], color='r', linestyle='dashed')
    ax.legend()


def draw_empirical_density(sampling: numpy.ndarray) -> None:
    """
    Рисует график эмпирической плотности (гистрограмму) по выборке 'sampling'.

    :param sampling: выборка
    """

    points, density = get_density_function(sampling, a, b)

    x: numpy.ndarray = numpy.linspace(a, b, 100000)
    y: List[float] = []

    for i in x:
        for j in range(points.size - 1):
            if points[j] <= i <= points[j + 1]:
                y.append(density[j])
                break

    _, ax = matplotlib.pyplot.subplots()
    ax.plot(x, y, linewidth=3, color='g', label='Гистрограмма')
    ax.legend()


def first_part(sampling: numpy.ndarray, max_distance_point: float) -> None:
    draw_distribution_functions(sampling, max_distance_point)
    draw_empirical_density(sampling)


def second_part(sampling: numpy.ndarray) -> float:
    # 2-a
    n: int = sampling.size

    # Массивы разниц эмпирической и теоретической функций распределения в атомах (только там может достигаться
    # наибольшая разница)
    deltas1: List[float] = [abs(sampling[k] - k / n) for k in range(n)]
    deltas2: List[float] = [abs(sampling[k] - (k + 1) / n) for k in range(n)]

    # Наибольшее расстояние между эмпирической и теоретической функциями распределения
    max_delta: float = max(max(deltas1), max(deltas2))

    max_distance_point: float = 0  # Точка, в которой достигается наибольшее расстояние

    try:
        index: int = deltas1.index(max_delta)
        max_distance_point = sampling[index]
    except ValueError:
        pass

    try:
        index: int = deltas2.index(max_delta)
        max_distance_point = sampling[index]
    except ValueError:
        pass

    dk: float = max_delta * math.sqrt(n)  # Расстояние Колмогорова для эмпирической функции распределения
    c: float = get_kolmogorov_quantile(1 - epsilon)  # Квантиль для распределения Колмогорова
    p_value1: float = 1 - get_kolmogorov_distribution_function_value(dk)  # Реально достигнутый уровень значимости
    result1: str = f'ОСНОВНАЯ, то есть выборка из равномерного распределения на [{a}, {b}]' if dk <= c \
        else f'АЛЬТЕРНАТИВНАЯ, то есть выборка из равномерного распределения на [{a}, {b}]'
    print('Критерий Колмогорова')
    print(f'    Критерий говорит, что если dK < c, то верна основная гипотеза, иначе - альтернативная')
    print(f'    В нашем случае {dk} < {c}, значит верна {result1}')
    print(f'    Реально достигнутый уровень значимости = {p_value1}\n')

    # 2-b
    points, density = get_density_function(sampling, a, b)
    k: int = points.size - 1

    p: numpy.ndarray = numpy.array([1 / k] * 5)  # p[j] = предположительное число наблюдений, попавших в j-ий промежуток
    v: List[float] = [i * n for i in density]  # v[j] = полученное число наблюдений, попавших в j-ий промежуток
    dx: float = sum([(v[j] - n * p[j]) ** 2 / (n * p[j]) for j in range(k)])  # Расстояние хи-квадрат
    chi2 = scipy.stats.chi2(k - 1)  # Распределение хи-квадрат с k-1 степенями свободы
    h: float = get_quantile(1 - epsilon, scipy.stats.chi2(k - 1))  # Квантиль для распределения Пирсона
    p_value2: float = 1 - get_distribution_function_value(dx, chi2)  # Реально достигнутый уровень значимости

    result2: str = f'ОСНОВНАЯ, то есть выборка из равномерного распределения на [{a}, {b}]' if dx <= h \
        else f'АЛЬТЕРНАТИВНАЯ, то есть выборка из равномерного распределения на [{a}, {b}]'
    print('Критерий Пирсона')
    print(f'    Критерий говорит, что если dX < h, то верна основная гипотеза, иначе - альтернативная')
    print(f'    В нашем случае {dx} < {h}, значит верна {result2}')
    print(f'    Реально достигнутый уровень значимости = {p_value2}')

    return max_distance_point


def main() -> None:
    matplotlib.pyplot.rcParams['font.size'] = 12  # Установка размера шрифта для текста на графиках

    uniform_sampling: numpy.ndarray = read_sampling(sampling_input_file_name)
    max_distance_point: float = second_part(numpy.array(sorted(uniform_sampling)))
    first_part(uniform_sampling, max_distance_point)

    matplotlib.pyplot.show()


if __name__ == '__main__':
    main()
