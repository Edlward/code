import cv2
import torch
import train
import numpy as np
from torch.autograd import Variable
import time
import torchvision.datasets as dsets
import torchvision.transforms as transforms

batch_size = 50

transform = transforms.Compose([
    transforms.RandomCrop(20),
    transforms.ToTensor()
])

valify_dataset = dsets.ImageFolder(root = '/home/lxg/codedata/headDataset/test',
                                    transform=transform)
valify_loader = torch.utils.data.DataLoader(dataset=valify_dataset,
                                            batch_size = batch_size,
                                            shuffle=False)

model = train.testNet()
model.load_state_dict(torch.load('head.pkl'))

correct = 0
total = 0
for i, (images, labels) in enumerate(valify_loader):
    images = (images[:,0:1,:,:] + images[:,1:2,:,:] + images[:,2:3,:,:]) / 3
    images = Variable(images)
    outputs = model(images)
    _, predicted = torch.max(outputs.data, 1)
    total += labels.size(0)
    correct += (predicted == labels).sum()
    # print([labels, predicted])
    print('Iter[%d/%d] Accuracy %d %%' % (i, len(valify_dataset)/batch_size, 
                                            100*(predicted == labels).sum() / labels.size(0)))
print('Accuracy of the model on the test images: %d %%' % (100 * correct / total))
    
