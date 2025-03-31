%% 初始化
x = [0.25, 0.50, 0.75, 1.00, 1.25, 1.50, 1.75, 2.00, 2.25, 2.50];
y = [1.284, 1.648, 2.117, 2.718, 3.427, 2.798, 3.534, 4.456, 5.465, 5.894];
A = [sin(x'), cos(x')];

%% 计算系数(a,b)
G = A' * A;
R = A' * y';
alpha = G \ R;
a = alpha(1); b = alpha(2);
fprintf('a = %.15e, b = %.15e\n',a,b);

%% 计算均方误差
f = @(x) a * sin(x) + b * cos(x);

delta = f(x) - y;
Q = sum(delta.^2);
fprintf('均方误差 Q = %.15e',Q);

%% 输出图像
% fplot(f);
% axis([0, 3, 1, 6]);
% hold on;
% scatter(x,y);