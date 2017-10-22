import os
import os.path
import random
import matplotlib.pyplot as plt
import numpy as np
import cv2

from PIL import Image, ImageDraw

def readList(file):
    ''' read file list and return
    Args:
        file: image file path
    Returns:
        fnames: image name
        boxes: label boxes
    '''
    fnames = []
    boxes = []
    with open(file) as f:
        lines = f.readlines()
    for line in lines:
        splited = line.strip().split()
        fnames.append(splited[0])
        num_objs = int(splited[1])
        box = []
        for i in range(num_objs):
            xmin = splited[2+4*i]
            ymin = splited[3+4*i]
            width = splited[4+4*i]
            height = splited[5+4*i]
            box.append([int(xmin), int(ymin), int(width)+int(xmin), int(height)+int(ymin)])
        boxes.append(box)
    return fnames, boxes            

def computeExpect(root_path, names):
    print('file number', len(names))
    pixel_r = 0.
    pixel_g = 0.
    pixel_b = 0.
    for name in names:
        img = cv2.imread(os.path.join(root_path, name))
        img = img.astype(float) / 255
        pixel_r += img[0].mean()
        pixel_g += img[1].mean()
        pixel_b += img[2].mean()
    pixel_r /= len(names)
    pixel_g /= len(names)
    pixel_b /= len(names)
    # return [int(pixel_r*255), int(pixel_g*255), int(pixel_b*255)]  
    return [pixel_r, pixel_g, pixel_b]  

# [0.40391271702479492, 0.40460396096423101, 0.40580669011790821]
# [102, 103, 103]
root_path = "/home/lxg/codedata/headXml/srcImage/"
names, boxes = readList(root_path + 'data_label.txt')
color_expect = computeExpect(root_path, names)
print(color_expect)