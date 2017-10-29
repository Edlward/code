import torch
import torch.nn as nn
import torch.nn.functional as F

# dropout example

class convNet(nn.Module):
    def __init__(self):
        super(convNet, self).__init__()
        self.layer1 = nn.Sequential(
            nn.BatchNorm2d(1),
            nn.ReLU(),
            nn.Conv2d(1, 32, kernel_size=3, stride=1, padding=1)
        )
        self.layer2 = nn.Sequential(
            nn.BatchNorm2d(32),
            nn.ReLU(),
            nn.Conv2d(32, 128, kernel_size=3, stride=1, padding=1),
            nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        )
        self.layer3 = nn.Sequential(
            nn.BatchNorm2d(128),
            nn.ReLU(),
            nn.Conv2d(128, 64, kernel_size=3, stride=1, padding=1),
            # nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        )
        self.layer4 = nn.Sequential(
            nn.BatchNorm2d(64),
            nn.ReLU(),
            nn.Conv2d(64, 32, kernel_size=3, stride=1, padding=1),
            nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        )
        self.layer5 = nn.Sequential(
            nn.BatchNorm2d(32),
            nn.ReLU(),
            nn.Conv2d(32, 32, kernel_size=3, stride=1, padding=1),
            nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        )
        # self.conv2_drop = nn.Dropout2d(p=0.5)
        self.fc1 = nn.Linear(32*4*4, 128)
        self.fc2 = nn.Linear(128, 10)

    def forward(self, x):
        out = self.layer1(x)
        out = self.layer2(out)
        out = self.layer3(out)
        out = self.layer4(out)
        out = self.layer5(out)
        # out = self.conv2_drop(out)
        # print('cnn:', out.size())
        out = out.view(out.size(0), -1)
        # print('fc:', out.size())
        out = F.relu(self.fc1(out))
        #  x = F.dropout(x, training=self.training)
        out = self.fc2(out)
        return out




if __name__ == '__main__':
    from torch.autograd import Variable

    model = convNet()
    data = Variable(torch.randn(1,1,28,28))
    result = model(data)
    print('data: ', data.size())
    print('result: ', result.size())