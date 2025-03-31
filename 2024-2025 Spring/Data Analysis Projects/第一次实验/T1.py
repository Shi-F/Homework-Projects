n = int(input())
a_0 = 0
a_1 = 1
if n == 0:
    print(a_0)
else:
    for i in range(1,n):
        temp = a_1
        a_1 = a_0 + a_1
        a_0 = temp
    print(a_1)