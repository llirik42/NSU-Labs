fi = 0:0.01:4*%pi;
r = 3 * fi.^2 .+ fi;
polarplot(fi, r);
