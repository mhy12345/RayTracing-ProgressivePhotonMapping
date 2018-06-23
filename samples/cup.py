import cv2
import numpy as np

def bazier(pts,t):
    n = len(pts)
    return 1 * pts[0] * t**0 * (1-t)**3 + \
            3 * pts[1] * t**1 * (1-t)**2 + \
            3 * pts[2] * t**2 * (1-t)**1 + \
            1 * pts[3] * t**3 * (1-t)**0

ctrl_pts = np.array([
        ((-1,-.04),(0,-.04),(1.1,-.04)),
        ((1.2,0),(2,0),(2.2,0)),
        ((2,-.1),(1.9,-.15),(1.73,-.23)),
        ((.4,.2),(.2,-.4),(-.1,-1.3)),
        ((2.8,-1.4),(2.8,-2),(2.8,-2.2)),
        ((2.6,-2.2),(2.6,-2),(2.6,-1.9)),
        ((0.25,-1.3),(0,-1.3),(-.25,-1.3))
        ])

SIZE = 300
CENTER = SIZE // 2
canvas = np.ones((SIZE,SIZE,3),dtype="uint8")*255

print(ctrl_pts)
for i in range(len(ctrl_pts)-1):
    a = ctrl_pts[i][1:]
    b = ctrl_pts[i+1][:-1]
    pts = np.concatenate([a,b])
    res = []
    for t in np.linspace(0,1,100):
        res.append(bazier(pts,t))
    res = np.array(res)
    res = (res*50).astype(np.int)+CENTER
    for r in res:
        canvas[r[1],r[0],2] = 0
    print(res)

ctrl_pts = (ctrl_pts*50).astype(np.int)+CENTER
for start,pos,end in ctrl_pts:
    print("Start = ",start)
    cv2.line(canvas,tuple(start.tolist()),tuple(pos.tolist()),(0,0,0),1)
    cv2.line(canvas,tuple(end.tolist()),tuple(pos.tolist()),(0,0,0),1)
    cv2.circle(canvas,tuple(start.tolist()),2,(0,0,255))
    cv2.circle(canvas,tuple(pos.tolist()),2,(0,255,0))
    cv2.circle(canvas,tuple(end.tolist()),2,(255,0,0))

cv2.imshow("Canvas",canvas)
cv2.waitKey(0)
