a = input("x左端点：");
b = input("x右端点：");
c = input("y左端点：");
d = input("y右端点：");
m = input("x轴段数m：");
n = input("y轴段数n：");
h = (b-a)/m;
k = (d-c)/n;

function z = f(x,y)
    z = 1/(x+y);
end

C = ones(m+1,n+1);
C(1,:) = C(1,:)/2;
C(m+1,:) = C(m+1,:)/2;
C(:,1) = C(:,1)/2;
C(:,n+1) = C(:,n+1)/2;

result = 0;
for i = 0:m
    for j = 0:n
        result = result + C(i+1,j+1)*f(a+h*i,c+k*j);
    end
end
result = result * h * k;

fprintf('复化梯形公式近似结果：%.5f\n',result);
func = @(x,y) 1./(x+y);
fprintf('准确结果：%.5f\n',integral2(func,a,b,c,d));


