import torch.nn as nn
from inceptionlib import inception_v3

# input size 299
def inceptionv3():
    kwargs = dict()
    # kwargs['num_classes'] = 49
    # kwargs['aux_logits'] = True
    model = inception_v3(pretrained=True, **kwargs)
    model.fc = nn.Linear(2048, 49)
    return model