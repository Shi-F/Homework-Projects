num = list(input().split())
sum = 0
for ch1 in num[0]:
    for ch2 in num[1]:
        sum += int(ch1)*int(ch2)
print(sum)