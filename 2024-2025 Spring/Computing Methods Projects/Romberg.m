a = input("左端点：");
b = input("右端点：");
e = input("精度：");
M = input("循环次数：");
h = b-a;
R = zeros(M,M);

function y = f(x)
    y = 1/x;
end

R(1,1) = (f(a)+f(b))*h/2;

for k = 2:M
    sum = 0;
    for i = 1:2^(k-2)
        sum = sum + f(a+(2*i-1)*h/2^(k-1));
    end
    R(k,1) = (R(k-1,1) + h/2^(k-2)*sum)/2;
    for j = 2:k
        R(k,j) = R(k,j-1) + (R(k,j-1)-R(k-1,j-1))/(4^(j-1)-1);
    end
    if abs(R(k,k)-R(k-1,k-1)) < e
        break;
    end
end

disp(num2str(R, '%.5f '));
