n = int(input())
binary_n = bin(n)[2:]
count = 0
for bit in binary_n:
    if bit == '1':
        count += 1
print(count)