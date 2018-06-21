#coding=utf-8
# 二次贝塞尔曲线绘图程序
# 木野狐(Neil Chen) http://rchen.cnblogs.com
# 2007-5-16
# 根据 c 程序改写为 python 类的形式，参考：
# http://hi.baidu.com/sunyante/blog/item/2627d911b3061810b8127b70.html

from math import sqrt
import Image, ImageDraw

class Bezier2(object):
    def __init__(self, draw, points, line_width, line_color):
        self.draw = draw
        self.points = points
        self.line_width = line_width
        self.line_color = line_color
        self.current_point = (0, 0)

    def moveto(self, p):
        self.current_point = p
    
    def lineto(self, p):
        self.draw.line((self.current_point, p), width=self.line_width, fill=self.line_color)
        self.current_point = p

    def render(self):
        NO = 3
        KT = 5
        m = NO - 1
        p = {} # p[3][2]
        for i in range(0, NO, 2):
            p[i] = self.points[i]
        
        l1 = 1.0 * (self.points[0][0] - self.points[1][0])
        ll = 1.0 * (self.points[0][1] - self.points[1][1])
        l1 = sqrt(l1 * l1 + ll * ll)
        
        l2 = 1.0 * (self.points[2][0] - self.points[1][0])
        ll = 1.0 * (self.points[2][1] - self.points[1][1])
        l2 = sqrt(l2 * l2 + ll * ll)
        
        p[1] = (
            ((l1 + l2) * (l1 + l2) * self.points[1][0] - l2 * l2 * self.points[0][0] - l1 * l1 * self.points[2][0]) / (2 * l1 * l2),
            ((l1 + l2) * (l1 + l2) * self.points[1][1] - l2 * l2 * self.points[0][1] - l1 * l1 * self.points[2][1]) / (2 * l1 * l2)
        )
        '''
        # 画出切线
        self.moveto(p[0])
        for i in range(1, m+1):
            self.lineto(p[i])
        '''
        
        pk = {}  # pk[129][2]
        for i in range(m + 1):
            pk[i] = p[i]
        
        pt = {} # pt[129][2]
        for k in range(KT + 1):
            for i in range(0, m + 1, 2):
                pt[2*i] = pk[i]
            for i in range(m):
                pt[2*i + 1] = (
                    int(pk[i][0] + pk[i+1][0]) >> 1,
                    int(pk[i][1] + pk[i+1][1]) >> 1
                )
            for i in range(1, m):
                pt[2*i] = (
                    int(pt[2*i-1][0] + pt[2*i+1][0]) >> 1,
                    int(pt[2*i-1][1] + pt[2*i+1][1]) >> 1
                )
            for i in range(2*m + 1):
                pk[i] = pt[i]
            
            if k == KT:
                break
            m <<= 1
        self.moveto(pk[0])
        for i in range(1, 2*m + 1):
            self.lineto(pk[i])

# 测试代码
if __name__ == '__main__':
    im = Image.new('RGBA', (1024,1024), (255,255,255))
    draw = ImageDraw.Draw(im)

    points = ((150,300), (300,200), (485,300))
    b = Bezier2(draw, points, 1, (0,0,255))
    b.render()
    
    points2 = ((150,300), (100,400), (485,300))
    b2 = Bezier2(draw, points2, 3, (0,255,0))
    b2.render()
    
    del draw
    im.show()
