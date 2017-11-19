import os
import torch
import pandas as pd
import cv2
import numpy as np
import time
#from transform import *
from dataset.transform import *
from torch.utils.data import Dataset, DataLoader
#import torchvision.transforms as transforms
batch_size = 128



class xiongDataset(Dataset):
	def __init__(self, csv_file, root_dir, transform=None):
		print('---------------init-----------------')
		self.train_names=[]
		self.root_dir=root_dir
		self.transform = transform
		train_images = pd.read_csv(csv_file)
		train_ids = list(train_images['product_id'])
		train_idxs = list(train_images['img_idx'])
		self.labels = list(train_images['category_idx'])
		num_train = len(train_images)
		for i in range(num_train):
			train_name = '{}_{}.jpg'.format(train_ids[i],train_idxs[i])
			self.train_names.append(train_name)

	def __len__(self):
		return len(self.train_names)

	def __getitem__(self, idx):
		img = cv2.imread(self.root_dir + self.train_names[idx])
		img = img[:,:,::-1]
		#img = mx.image.imdecode(open(self.root_dir + self.train_names[idx]).read())
		img = img[10:-10,10:-10,:]
		#img = cv2.resize(img,(299,299))
		label = self.labels[idx]
		if self.transform is not None:
			for t in self.transform:
				img = t(img)

		return img,label

def image_to_tensor_transform(image):
    mean   = [102.9801, 115.9465, 122.7717]
    image  = image.astype(np.float32) - np.array(mean, np.float32)
    image  = image.transpose((2,0,1))
    #print(np.max(image))
    tensor = torch.from_numpy(image)

    return tensor




def train_augment(image):

    if random.random() < 0.5:
        image = random_shift_scale_rotate(image,
                  # shift_limit  = [0, 0],
                  shift_limit=[-0.07, 0.07],
                  scale_limit=[0.9, 1.2],
                  rotate_limit=[-10, 10],
                  aspect_limit=[1, 1],
                  # size=[1,299],
                  borderMode=cv2.BORDER_REFLECT_101, u=1)
    else:
        pass

    # flip  random ---------
    image = random_horizontal_flip(image, u=0.5)
    tensor = image_to_tensor_transform(image)
    #print(tensor.size())
    return tensor



def valid_augment(image):

    tensor = image_to_tensor_transform(image)
    return tensor

train_transform = transforms.Compose([
	#RandomRotate(10),
	transforms.ToTensor(),])

#train_data = xiongDataset('../train_images.csv','../train/',transform=transforms.ToTensor())
#t0 = time.time()
train_dataset = xiongDataset('../train_images.csv','../train/',transform =[ lambda x:train_augment(x)])

train_dataloader= DataLoader(train_dataset,batch_size=batch_size,shuffle=True,num_workers=4)
#t1 = time.time()
#print (t1-t0)
#print(len(data_loader))

val_dataset = xiongDataset('../val_images.csv','../val/',transform =[ lambda x:valid_augment(x),])
val_dataloader= DataLoader(val_dataset,batch_size=batch_size,shuffle=False,num_workers=4)

data_iter = iter(train_dataloader)
for i in range(len(train_dataloader)):
	t0=time.time()
	images,labels = data_iter.next()
	t1 = time.time()
	print('---------{}---------'.format(i))
	print('load time {}'.format(t1-t0))
	print(images.size(),labels.size())

'''t0 = time.time()
for i ,(images,labels)in enumerate(data_loader):
	
	
	print('---------{}---------'.format(i))
	print(images.size(),labels.size())
	t1 = time.time()
	print('load time {}'.format(t1-t0))
	t0 = t1'''

