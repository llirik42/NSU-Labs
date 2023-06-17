import numpy
from scipy.stats import *
from math import sqrt


def get_quantile(level: float, stat) -> float:
    return stat.ppf(level)


def get_unbiased_variance(sampling: numpy.ndarray) -> float:
    return sampling.var() * sampling.size / (sampling.size - 1)


a = 2
sigma_squared = 1.1
epsilon = 0.2
n = 20
m = 30
N = n + m

with open("norm_sampling.txt", "r") as input_file:
    norm_sampling = numpy.array([float(str_value) for str_value in input_file.readline().split(' ')])

    # 1-а

    average = norm_sampling.mean()
    q1 = get_quantile(1 - epsilon / 2, norm)
    tmp1 = average - q1 * sqrt(sigma_squared) / sqrt(N)
    print(f'a: [{average - tmp1}; {average + tmp1}]')

    # 1-б

    S0 = sqrt(get_unbiased_variance(norm_sampling))
    q2 = get_quantile(1 - epsilon / 2, nct(N - 1, 0))
    tmp2 = S0 * q2 / sqrt(N)
    print(f'a: [{average - tmp2}; {average + tmp2}]')

    # 1-в

    xi2_1 = chi2(N)
    h1_1 = get_quantile(epsilon / 2, xi2_1)
    h2_1 = get_quantile(1 - epsilon / 2, xi2_1)
    S1_squared = sum([(x - a) ** 2 for x in norm_sampling]) / N
    print(f'sigma^2: [{S1_squared * N / h2_1}; {N * S1_squared / h1_1}]')

    # 1-г

    xi2_2 = chi2(N - 1)
    h1_2 = get_quantile(epsilon / 2, xi2_2)
    h2_2 = get_quantile(1 - epsilon / 2, xi2_2)
    print(f'sigma^2: [{N * norm_sampling.var() / h2_2}; {N * norm_sampling.var() / h1_2}]\n')

    # 2-а

    X: numpy.ndarray = norm_sampling[0:20]
    Y: numpy.ndarray = norm_sampling[20:]

    S0_X_squared = get_unbiased_variance(X)
    S0_Y_squared = get_unbiased_variance(Y)
    df = S0_X_squared / S0_Y_squared
    F = ncf(n - 1, m - 1, 0)
    c1 = get_quantile(epsilon / 2, F)
    c2 = get_quantile(1 - epsilon / 2, F)
    print(f'dF={df}')

    # 2-б

    dT = (X.mean() - Y.mean()) * sqrt(m + n - 2) / sqrt((1 / n + 1 / m) * (n * X.var() + m * Y.var()))
    print(f'dT={dT}')
    c = get_quantile(1 - epsilon / 2, nct(n + m - 2, 0))
