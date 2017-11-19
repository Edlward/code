import torch.nn as nn
import torch.nn.functional as F

class MultiLoss(nn.Module):
    def __init__(self):
        super(MultiLoss, self).__init__()
        self.criterion_level1 = nn.CrossEntropyLoss()
        self.criterion_subclass = nn.CrossEntropyLoss()

    def forward(self, level1_label, level1_pred, subclass_label, subclass_pred):
        subclass_loss = 0.
        for i in range(len(subclass_label)):
            subclass_loss += self.criterion_subclass(subclass_pred[i], subclass_label[i])
        level1_loss = self.criterion_level1(level1_pred, level1_label)
        
        return level1_loss + subclass_loss

        # level1_loss = self.criterion_level1(level1_pred, level1_label)
        # return level1_loss