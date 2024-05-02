// Параметры дискретизации
minValue = 0;
maxValue = 50;
step0 = 1;
step1 = 0.1;
step2 = 0.01;
step3 = 0.001;
count0 = (maxValue - minValue) / step0 + 1;
count1 = (maxValue - minValue) / step1 + 1;
count2 = (maxValue - minValue) / step2 + 1;
count3 = (maxValue - minValue) / step3 + 1;

// Параметры рассматривемого объекта
n = 4;
T0 = 1.07;

// Измеренные в первом задании критические значения
Tcr = 6.787;
Kcr = 1.97;

// Настроенные в первом задании оптимальные параметры
K = 0.45 * Kcr - 0.2;
Ti = Tcr / 1.2 - 0.7;

// Построение переходной характеристики системы
s = poly(0, 's');
W0 = 1 / (1 + T0 * s)^n; // Переходная характеристика рассматриваемого объекта
W1 = (1 + 1 / (Ti * s)) * W0 * K; // Переходная характеристика звена без обратной связи
W = W1 / (1 + W1); // Переходная характеристика системы
b = 0.105679 + 0.5237276 * s; // Числитель W
a = 0.105679 + 1.2866228 * s + 3.2651915 * s^2 + 5.2406324 * s^3 + 3.7383178 * s^4 + s^5; // Знаменатель W

// Вычисление матрицы A в форме Фробениуса. Она будет иметь размеры 5x5 потому что 5 - степень a(s)
A = [zeros(1, 4); eye(4, 4)];
ac = [0.105679; 1.2866228; 3.2651915; 5.2406324; 3.7383178]; // коэффициенты из a
A = [A, -ac];

// Вычисление матриц B и C
B = [0.105679; 0.5237276; 0; 0; 0]; // коэффициенты из b, имеет длину как сторона A
C = [0 0 0 0 1]; // имеет длину как сторона A

function result = calculate_s(step)
	result = inv(A) * (eye(A) - expm(-A * step));
endfunction;

function result = calculate_ad(step)
	result = eye(A) + A * step;
endfunction;

function result = calculate_bd(Ad, S)
	result = Ad * S * B;
endfunction;

function result = calculate_error(realH, approxH)
	errors = (realH - approxH) .^ 2;
	errors_sum = sum(errors);
	result = errors_sum / length(realH);
endfunction;

// Чтение переходной характеристики, полученной из Microcap
h0 = read("1.txt", 1, count0); // переходная характеристика при шаге 1
h1 = read("0,1.txt", 1, count1); // переходная характеристика при шаге 0.1
h2 = read("0,01.txt", 1, count2); // переходная характеристика при шаге 0.01
h3 = read("0,001.txt", 1, count3); // переходная характеристика при шаге 0.001

function result = calculate_h(Ad, Bd, Cd, count)
	vk = zeros(Bd);
	h = [];
	for k = 1:count
		h = [h Cd * vk];
		vk = Ad * vk + Bd;
    end
    result = h;
endfunction;

// Дискретизация, использующая встроенные средства Scilab при шаге 1
sys0 = syslin('c', W);
sysd0 = dscr(sys0, step0);
Ad10 = sysd0.A;
Bd10 = sysd0.B;
Cd10 = sysd0.C;
h10 = calculate_h(Ad10, Bd10, Cd10, count0);

// Дискретизация, использующая встроенные средства Scilab при шаге 0.1
sys1 = syslin('c', W);
sysd1 = dscr(sys1, step1);
Ad11 = sysd1.A;
Bd11 = sysd1.B;
Cd11 = sysd1.C;
h11 = calculate_h(Ad11, Bd11, Cd11, count1);

// Дискретизация, использующая встроенные средства Scilab при шаге 0.01
sys2 = syslin('c', W);
sysd2 = dscr(sys2, step2);
Ad12 = sysd2.A;
Bd12 = sysd2.B;
Cd12 = sysd2.C;
h12 = calculate_h(Ad12, Bd12, Cd12, count2);

// Дискретизация, использующая встроенные средства Scilab при шаге 0.001
sys3 = syslin('c', W);
sysd3 = dscr(sys3, step3);
Ad13 = sysd3.A;
Bd13 = sysd3.B;
Cd13 = sysd3.C;
h13 = calculate_h(Ad13, Bd13, Cd13, count3);

// Ручная дискретизация при шаге 1
Ad20 = calculate_ad(step0);
S0 = calculate_s(step0);
Bd20 = calculate_bd(Ad20, S0);
h20 = calculate_h(Ad20, Bd20, C, count0);

// Ручная дискретизация при шаге 0.1
Ad21 = calculate_ad(step1);
S1 = calculate_s(step1);
Bd21 = calculate_bd(Ad21, S1);
h21 = calculate_h(Ad21, Bd21, C, count1);

// Ручная дискретизация при шаге 0.01
Ad22 = calculate_ad(step2);
S2 = calculate_s(step2);
Bd22 = calculate_bd(Ad22, S2);
h22 = calculate_h(Ad22, Bd22, C, count2);

// Ручная дискретизация при шаге 0.001
Ad23 = calculate_ad(step3);
S3 = calculate_s(step3);
Bd23 = calculate_bd(Ad23, S3);
h23 = calculate_h(Ad23, Bd23, C, count3);

// Ошибки для шага 1
calculate_error(h0, h10)
calculate_error(h0, h20)

// Ошибки для шага 0.1
calculate_error(h1, h11)
calculate_error(h1, h21)

// Ошибки для шага 0.01
calculate_error(h2, h12)
calculate_error(h2, h22)

// Ошибки для шага 0.001
calculate_error(h3, h13)
calculate_error(h3, h23)
