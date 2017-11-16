# coding=-utf8
import math
import torch.nn as nn
import torch.nn.functional as F

def conv3x3(in_planes, out_planes, stride=1):
    '''   no bias    '''
    return nn.Conv2d(in_planes, out_planes, kernel_size=3, stride=stride,
                padding=1, bias=False)

class BasicBlock(nn.Module):
    expansion = 1  # 表示block内，卷积维度没有变化???

    def __init__(self, in_planes, out_planes, stride=1, downsample=None):
        super(BasicBlock, self).__init__()
        self.conv1 = conv3x3(in_planes, out_planes, stride) # 第一层卷积stride可以为2
        self.bn1 = nn.BatchNorm2d(out_planes)
        self.relu = nn.ReLU(inplace=True)  # 使用inplace可以使这一层网络重复调用，还是因为这样比较省计算量
        self.conv2 = conv3x3(out_planes, out_planes)  # 第二层卷积的stride默认为1
        self.bn2 = nn.BatchNorm2d(out_planes)
        self.downsample = downsample
        self.stride = stride

    def forward(self, x):
        ''' 流水线完全按照论文里的block来的'''
        residual = x

        out = self.conv1(x)
        out = self.bn1(out)
        out = self.relu(out)

        out = self.conv2(out)
        out = self.bn2(out)

        if self.downsample is not None:
            residual = self.downsample(x)
        
        out += residual
        out = self.relu(out)

        return out

class ResNet(nn.Module):
    def __init__(self, block, layers, num_classes=10):
        self.in_planes = 32
        super(ResNet, self).__init__()
        
        self.conv1 = nn.Conv2d(1, 32, kernel_size=3, stride=1, padding=1, bias=False)
        self.bn1 = nn.BatchNorm2d(32)
        self.relu1 = nn.ReLU(inplace=True) 
        
        self.maxpool = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        self.layer1 = self._make_layer(block, 64, layers[0], stride=1)
        self.layer2 = self._make_layer(block, 128, layers[1], stride=2)
        self.layer3 = self._make_layer(block, 256, layers[2], stride=2)
        self.layer4 = self._make_layer(block, 512, layers[3], stride=2)
        self.avgpool = nn.AvgPool2d(2, stride=1)  # global average pool
        self.fc = nn.Linear(512*block.expansion, num_classes)

        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                n = m.kernel_size[0] * m.kernel_size[1] * m.out_channels
                m.weight.data.normal_(0, math.sqrt(2. /n))
            elif isinstance(m, nn.BatchNorm2d):
                m.weight.data.fill_(1)
                m.bias.data.zero_()


    def _make_layer(self, block, out_planes, blocks, stride=1):
        downsample=None
        if stride != 1 or self.in_planes != out_planes * block.expansion:
            downsample = nn.Sequential(
                nn.Conv2d(self.in_planes, out_planes * block.expansion, 
                        kernel_size=1, stride=stride, bias=False),
                nn.BatchNorm2d(out_planes * block.expansion),
            )
        layers = []
        layers.append(block(self.in_planes, out_planes, stride, downsample))  # 用于调整维度、特征图尺寸
        self.in_planes = out_planes * block.expansion
        for i in range(1, blocks):
            layers.append(block(self.in_planes, out_planes))
        
        return nn.Sequential(*layers)

    def forward(self, x):
        out = self.conv1(x)
        out = self.bn1(out)
        out = self.relu1(out)
        out = self.maxpool(out)
        
        out = self.layer1(out)
        out = self.layer2(out)
        out = self.layer3(out)
        out = self.layer4(out)

        # print("out:", out.size())
        out = self.avgpool(out)
        out = out.view(out.size(0), -1)
        out = self.fc(out)
        
        return out
    
def resnet18():
    model = ResNet(BasicBlock, [2,2,2,2])  # 2*(2+2+2+2)+2
    return model

if __name__ == '__main__':
    import torch
    from torch.autograd import Variable

    model = resnet18()
    data = Variable(torch.randn(1,1,28,28))
    result = model(data)
    print("data:", data.size())
    print("result:", result.size())
