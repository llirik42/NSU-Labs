import numpy as np


def sign(x) -> int:
    if x > 0:
        return 1
    if x < 0:
        return -1

    return 0


def read_h() -> np.ndarray:
    elements: list[list[float]] = []
    with open('h.txt') as f:
        for line in f.readlines():
            elements.append([float(x) for x in line.split()])
    return np.array(elements)


def read_y() -> np.ndarray:
    elements: list[float] = []
    with open('y.txt') as f:
        elements.extend([float(x) for x in f.readlines()[0].split()])
    return np.array(elements)


def calculate_m_minus(h: np.ndarray, s: np.ndarray, m_plus: np.ndarray) -> np.ndarray:
    result: np.ndarray = np.empty(shape=m_plus.shape)

    for j in range(m_plus.shape[0]):
        for i in range(m_plus.shape[1]):
            result[j][i] = h[j][i] * (s[i] - m_plus[j][i])

    return result


def calculate_m_plus(h: np.ndarray, m_minus: np.ndarray) -> np.ndarray:
    result: np.ndarray = np.empty(shape=m_minus.shape)

    for j in range(m_minus.shape[0]):
        for i in range(m_minus.shape[1]):
            signum: float = 1
            min_value: float = np.inf

            for k in range(m_minus.shape[1]):
                if k == i or h[j][k] != 1:
                    continue

                mjk: float = m_minus[j][k]
                min_value = min(min_value, abs(mjk))
                signum *= sign(mjk)

            result[j][i] = h[j][i] * signum * min_value

    return result


def calculate_s(old_s: np.ndarray, m_plus: np.ndarray) -> np.ndarray:
    result: np.ndarray = np.empty(len(old_s))

    sums: np.ndarray = m_plus.sum(axis=0)
    for i in range(len(old_s)):
        result[i] = old_s[i] + sums[i]

    return result


def calculate_syndrome(y: np.ndarray, h: np.ndarray) -> np.ndarray:
    def single_f(value: int) -> int:
        return value % 2

    f = np.vectorize(single_f)
    mult_result: np.ndarray = h.dot(y.reshape(y.shape[0], -1))
    return f(mult_result.reshape(1, -1)[0])


def all_zeros(a: np.ndarray) -> bool:
    return not np.any(a)


def is_code_word(y: np.ndarray, h: np.ndarray) -> bool:
    return all_zeros(calculate_syndrome(y=y, h=h))


def s2y(s: np.ndarray) -> np.ndarray:
    def s2y_single(value: float) -> float:
        return 1 if value < 0 else 0

    return np.vectorize(s2y_single)(s)


def y2s(y: np.ndarray) -> np.ndarray:
    def y2s_single(value: int) -> float:
        return -1 if value == 1 else 1

    return np.vectorize(y2s_single)(y)


def normalize_s(s: np.ndarray) -> np.ndarray:
    result = np.empty(len(s))

    for i in range(len(result)):
        result[i] = sign(s[i])

    return result


def main() -> None:
    max_steps: int = 4
    y = read_y()
    h = read_h()

    if is_code_word(y=y, h=h):
        print('Исходный вектор является кодовый словом')

    s = y2s(y)
    m_plus = np.zeros(shape=h.shape)
    np.empty(shape=m_plus.shape)

    print('----INIT----')
    print(f'M+\n{m_plus}')
    print(f's = {s}\n')

    for i in range(max_steps):
        print(f'----ITERATION {i + 1}----')

        m_minus = calculate_m_minus(h=h, s=s, m_plus=m_plus)
        m_plus = calculate_m_plus(h=h, m_minus=m_minus)
        s = calculate_s(old_s=normalize_s(s), m_plus=m_plus)
        y = s2y(s)

        print(f'M-\n{m_minus}')
        print(f'M+\n{m_plus}')
        print(f's = {s}')
        print(f'y = {y}\n')

        if is_code_word(y=y, h=h):
            print(f'Ошибки восстановлены. Итог: {y}')
            return

    print(f'Восстановить ошибки полностью не получилось, y = {y}')


if __name__ == '__main__':
    main()
