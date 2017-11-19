import torch.nn as nn
import torch.nn.functional as F
import math

def conv3x3(in_planes, out_planes, stride=1):
    ''' no bias'''
    return nn.Conv2d(in_planes, out_planes, kernel_size=3, stride=stride, 
                    padding=1, bias=False)

class BasicBlock(nn.Module):
    '''
    ResNet basic block, the structure is 2 convolution layers
    '''
    expansion = 1     # expansion in dimension
    def __init__(self, in_planes, out_planes, stride=1, downsample=None):
        super(BasicBlock, self).__init__()
        self.conv1 = conv3x3(in_planes, out_planes, stride)
        self.bn1 = nn.BatchNorm2d(out_planes)
        self.relu = nn.ReLU(inplace=True)
        self.conv2 = conv3x3(out_planes, out_planes)
        self.bn2 = nn.BatchNorm2d(out_planes)
        self.downsample = downsample
        self.stride = stride

    def forward(self, x):
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


class SubClassNet(nn.Module):
    '''
    Used for classify the different class in level1
    '''
    def __init__(self, in_planes, out_classes, block=BasicBlock):
        self.in_planes = in_planes
        super(SubClassNet, self).__init__()
        self.layer1 = self._make_layer(block, 256, 2, stride=2) # 23
        self.layer2 = self._make_layer(block, 512, 2, stride=2)
        # self.layer3 = self._make_layer(block, 512, 2, stride=2)  # 6
        self.avgpool = nn.AvgPool2d(6, stride=1)  # global average pool
        self.fc = nn.Linear(512*block.expansion, out_classes) # level1 class num

    def _make_layer(self, block, out_planes, block_num, stride=1):
        downsample = None
        if stride != 1 or self.in_planes != out_planes * block.expansion:
            downsample = nn.Sequential(
                nn.Conv2d(self.in_planes, out_planes*block.expansion, 
                            kernel_size=1, stride=stride, bias=False),
                nn.BatchNorm2d(out_planes*block.expansion),
            )
        layers = []
        layers.append(block(self.in_planes, out_planes, stride, downsample))
        self.in_planes = out_planes * block.expansion
        for i in range(1, block_num):
            layers.append(block(self.in_planes, out_planes))
        
        return nn.Sequential(*layers)
    
    def forward(self, x):
        out = self.layer1(x)
        out = self.layer2(out)
        # out = self.layer3(out)
        # print('subclassnet out:', out.size())
        out = self.avgpool(out)
        out = out.view(out.size(0), -1)
        out = self.fc(out)
        return out


class MultiSubNet(nn.Module):
    out_classes = [1,   9, 158,   1, 237,  48, 440,  57,  88,  50, 441,  33,   6,
                42, 206,  81,  16, 196,  83, 101,   1, 184, 137,  84, 230, 230,
                74, 162,  26, 220,  19,  45, 104,  33, 106,  29,  99, 103,  43,
                 2,   1, 180,  38, 555,  25,  85,  42,  89,  30]
    def __init__(self, in_planes, class_net=SubClassNet):
        super(MultiSubNet, self).__init__()
        self.conf_layers = nn.ModuleList()
        for i in range(len(self.out_classes)):
            self.conf_layers.append(class_net(in_planes, self.out_classes[i]))

    def forward(self, x, label):
        # print('multisubnet x:', x.size(), 'label:', label)
        outs = []
        label_data = label.data.int()
        for i in range(len(label)):  # batch circle not subnet circle
            # print('range:',i)
            out = self.conf_layers[label_data[i]](x[None,i,:,:,:])
            outs.append(out)
        # outs = torch.cat(outs, 1)
        return outs


class ResNet(nn.Module):
    def __init__(self, block):
        self.in_planes = 64
        super(ResNet, self).__init__()
        self.conv1 = nn.Conv2d(3, 64, kernel_size=7, stride=2, padding=3, bias=False)
        self.bn1 = nn.BatchNorm2d(64)
        self.relu = nn.ReLU(inplace=True)
        self.maxpool = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)

        self.layer1 = self._make_layer(block, 128, 2)
        self.layer2 = self._make_layer(block, 128, 2, stride=2)

        self.level1_layer1 = self._make_layer(block, 128, 2, stride=2)
        self.level1_layer2 = self._make_layer(block, 256, 2, stride=2)
        self.level1_layer3 = self._make_layer(block, 512, 2, stride=2)
        # self.level1_layer4 = self._make_layer(block, 512, 2, stride=2)
        self.avgpool = nn.AvgPool2d(3, stride=1)  # global average pool
        self.fc = nn.Linear(512*block.expansion, 49) # level1 class num

        self.multisubnet = MultiSubNet(128)

        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                n = m.kernel_size[0] * m.kernel_size[1] * m.out_channels # jus a number, not assosiated to conv2d parameter size
                m.weight.data.normal_(0, math.sqrt(2. / n))
            elif isinstance(m, nn.BatchNorm2d):
                m.weight.data.fill_(1)
                m.bias.data.zero_()

    def _make_layer(self, block, out_planes, block_num, stride=1):
        downsample = None
        if stride != 1 or self.in_planes != out_planes * block.expansion:
            downsample = nn.Sequential(
                nn.Conv2d(self.in_planes, out_planes*block.expansion, 
                            kernel_size=1, stride=stride, bias=False),
                nn.BatchNorm2d(out_planes*block.expansion),
            )
        layers = []
        layers.append(block(self.in_planes, out_planes, stride, downsample))
        self.in_planes = out_planes * block.expansion
        for i in range(1, block_num):
            layers.append(block(self.in_planes, out_planes))
        
        return nn.Sequential(*layers)
    
    def forward(self, x, level1_label):
        out = self.conv1(x)
        out = self.bn1(out)
        out = self.relu(out)
        out = self.maxpool(out)

        out = self.layer1(out)
        out = self.layer2(out)

        # print('base net output size:', out.size())
        out_level1 = self.level1_layer1(out)
        out_level1 = self.level1_layer2(out_level1)
        out_level1 = self.level1_layer3(out_level1)
        # out_level1 = self.level1_layer4(out_level1)
        # print('conv net output size:', out_level1.size())

        # level1 class
        out_level1 = self.avgpool(out_level1)
        out_level1 = out_level1.view(out_level1.size(0), -1)
        out_level1 = self.fc(out_level1)
        # print('level1 class output size:', out_level1.size())

        # final class
        out_classes = self.multisubnet(out, level1_label)
        # print('final class size:', out_classes.size()) # list has no size

        if(self.training):
            pass
        else:
            pass


        return out_level1, out_classes




def resNetTest():
    model = ResNet(BasicBlock)
    return model

if __name__ == '__main__':
    import torch
    from torch.autograd import Variable
    
    model = resNetTest()
    data = Variable(torch.randn(4,3,180,180))
    label = Variable(torch.LongTensor([0,1,2,0]))
    level1, out_classes = model(data, label)
    print('data:', data.size())
    print('output_level1:', level1.size())
    # print('output_classes:', out_classes.size(), out_classes) # list doesn't has size
    print('output_classes:', out_classes) 
    
    



