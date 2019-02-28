#coding=utf-8
from numpy import *
import numpy as np
import cv2

def read_y(filename, dim):
    fp = open(filename,'rb')
    Yt = np.zeros((dim[0],dim[1]),uint8,'C')
    for m in range(dim[0]):
        for n in range(dim[1]):
            Yt[m,n]=ord(fp.read(1))
    fp.close()
    return Yt

class ShowY(object):
    def __init__(self, dim=(3,3), win=(300, 400)):
        self.win = win
        self.dim = dim
        self.space = 2
        self.img = np.zeros((self.dim[0]*(self.win[0]+self.space),self.dim[1]*(self.win[1]+self.space)),uint8,'C')

    def show(self, n, filename, dim=(0,0)):
        img = read_y(filename, dim)
        r = n / self.dim[0]
        c = n % self.dim[0]
        if r >= self.dim[0] or c >= self.dim[1] :
            print ("error: (%d,%d)" %r %c)
        sr = r * (self.win[0] + self.space)
        sc = c * (self.win[1] + self.space)
        er = sr + self.win[0]
        ec = sc + self.win[1]
        self.img[sr:er,sc:ec] = img
        cv2.imshow("picture", self.img)

    def clearall(self):
        self.img = np.zeros((self.dim[0]*(self.win[0]+self.space),self.dim[1]*(self.win[1]+self.space)),uint8,'C')
        cv2.imshow("picture", self.img)

if __name__ == '__main__':
    yshow = ShowY();
    yshow.show(0, '1.yuv', (300, 400))
    yshow.show(1, '1.yuv', (300, 400))
    yshow.show(2, '1.yuv', (300, 400))
    yshow.show(3, '1.yuv', (300, 400))
    yshow.show(4, '1.yuv', (300, 400))
    yshow.show(5, '1.yuv', (300, 400))
    yshow.show(6, '1.yuv', (300, 400))
    yshow.show(7, '1.yuv', (300, 400))
    yshow.show(8, '1.yuv', (300, 400))
    while (1):
        if cv2.waitKey(20) & 0xFF == ord('q'):
            break

