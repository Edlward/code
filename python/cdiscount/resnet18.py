import torch.nn as nn
from resnetlib import resnet18

# input size 224*224*3
def resnet_18():
    model = resnet18(pretrained=True)
    model.fc = nn.Linear(512, 49)
    return model