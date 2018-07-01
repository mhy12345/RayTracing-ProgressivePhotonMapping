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
_bazier_curves = loadObject('bazier_curves')

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

simple_floor = copy.deepcopy(_floor)
simple_floor['texture'] = getColor(0,1,0)
back_light = copy.deepcopy(_area_light)
back_light['name'] = 'back_light'
back_light['position'] = getPosition(3,-3,40)
back_light['dx'] = getPosition(0,6,0)
back_light['dy'] = getPosition(2,0,-0.5)
no_refl_floor = copy.deepcopy(_floor)
no_refl_floor['material']['refl'] = 0
no_refl_floor['material']['diff'] = 1.0
marble_ball2 = copy.deepcopy(_marble_ball)
marble_ball2['texture']['filename'] = 'materials/marble2.jpg'
marble_ball2['center'] = getPosition(3,-6,29)

near_ball = copy.deepcopy(_marble_ball)
near_ball['center'] = getPosition(35,1,-40)
near_ball['radius'] = .2
near_ball['texture']['filename'] = 'materials/marble2.jpg'
near_ball['material']['refl'] = 0
near_ball['material']['refr'] = 0
near_ball['material']['diff'] = 1.0

near_ball_light = copy.deepcopy(_point_light)
near_ball_light['position'] = getPosition(37,.6,-45)
near_ball_light['brightness'] = 50

low_area_light = copy.deepcopy(_area_light)
low_area_light['position'] = getPosition(20,-2.5,30)
low_area_light['dx']['y'] = 5
low_area_light['dy']['z'] = -5


high_sphere = copy.deepcopy(_sphere)
high_sphere['center']['x'] = 4
high_sphere['radius'] = 2

def scene1():
    '''
    cfg['initial_r'] /= 15
    cfg['total_brightness'] *= 39
    '''
    cfg['lights'].append(_area_light)
    cfg['objects'].append(high_sphere)
    cfg['objects'].append(no_refl_floor)

def scene2():
    cfg['total_brightness'] *= 12.5
    cfg['lights'].append(_area_light)
    cfg['lights'].append(_point_light)
    cfg['lights'].append(copy.copy(_point_light))
    cfg['lights'][-1]['position']['x'] = 15
    #cfg['lights'].append(back_light)
    for i in range(0,3):
        for j in range(-1,2):
            for k in range(-1,2):
                sphere = copy.deepcopy(_sphere)
                sphere['name'] = 'ball_%d_%d_%d'%(i,j,k)
                sphere['radius'] = 1
                sphere['center']['x'] = 2*i + 2.5
                sphere['center']['y'] = 2*j*sin(15)+2*k*cos(15) -4
                sphere['center']['z'] = 2*k*sin(15)-2*j*cos(15) +24
                sphere['absorb']['r'] = random.random()
                sphere['absorb']['g'] = random.random()
                sphere['absorb']['b'] = random.random()
                sphere['texture'] = sphere['absorb']
                if (i == 1 and j==0 and k ==0):
                    sphere['absorb'] = getColor(0,0,0)
                cfg['objects'].append(sphere)
    cfg['objects'].append(_marble_ball)
    cfg['objects'].append(_floor)

def scene3():
    '''
    cfg['initial_r'] /= 15
    cfg['total_brightness'] *= 39
    '''
    cfg['total_brightness'] *= 65.5
    cfg['camera']['fdepth'] = .02
    _sphere['center']['z'] -= 5
    _sphere['center']['x'] += 1
    #cfg['lights'].append(_area_light)
    cfg['lights'].append(_point_light)
    cfg['lights'].append(near_ball_light)
    cfg['objects'].append(_marble_ball)
    cfg['objects'].append(marble_ball2)
    cfg['objects'].append(_sphere)
    cfg['objects'].append(near_ball)
    cfg['objects'].append(_bazier_curves)
    cfg['objects'].append(_floor)

'''
    ratio = .5
    for i in range(0,1):
        for j in range(-1,1):
            for k in range(-1,1):
                sphere = copy.deepcopy(_sphere)
                sphere['name'] = 'ball_%d_%d_%d'%(i,j,k)
                sphere['radius'] = ratio
                sphere['center']['x'] = ratio*2*i + 2.5
                sphere['center']['y'] = ratio*2*j*sin(15)+ratio*2*k*cos(15)
                sphere['center']['z'] = ratio*2*k*sin(15)-ratio*2*j*cos(15)+21
                sphere['absorb']['r'] = random.random()
                sphere['absorb']['g'] = random.random()
                sphere['absorb']['b'] = random.random()
                sphere['texture'] = sphere['absorb']
                if (i == 1 and j==0 and k ==0):
                    sphere['absorb'] = getColor(0,0,0)
                cfg['objects'].append(sphere)
                '''

scene3()

with open('config.json','w') as fout:
    json.dump(cfg,fout,indent=1)
print(json.dumps(cfg))

