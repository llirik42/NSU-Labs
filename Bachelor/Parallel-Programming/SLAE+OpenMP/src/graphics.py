import matplotlib.pyplot

matplotlib.pyplot.rcParams['font.size'] = 34

VECTORIZATION_LABEL = "Vectorization"
AUTO_LABEL = "Auto"
STATIC_LABEL = "Static"
DYNAMIC_LABEL = "Dynamic"
GUIDED_LABEL = "Guided"
THREADS_IN_SCHEDULE_TESTS = 12

SEQUENTIAL_TIME = 29.921492
AUTO_TIME = {
    1: 30.656595,
    2: 15.337238,
    3: 10.414434,
    4: 7.754618,
    5: 6.403331,
    6: 5.310859,
    7: 4.852064,
    8: 4.451385,
    9: 4.010223,
    10: 3.873971,
    11: 3.649417,
    12: 3.405373,
    13: 3.585929,
    14: 3.719633,
    15: 3.720170,
    16: 3.869554
}
VECTORIZATION_TIME = {
    1: 21.448429,
    2: 10.765466,
    3: 7.242099,
    4: 5.639607,
    5: 4.530628,
    6: 3.959578,
    7: 3.510206,
    8: 3.093551,
    9: 2.678271,
    10: 2.462821,
    11: 2.298102,
    12: 2.156147,
    13: 3.771910,
    14: 3.573371,
    15: 3.338310,
    16: 3.139209
}
DYNAMIC_TIME = {
    1: 14.941706,
    2: 10.232851,
    4: 7.066794,
    8: 4.181839,
    16: 2.874797,
    32: 3.095818,
    64: 3.172072,
    128: 4.048129,
    256: 4.003789,
    512: 7.708296,
    1024: 15.185856,
}
GUIDED_TIME = {
    1: 3.504476,
    2: 3.353163,
    4: 3.250051,
    8: 3.035194,
    16: 2.987457,
    32: 2.997436,
    64: 3.236069,
    128: 4.045351,
    256: 3.996891,
    512: 7.656672,
    1024: 15.167127,
}
STATIC_TIME = {
    1: 4.301964,
    2: 4.132787,
    4: 4.053765,
    8: 4.026089,
    16: 3.404995,
    32: 4.022443,
    64: 3.536644,
    128: 4.047779,
    256: 4.076902,
    512: 7.645804,
    1024: 15.140869,
}

auto_acceleration_t = {p: SEQUENTIAL_TIME / AUTO_TIME[p] for p in AUTO_TIME}
vectorization_acceleration = {p: SEQUENTIAL_TIME / VECTORIZATION_TIME[p] for p in VECTORIZATION_TIME}
dynamic_acceleration = {n: SEQUENTIAL_TIME / DYNAMIC_TIME[n] for n in DYNAMIC_TIME}
guided_acceleration = {n: SEQUENTIAL_TIME / GUIDED_TIME[n] for n in GUIDED_TIME}
static_acceleration = {n: SEQUENTIAL_TIME / STATIC_TIME[n] for n in STATIC_TIME}

auto_efficiency_t = {p: 100 * auto_acceleration_t[p] / p for p in AUTO_TIME}
vectorization_efficiency = {p: 100 * vectorization_acceleration[p] / p for p in VECTORIZATION_TIME}
dynamic_efficiency = {n: 100 * dynamic_acceleration[n] / THREADS_IN_SCHEDULE_TESTS for n in DYNAMIC_TIME}
guided_efficiency = {n: 100 * guided_acceleration[n] / THREADS_IN_SCHEDULE_TESTS for n in GUIDED_TIME}
static_efficiency = {n: 100 * static_acceleration[n] / THREADS_IN_SCHEDULE_TESTS for n in STATIC_TIME}
auto_efficiency_n = {n: auto_efficiency_t[THREADS_IN_SCHEDULE_TESTS] for n in STATIC_TIME}

str_auto_time_t = {str(p): AUTO_TIME[p] for p in AUTO_TIME}
str_vectorization_time = {str(p): VECTORIZATION_TIME[p] for p in VECTORIZATION_TIME}
str_dynamic_time = {str(n): DYNAMIC_TIME[n] for n in DYNAMIC_TIME}
str_guided_time = {str(n): GUIDED_TIME[n] for n in GUIDED_TIME}
str_static_time = {str(n): STATIC_TIME[n] for n in STATIC_TIME}
str_auto_time_n = {str(n): AUTO_TIME[THREADS_IN_SCHEDULE_TESTS] for n in STATIC_TIME}

str_auto_acceleration_t = {str(p): auto_acceleration_t[p] for p in auto_acceleration_t}
str_vectorization_acceleration = {str(p): vectorization_acceleration[p] for p in vectorization_acceleration}
str_dynamic_acceleration = {str(n): dynamic_acceleration[n] for n in dynamic_acceleration}
str_guided_acceleration = {str(n): guided_acceleration[n] for n in guided_acceleration}
str_static_acceleration = {str(n): static_acceleration[n] for n in static_efficiency}
str_auto_acceleration_n = {str(n): auto_acceleration_t[THREADS_IN_SCHEDULE_TESTS] for n in STATIC_TIME}

str_auto_efficiency_t = {str(p): auto_efficiency_t[p] for p in auto_efficiency_t}
str_vectorization_efficiency = {str(p): vectorization_efficiency[p] for p in vectorization_efficiency}
str_dynamic_efficiency = {str(n): dynamic_efficiency[n] for n in dynamic_efficiency}
str_guided_efficiency = {str(n): guided_efficiency[n] for n in guided_efficiency}
str_static_efficiency = {str(n): static_efficiency[n] for n in static_efficiency}
str_auto_efficiency_n = {str(n): auto_efficiency_t[THREADS_IN_SCHEDULE_TESTS] for n in static_efficiency}

fig1, ax1 = matplotlib.pyplot.subplots()
ax1.plot(str_auto_time_t.keys(), str_auto_time_t.values(), label=AUTO_LABEL, linewidth=3, color='g')
ax1.plot(str_vectorization_time.keys(), str_vectorization_time.values(), label=VECTORIZATION_LABEL, linewidth=3, color='m')
ax1.set_xlabel('Количество потоков')
ax1.set_ylabel('Время на итерацию, мс')
ax1.legend()

fig2, ax2 = matplotlib.pyplot.subplots()
ax2.plot(str_auto_acceleration_t.keys(), str_auto_acceleration_t.values(), label=AUTO_LABEL, linewidth=3, color='g')
ax2.plot(str_vectorization_acceleration.keys(), str_vectorization_acceleration.values(), label=VECTORIZATION_LABEL, linewidth=3, color='m')
ax2.set_xlabel('Количество потоков')
ax2.set_ylabel('Ускорение')
ax2.set_yticks([n for n in range(1, 17)])
ax2.legend()

fig3, ax3 = matplotlib.pyplot.subplots()
ax3.plot(str_auto_efficiency_t.keys(), str_auto_efficiency_t.values(), label=AUTO_LABEL, linewidth=3, color='g')
ax3.plot(str_vectorization_efficiency.keys(), str_vectorization_efficiency.values(), label=VECTORIZATION_LABEL, linewidth=3, color='m')
ax3.set_xlabel('Количество потоков')
ax3.set_ylabel('Эффективность, %')
ax3.legend()

fig4, ax4 = matplotlib.pyplot.subplots()
ax4.plot(str_static_time.keys(), str_static_time.values(), label=STATIC_LABEL, linewidth=3, color='y')
ax4.plot(str_dynamic_time.keys(), str_dynamic_time.values(), label=DYNAMIC_LABEL, linewidth=3, color='r')
ax4.plot(str_guided_time.keys(), str_guided_time.values(), label=GUIDED_LABEL, linewidth=3, color='b')
ax4.plot(str_auto_time_n.keys(), str_auto_time_n.values(), label=AUTO_LABEL, linewidth=3, color='g')
ax4.set_xlabel('Параметр распределения')
ax4.set_ylabel('Время на итерацию, мс')
ax4.legend()

fig5, ax5 = matplotlib.pyplot.subplots()
ax5.plot(str_static_acceleration.keys(), str_static_acceleration.values(), label=STATIC_LABEL, linewidth=3, color='y')
ax5.plot(str_dynamic_acceleration.keys(), str_dynamic_acceleration.values(), label=DYNAMIC_LABEL, linewidth=3, color='r')
ax5.plot(str_guided_acceleration.keys(), str_guided_acceleration.values(), label=GUIDED_LABEL, linewidth=3, color='b')
ax5.plot(str_auto_acceleration_n.keys(), str_auto_acceleration_n.values(), label=AUTO_LABEL, linewidth=3, color='g')
ax5.set_xlabel('Параметр распределения')
ax5.set_ylabel('Ускорение')
ax5.set_yticks([n for n in range(1, 11)])
ax5.legend()

fig6, ax6 = matplotlib.pyplot.subplots()
ax6.plot(str_static_efficiency.keys(), str_static_efficiency.values(), label=STATIC_LABEL, linewidth=3, color='y')
ax6.plot(str_dynamic_efficiency.keys(), str_dynamic_efficiency.values(), label=DYNAMIC_LABEL, linewidth=3, color='r')
ax6.plot(str_guided_efficiency.keys(), str_guided_efficiency.values(), label=GUIDED_LABEL, linewidth=3, color='b')
ax6.plot(str_auto_efficiency_n.keys(), str_auto_efficiency_n.values(), label=AUTO_LABEL, linewidth=3, color='g')
ax6.set_xlabel('Параметр распределения')
ax6.set_ylabel('Эффективность, %')
ax6.legend()

matplotlib.pyplot.show()
