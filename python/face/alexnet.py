import torch
from torch.autograd import Variable
import torch.nn as nn
import torch.nn.functional as F

class alexNet(nn.Module):
    def __init__(self):
        super(alexNet, self).__init__()
        self.layer1 = nn.Sequential(
            nn.Conv2d(3, 64, kernel_size = 7, stride = 3, padding = 0), # 应该让padding=0就没有了预测框的白色边界
            nn.BatchNorm2d(64),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size = 3, stride = 2))
        self.layer2 = nn.Sequential(
            nn.Conv2d(64, 192, kernel_size = 5, stride = 1, padding = 0),
            nn.BatchNorm2d(192),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size = 3, stride = 2))
        self.layer3 = nn.Sequential(
            nn.Conv2d(192, 384, kernel_size = 3, stride = 1, padding = 0),
            nn.BatchNorm2d(384),
            nn.ReLU())
        self.layer4 = nn.Sequential(
            nn.Conv2d(384, 192, kernel_size = 3, stride = 1, padding = 0),
            nn.BatchNorm2d(192),
            nn.ReLU())
        self.layer5 = nn.Sequential(
            nn.Conv2d(192, 64, kernel_size = 3, stride = 1, padding = 0),
            nn.BatchNorm2d(64),
            nn.ReLU())
        self.layer6 = nn.Sequential(
            nn.Conv2d(64, 1, kernel_size = 3, stride = 1, padding = 0),
            nn.BatchNorm2d(1),
            nn.ReLU())
            # nn.MaxPool2d(kernel_size = 3, stride = 2))
            
    
    def forward(self, x):
        out = self.layer1(x)
        out = self.layer2(out)
        out = self.layer3(out)
        out = self.layer4(out)
        out = self.layer5(out)
        out = self.layer6(out)
        # out = out.view(out.size(0), -1)
        # print(out.size())
        return out

if __name__ == '__main__':
    learning_rate = 0.0001

    model = alexNet()
    # model.cuda()

    criterion = nn.CrossEntropyLoss()
    optimizer = torch.optim.SGD(model.parameters(), lr = learning_rate)
    
    data = Variable(torch.randn(1,3,512,512))
    result = model(data)
    print('data', data.size())
    print('result', result.size())
    
    # for epoch in range(num_epochs):
    