#encoding:utf-8
import torch
import torchvision.transforms as transforms
from torch.utils.data.dataset import Dataset
from torch.utils.data import DataLoader

from sklearn.model_selection import train_test_split
import numpy as np
from numpy import random

import cv2

txt_file = "/home/lxg/codedata/aflw/data/d.txt" #描述文本文件
train_file_root = "" #训练集文件夹目录
val_file_root = "" #验证集文件夹目录
image_root = "/home/lxg/codedata/aflw/data/all/" #图片目录

train_label_root = "" #训练集label文件夹
val_label_root = "" #验证集label文件夹
label_root = "/home/lxg/codedata/aflw/label/all/" #标签目录

WIDTH = 512
HEIGHT = 512
OUTPUT_W = 41
OUTPUT_H = 41
batch_size = 20

f = open(txt_file)
lines = f.readlines() #读入所有行，其中图片名在每一行的第7位

image_names = []
for i in range(2,len(lines)): #忽略第一行
	name = lines[i].split()
	if len(name) < 8:
		continue
	name = name[7]
	image_names.append(name)

train_images, val_images = train_test_split(image_names, test_size = 0.2 , random_state = 9) #分为0.8训练集和0.2的验证集

def randomHorizontalFlip2(image, label, u = 0.5):
	if random.random() < u:
		image = cv2.flip(image,1)
		label = cv2.flip(label,1)

	return image, label

def label_to_tensor(label):
	label = label
	tensor = torch.from_numpy(label).type(torch.FloatTensor)
	return tensor

def image_to_tensor(image, mean=0, std=1.):
	image = image.astype(np.float32)
	image = (image-mean)/std
	image = image.transpose(2,0,1)
	tensor = torch.from_numpy(image)
	return tensor

class faceDataset(Dataset):
	def __init__(self, split , transforms=None , isTrain = True):
		self.split = split #训练的图片名，或者验证的图片集
		self.transforms = transforms #训练集可能用到的数据扩充方法
		self.isTrain = isTrain #区分是否是训练集，决定是否有transforms
	def __getitem__(self,index):
		image_name = self.split[index]
		image_absolute_name = image_root + image_name
		img = cv2.imread(image_absolute_name)
		img = cv2.resize(img,(WIDTH,HEIGHT))
		# img = img.astype(np.float32)/255

		label_absolute_name = label_root + "label_" +image_name
		label = cv2.imread(label_absolute_name,cv2.IMREAD_GRAYSCALE)
		label = cv2.resize(label,(OUTPUT_W,OUTPUT_H))
		# label = label.astype(np.float32)/255

		if self.isTrain:
			for t in self.transforms:
				img,label = t(img,label)
		img = image_to_tensor(img)
		label = label_to_tensor(label)

		return img,label
	def __len__(self):
		return len(self.split)

train_Dataset = faceDataset(split = train_images,transforms = [lambda x,y: randomHorizontalFlip2(x,y)], isTrain = True)
train_Dataloader = DataLoader(train_Dataset,batch_size=batch_size,shuffle=True,num_workers=4)

val_Dataset = faceDataset(split = val_images, transforms= None, isTrain = False)
val_Dataloader = DataLoader(val_Dataset,batch_size = batch_size, shuffle=False,num_workers=4)

if __name__ == '__main__':
	from skimage import io
	import matplotlib.pyplot as plt
	''''for i,(image,label) in enumerate(train_Dataloader):
		print(image.size(),label.size())
		img = image[0].numpy()
		print img.shape()
		io.imshow(image[0].numpy().transpose(1,2,0))
		#io.imshow(label)'''
	for i in range(len(train_Dataset)):
		img,label = train_Dataset[i]
		plt.subplot(1,2,1)
		io.imshow(img)
		plt.subplot(1,2,2)
		io.imshow(label)
		plt.show()
