import torch
import torch.nn as nn
import torch.nn.functional as F
# import torch.nn.init as init

from torch.autograd import Variable
from simMultiboxLayer import MultiboxLayer

import logging 
# logging.basicConfig(level=logging.WARNING)

class SIMSSD(nn.Module):
    
    def __init__(self):
        super(SIMSSD, self).__init__()

        # model
        self.conv1 = nn.Conv2d(3, 32, kernel_size=3, padding=0, stride=2)
        self.conv2 = nn.Conv2d(32, 32, kernel_size=3, padding=0, stride=2)
        self.multibox = MultiboxLayer()
    
    def forward(self, x):
        outs = []

        out = F.relu(self.conv1(x))
        out = F.max_pool2d(out, kernel_size=3, padding=0, stride=2)
        out = F.relu(self.conv2(out))
        # print(out.size())
        outs.append(out)
        
        loc_preds, conf_preds = self.multibox(outs)
        return loc_preds, conf_preds


if __name__ == '__main__':
    model = SIMSSD()
    data = Variable(torch.randn(1,3,300,300)) # 1,32,36,36
    resutl = model(data)
