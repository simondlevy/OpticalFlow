#!/usr/bin/python3

R = 14
C = 12
P = 2

Q = P * C

for k in range(R * C):
    r = k // Q
    c = (k % C) // P
    print('%3d: %d %d' % (k, r, c))
