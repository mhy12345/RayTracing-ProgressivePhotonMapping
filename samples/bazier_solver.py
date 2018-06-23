import numpy as np
from math import *

#Cup Info
Q = np.matrix([[0],[0],[0]])
P = np.matrix([(0,-.04),(1.1,-.04),(1.2,0),(2,0)])

#Ray Info
O = np.matrix("0.7071067811865475;0.7071067811865475;0")
D = np.matrix("0;0;1")


def getP(t):
    res =  1 * P[0] * t**0 * (1-t)**3 + \
            3 * P[1] * t**1 * (1-t)**2 + \
            3 * P[2] * t**2 * (1-t)**1 + \
            1 * P[3] * t**3 * (1-t)**0
    return res

def getdP(t):
    res = - 3 * P[0] * (1-t)**2 + \
            3 * P[1] * (1-t)**2 + \
            - 6 * P[1] * t * (1-t) + \
            6 * P[2] * (1-t) * t  + \
            - 3 * P[2] * t**2 + \
            3 * P[3] * t**2
    return res

def getPx(t):
    return getP(t)[0,0]

def getPy(t):
    return getP(t)[0,1]

def getdPx(t):
    return getdP(t)[0,0]

def getdPy(t):
    return getdP(t)[0,1]

def getC(t):
    return O+t*D

def getdC(t):
    return D

def getS(u,theta):
    return Q + np.matrix(
            [
                [sin(theta)*getPx(u)],
                [cos(theta)*getPx(u)],
                [getPy(u)]
                ]
            )

def getF(t,u,theta):
    return getC(t) - getS(u,theta)

def getdF(t,u,theta):
    return np.matrix([
        [D[0,0],-sin(theta)*getdPx(u),-cos(theta)*getPx(u)],
        [D[1,0],-cos(theta)*getdPx(u),+sin(theta)*getPx(u)],
        [D[2,0],-getdPy(u),0]
        ])

#print(getF(0,0,0))
lr = 1

u = 0.01
theta = 1
t = 0
print(getC(t),getS(u,theta))
args = np.matrix([[t],[u],[theta]])
for i in range(10):
    t,u,theta = np.array(args)[:,0].tolist()
    print("t,u,theta = ",t,u,theta)
    F = getF(t,u,theta)
    print("F = \n",F)
    dF = getdF(t,u,theta)
    print("...",u,getPx(u))
    print("dF = \n",dF)
    if not (abs(F)>1e-8).any():
        print("OKAY")
        break
    args = args - (dF.I*F)*lr
    args[2]%=3.1415926535
print("Result = ",args)
