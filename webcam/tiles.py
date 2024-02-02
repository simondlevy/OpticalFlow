#!/usr/bin/python3

R = 14
C = 12
P = 2

for k in range(R * C):
    r = k // (P * C)
    c = (k % C) // P
    print('%3d: %d %d' % (k, r, c))
