'''load image/box from a annotation file
the annotation file is organized as:
image_name #obj xmin ymin width height
#obj represents the number of obj'''

import os
import sys
import os.path

import random
import numpy as np

import torch
import torch.utils.data as data
import torchvision.transforms as transforms

from simEncoder import DataEncoder
from PIL import Image, ImageOps, ImageDraw

import logging 
# logging.basicConfig(level=logging.WARNING)

class ListDataset(data.Dataset):
    # src_img_width = 800 # source image size
    # src_img_height = 600
    img_width = 300
    img_height = 300
    # img_scale_width = 
    # img_scale_height = 

    def __init__(self, root, list_file, train, transform):
        '''
        Args:
            root: (str) directory to images
            list_file: (str)path to index file,label
            train: (boolean) train or test
            transform: ([transforms]) image transforms
        '''
        self.root = root
        self.train = train
        self.transform = transform

        self.fnames = []
        self.boxes = []
        self.labels = []

        self.data_encoder = DataEncoder()

        with open(list_file) as f:
            lines = f.readlines()
            self.num_samples = len(lines)
        
        for line in lines:
            splited = line.strip().split()
            self.fnames.append(splited[0])

            num_objs = int(splited[1])
            box = []
            label = []
            for i in range(num_objs):
                xmin = splited[2+4*i] 
                ymin = splited[3+4*i] 
                width = splited[4+4*i]
                height = splited[5+4*i]
                c = 0
                box.append([float(xmin), float(ymin), float(width)+float(xmin), 
                            float(height)+float(ymin)])
                label.append(int(c))
            self.boxes.append(torch.Tensor(box))
            self.labels.append(torch.LongTensor(label))
    
    def idxTest(self, idx):
         # load image, bbox locations and labels
        fname = self.fnames[idx]
        img = Image.open(os.path.join(self.root, fname)) # img is normalised to 1
        boxes = self.boxes[idx].clone()
        labels = self.labels[idx]

        # data augmentation while training
        if self.train:
            img, boxes = self.random_flip(img, boxes)
            img, boxes, labels = self.random_crop(img, boxes, labels)
        
        # Scale bbox locations to [0,1]
        w, h = img.size
        boxes /= torch.Tensor([w,h,w,h]).expand_as(boxes)

        img = img.resize((self.img_width, self.img_height))

        # draw = ImageDraw.Draw(img)
        # for item in boxes:
        #     w,h = img.size
        #     item *= torch.Tensor([w,h,w,h])
        #     # logging.info(item.tolist())
        #     draw.rectangle(item.tolist())
        #     # logging.info('boxes')
        #     # logging.info(item)

        # img.save(fname, 'jpeg')
        
        img = self.transform(img)

        logging.info('boxes before encode')
        logging.info(boxes)
        logging.info('labels before encode')
        logging.info(labels)
        print('boxes', boxes, 'labels', labels)
        # Encode loc & conf targets
        # box xmin,ymin,xmax,ymax
        iou = self.data_encoder.encodeTest(boxes, labels)
        return img, iou

    def __getitem__(self, idx):
        '''load a image, and encode its bounding box locations and class labels

        Args:
            idx: (int) image index
        
        Returns:
            img: (tensor) image tensor
            the following N is the number ofdefault box
            loc_target: (tensor) locations targes, sized [N,4]
            conf_target: (tensor) label targets, sized [N,4], 
        '''
        # load image, bbox locations and labels
        fname = self.fnames[idx]
        img = Image.open(os.path.join(self.root, fname)) # img is normalised to 1
        boxes = self.boxes[idx].clone()
        labels = self.labels[idx]

        # data augmentation while training
        if self.train:
            img, boxes = self.random_flip(img, boxes)
            img, boxes, labels = self.random_crop(img, boxes, labels)
        
        # Scale bbox locations to [0,1]
        w, h = img.size
        boxes /= torch.Tensor([w,h,w,h]).expand_as(boxes)

        img = img.resize((self.img_width, self.img_height))

        # draw = ImageDraw.Draw(img)
        # for item in boxes:
        #     w,h = img.size
        #     item *= torch.Tensor([w,h,w,h])
        #     # logging.info(item.tolist())
        #     draw.rectangle(item.tolist())
        #     # logging.info('boxes')
        #     # logging.info(item)

        # img.save(fname, 'jpeg')
        
        img = self.transform(img)

        logging.info('boxes before encode')
        logging.info(boxes)
        logging.info('labels before encode')
        logging.info(labels)
        # print('boxes', boxes, 'labels', labels)
        # Encode loc & conf targets
        # box xmin,ymin,xmax,ymax
        loc_target, conf_target = self.data_encoder.encode(boxes, labels)
        return img, loc_target, conf_target

    def random_flip(self, img, boxes):
        '''randomly flip the image and adjust the bbox locations

        for bbox (xmin, ymin, xmax, ymax), the flipped bbox is:
            (w-xmax, h-ymax, w-xmin, h-ymin)

        Args:
            img: (PIL.Image) image
            boxes: (Tensor) boxes locations, sized [#obj, 4]

        Returns:
            img: (PIL.Image) randomly flipped image
            boxes: (tensor) randomly flipped bbox locations, sized [#obj, 4]
        '''
        if random.random() < 0.5:
            img = img.transpose(Image.FLIP_LEFT_RIGHT) # can this high and low?
            w = img.width
            xmin = w - boxes[:,2] 
            xmax = w - boxes[:,0]
            boxes[:,0] = xmin
            boxes[:,2] = xmax
        return img, boxes
    
    def random_crop(self, img, boxes, labels):
        '''randomly crop the image and adjust the bbox locations
        
        Args:
            img: (PIL.Image) image
            boxes: (tensor) bbox locations, sized [#obj, 4]
            labels: (tensor) bbox labels, sized [#obj,]
        
        Returns:
            img: (PIL.Image) cropped image
            selected_boxes: (tensor) selected bbox locations
            labels: (tensor) selected bbox labels
        '''
        
        return img, boxes, labels
    
    def __len__(self):
        return self.num_samples