t = %pi/2:0.01:7*%pi;

param3d(sin(t), sin(2 * t), t / 5, 45, 35, "X@Y@Z");
curve = gce();
curve.foreground = color("blue")
curve.thickness = 4;

param3d(cos(t), cos(2 * t), sin(t), 45, 35, "X@Y@Z");
curve = gce();
curve.foreground = color("red")
curve.thickness = 4;
