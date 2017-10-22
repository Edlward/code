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

    def __init__(self, root, list_file, train, transform, enlargeWithBg=False):
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
        
        # used by data augmentation
        self.enlargeWithBg = enlargeWithBg
        self.bg_name = []
        if enlargeWithBg:
            with open('/home/lxg/codedata/headXml/srcImage/bg.txt') as f:
                lines = f.readlines()
                self.bg_num = len(lines)
            for line in lines:
                splited = line.strip().split()
                self.bg_name.append(splited[0])

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
            if self.enlargeWithBg:
                img, boxes, labels = self.my_random_crop(img, boxes, labels)
            else:
                img, boxes, labels = self.random_crop(img, boxes, labels)
                # pass
        
        # Scale bbox locations to [0,1]
        w, h = img.size
        boxes /= torch.Tensor([w,h,w,h]).expand_as(boxes)

        img = img.resize((self.img_width, self.img_height))

        # # draw = ImageDraw.Draw(img)
        # # for item in boxes:
        # #     w,h = img.size
        # #     item *= torch.Tensor([w,h,w,h])
        # #     # logging.info(item.tolist())
        # #     draw.rectangle(item.tolist())
        # #     # logging.info('boxes')
        # #     # logging.info(item)

        # # img.save(fname, 'jpeg')
        
        img = self.transform(img)

        # logging.info('boxes before encode')
        # logging.info(boxes)
        # logging.info('labels before encode')
        # logging.info(labels)
        # print('boxes', boxes, 'labels', labels)
        # # Encode loc & conf targets
        # # box xmin,ymin,xmax,ymax
        loc_target, conf_target = self.data_encoder.encode(boxes, labels)
        
        return fname, img, boxes, conf_target

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
            if(self.enlargeWithBg):
                img, boxes, labels = self.my_random_crop(img, boxes, labels) # test
                # pass
            else:
                img, boxes, labels = self.random_crop(img, boxes, labels) # test
                # pass
        
        # Scale bbox locations to [0,1]
        w, h = img.size
        boxes /= torch.Tensor([w,h,w,h]).expand_as(boxes)

        img = img.resize((self.img_width, self.img_height))

        img = self.transform(img)
        # img = img - 0.4

        logging.info('boxes before encode')
        logging.info(boxes)
        logging.info('labels before encode')
        logging.info(labels)
        # print('boxes', boxes, 'labels', labels)
        # Encode loc & conf targets
        # box xmin,ymin,xmax,ymax
        loc_target, conf_target = self.data_encoder.encode(boxes, labels)
        # pos = conf_target > 0
        # if pos.long().sum() == 0:
        #     print('none matched', fname)

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
        imw, imh = img.size # source image size 640*480 not 300*300
        while True:
            # min_iou = random.choice([None, 0.1, 0.3, 0.5, 0.7, 0.9])
            min_iou = random.choice([None, 0.1, 0.3, 0.5, 0.7, 0.9])
            
            if min_iou is None:
                return img, boxes, labels
            
            for _ in range(100):
                # the selected crop image width not boxes 
                w = random.randrange(int(0.5*imw), imw) 
                h = random.randrange(int(0.5*imh), imh)
                # print(imw, imh, w, h)

                if h > 2*w or w > 2*h:
                    continue

                x = random.randrange(imw - w)
                y = random.randrange(imh - h)
                roi = torch.Tensor([[x, y, x+w, y+h]])
                # print('roi', roi, 'boxes', boxes)

                center = (boxes[:,:2] + boxes[:,2:]) / 2
                roi2 = roi.expand(len(center), 4)
                mask = (center > roi2[:,:2]) & (center < roi2[:,2:])
                mask = mask[:,0] & mask[:,1]
                if not mask.any():
                    continue

                selected_boxes = boxes.index_select(0, mask.nonzero().squeeze(1))
                
                # iou = self.data_encoder.iou(selected_boxes, roi)
                # if iou.min() < min_iou:
                #     continue
                
                img = img.crop((x,y,x+w,y+h))
                selected_boxes[:,0].add_(-x).clamp_(min=0,max=w)
                selected_boxes[:,1].add_(-y).clamp_(min=0,max=h)
                selected_boxes[:,2].add_(-x).clamp_(min=0,max=w)
                selected_boxes[:,3].add_(-y).clamp_(min=0,max=h)
        
                return img, selected_boxes, labels[mask]

    def my_random_crop(self, img, boxes, labels):
        imw, imh = img.size # source image size 640*480 not 300*300
        # print('img size: {0}'.format(img.size))

        while True:
            min_iou = random.choice([None, 0.1, 0.3, 0.5, 0.7, 0.9])
            
            if min_iou is None:
                return img, boxes, labels
            
            for _ in range(100):
                # the selected crop image width not boxes 
                w = random.randrange(int(0.7*imw), imw) 
                h = random.randrange(int(0.7*imh), imh)
                # print(imw, imh, w, h)

                if h > 1.5*w or w > 1.5*h:
                    continue

                x = random.randrange(imw - w)
                y = random.randrange(imh - h)
                roi = torch.Tensor([[x, y, x+w, y+h]])
                # print('roi', roi, 'boxes', boxes)

                center = (boxes[:,:2] + boxes[:,2:]) / 2
                roi2 = roi.expand(len(center), 4)
                mask = (center > roi2[:,:2]) & (center < roi2[:,2:])
                # mask = (boxes[:,:2] > roi2[:,:2]) & (boxes[:,2:] < roi2[:,2:])
                
                mask = mask[:,0] & mask[:,1]
                if not mask.any():
                    continue

                selected_boxes = boxes.index_select(0, mask.nonzero().squeeze(1))
                
                # iou = self.data_encoder.iou(selected_boxes, roi)
                # if iou.min() < min_iou:
                #     continue
                
                img = img.crop((x,y,x+w,y+h))
                selected_boxes[:,0].add_(-x).clamp_(min=0,max=w)
                selected_boxes[:,1].add_(-y).clamp_(min=0,max=h)
                selected_boxes[:,2].add_(-x).clamp_(min=0,max=w)
                selected_boxes[:,3].add_(-y).clamp_(min=0,max=h)
        
                return img, selected_boxes, labels[mask]
        
    def my_random_complement(self, img, boxes, labels):
        ''' enlarge the picture, than randomly crop, this is something wrong, just used for image classification, not for location
        
        randomly crop the image and adjust the bbox locations
        
        Args:
            img: (PIL.Image) image
            boxes: (tensor) bbox locations, sized [#obj, 4]
            labels: (tensor) bbox labels, sized [#obj,]
        
        Returns:
            img: (PIL.Image) cropped image
            selected_boxes: (tensor) selected bbox locations
            labels: (tensor) selected bbox labels
        '''
        imw, imh = img.size # source image size 640*480 not 300*300
        
        bgidx = random.randrange(0, self.bg_num)
        fname = self.bg_name[bgidx]
        bg_image = Image.open(os.path.join(self.root, fname)) # img is normalised to 1
        bg_image = bg_image.resize((int(imw*1.5), int(imh*1.5)))
        add_w = int(imw*0.25)
        add_h = int(imh*0.25)
        bg_image.paste(img, (add_w, add_h))
        bg_imw, bg_imh = bg_image.size
        bg_boxes = boxes.clone() # pixel size
        bg_boxes[:,0].add_(add_w)
        bg_boxes[:,1].add_(add_h)
        bg_boxes[:,2].add_(add_w)
        bg_boxes[:,3].add_(add_h)
        # bg_image.show()
        while True:
            # min_iou = random.choice([None, 0.1, 0.3, 0.5, 0.7, 0.9])
            min_iou = random.choice([None, 0.1, 0.3, 0.5, 0.7, 0.9])
            
            if min_iou is None:
                return img, boxes, labels
            
            for _ in range(100):
                # the selected crop image width not boxes 
                # w = random.randrange(int(0.57*bg_imw), int(0.77*bg_imw)) 
                # h = random.randrange(int(0.57*bg_imh), int(0.77*bg_imh))
                w = imw
                h = imh
                # print(imw, imh, w, h)

                if h > 2*w or w > 2*h:
                    continue

                x = random.randrange(bg_imw - w)
                y = random.randrange(bg_imh - h)
                roi = torch.Tensor([[x, y, x+w, y+h]])
                # print('roi', roi, 'boxes', boxes)

                center = (bg_boxes[:,:2] + bg_boxes[:,2:]) / 2  # [#obj, 2]
                roi2 = roi.expand(len(center), 4) # [#obj, 4]
                mask = (bg_boxes[:,:2] > roi2[:,:2]) & (bg_boxes[:,2:] < roi2[:,2:])
                # mask = (center > roi2[:,:2]) & (center < roi2[:,2:])
                
                mask = mask[:,0] & mask[:,1]
                if not mask.any():
                    continue

                selected_boxes = bg_boxes.index_select(0, mask.nonzero().squeeze(1))
                
                # iou = self.data_encoder.iou(selected_boxes, 
                # if iou.min() < min_iou:
                #     continue
                
                bg_image = bg_image.crop((x,y,x+w,y+h))
                selected_boxes[:,0].add_(-x).clamp_(min=0,max=w)
                selected_boxes[:,1].add_(-y).clamp_(min=0,max=h)
                selected_boxes[:,2].add_(-x).clamp_(min=0,max=w)
                selected_boxes[:,3].add_(-y).clamp_(min=0,max=h)
        
                return bg_image, selected_boxes, labels[mask]
    
    def __len__(self):
        return self.num_samples