import os
import torch
import torchvision
import torch.utils.data as data
import pandas as pd
import cv2
import random

class Cdataset_imblance(data.Dataset):
    def __init__(self, path, csv_file, train, transform=[]):
        data = pd.read_csv(os.path.join(path, csv_file))
        image_path = '/home/we/zsw/train/'
        data['img_file'] = image_path + data['_id'].astype(str) + '_' \
                            + data['cum_sum'].astype(str) + '.jpg'

        gb = data.groupby('level1')
        keys = gb.groups.keys()
        
        self.transform = transform
        self.num_file = gb.size().max()   # subclass image num max
        self.train = train
        self.img_file = []
        self.level1 = []
        self.subcategory = []
        self.sublength = []
        for item in keys:
            group = gb.get_group(item)
            self.img_file.append(list(group['img_file']))
            self.level1.append(item)
            self.subcategory.append(list(group['level1_sub_class']))
            self.sublength.append(len(group))
            
    
    def __getitem__(self, idx):
        level1_class = random.randrange(0,49)
        sub_class = idx % self.sublength[level1_class]

        img_list = self.img_file[level1_class]
        level1 = self.level1[level1_class]
        subcategory_list = self.subcategory[level1_class]

        img = cv2.imread(img_list[sub_class])
        img = cv2.resize(img, (224,224))

        if self.train:
            if random.random() < 0.5:
                img = cv2.flip(img, 1)
        else:
            pass
        
        img = self.transform(img)
        
        return img, level1, subcategory_list[sub_class]

    def __len__(self):
        return self.num_file
        

# test Cdataset_imblance
if __name__ == '__main__':
    import torchvision.transforms as transforms
    import numpy as np
    transform = transforms.Compose([transforms.ToTensor()])

    dataset = Cdataset_imblance(path = '/home/lxg/codedata/cdiscount/',
                        csv_file = 'test.csv',
                        train = True,
                        transform=transform)
    for idx in range(len(dataset)):
        img, level1, sub_category = dataset[idx]
        imgn = img.numpy()*255 # this is float32
        imgn = imgn.transpose(1,2,0).astype(np.uint8)
        cv2.imwrite('tmp'+str(idx)+'.jpg', imgn)
        print('level1:', level1, 'sub_category:', sub_category)
        if idx > 20:
            break



