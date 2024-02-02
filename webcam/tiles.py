#!/usr/bin/python3

R = 14
C = 12
P = 2

for j in range(R * C):
    r = j // (P * C)
    c = (j % C) // P
    k = j % (P*P) 
    print('%3d: %d %d %d' % (j, r, c, k))
