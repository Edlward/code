import cv2
import os
import sys
import os.path
import torch
import torchvision
import torchvision.transforms as transforms
import torch.nn.functional as F
from torch.autograd import Variable
from PIL import Image, ImageDraw

from simssd import SIMSSD
from simEncoder import DataEncoder

use_cuda = True

net = SIMSSD()
if use_cuda:
    torch.cuda.set_device(0)
    net = torch.nn.DataParallel(net, device_ids=[0])
    net.cuda()

net.load_state_dict(torch.load('checkpoint.pth'))
net.eval()
transform = transforms.Compose([transforms.ToTensor()])
data_encoder = DataEncoder()
img_size = 300

def predict(path):
    cap = cv2.VideoCapture(path)
    while True:
        success, im_src = cap.read()
        if not success:
            break
        im_src = cv2.resize(im_src, (img_size, img_size))

        im_tensor = transform(im_src)
        loc, conf = net(Variable(im_tensor[None,:,:,:], volatile=True))
        loc = loc.cpu()
        conf = conf.cpu()
        boxes, labels, scores, have_box = data_encoder.decode(loc.data.squeeze(0), F.softmax(conf.squeeze(0)).data)
        if have_box:
            print(boxes)
            for box in boxes:
                x1 = int(box[0] * img_size)
                y1 = int(box[1] * img_size)
                x2 = int(box[2] * img_size)
                y2 = int(box[3] * img_size)
                
                cv2.rectangle(im_src, (x1, y1), (x2, y2), (0,0,255))
                print(type(box), box)

        cv2.imshow('testVideo', im_src)
        if cv2.waitKey(60) == 27:
            break

# predict('/home/lxg/output90.avi')        
# predict('/home/lxg/zqq2017090804.avi')        
# predict('/home/lxg/zqq2017090803.avi')   
predict('/home/lxg/zqq2017090802.avi')       
# predict('/home/lxg/zqq2017090801.avi')     
# predict('/home/lxg/output201709.avi')        
   
     




    