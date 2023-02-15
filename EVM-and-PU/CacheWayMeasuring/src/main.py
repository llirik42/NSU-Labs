from matplotlib.ticker import MultipleLocator
from matplotlib.pyplot import subplots
from matplotlib.pyplot import show

DEFAULT_PATH = '../;'
FILES_COUNT = 3


def draw(index: int) -> None:
    with open(f'{DEFAULT_PATH}{index}') as file:
        pairs = [line.rstrip().split() for line in file]

    x = []
    y = []

    for pair in pairs:
        x.append(int(pair[0]))
        y.append(int(pair[1]))

    fig, ax = subplots()

    ax.plot(x, y, label=f'L{index}', linewidth=2, color='k', marker='*')

    ax.xaxis.set_major_locator(MultipleLocator(1))
    ax.yaxis.set_major_locator(MultipleLocator(2))

    ax.set_xlabel('Число фрагментов')
    ax.set_ylabel('Время чтения элемента в тактах')
    ax.legend()


for i in range(1, FILES_COUNT + 1):
    draw(i)

show()
