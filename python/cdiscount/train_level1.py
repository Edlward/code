import os
import torch
import torchvision.transforms as transforms
import numpy as np
from torch.autograd import Variable
from cdataset import Cdataset
from imblanceDataset import Cdataset_imblance
from multiloss import MultiLoss
from resNet import resNetTest
from tool import saveCheckpoint
from inception import inceptionv3
from resnet18 import resnet_18

batch_size = 128  # because the final class fc'units is not sure, so the memory is floated
use_cuda = True
best_loss = float('inf')
train_loss_all = []
train_loss_lr = np.ndarray((5), float)
train_loss = 0.
test_loss = 0.

path = '/home/lxg/codedata/cdiscount'
num_epoch_has_trained = 0
num_iter_has_trained = 0

transform = transforms.Compose([
    transforms.ToTensor()
])

print('loading data.....')
train_data = Cdataset_imblance(path = '/home/lxg/codedata/cdiscount/',
                      csv_file = 'train.csv',
                      train = True,
                      transform=transform)
train_loader = torch.utils.data.DataLoader(train_data,
                        batch_size=batch_size,
                        shuffle=True,
                        num_workers=4)
test_data = Cdataset(path = '/home/lxg/codedata/cdiscount/',
                      csv_file = 'test_small.csv',
                      train = False,
                      transform=transform)
test_loader = torch.utils.data.DataLoader(test_data,
                        batch_size=batch_size,
                        shuffle=False,
                        num_workers=4)


print('define model.....')
# num_epoch = 2; model = resNetTest(); learning_rate = 0.001; optimizer = torch.optim.SGD(model.parameters(), lr=learning_rate, momentum=0.9)
# num_epoch = 2; model = resNetTest();learning_rate = 0.01; optimizer = torch.optim.Adam(model.parameters(), lr=learning_rate)
# num_epoch = 4; model = inceptionv3(); learning_rate = 0.01; optimizer = torch.optim.SGD(model.parameters(), lr=learning_rate, momentum=0.9)
num_epoch = 6; model = resnet_18(); learning_rate = 0.01; optimizer = torch.optim.SGD(model.parameters(), lr=learning_rate, momentum=0.9)


if use_cuda:
    torch.cuda.set_device(0)
    model = torch.nn.DataParallel(model, device_ids=[0])
    model.cuda()

initial_checkpoint = os.path.join(path, 'resnetCheckpoing.pth')
# initial_checkpoint = None
if initial_checkpoint is not None:
    print('load trained model....')
    checkpoint = torch.load(initial_checkpoint)
    model.load_state_dict(checkpoint['state_dict'])
    num_epoch_has_trained = checkpoint['epoch']
    num_iter_has_trained = checkpoint['iter']
    best_loss = checkpoint['best_loss']
    print('num_iter_has_trained: {}'.format(num_iter_has_trained))

# criterion = MultiLoss()
criterion = torch.nn.CrossEntropyLoss()

print('batch_size: %d' % (batch_size))                                    
print('train_dataset: %d idx: %d' % (len(train_data), len(train_loader)))
print('test_dataset: %d idx: %d' % (len(test_data), len(test_loader)))

print('begin to train.....')
def train(epoch):
    model.train()
    global train_loss
    global learning_rate
    total_num = 0
    correct_level1 = 0
    train_loss = 0
    correct_subclass = 0
    
    for batch_idx, (images, labels_level1, labels_subcategory) in enumerate(train_loader):
        # print('train labels_level1:', labels_level1.size())
        # print('train labels_subcategory:', labels_subcategory.size())
        
        batch_idx += num_iter_has_trained
        if batch_idx == len(train_loader):
            break

        if use_cuda:
            images = images.cuda()
            labels_level1 = labels_level1.cuda()
            labels_subcategory = labels_subcategory.cuda()
        images = Variable(images)
        labels_level1 = Variable(labels_level1)
        labels_subcategory = Variable(labels_subcategory)
        
        optimizer.zero_grad()
        output_level1 = model(images)
        loss = criterion(output_level1, labels_level1)
        loss.backward()
        optimizer.step()
        
        _, predicted_level1 = torch.max(output_level1.data, 1)
        total_num += labels_level1.data.size(0)
        correct_level1 += (predicted_level1 == labels_level1.data).sum()
        accuracy_level1 = 100. * correct_level1 / total_num
        # print('correct_level1:', correct_level1)

        # correct_subclass_idx = 0
        # for i in range(len(output_subcategory)):
        #     _, predicted_subclass = torch.max(output_subcategory[i].data, 1)
        #     correct_subclass_idx += (predicted_level1[i] == labels_level1.data[i] and
        #                     predicted_subclass == labels_subcategory.data[i])#.sum()
        # correct_subclass += correct_subclass_idx.sum()
        # print('correct_subclass:', correct_subclass)
        # accuracy_subclass = 100. * correct_subclass / total_num

        train_loss += loss.data[0]
        if (batch_idx+1) % 10 == 0:
            print ('train Epoch [%d/%d], Iter [%d/%d] lr: %.10f Loss: %.4f accuracy: %.3f'  \
                   %(epoch+1, num_epoch, batch_idx+1, len(train_loader), learning_rate, 
                        loss.data[0] / len(labels_level1), accuracy_level1))
        
        # if learning_rate > 0.0001 and (batch_idx+1) % 100 ==0:
        #     # update loss
        #     train_loss_lr[0] = train_loss_lr[1]
        #     train_loss_lr[1] = train_loss_lr[2]
        #     train_loss_lr[2] = train_loss_lr[3]
        #     train_loss_lr[3] = train_loss_lr[4]
        #     train_loss_lr[4] = loss.data[0]

        #     # adjust learning rate
        #     if batch_idx > 60 and (train_loss_lr[4]+train_loss_lr[3]) / 2 >             \
        #             (train_loss_lr[2] + train_loss_lr[1] + train_loss_lr[0]) / 3:
        #         learning_rate *= 0.1
        
        if (batch_idx+1) % 1000 == 0:
            print('saving...')
            saveCheckpoint({
                'epoch':epoch+1,
                'iter':batch_idx+1,
                'state_dict':model.state_dict(),
                'best_loss':best_loss,
            }, False, path)

            total_num = 0
            correct_level1 = 0
            correct_subclass = 0
            
        if (batch_idx+1) % 2000 == 0:
            test((batch_idx+1) / 2000)
            model.train()

    train_loss = train_loss / len(train_loader)
    train_loss_all.append(train_loss)

def test(epoch):
    model.eval()
    global test_loss
    test_loss = 0
    total_num = 0
    correct_level1 = 0
    correct_subclass = 0

    for batch_idx, (images, labels_level1, labels_subcategory) in enumerate(test_loader):
        if use_cuda:
            images = images.cuda()
            labels_level1 = labels_level1.cuda()
            labels_subcategory = labels_subcategory.cuda()
            
        images = Variable(images, requires_grad=False)
        labels_level1 = Variable(labels_level1, requires_grad=False)
        labels_subcategory = Variable(labels_subcategory, requires_grad=False)
        
        output_level1 = model(images)
        loss = criterion(output_level1, labels_level1)
        test_loss += loss.data[0]

        _, predicted_level1 = torch.max(output_level1.data, 1)
        total_num += labels_level1.data.size(0)
        correct_level1 += (predicted_level1 == labels_level1.data).sum()

        # correct_subclass_idx = 0
        # for i in range(len(output_subcategory)):
        #     _, predicted_subclass = torch.max(output_subcategory[i].data, 1)
        #     correct_subclass_idx += (predicted_level1[i] == labels_level1.data[i] and
        #                     predicted_subclass == labels_subcategory.data[i])#.sum()
        # correct_subclass += correct_subclass_idx.sum()

        accuracy_level1 = 100. * correct_level1 / total_num
        if (batch_idx+1) % 10 == 0:
            print ('test Epoch [%d/%d], Iter [%d/%d] Loss: %.4f accuracy: %.3f' \
                   %(epoch+1, num_epoch, batch_idx+1, len(test_loader), loss.data[0] / len(labels_level1) \
                   , accuracy_level1))

    global best_loss
    if test_loss < best_loss:
        best_loss = test_loss
        print('saving...')
        saveCheckpoint({
                'epoch':epoch+1,
                'iter':batch_idx+1,
                'state_dict':model.state_dict(),
                'best_loss':best_loss,
            }, True, path)
    
    test_loss = test_loss / len(test_loader)
    accuracy_level1 = 100. * correct_level1 / total_num
    # accuracy_subcategory = 100. * correct_subclass / total_num

    print('Test Accuracy_level1 of the model on the %d test images: %f %%' % (len(test_data), accuracy_level1))
    print("epoch %d train_loss:%.5f, test_loss %.5f, best_test_loss %.3f, accuracy_level1 %.3f\n" % (
            epoch, train_loss, test_loss, best_loss/len(test_loader), accuracy_level1))

for i in range(num_epoch_has_trained-1, num_epoch):
    train(i)    
    # test(i)
    num_iter_has_trained = 0
    
