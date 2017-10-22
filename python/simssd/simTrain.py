'''### 3. Open the TensorBoard
To run the TensorBoard, open a new terminal and run the command below. Then, open http://localhost:6006/ in your web browser.
```bash
$ tensorboard --logdir='./logs' --port=6006
```'''
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

from logger import Logger
logger = Logger('./logs')

use_cuda = torch.cuda.is_available()
use_cuda = True
print('use_cuda', use_cuda)
best_loss = float('inf')  # best test loss
batch_size = 20
transform = transforms.Compose([transforms.ToTensor()])

trainset = ListDataset(root='/home/lxg/codedata/headXml/srcImage/',
                        list_file='/home/lxg/codedata/headXml/srcImage/train_label.txt',
                        train=True,
                        transform=transform,
                        enlargeWithBg=True)
# trainset = ListDataset(root='/home/lxg/codedata/headXml/walmat/',
#                         list_file='/home/lxg/codedata/headXml/walmat/label_walmat_test.txt',
#                         train=False,
#                         transform=transform,
#                         enlargeWithBg=False)

trainloader = torch.utils.data.DataLoader(trainset, batch_size=batch_size, 
                        shuffle=True, num_workers=2)

testset = ListDataset(root='/home/lxg/codedata/headXml/srcImage/',
                        list_file='/home/lxg/codedata/headXml/srcImage/test_label.txt',
                        train=False,
                        transform=transform,
                        enlargeWithBg=False)
testloader = torch.utils.data.DataLoader(testset, batch_size=batch_size, 
                        shuffle=False, num_workers=2)


print('batch_size: %d' % (batch_size))
print('train_dataset: %d idx: %d' % (len(trainset), len(trainset)/batch_size))                        
print('test_dataset: %d idx: %d' % (len(testset), len(testset)/batch_size))

net = SIMSSD()

if use_cuda:
    torch.cuda.set_device(0)
    net = torch.nn.DataParallel(net, device_ids=[0])
    net.cuda()
    # cudnn.benchmark = True

learning_rate = 0.005    
criterion = MultiBoxLoss(use_gpu=use_cuda)
# optimizer = optim.SGD(net.parameters(), lr=learning_rate, momentum=0.9, weight_decay=1e-4)  # 0.003
# optimizer = optim.SGD(net.parameters(), lr=learning_rate, weight_decay=1e-4)
optimizer = torch.optim.Adam(net.parameters(), lr = learning_rate)

step_train = 0
step_test = 0

def train(epoch):
    print('\ntrain')
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
        # if loss.data.cpu().numpy() == 0:
        #     print('train epoch %d, batch_idx %d/%d, zero box matched' % (epoch, batch_idx, len(trainset)/batch_size))
        #     continue
        loss.backward()
        optimizer.step()

        train_loss += loss.data[0]
        print('train epoch %d, batch_idx %d/%d, batch_loss: %.3f, epoch_average_loss: %.3f' % (epoch, batch_idx, len(trainset)/batch_size, loss.data[0], train_loss/(batch_idx+1)))
        info = {
            'train_loss': loss.data[0],
        }
        global step_train
        step_train += 1
        for tag, value in info.items():
            logger.scalar_summary(tag, value, step_train)
    return train_loss / (len(trainset) / batch_size + 1)

def test(epoch):
    print('\ntest')
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
        print('test epoch %d, batch_idx %d/%d, batch_loss: %.3f, epoch_average_loss %.3f' % (epoch, batch_idx, len(testset)/batch_size, loss.data[0], test_loss/(batch_idx+1)))
        
        info = {
            'test_loss': loss.data[0],
        }
        global step_test
        step_test += 1
        for tag, value in info.items():
            logger.scalar_summary(tag, value, step_test)

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
    return test_loss 

for epoch in range(30):
    train_loss = train(epoch)
    test_loss = test(epoch)
    print('epoch %d, train_loss %.3f, test_loss %.3f, best_loss %.3f' % (epoch, train_loss, test_loss, best_loss))

