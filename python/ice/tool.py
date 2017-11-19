#codeing=-utf8
import random
import cv2
from scipy.ndimage.filters import uniform_filter
from scipy.ndimage.measurements import variance

def randomCrop(img, w, h):
    '''
    img: WxHxC
    '''
    (img_h, img_w) = img.shape[:2]
    left = random.randrange(0, img_w-w)
    top = random.randrange(0, img_h-h)
    return img[top:top+w, left:left+h]

def rotate(img, angle, scale=1.0):
    '''
    img: object to rotate
    angle: degree not radian to ratate clockwise
    scale: Isotropic scale factor

    Args:
    BORDER_REFLECT: border is reflected (copied) http://answers.opencv.org/question/50706/border_reflect-vs-border_reflect_101/
    BORDER_REFLECT101: same as BORDER_REFLECT, but the outer side pixel is not copied
    '''
    (h,w) = img.shape[:2]
    rotated = cv2.getRotationMatrix2D((w/2,h/2), angle, scale)
    img = cv2.warpAffine(img, rotated, (w,h), borderMode=cv2.BORDER_REFLECT_101)
    return img

def lee_filter(img):
    # speckle filter https://stackoverflow.com/questions/39785970/speckle-lee-filter-in-python
    w,h,c = img.shape
    img_mean = uniform_filter(img, (w, w, 1))
    img_sqr_mean = uniform_filter(img**2, (w, w, 1))
    img_variance = img_sqr_mean - img_mean**2
    overall_variance = variance(img)
    img_weights = img_variance**2 / (img_variance**2 + overall_variance**2)
    img_output = img_mean + img_weights * (img - img_mean)
    return img_output
    