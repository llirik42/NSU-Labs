import matplotlib.pyplot

x = [5 * 10**8, 10**9, 2 * 10**9, 4 * 10**9]

y0 = [13306, 26346, 52441, 105000]
y1 = [11346, 22428, 44904, 88928]
y2 = [11342, 22424, 44588, 88936]
y3 = [11378, 22681, 44595, 89702]
y4 = [11653, 23050, 45847, 91966]
y5 = [11601, 22000, 44300, 89153]
y6 = [12428, 24603, 48921, 98164]

fig, ax = matplotlib.pyplot.subplots()
ax.plot(x, y0, label='-O0', linewidth=2, color='b')
ax.plot(x, y1, label='-O1', linewidth=2, color='g')
ax.plot(x, y2, label='-O2', linewidth=2, color='r')
ax.plot(x, y3, label='-O3', linewidth=2, color='c')
ax.plot(x, y4, label='-Os', linewidth=2, color='m')
ax.plot(x, y5, label='-Ofast', linewidth=2, color='y')
ax.plot(x, y6, label='-Og', linewidth=2, color='k')

ax.set_xlabel('N')
ax.set_ylabel('Time, ms')
ax.legend()

matplotlib.pyplot.show()
