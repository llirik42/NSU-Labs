import matplotlib.pyplot

server_sequential = 90
server_point_to_point = [45, 23, 12, 11, 8, 11]
server_collective = [46, 23, 12, 11, 8, 11]

my_sequential = 47
my_point_to_point = [25, 13, 8, 9, 9, 9]
my_collective = [24, 12, 8, 9, 9, 9]

processes_count = [2, 4, 8, 16, 24, 32]
processes_count_str = [str(i) for i in processes_count]

server_point_to_point_acceleration = [server_sequential / i for i in server_point_to_point]
server_collective_acceleration = [server_sequential / i for i in server_collective]
my_point_to_point_acceleration = [my_sequential / i for i in my_point_to_point]
my_collective_acceleration = [my_sequential / i for i in my_collective]

server_point_to_point_efficiency = [100 * server_point_to_point_acceleration[i] / processes_count[i] for i in range(len(processes_count))]
server_collective_efficiency = [100 * server_collective_acceleration[i] / processes_count[i] for i in range(len(processes_count))]
my_point_to_point_efficiency = [100 * my_point_to_point_acceleration[i] / processes_count[i] for i in range(len(processes_count))]
my_collective_efficiency = [100 * my_collective_acceleration[i] / processes_count[i] for i in range(len(processes_count))]

fig1, ax1 = matplotlib.pyplot.subplots()
fig2, ax2 = matplotlib.pyplot.subplots()
fig3, ax3 = matplotlib.pyplot.subplots()
fig4, ax4 = matplotlib.pyplot.subplots()
fig5, ax5 = matplotlib.pyplot.subplots()
fig6, ax6 = matplotlib.pyplot.subplots()

ax1.plot(processes_count_str, server_point_to_point, label='точка-точка (кафедральный сервер)', linewidth=2, color='g')
ax1.plot(processes_count_str, server_collective, label='коллективные коммуникации (кафедральный сервер)', linewidth=2, color='r')
ax1.set_xlabel('Количество процессов')
ax1.set_ylabel('Время работы')
ax1.legend()

ax2.plot(processes_count_str, server_point_to_point_acceleration, label='точка-точка (кафедральный сервер)', linewidth=2, color='g')
ax2.plot(processes_count_str, server_collective_acceleration, label='коллективные коммуникации (кафедральный сервер)', linewidth=2, color='r')
ax2.set_xlabel('Количество процессов')
ax2.set_ylabel('Ускорение')
ax2.legend()

ax3.plot(processes_count_str, server_point_to_point_efficiency, label='точка-точка (кафедральный сервер)', linewidth=2, color='g')
ax3.plot(processes_count_str, server_collective_efficiency, label='коллективные коммуникации (кафедральный сервер)', linewidth=2, color='r')
ax3.set_xlabel('Количество процессов')
ax3.set_ylabel('Эффективность')
ax3.legend()

ax4.plot(processes_count_str, my_point_to_point, label='точка-точка (домашнее устрйоство)', linewidth=2, color='g')
ax4.plot(processes_count_str, my_collective, label='коллективные коммуникации (домашнее устрйоство)', linewidth=2, color='r')
ax4.set_xlabel('Количество процессов')
ax4.set_ylabel('Время работы')
ax4.legend()

ax5.plot(processes_count_str, my_point_to_point_acceleration, label='точка-точка (домашнее устрйоство)', linewidth=2, color='g')
ax5.plot(processes_count_str, my_collective_acceleration, label='коллективные коммуникации (домашнее устрйоство)', linewidth=2, color='r')
ax5.set_xlabel('Количество процессов')
ax5.set_ylabel('Ускорение')
ax5.legend()

ax6.plot(processes_count_str, my_point_to_point_efficiency, label='точка-точка (домашнее устрйоство)', linewidth=2, color='g')
ax6.plot(processes_count_str, my_collective_efficiency, label='коллективные коммуникации (домашнее устрйоство)', linewidth=2, color='r')
ax6.set_xlabel('Количество процессов')
ax6.set_ylabel('Эффективность')
ax6.legend()

matplotlib.pyplot.show()
