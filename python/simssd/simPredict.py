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

# img = Image.open('img.jpg')
# img = Image.open('videoImage/walmat_0_206.jpg')
# img = Image.open('/home/lxg/codedata/headXml/srcImage/livet1/videoImage/livet_1_180.jpg')


# img1 = img.resize((300,300))
# transform = transforms.Compose([transforms.ToTensor()])
# img1 = transform(img1)

# loc, conf = net(Variable(img1[None,:,:,:], volatile=True))
# print('loc', type(loc), loc.size())
# print('conf', type(conf), conf.size())

# loc = loc.cpu()
# conf = conf.cpu()
# # loc.type_as(torch.Tensor())
# # conf.type_as(torch.Tensor())
# # conf =  F.softmax(conf.squeeze(0)).data
# # loc = loc.data.squeeze(0)

# data_encoder = DataEncoder()
# boxes, labels, scores = data_encoder.decode(loc.data.squeeze(0), F.softmax(conf.squeeze(0)).data)

# draw = ImageDraw.Draw(img)
# for box in boxes:
#     box[::2] *= img.width
#     box[1::2] *= img.height
#     draw.rectangle(list(box), outline='red')
# img.save('predict.jpg', 'jpeg')
# img.show()

# sample  predict
list_file = '/home/lxg/codedata/headXml/srcImage/data_label.txt'

with open(list_file) as f: 
    lines = f.readlines()

transform = transforms.Compose([transforms.ToTensor()])
index = 0
data_encoder = DataEncoder()

f_predict = open('/home/lxg/codedata/headXml/srcImage/test_predict.txt', 'w')
f_label = open('/home/lxg/codedata/headXml/srcImage/test_predict_label.txt', 'w')
f_imlist = open('/home/lxg/codedata/headXml/srcImage/test_list.txt', 'w')

for line in lines:
    splited = line.strip().split()
    fname = splited[0]
    img = Image.open(os.path.join('/home/lxg/codedata/headXml/srcImage/', fname))
    img1 = img.resize((300,300))
    img1 = transform(img1)

    f_imlist.write(fname)
    f_imlist.write('\n')
    
    f_predict.write(fname)
    f_predict.write('\n')

    f_label.write(fname)
    f_label.write('\n')
    f_label.write(splited[1])
    f_label.write('\n')
    for i in range(int(splited[1])):
        # eclipse
        x = int(splited[4*i+2])
        y = int(splited[4*i+3])
        w = int(splited[4*i+4]) / 2
        h = int(splited[4*i+5]) / 2
        f_label.write(str(w))     ; f_label.write('\t') 
        f_label.write(str(h))     ; f_label.write('\t') 
        f_label.write(str(0))     ; f_label.write('\t')
        f_label.write(str(x+w/2)) ; f_label.write('\t') 
        f_label.write(str(y+h/2)) ; f_label.write('\t') 
        f_label.write('1')        ; f_label.write('\n')
        # rectangle 
        # f_label.write(splited[4*i+2])
        # f_label.write('\t')
        # f_label.write(splited[4*i+3])
        # f_label.write('\t')
        # f_label.write(splited[4*i+4])
        # f_label.write('\t')
        # f_label.write(splited[4*i+5])
        # f_label.write('\n')
    loc, conf = net(Variable(img1[None,:,:,:], volatile=True))
    loc = loc.cpu()
    conf = conf.cpu()
    boxes, labels, scores, have_box = data_encoder.decode(loc.data.squeeze(0), F.softmax(conf.squeeze(0)).data)
    f_predict.write(str(len(boxes)))
    f_predict.write('\n')

    draw = ImageDraw.Draw(img)
    for i in range(len(boxes)):
        box = boxes[i]
        box[::2] *= img.width
        box[1::2] *= img.height
        draw.rectangle(list(box), outline='red')
        f_predict.write(str(box[0]))
        f_predict.write('\t')
        f_predict.write(str(box[1]))
        f_predict.write('\t')
        f_predict.write(str(box[2] - box[0]))
        f_predict.write('\t')
        f_predict.write(str(box[3] - box[1]))
        f_predict.write('\t')
        f_predict.write(str(scores[i]))
        f_predict.write('\t')
        f_predict.write('1')        
        f_predict.write('\n')
        
    # img.show()
    img.save(os.path.join('/home/lxg/codedata/headXml/srcImage/predict/','predic_'+str(index)+'.jpg'), 'jpeg')
    index = index+1
    print('predict', index)
