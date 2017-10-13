import os 
import argparse
import itertools

import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
import torch.backends.cudnn as cudnn

import torchvision
import torchvision.transforms as transforms

import logging

from torch.autograd import Variable

from simssd import SIMSSD
from simDatagen import ListDataset
from simMultiLoss import MultiBoxLoss

use_cuda = torch.cuda.is_available()
use_cuda = True
print(use_cuda)
best_loss = float('inf')  # best test loss

transform = transforms.Compose([transforms.ToTensor()])

trainset = ListDataset(root='/home/lxg/codedata/headXml/walmat/',
                        list_file='/home/lxg/codedata/headXml/walmat/label_walmat_train.txt',
                        train=True,
                        transform=transform)
trainloader = torch.utils.data.DataLoader(trainset, batch_size=3, 
                        shuffle=True, num_workers=2)

testset = ListDataset(root='/home/lxg/codedata/headXml/walmat/',
                        list_file='/home/lxg/codedata/headXml/walmat/label_walmat_test.txt',
                        train=False,
                        transform=transform)
testloader = torch.utils.data.DataLoader(trainset, batch_size=2, 
                        shuffle=False, num_workers=2)
                        
net = SIMSSD()

if use_cuda:
    torch.cuda.set_device(1)
    net = torch.nn.DataParallel(net, device_ids=[1])
    net.cuda()
    # cudnn.benchmark = True
    
criterion = MultiBoxLoss(use_gpu=use_cuda)
optimizer = optim.SGD(net.parameters(), lr=0.001, momentum=0.9, weight_decay=1e-4)

def train(epoch):
    net.train()
    train_loss = 0
    for batch_idx, (images, loc_targets, conf_targets) in enumerate(trainloader):
        if use_cuda:
            images = images.cuda()
            loc_targets = loc_targets.cuda()
            conf_targets = conf_targets.cuda()
        
        images = Variable(images)
        loc_targets = Variable(loc_targets)
        conf_targets = Variable(conf_targets)

        optimizer.zero_grad()
        loc_preds, conf_preds = net(images)
        loss = criterion(loc_preds, loc_targets, conf_preds, conf_targets)
        loss.backward()
        optimizer.step()

        train_loss += loss.data[0]
        print('epoch %d, batch_idx %d,  loss: %.3f, total_loss: %.3f' % (epoch, batch_idx, loss.data[0], train_loss/(batch_idx+1)))

def test(epoch):
    net.eval()
    test_loss = 0
    for batch_idx, (images, loc_targets, conf_targets) in enumerate(testloader):
        if use_cuda:
            images = images.cuda()
            loc_targets = loc_targets.cuda()
            conf_targets = conf_targets.cuda()
        images = Variable(images)
        loc_targets = Variable(loc_targets)
        conf_targets = Variable(conf_targets)
        loc_preds, conf_preds = net(images)
        loss = criterion(loc_preds, loc_targets, conf_preds, conf_targets)
        test_loss += loss.data[0]
    print('epoch %d, test_loss %.3f' % (epoch, test_loss/(len(testset)/(2))))

    # save checkpoint
    global best_loss
    test_loss /= len(testloader)
    if test_loss < best_loss:
        print('Saving..')
        state = {
            'net': net.module.state_dict(),
            # 'loss': test_loss,
            'epoch' : epoch,
        }
        torch.save(net.state_dict(), 'checkpoint.pth')
        best_loss = test_loss

for epoch in range(10):
    train(epoch)
    test(epoch)

