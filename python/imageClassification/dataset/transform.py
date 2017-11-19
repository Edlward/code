from common import *

## for debug
def dummy_transform(img,text='dummy_transform'):
    print ('\t\t%s',text)
    return img

## custom data transform  -----------------------------------
## https://github.com/pytorch/vision/blob/master/test/preprocess-bench.py
## http://pytorch-zh.readthedocs.io/en/latest/torchvision/models.html
##     All pre-trained models expect input images normalized in the same way,
##     i.e. mini-batches of 3-channel RGB images of shape (3 x H x W),
##     where H and W are expected to be atleast 224. The images have to be
##     loaded in to a range of [0, 1] and then normalized using
##     mean = [0.485, 0.456, 0.406] and std = [0.229, 0.224, 0.225]

def pytorch_image_to_tensor_transform(image):

    mean = [0.485, 0.456, 0.406 ]
    std  = [0.229, 0.224, 0.225 ]

    image = cv2.cvtColor(image,cv2.COLOR_BGR2RGB)
    image = image.transpose((2,0,1))
    tensor = torch.from_numpy(image).float().div(255)

    tensor[0] = (tensor[0] - mean[0]) / std[0]
    tensor[1] = (tensor[1] - mean[1]) / std[1]
    tensor[2] = (tensor[2] - mean[2]) / std[2]

    return tensor

def pytorch_tensor_to_image(tensor):
    mean = [0.485, 0.456, 0.406 ]
    std  = [0.229, 0.224, 0.225 ]

    tensor[0] = tensor[0]*std[0] + mean[0]
    tensor[1] = tensor[1]*std[1] + mean[1]
    tensor[2] = tensor[2]*std[2] + mean[2]


    image = tensor.numpy()*255
    image = np.transpose(image, (1, 2, 0))
    image = image.astype(np.uint8)
    image = cv2.cvtColor(image , cv2.COLOR_BGR2RGB)
    return image





#--------------------------------------------
def fix_crop(image, roi=[0,0,256,256]):
    x0,y0,x1,y1 = roi
    image = image[y0:y1,x0:x1,:]
    return image

def fix_resize(image, w, h):
    image = cv2.resize(image,(w,h))
    return image

def random_horizontal_flip(image, u=0.5):
    if random.random() < u:
        image = cv2.flip(image,1)  #np.fliplr(img) ##left-right
    return image



def random_shift_scale_rotate(image, shift_limit=[-0.0625,0.0625], scale_limit=[1/1.2,1.2],
                               rotate_limit=[-15,15], aspect_limit = [1,1],  size=[-1,-1], borderMode=cv2.BORDER_REFLECT_101 , u=0.5):
    #cv2.BORDER_REFLECT_101  cv2.BORDER_CONSTANT

    if random.random() < u:
        height,width,channel = image.shape
        if size[0]==-1: size[0]=width
        if size[1]==-1: size[1]=height

        angle  = random.uniform(rotate_limit[0],rotate_limit[1])  #degree
        scale  = random.uniform(scale_limit[0],scale_limit[1])
        aspect = random.uniform(aspect_limit[0],aspect_limit[1])
        sx    = scale*aspect/(aspect**0.5)
        sy    = scale       /(aspect**0.5)
        dx    = round(random.uniform(shift_limit[0],shift_limit[1])*width )
        dy    = round(random.uniform(shift_limit[0],shift_limit[1])*height)

        cc = math.cos(angle/180*math.pi)*(sx)
        ss = math.sin(angle/180*math.pi)*(sy)
        rotate_matrix = np.array([ [cc,-ss], [ss,cc] ])

        box0 = np.array([ [0,0], [width,0],  [width,height], [0,height], ])
        box1 = box0 - np.array([width/2,height/2])
        box1 = np.dot(box1,rotate_matrix.T) + np.array([width/2+dx,height/2+dy])

        box0 = box0.astype(np.float32)
        box1 = box1.astype(np.float32)
        mat = cv2.getPerspectiveTransform(box0,box1)

        image = cv2.warpPerspective(image, mat, (size[0],size[1]),flags=cv2.INTER_LINEAR,borderMode=borderMode,borderValue=(0,0,0,))  #cv2.BORDER_CONSTANT, borderValue = (0, 0, 0))  #cv2.BORDER_REFLECT_101

    return image





