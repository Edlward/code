# encoding:utf-8
# 01为 pos
# 10为 neg

import cv2
import torch
import train
import numpy as np
from torch.autograd import Variable
import time

model = train.testNet()
model.load_state_dict(torch.load('head.pkl'))

# im = cv2.imread('/home/lxg/codedata/headDataset/pos_20X20/head_1606.jpg')
# im = cv2.imread('/home/lxg/codedata/headDataset/negdata/013134.jpg')
im = cv2.imread('/home/lxg/codedata/headXml/headPos/1673.jpg') 
im = cv2.imread('/home/lxg/codedata/headXml/headNeg/210.jpg') 

im = cv2.resize(im, (20,20))

im = im.astype(float)
im = im.transpose(2,0,1) / 255
im = im[np.newaxis, 0:1, :, :]

im = Variable(torch.Tensor(im))


begin = time.time()
result = model(im)

end = time.time()
print(end-begin)

_, predicted = torch.max(result.data, 1)

print(result)
print(predicted)
