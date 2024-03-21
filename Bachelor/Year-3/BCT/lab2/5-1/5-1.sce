x = [3 3.1 3.2 3.3 3.4 3.5 3.6 3.7 3.8 3.9];
y = [0.61, 0.6, 0.592, 0.58, 0.585, 0.583, 0.582, 0.57, 0.572, 0.571];
plot2d(x, y);

function [zr] = G(c, z)
	zr = z(2) - z(1) / (c(1) * z(1) - c(2))
endfunction

z = [x;y];
c = [1; 1]

[a, err] = datafit(G ,z, c);

x1 = min(x):0.01:max(x);
y1 = x1 ./ (a(1) * x1 - a(2));
plot(x1, y1);
