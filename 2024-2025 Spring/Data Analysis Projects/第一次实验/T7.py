array = [[None for _ in range(5001)] for _ in range(5001)]
for i in range(5001):
    array[0][i] = 0
    array[i][0] = 0
array[1][0] = 1
for i in range(1,5001):
    for j in range(1,5001):
        array[i][j] = int((array[i-1][j] + array[i][j-1])%(10**9+7))
n,m,q = list(map(int,input().split()))
result = []
for _ in range(q):
    point = list(map(int,input().split()))
    right = point[2]-point[0]+1
    down  = point[3]-point[1]+1
    if right >= 1 and down >= 1:
        result.append(array[right][down])
for i in range(q):
    print(result[i])