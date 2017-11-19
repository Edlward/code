
import math
import random
import cv2
import numpy as np
class RandomHorizonFlip(object):
    def __call__(self, image):
        if random.random()<0.5:
            hflip = image[:,::-1,:]
            return hflip
        return image
