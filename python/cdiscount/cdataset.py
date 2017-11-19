import os
import torch
import torchvision
import torch.utils.data as data
import pandas as pd
import cv2
import random

class Cdataset(data.Dataset):

    def __init__(self, path, csv_file, train, transform=[]):
        data = pd.read_csv(os.path.join(path, csv_file))
        # _id  category_id  img_num  cum_sum  level1  category_simple_id
        image_path = '/home/we/zsw/train/'

        data['img_file'] = image_path + data['_id'].astype(str) + \
                                    '_' + data['cum_sum'].astype(str) + '.jpg'
        self.img_file = list(data['img_file'])
        self.num_file = len(self.img_file)
        self.level1 = list(data['level1'])
        self.subcategory = list(data['level1_sub_class'])
        self.transform = transform
        self.train = train

    def __getitem__(self, idx):
        img = cv2.imread(self.img_file[idx])
        img = cv2.resize(img, (224,224))

        if self.train:
            if random.random() < 0.5:
                img = cv2.flip(img, 1)
        else:
            pass
        
        img = self.transform(img)
        # equal probability select one image from 49 level1, than random select one from that class

        return img, self.level1[idx], self.subcategory[idx]

    def __len__(self):
        return self.num_file


# test Cdataset
if __name__ == '__main__':
    import torchvision.transforms as transforms
    import numpy as np
    transform = transforms.Compose([transforms.ToTensor()])

    dataset = Cdataset(path = '/home/lxg/codedata/cdiscount/',
                        csv_file = 'train.csv',
                        train = True,
                        transform=transform)
    for idx in range(len(dataset)):
        img, category, level1 = dataset[idx]
        imgn = img.numpy()*255 # this is float32
        imgn = imgn.transpose(1,2,0).astype(np.uint8)
        cv2.imwrite('tmp'+str(idx)+'.jpg', imgn)
        print('category:', category, 'level1:', level1)
        if idx > 1:
            break