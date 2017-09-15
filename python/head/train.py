# encoding:utf-8
import torch
from torch.autograd import Variable
import torch.nn as nn
import torchvision.datasets as dsets
import torchvision.transforms as transforms

# Hyper Parameters
num_epochs = 100
batch_size = 50
learning_rate = 0.0001
momentum = 0.5  #SGD
use_cuda = False


transform = transforms.Compose([
    transforms.RandomCrop(20),
    # transforms.Scale(20),
    transforms.RandomHorizontalFlip(),
    transforms.ToTensor()
])

# train_dataset = dsets.ImageFolder(root = '/home/lxg/codedata/headDataset/train', 
#                                     transform = transform)
# test_dataset = dsets.ImageFolder(root = '/home/lxg/codedata/headDataset/test', 
#                                     transform = transforms.ToTensor())

train_dataset = dsets.ImageFolder(root = '/home/lxg/codedata/headDataset/trainHair', 
                                    transform = transform)
test_dataset = dsets.ImageFolder(root = '/home/lxg/codedata/headDataset/testHair', 
                                    transform = transform)

# Data Loader (Input Pipeline)
train_loader = torch.utils.data.DataLoader(dataset=train_dataset,
                                           batch_size=batch_size, 
                                           shuffle=True)

test_loader = torch.utils.data.DataLoader(dataset=test_dataset,
                                          batch_size=batch_size, 
                                          shuffle=False)


class testNet(nn.Module):
    def __init__(self):
        super(testNet, self).__init__()
        self.layer1 = nn.Sequential(
            nn.Conv2d(1, 16, kernel_size = 3),
            # nn.BatchNorm2d(16), #会降低模型容量？？降低泛化性能？？去掉后模型在测试集上准确率稳定上升
            nn.ReLU(),
            nn.MaxPool2d(2)
        )
        self.layer2 = nn.Sequential(
            nn.Conv2d(16, 32, kernel_size = 3),
            # nn.BatchNorm2d(32),
            nn.ReLU(),
            nn.MaxPool2d(2)
        )
        self.fc1 = nn.Linear(32*3*3, 2)
        # self.fc2 = nn.Linear(1024, 2)
    def forward(self, x):
        out = self.layer1(x)
        out = self.layer2(out)
        # print(out.size())
        out = out.view(out.size(0), -1)
        out = self.fc1(out)
        # out = self.fc2(out)
        return out

def train_model(model, criterion, optimizer, num_epochs):
    # Train the Model
    for epoch in range(num_epochs):
        for i, (images, labels) in enumerate(train_loader):
            images = (images[:,0:1,:,:] + images[:,1:2,:,:] + images[:,2:3,:,:]) / 3
            if use_cuda:
                images = images.cuda()
                labels = labels.cuda()
            images = Variable(images)
            labels = Variable(labels)
            # Forward + Backward + Optimize
            optimizer.zero_grad()
            outputs = model(images)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()

            if (i+1) % 10 == 0:
                print ('Epoch [%d/%d], Iter [%d/%d] Loss: %.4f' 
                       %(epoch+1, num_epochs, i+1, len(train_dataset)//batch_size, loss.data[0]))

        torch.save(model.state_dict(), 'head.pkl')
        # Test
        correct = 0
        total = 0
        for i, (images, labels) in enumerate(test_loader):
            images = (images[:,0:1,:,:] + images[:,1:2,:,:] + images[:,2:3,:,:]) / 3
            if use_cuda:
                images = images.cuda()
                labels = labels.cuda()
            images = Variable(images)
            outputs = model(images)
            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum()
            print i, 
        print()
        print('Accuracy of the model on the test images: %d %%' % (100 * correct / total))



model = testNet()    
if use_cuda:    
    model.cuda()

# Loss and Optimizer
criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=learning_rate)
# optimizer = torch.optim.SGD(model.parameters(), lr=learning_rate, momentum=momentum)


if __name__ == '__main__':
    # model = testNet()
    # data = Variable(torch.randn(1,1,20,20))
    # result = model(data)
    
    train_model(model, criterion, optimizer, num_epochs)
    
