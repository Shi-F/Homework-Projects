n = int(input())
num = 0
if n <= 1:
    print(0)
else:
    for i in range(2,n+1):
        flag = 0
        for j in range(2,i):
            if i%j == 0:
                flag = 1
                break
        if flag == 0:
            num += 1
    print(num)