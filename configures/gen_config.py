import json
import math
import random
import copy
from math import *

def loadObject(name):
    with open('objects/'+name+'.json','r') as fin:
        cfg = json.load(fin)
    return cfg

cfg = loadObject('template')
_area_light = loadObject('area_light')
_sphere = loadObject('sphere')
_floor = loadObject('floor')
_point_light = loadObject('point_light')
_marble_ball = loadObject('marble_ball')

def getPosition(x,y,z):
    return {'x':x,'y':y,'z':z}

def getColor(r,g,b):
    return {'r':r,'g':g,'b':b,'type':'pure'}

def getGlassSphere(center,radius,color):
    return {
            'type':'sphere', 
            'name':'ball',
            'center':center,
            'radius':radius,
            'absorb':color,
            'material':{'refl':0.0,'diff':0.0,'spec':0.6,'refr':0.9,'refr_k':1.3}
    }

simple_floor = copy.copy(_floor)
simple_floor['texture'] = getColor(0,1,0)
back_light = copy.deepcopy(_area_light)
back_light['name'] = 'back_light'
back_light['position'] = getPosition(3,-1,35)
back_light['dx'] = getPosition(0,6,0)
back_light['dy'] = getPosition(2,0,-0.5)


def scene1():
    #cfg['lights'].append(_area_light)
    cfg['lights'].append(back_light)
    cfg['objects'].append(_sphere)
    cfg['objects'].append(simple_floor)
    '''
    for i in range(1):
        sphere = copy.deepcopy(_sphere)
        sphere['name'] = 'ball_%d'%i
        sphere['radius'] = 2
        sphere['center']['x'] = i+2
        sphere['center']['y'] = math.sin(math.pi/4*i)
        sphere['center']['z'] = 27 + math.cos(math.pi/4*i)
        sphere['absorb']['r'] = random.random()
        sphere['absorb']['g'] = random.random()
        sphere['absorb']['b'] = random.random()
        cfg['objects'].append(sphere)
        '''

def scene2():
    cfg['lights'].append(_point_light)
    for i in range(0,3):
        for j in range(-1,2):
            for k in range(-1,2):
                sphere = copy.deepcopy(_sphere)
                sphere['name'] = 'ball_%d_%d_%d'%(i,j,k)
                sphere['radius'] = 1
                sphere['center']['x'] = 2*i + 1.5
                sphere['center']['y'] = 2*j*sin(15)+2*k*cos(15)
                sphere['center']['z'] = 2*k*sin(15)-2*j*cos(15)+22
                sphere['absorb']['r'] = random.random()
                sphere['absorb']['g'] = random.random()
                sphere['absorb']['b'] = random.random()
                if (i == 1 and j==0 and k ==0):
                    sphere['absorb'] = getColor(0,0,0)
                cfg['objects'].append(sphere)
    cfg['objects'].append(_marble_ball)
    cfg['objects'].append(_floor)

scene2()

with open('config.json','w') as fout:
    json.dump(cfg,fout,indent=1)
print(json.dumps(cfg))

