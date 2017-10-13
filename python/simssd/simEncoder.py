#encoding=utf-8
''' encode target locations and lables'''

import torch
import math
import itertools

import logging 
# logging.basicConfig(level=logging.WARNING)

class DataEncoder:
    def __init__(self):
        '''compute default box sizes with scale and aspect transform.abs
        center, width, height normalized to 1
        '''
        scale = 300. # (float) image width 
        steps = [s / scale for s in (8,)] # steps 36.5*8=292 because of maxpool2d
        sizes = [s / scale for s in (30,)] # bounding box size
        aspect_ratios = ((1,)) # width and height ratio
        feature_map_sizes = (36,)

        num_layers = len(feature_map_sizes)

        boxes = []
        for i in range(num_layers):
            fmisze = feature_map_sizes[i]
            for h,w in itertools.product(range(fmisze), repeat=2):
                cx = (w+0.5)*steps[i] # anchor center
                cy = (h+0.5)*steps[i]

                s = sizes[i] # bounding box size
                boxes.append((cx, cy, s, s))

                '''
                something simplified
                '''

        self.default_boxes = torch.Tensor(boxes)
        logging.info(self.default_boxes.size())  # [1296,4]
    
    def iou(self, box1, box2):
        '''compute the intersection over union of two set of boxes, each box
        box is [x1,y1,x2,y2].

        Args:
            box1: (tensor) bounding boxes, sized [N,4]
            box2: (tensor) bounding boxes, sized [M,4]
        
        Returns:
            iou: (tensor) sized [N,M]
        '''
        N = box1.size(0)
        M = box2.size(0)

        lt = torch.max(
            box1[:,:2].unsqueeze(1).expand(N,M,2),  # [N,2] -> [N,1,2] -> [N,M,2]
            box2[:,:2].unsqueeze(0).expand(N,M,2),  # [M,2] -> [1,M,2] -> [N,M,2]
        )

        rb = torch.min(
            box1[:,2:].unsqueeze(1).expand(N,M,2),  # [N,2] -> [N,1,2] -> [N,M,2]
            box2[:,2:].unsqueeze(0).expand(N,M,2),  # [M,2] -> [1,M,2] -> [N,M,2]
        )
        
        wh = rb - lt # [N,M,2]
        wh[wh<0] = 0 # clip at 0
        inter = wh[:,:,0] * wh[:,:,1] # [N,M]

        area1 = (box1[:,2]-box1[:,0]) * (box1[:,3]-box1[:,1]) # [N,]
        area2 = (box2[:,2]-box2[:,0]) * (box2[:,3]-box2[:,1]) # [M,]
        area1 = area1.unsqueeze(1).expand_as(inter) # [N,] -> [N,1,]-> [N,M]
        area2 = area2.unsqueeze(0).expand_as(inter) # [M,] -> [1,M] -> [N,M]

        iou = inter / (area1 + area2 - inter)
        return iou

    def encodeTest(self, boxes, classes, threshold=0.5):
        logging.info('\n\n encode')

        default_boxes = self.default_boxes
        num_default_boxes = default_boxes.size(0)
        num_objs = boxes.size(0)

        iou = self.iou( # [N,M]
            boxes,
            torch.cat([default_boxes[:,:2] - default_boxes[:,2:]/2,
                    default_boxes[:,:2] + default_boxes[:,2:]/2], 1) # x1,y1,x2,y2
        )
        return iou

    def encode(self, boxes, classes, threshold=0.5):
        '''transform target bounding boxes and class labels to SSD boxes and classes
        
        match each object box to all the default boxes, pick the ones with the
        jaccard_Index > 0.5:
            jaccard_Index(A,B) = AB / (A+B-AB)
        
        Args:
            boxes: (tensor) object bounding boxes (x1,y1,x2,y2) of aimage, sized [#obj, 4]
            classes: (tensor) object class labels of a image, sized [#obj,]
            threshold: (float) jaccard index threshold
        Returns:
            boxes: (tensor) bounding boxes, sized [M, 4], M is the number of default boxes
            classes: (tensor) class labels, sized [M,]
        '''
        logging.info('\n\n encode')

        default_boxes = self.default_boxes
        num_default_boxes = default_boxes.size(0)
        num_objs = boxes.size(0)

        iou = self.iou( # [N,M]
            boxes,
            torch.cat([default_boxes[:,:2] - default_boxes[:,2:]/2,
                    default_boxes[:,:2] + default_boxes[:,2:]/2], 1) # x1,y1,x2,y2
        )
        
        logging.info('iou.size()')
        logging.info(iou.size())

        iou, max_idx = iou.max(0)  # [,M] pick the max iou label box for every default box 
        
        logging.info('iou.max()')
        logging.info(iou.size())

        max_idx.squeeze_(0)
        iou.squeeze_(0)
        
        logging.info('boxes1')
        logging.info(boxes.size())

        boxes = boxes[max_idx] # [M, 4] every default box is most jaccard_index to boxes_label

        logging.info('boxes2')
        logging.info(boxes.size())

        variances = [0.1, 0.2]
        cxcy = (boxes[:,:2] + boxes[:,2:])/2 - default_boxes[:,:2]  # center distance
        cxcy /= variances[0] * default_boxes[:,2:]  # why
        wh = (boxes[:,2:] - boxes[:,:2]) / default_boxes[:,2:] # size
        wh = torch.log(wh) / variances[1]
        loc = torch.cat([cxcy, wh], 1) # [M,4]

        logging.info('label loc')
        logging.info(loc.size())

        conf = 1 + classes[max_idx]
        conf[iou<threshold] = 0
        
        logging.info('loc')
        logging.info(loc.size())
        logging.info('conf')
        logging.info(conf.size())
        return loc, conf
    
    def nms(self, bboxes, scores, threshold=0.5, mode='union'):
        x1 = bboxes[:,0]
        y1 = bboxes[:,1]
        x2 = bboxes[:,2]
        y2 = bboxes[:,3]

        areas = (x2-x1) * (y2-y1)
        _, order = scores.sort(0, descending=True)

        keep = []
        while order.numel() > 0:
            i = order[0]
            keep.append(i)

            if order.numel() == 1:
                break
            
            xx1 = x1[order[1:]].clamp(min=x1[i])
            yy1 = y1[order[1:]].clamp(min=y1[i])
            xx2 = x2[order[1:]].clamp(max=x2[i])
            yy2 = y2[order[1:]].clamp(max=y2[i])

            w = (xx2-xx1).clamp(min=0)
            h = (yy2-yy1).clamp(min=0)
            inter = w*h

            if mode == 'union':
                ovr = inter / (areas[i] + areas[order[1:]] - inter)
            elif mode == 'min':
                a = 1
            
            ids = (ovr<=threshold).nonzero().squeeze()
            if ids.numel() == 0:
                break
            order = order[ids+1]
        return torch.LongTensor(keep)

    def decode(self, loc, conf):
        variances = [0.1, 0.2]

        wh = torch.exp(loc[:,2:]*variances[1]) * self.default_boxes[:,2:]
        cxcy = loc[:,:2]*variances[0]*self.default_boxes[:,2:] + self.default_boxes[:,:2]
        boxes = torch.cat([cxcy-wh/2, cxcy+wh/2],1)

        max_conf, labels = conf.max(1)
        # squeeze是用于多类别的？？？
        # ids = labels.squeeze(1).nonzero().squeeze(1)
        # print('labels', labels)
        ids = labels.nonzero()
        if ids.sum() == 0:
            _,confs = conf.sort(0,descending=True)
            ids = confs[:3,1]
            print('has none foreground')
        else:
            print('has forground')
        print('ids', ids)
        keep = self.nms(boxes[ids], max_conf[ids])
        print('nms over')
        return boxes[ids][keep], labels[ids][keep], max_conf[ids][keep]