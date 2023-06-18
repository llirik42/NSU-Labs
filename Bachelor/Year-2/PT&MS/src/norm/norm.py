import math

import numpy
import scipy

from utils import *
from norm_config import *


def first_part(norm_sampling: numpy.ndarray) -> None:
    print('Первая часть')

    # 1-а
    interval_width: int = 45
    size: float = norm_sampling.size  # Объём выборки
    mean: float = norm_sampling.mean()  # Выборочное среднее
    var: float = norm_sampling.var()  # Выборочная дисперсия (смещённая)
    sqrt_of_size: float = math.sqrt(size)

    q1: float = get_quantile(1 - epsilon / 2, scipy.stats.norm)
    delta1: float = q1 * math.sqrt(sigma_squared) / sqrt_of_size
    interval1: str = f'[{mean - delta1}; {mean + delta1}]'
    print(f'    {align_string_to_width(interval1, interval_width)} a, когда сигма известна')

    # 1-б
    student_distribution = scipy.stats.nct(size - 1, 0)  # Распределение Стьюдента
    sqrt_of_unbiased_var: float = math.sqrt(calculate_unbiased_var(norm_sampling))
    q2: float = get_quantile(1 - epsilon / 2, student_distribution)
    delta2: float = sqrt_of_unbiased_var * q2 / sqrt_of_size
    interval2: str = f'[{mean - delta2}; {mean + delta2}]'
    print(f'    {align_string_to_width(interval2, interval_width)} a, когда сигма неизвестна')

    # 1-в
    xi2_1 = scipy.stats.chi2(size)  # Распределение хи-квадрат с n=N+M степенями свободы
    h11: float = get_quantile(epsilon / 2, xi2_1)
    h21: float = get_quantile(1 - epsilon / 2, xi2_1)
    s1_squared: float = calculate_s1(norm_sampling, a) ** 2
    interval3: str = f'[{s1_squared * size / h21}; {size * s1_squared / h11}]'
    print(f'    {align_string_to_width(interval3, interval_width)} сигма^2, когда a известна')

    # 1-г
    xi2_2 = scipy.stats.chi2(size - 1)  # Распределение хи-квадрат с n-1=N+M-1 степенями свободы
    h12: float = get_quantile(epsilon / 2, xi2_2)
    h22: float = get_quantile(1 - epsilon / 2, xi2_2)
    interval4: str = f'[{size * var / h22}; {size * var / h12}]'
    print(f'    {align_string_to_width(interval4, interval_width)} сигма^2, когда a неизвестна\n')


def second_part(norm_sampling: numpy.ndarray) -> None:
    print('Вторая часть')

    m: int = norm_sampling.size - n  # Объём второй нормальной выборки

    x: numpy.ndarray = norm_sampling[0:n]
    y: numpy.ndarray = norm_sampling[n:]
    x_unbiased_var: float = calculate_unbiased_var(x)
    y_unbiased_var: float = calculate_unbiased_var(y)

    # 2-а
    print(f'    Совпадение дисперсий')
    df: float = x_unbiased_var / y_unbiased_var
    f = scipy.stats.ncf(n - 1, m - 1, 0)  # Распределение Фишера с (N-1, m-1) степенями свободы
    c1: float = get_quantile(epsilon / 2, f)
    c2: float = get_quantile(1 - epsilon / 2, f)
    result1: str = 'ОСНОВНАЯ, то есть выборки имеют одинаковые дисперсии' if c1 <= df <= c2 else 'АЛЬТЕРНАТИВНАЯ, то ' \
                                                                                                 'есть выборки имеют ' \
                                                                                                 'разные дисперсии'
    print(f'        Критерий Фишера говорит, что если c1 <= dF <= c2, то верна основная гипотеза, '
          f'иначе - альтернативная\n'
          f'        В нашем случае {c1} <= {df} <= {c2}, значит верна {result1}\n')

    # 2-б
    print(f'    Совпадение математических ожиданий')

    # Распределение Стьюдента с (n+m-2) степенями свободы
    student_distribution_2 = scipy.stats.nct(n + m - 2, 0)

    dt: float = (x.mean() - y.mean()) * math.sqrt(m + n - 2) / math.sqrt((1 / n + 1 / m) * (n * x.var() + m * y.var()))
    c: float = get_quantile(1 - epsilon / 2, student_distribution_2)
    result2: str = 'ОСНОВНАЯ, то есть выборки имеют одинаковые математическое ожидания' if abs(dt) < c \
        else 'АЛЬТЕРНАТИВНАЯ, то есть выборки имеют разные математические ожидания'

    print(f'        Критерий Стьюдента говорит, что если |dT| <= c, то верна основная гипотеза, '
          f'иначе - альтернативная\n'
          f'        В нашем случае {abs(dt)} <= {c}, значит верна {result2}')


def main() -> None:
    # Общая нормальная выборка (первая выборка + вторая выборка)
    norm_sampling: numpy.ndarray = read_sampling(sampling_input_file_name)

    first_part(norm_sampling)
    second_part(norm_sampling)


if __name__ == '__main__':
    main()
