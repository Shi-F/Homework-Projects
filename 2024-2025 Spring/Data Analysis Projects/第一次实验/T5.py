n, m = map(int, input().split())
milk = []
for _ in range(m):
    p, a = map(int, input().split())
    milk.append((p, a))
sorted_milk = sorted(milk, key=lambda x: x[0])
cost = 0
need = n
for price, amount in sorted_milk:
    if need >= amount:
        cost += price * amount
        need -= amount
    else:
        cost += price * need
        need = 0
        break
print(cost)