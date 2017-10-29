import torch
import pandas as pd
import os
import torch.utils.data as data
import numpy as np
import random
from torchvision import transforms
from PIL import Image

transform_pad = transforms.Compose([
    transforms.Pad(2)
])
transform_crop = transforms.Compose([
                        transforms.RandomCrop([28,28])
])
class Minist(data.Dataset):
    
    def __init__(self, root, file_list, train, transform=None):
        dataframe = pd.read_csv(os.path.join(root, file_list))
        array = dataframe.as_matrix()
        self.label = array[:,0]
        self.data = array[:,1:].reshape(-1,28,28) # - 33.38  # substract mean, loss is down , but accuracy doesn't change
        self.transform = transform
        self.training = train
        self.transform_default = transforms.Compose([
            transforms.ToTensor()
        ])

    def __getitem__(self, idx):
        data = self.data[idx,:,:]
        data = data.astype(np.int32)
        img = Image.fromarray(data, mode='I')
        if self.training:
            degree = (random.random()-0.5)*10
            img = img.rotate(degree)

            if random.random() < 0.5:
                img = img.resize([32,32], resample=Image.BICUBIC)  # 28->32
                # print('resize image size:', img.size)
                img = transform_crop(img)  # random crop 32->28
            else:
                img = transform_pad(img)  # padding 28->32
                # print('pad image size:', img.size)
                img = transform_crop(img) # random crop 32->28
            # img.resize()

        # print('size 1:', img.size)
        img_array = np.array(img).reshape(28,28,-1)
        # print('size 2:', img_array.shape)
        
        img_array = self.transform_default(img_array)# HxWxC

        label = self.label[idx]
        # label = torch.LongTensor([label]).unsqueeze_(1)
        # label = torch.zeros(1, 10).scatter_(1,label,1).type_as(torch.LongTensor())
        # label = torch.LongTensor([label])  # just need class number
        return img_array, label

    def __len__(self):
        return self.label.shape[0]

    def showSample(self, idx):
        plt.figure('minist_'+str(idx))
        plt.imshow(self.data[idx,:,:])
        plt.show()


if __name__ == '__main__':
    import matplotlib.pyplot as plt
    from torchvision import transforms
    transform = transforms.Compose(
                transforms.ToTensor()
    )
    train_data = Minist(root='/home/lxg/codedata/minist',
                        file_list='train.csv',
                        train=True,
                        transform=transforms.ToTensor())
    for i in range(len(train_data)):
        data, label = train_data[i]
        img = data.numpy()
        print('data.shape: ', img.shape)
        print('label: ', label)
        plt.figure('minist_'+str(i))
        plt.imshow(img[0,:,:])
        plt.show()

