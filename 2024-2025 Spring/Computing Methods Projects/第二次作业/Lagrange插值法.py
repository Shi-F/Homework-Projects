def lagrange(x,y,xx,n):
    L = 0
    for i in range(n):
        yy = y[i]
        for j in range(n):
            if i != j:
                yy *= (xx-x[j])/(x[i]-x[j])
        L += yy
    return L

x = [81,100,121]
y = [9,10,11]
xx= 105
print(lagrange(x,y,xx,3))
