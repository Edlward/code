import torch
import pandas as pd
import os
import torch.utils.data as data

class Minist(data.Dataset):
    
    def __init__(self, root, file_list, train, transform=None):
        dataframe = pd.read_csv(os.path.join(root, file_list))
        array = dataframe.as_matrix()
        self.label = array[:,0]
        self.data = array[:,1:].reshape(-1,28,28)
        self.transform = transform
        
    def __getitem__(self, idx):
        data = self.data[idx,:,:]
        data = self.transform(data.reshape(28,28,-1))

        label = self.label[idx]
        # label = torch.LongTensor([label]).unsqueeze_(1)
        # label = torch.zeros(1, 10).scatter_(1,label,1).type_as(torch.LongTensor())
        # label = torch.LongTensor([label])  # just need class number
        return data, label

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
        print(label)
        plt.figure('minist_'+str(i))
        plt.imshow(img[0,:,:])
        plt.show()

