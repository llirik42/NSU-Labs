import matplotlib.pyplot

SEQUENTIAL_TIME_MS = 198544
SEQUENTIAL_ITERATIONS_COUNT = 14906

TIME_MEASUREMENTS_MS = {
    1: 199749,
    2: 93662,
    4: 46368,
    6: 49745,
    8: 37535,
    12: 22327,
    14: 26891,
    16: 16933,
    20: 11782,
    22: 9308,
    24: 9618,
}
ITERATIONS_MEASUREMENTS = {
    1: 14906,
    2: 13943,
    4: 13498,
    6: 21646,
    8: 21368,
    12: 18579,
    14: 22476,
    16: 18665,
    20: 16074,
    22: 13662,
    24: 15369,
}

parallel_ms_on_one_iteration = {p: TIME_MEASUREMENTS_MS[p] / ITERATIONS_MEASUREMENTS[p] for p in TIME_MEASUREMENTS_MS}
sequential_ms_on_one_iteration = SEQUENTIAL_TIME_MS / SEQUENTIAL_ITERATIONS_COUNT

acceleration = {p: sequential_ms_on_one_iteration / parallel_ms_on_one_iteration[p] for p in TIME_MEASUREMENTS_MS}
efficiency = {p: 100 * acceleration[p] / p for p in TIME_MEASUREMENTS_MS}

str_time_s = {str(p): TIME_MEASUREMENTS_MS[p] / 1000 for p in TIME_MEASUREMENTS_MS}
str_acceleration = {str(p): acceleration[p] for p in acceleration}
str_efficiency = {str(p): efficiency[p] for p in efficiency}
str_ms_on_one_iteration = {str(p): parallel_ms_on_one_iteration[p] for p in parallel_ms_on_one_iteration}
str_iterations_count_thousands = {str(p): ITERATIONS_MEASUREMENTS[p] / 1000 for p in ITERATIONS_MEASUREMENTS}

font = {'size': 30}
matplotlib.rc('font', **font)

fig1, ax1 = matplotlib.pyplot.subplots()
ax1.plot(str_time_s.keys(), str_time_s.values(), label='Время работы, с', linewidth=2, color='g')
ax1.set_xlabel('Количество процессов')
ax1.set_ylabel('Время работы, с')
ax1.legend()

fig2, ax2 = matplotlib.pyplot.subplots()
ax2.plot(str_acceleration.keys(), str_acceleration.values(), label='Ускорение', linewidth=2, color='g')
ax2.set_xlabel('Количество процессов')
ax2.set_ylabel('Ускорение')
ax2.legend()

fig3, ax3 = matplotlib.pyplot.subplots()
ax3.plot(str_efficiency.keys(), str_efficiency.values(), label='Эффективность', linewidth=2, color='g')
ax3.set_xlabel('Количество процессов')
ax3.set_ylabel('Эффективность')
ax3.legend()

fig4, ax4 = matplotlib.pyplot.subplots()
ax4.plot(str_ms_on_one_iteration.keys(), str_ms_on_one_iteration.values(), label='Время на одну итерацию, мс', linewidth=2, color='g')
ax4.set_xlabel('Количество процессов')
ax4.set_ylabel('Время на одну итерацию, мс')
ax4.legend()

fig5, ax5 = matplotlib.pyplot.subplots()
ax5.plot(str_iterations_count_thousands.keys(), str_iterations_count_thousands.values(), label='Количество итераций, тыс.', linewidth=2, color='g')
ax5.set_xlabel('Количество процессов')
ax5.set_ylabel('Количество итераций, тыс.')
ax5.legend()

matplotlib.pyplot.show()
