# encoding:utf-8
import torch
import torch.nn as nn
from alexnet import *
from faceDataset import *
import cv2

def train_model(model, criterion, optimizer, num_epochs = 26):
    for epoch in range(num_epochs):
        print('Epoch {}/{}'.format(epoch, num_epochs-1))
        print('-' * 10)

        for idx, (image, label) in enumerate(train_Dataloader):
            # label = torch.log(label)
            # print(image[0].numpy().shape)
            cv2.imwrite('b.jpg', image[0].numpy().transpose(1,2,0))
            # cv2.imwrite('b.jpg', image[0].numpy().transpose(1,2,0)*255)
            # print(label[0].numpy().shape)
            label0 = label[0].numpy()
            label0 = cv2.resize(label0, (WIDTH, HEIGHT))
            cv2.imwrite('c.jpg', label0)

            if use_cuda:
                image = image.cuda()
                label = label.cuda()
            image = Variable(image)
            label = Variable(label)
            optimizer.zero_grad()
            outputs = model(image)
            
            # print outputs[0].data
            out0 = outputs[0].data.numpy()
            out0 = out0.transpose(1,2,0)
            max0 = out0.max()
            out0 = cv2.resize(out0, (WIDTH, HEIGHT))
            # cv2.imwrite('a.jpg', out0 / max0)
            cv2.imwrite('a.jpg', out0 / max0 * 255)

            outputs = outputs.view(outputs.size(0), -1)
            # outputs = torch.log(outputs)
            label = label.view(label.size(0), -1)
            # print(outputs.size())
            # print(label.size())
            # print(type(label))
            loss = criterion(outputs, label)
            loss.backward()
            optimizer.step()
            print('train idx {}/{}, loss {}'.format(idx, len(train_Dataset)//batch_size, loss.data[0]))
        
        torch.save(model.state_dict(), 'face.pkl')  # save model
        
        running_loss = 0.0
        for idx, (image, label) in enumerate(val_Dataloader):
            # label = torch.log(label)
            if use_cuda:
                image = image.cuda()
                label = label.cuda()
            image = Variable(image)
            label = Variable(label)    
            outputs = model(image)
            outputs = outputs.view(outputs.size(0), -1)
            # outputs = torch.log(outputs)
            label = label.view(label.size(0), -1)
            loss = criterion(outputs, label)
            running_loss += loss.data[0]
            print('val idx {}/{}, Loss {}'.format(idx, len(val_Dataset)//batch_size, loss.data[0]))
        print('val Epoch {}/{}, Loss {}'.format(epoch, num_epochs, running_loss//len(val_Dataset)))
    print()

learning_rate = 0.001
use_cuda = False

model = alexNet()
if use_cuda:
    model.cuda()

# Dice 系数
# criterion = nn.CrossEntropyLoss() # 目标只能是int类型的，用于分类
# criterion = nn.KLDivLoss()
# criterion = nn.BCEWithLogitsLoss()
criterion = nn.MSELoss()

optimizer = torch.optim.SGD(model.parameters(), lr = learning_rate, momentum = 0.9)
# optimizer = torch.optim.Adam(model.parameters(), lr = learning_rate)
train_model(model, criterion, optimizer)

        