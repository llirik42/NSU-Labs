import matplotlib.pyplot
from matplotlib import ticker


def convert(s):
    a = int(s)

    bytes_counter = a * 4

    if bytes_counter < 1024 * 1024:
        return str(round(bytes_counter / 1024, 1)) + '\nKB'

    return str(round(bytes_counter / 1024 / 1024, 1)) + '\nMB'


x1 = []
y1 = []

x2 = []
y2 = []

x3 = []
y3 = []

with open('C:\\CLionProjects\\CacheMeasuring\\1.txt') as file:
    pairs1 = [line.rstrip().split() for line in file]

with open('C:\\CLionProjects\\CacheMeasuring\\2.txt') as file:
    pairs2 = [line.rstrip().split() for line in file]

with open('C:\\CLionProjects\\CacheMeasuring\\3.txt') as file:
    pairs3 = [line.rstrip().split() for line in file]

for pair in pairs1:
    x1.append(convert(pair[0]))
    y1.append(int(pair[1]))

for pair in pairs2:
    x2.append(convert(pair[0]))
    y2.append(int(pair[1]))

for pair in pairs3:
    x3.append(convert(pair[0]))
    y3.append(int(pair[1]))

fig1, ax1 = matplotlib.pyplot.subplots()
fig2, ax2 = matplotlib.pyplot.subplots()
fig3, ax3 = matplotlib.pyplot.subplots()

ax1.plot(x1, y1, label='прямой обход', linewidth=2, color='g')
ax2.plot(x2, y2, label='обратный обход', linewidth=2, color='r')
ax3.plot(x3, y3, label='случайный обход', linewidth=2, color='b')

ax1.xaxis.set_major_locator(ticker.MultipleLocator(6))
ax2.xaxis.set_major_locator(ticker.MultipleLocator(6))
ax3.xaxis.set_major_locator(ticker.MultipleLocator(6))

ax1.set_xlabel('Размер массива')
ax1.set_ylabel('Время чтения элемента массива')
ax1.legend()

ax2.set_xlabel('Размер массива')
ax2.set_ylabel('Время чтения элемента массива')
ax2.legend()

ax3.set_xlabel('Размер массива')
ax3.set_ylabel('Время чтения элемента массива')
ax3.legend()

matplotlib.pyplot.show()
