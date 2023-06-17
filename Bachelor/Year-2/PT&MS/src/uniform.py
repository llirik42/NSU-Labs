import numpy
import matplotlib.pyplot
from math import log2, ceil, sqrt
from scipy.stats import *


a = 0
b = 1
epsilon = 0.15


def get_quantile(level: float, stat) -> float:
    return stat.ppf(level)


def calculate_p(x: numpy.ndarray, max_value, min_value=-1) -> float:
    return len([i for i in x if min_value <= i < max_value]) / x.size


def get_empirical_distribution_function(x: numpy.ndarray, sampling: numpy.ndarray) -> numpy.ndarray:
    return numpy.array([calculate_p(sampling, t) for t in x])


def draw_empirical_distribution_function(sampling: numpy.ndarray) -> None:
    x = numpy.linspace(a, b, 100000)
    y = get_empirical_distribution_function(x, sampling)

    _, ax = matplotlib.pyplot.subplots()
    ax.plot(x, y, linewidth=3, color='g', label='Эмпирическая функция распределения')
    ax.set_xticks(numpy.linspace(0, 1, 21))
    ax.set_yticks(numpy.linspace(0, 1, 21))
    ax.plot(x, x, linewidth=3, color='r', label='Теоретическая функция распределения')
    ax.legend()


def draw_empirical_density(sampling: numpy.ndarray) -> None:
    k = ceil(log2(sampling.size))  # Формула Стёрджесса
    points = numpy.linspace(a, b, k + 1)
    v = [calculate_p(sampling, points[i], points[i - 1]) for i in range(1, k + 1)]

    x = numpy.linspace(a, b, 100000)
    y = []

    for i in x:
        for j in range(points.size - 1):
            if points[j] <= i <= points[j + 1]:
                y.append(v[j])
                break

    _, ax = matplotlib.pyplot.subplots()
    ax.plot(x, y, linewidth=3, color='g', label='гистрограмма')
    ax.set_yticks(numpy.linspace(0.1, 0.3, 21))
    ax.legend()


matplotlib.pyplot.rcParams['font.size'] = 12

with open("uniform_sampling.txt", "r") as input_file:
    uniform_sampling = numpy.array(sorted([float(str_value) for str_value in input_file.readline().split(' ')]))

    # 3

    draw_empirical_distribution_function(uniform_sampling)
    draw_empirical_density(uniform_sampling)

    # 4-a

    n = uniform_sampling.size

    delta1 = [abs(uniform_sampling[k] - k / n) for k in range(n)]
    delta2 = [abs(uniform_sampling[k] - (k + 1) / n) for k in range(n)]
    sup = max(max(delta1), max(delta2))

    point_of_sup = 0

    try:
        point_of_sup = uniform_sampling[delta1.index(sup)]
    except ValueError:
        pass

    try:
        point_of_sup = uniform_sampling[delta2.index(sup)]
    except ValueError:
        pass

    dK = sup * sqrt(n)
    print(f'dK={dK}')

    # 4-b

    k = ceil(log2(uniform_sampling.size))  # Формула Стёрджесса
    points = numpy.linspace(a, b, k + 1)
    v = [calculate_p(uniform_sampling, points[i], points[i - 1]) for i in range(1, k + 1)]

    x = numpy.linspace(a, b, 100001)
    y = []

    for i in x:
        for j in range(points.size - 1):
            if points[j] <= i <= points[j + 1]:
                y.append(v[j])
                break

    P = numpy.array([1 / k] * 5)
    V = [i * n for i in v]
    dx = sum([(V[j] - n * P[j])**2 / (n * P[j]) for j in range(k)])
    h = get_quantile(1 - epsilon, chi2(k - 1))

    matplotlib.pyplot.show()
