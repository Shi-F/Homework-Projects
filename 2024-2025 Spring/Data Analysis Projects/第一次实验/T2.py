year = int(input())
if year%100 == 0:
    if year%400 == 0:
        print('yes')
    else:
        print('no')
else:
    if year%4 == 0:
        print('yes')
    else:
        print('no')