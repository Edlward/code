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
    torch.cuda.set_device(1)
    net = torch.nn.DataParallel(net, device_ids=[1])
    # net.cuda()

net.load_state_dict(torch.load('checkpoint.pth'))
net.eval()

# img = Image.open('img.jpg')
img = Image.open('videoImage/walmat_0_206.jpg')

img1 = img.resize((300,300))
transform = transforms.Compose([transforms.ToTensor()])
img1 = transform(img1)

loc, conf = net(Variable(img1[None,:,:,:], volatile=True))
print(type(loc), loc.size())
print(type(conf), conf.size())

loc = loc.cpu()
conf = conf.cpu()
# loc.type_as(torch.Tensor())
# conf.type_as(torch.Tensor())
# conf =  F.softmax(conf.squeeze(0)).data
# loc = loc.data.squeeze(0)

data_encoder = DataEncoder()
boxes, labels, scores = data_encoder.decode(loc.data.squeeze(0), F.softmax(conf.squeeze(0)).data)

draw = ImageDraw.Draw(img)
for box in boxes:
    box[::2] *= img.width
    box[1::2] *= img.height
    draw.rectangle(list(box), outline='red')
img.save('predict.jpg', 'jpeg')