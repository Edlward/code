import torch
import torch.nn as nn
import torch.nn.functional as F

class fcNet(nn.Module):
    def __init__(self):
        super(fcNet, self).__init__()
        self.fc1 = nn.Linear(784, 800)
        self.fc2 = nn.Linear(800, 800)
        self.fc3 = nn.Linear(800, 10)
    
    def forward(self, x):
        out = x.view(x.size(0), -1)
        out = F.relu(self.fc1(out))
        # out = F.dropout(out, training=self.training)
        out = F.relu(self.fc2(out))
        # out = F.dropout(out, training=self.training)
        out = self.fc3(out)
        return out