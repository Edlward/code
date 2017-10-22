# encoding=utf-8

import math

import torch
import torch.nn as nn
import torch.nn.init as init
import torch.nn.functional as F

from torch.autograd import Variable

import logging
# logging.basicConfig(level=logging.INFO)
# logging.basicConfig(level=logging.WARN)
logging.basicConfig(level=logging.ERROR)


class MultiBoxLoss(nn.Module):
    num_classes = 2

    def __init__(self, use_gpu):
        super(MultiBoxLoss, self).__init__()
        self.use_gpu = use_gpu

    def cross_entropy_loss(self, x, y):
        '''cross entropy loss w/o averaging across all samples???

        Args:
            x: (tensor) sized [N,D], N is the number of box, D is the class number
            y: (tensor) sized [N,]
        
        Returns:
            (tensor) cross entropy loss, sized [N,]
        '''
        logging.info('\n\ncross_entorpy_loss')
        logging.info('conf_preds')
        logging.info(x.size())
        logging.info('conf_targets')
        logging.info(y.size())
        
        xmax = x.data.max() # float
        
        logging.info('xmax')
        logging.info(type(xmax))
        # logging.info(xmax.shape)

        log_sum_exp = torch.log(torch.sum(torch.exp(x-xmax), 1, keepdim=True)) + xmax
        return log_sum_exp - x.gather(1, y.view(-1,1))
    
    def test_cross_entropy_loss(self):
        a = Variable(torch.randn(10,4))
        b = Variable(torch.ones(10).long())
        loss = self.cross_entropy_loss(a,b)
        print(loss.mean())
        print(F.cross_entropy(a,b))

    def hard_negative_mining(self, conf_loss, pos):
        '''Return negative indices that is 3x the number as positive indices

        Args:
            conf_loss: (tensor) cross entroy loss between conf_preds and conf_targets
            pos: (tensor) positive(matched) box indices
        
        Return:
            (tensor) negative indices
        '''
        
        logging.info('\n\nhard_negative_mining')
        logging.info('conf_loss')
        logging.info(conf_loss.size())
        logging.info('pos')
        logging.info(pos.size())
        logging.info(conf_loss.sum())

        batch_size, num_boxes = pos.size()  # [batch_size, M]
        # conf_loss = conf_loss.view(batch_size,-1)
        pos_temp = pos.view(-1,1)
        conf_loss[pos_temp] = 0    # set pos boxes = 0, the rest are neg conf_loss
        conf_loss = conf_loss.view(batch_size, -1)

        logging.info('conf_loss after view')
        logging.info(conf_loss.size())
        logging.info('conf_loss.sum')
        logging.info(type(conf_loss))

        _, idx = conf_loss.sort(1, descending=True)
        logging.info('idx')
        logging.info(idx.size())
        
        _, rank = idx.sort(1)

        logging.info('rank')
        logging.info(rank.size())

        num_pos = pos.long().sum(1, keepdim=True)
        logging.info('num_pos')
        logging.info(num_pos.size())

        num_neg = torch.clamp(3*num_pos, max=num_boxes-1)
        logging.info('num_neg')
        logging.info(num_neg.size())
        
        neg = rank < num_neg.expand_as(rank)   # [N, M]
        return neg

    def log_sum_exp2(self, x):
        """Utility function for computing log_sum_exp while determining
        This will be used to determine unaveraged confidence loss across
        all examples in a batch.
        Args:
            x (Variable(tensor)): conf_preds from conf layers
        """
        x_max = x.data.max()
        # return torch.log(torch.sum(torch.exp(x-x_max), 1, keepdim=True)) + x_max
        return torch.log(torch.sum(torch.exp(x-x_max), 1, keepdim=True)) + x_max
        

    def forward(self, loc_preds, loc_targets, conf_preds, conf_targets):
        '''compute loss between label and predict

        Args:
            loc_preds: Variable(tensor) sized [batch_size, M, 4], M is the number of default box
            loc_targets: Variable(tensor) sized [batch_size, M, 4]
            conf_preds: Variable(tensor) sized [batch_size, M, num_class]
            conf_targets: Variable(tensor) sized [batch_size, M]
        
        Returns:
            loss: (tensor) 
            loss = SmoothL1Loss(loc_preds, loc_targets) + CrossEntropyLoss(conf_preds, conf_targets).
        '''
        logging.info('\n\nmultiloss forward)')
        logging.info('loc_preds')
        logging.info(loc_preds.size())
        logging.info(type(loc_preds))
        logging.info('loc_targets')
        logging.info(loc_targets.size())
        logging.info(type(loc_targets))
        logging.info('conf_preds')
        logging.info(conf_preds.size())
        logging.info(type(conf_preds))
        logging.info('conf_targets')
        logging.info(conf_targets.size())
        logging.info(type(conf_targets))
        logging.info(type(conf_targets.data))  #torch.cuda.LongTensor
        

        # if self.use_gpu:
            # loc_targets = loc_targets.cuda()
            # conf_targets = conf_targets.cuda()

        batch_size, num_boxes, _ = loc_preds.size()
        pos = conf_targets > 0  # pos means the default box matched  [batch_size, M]
    
        num_matched_boxes = pos.data.long().sum()
        logging.warn('num_matched_boxes:')
        logging.warn(num_matched_boxes)
        if num_matched_boxes == 0:
            return Variable(torch.Tensor([0]).cuda(), requires_grad=True)

        # wrap targets
        # loc_targets = Variable(loc_targets, requires_grad=False)
        # conf_targets = Variable(conf_targets, requires_grad=False)

        # loc_loss = SmoothL1Loss(pos_loc_preds, pos_loc_targets)  
        # 对于单目标检测是不是可以只回归位置？？
        pos_mask = pos.unsqueeze(2).expand_as(loc_preds)     # [batch_size, M, 4]
        logging.info('pos_mask')
        logging.info(pos_mask.size())
        
        pos_loc_preds = loc_preds[pos_mask].view(-1,4)       # [#pos, 4]
        pos_loc_targets = loc_targets[pos_mask].view(-1,4)   # [#pos, 4]
        loc_loss = F.smooth_l1_loss(pos_loc_preds, pos_loc_targets, size_average=False)

        # conf_loss = CrossEntropyLoss(pos_conf_preds, pos_conf_targets)
        #           + CrossEntropyLoss(neg_conf_preds, neg_conf_targets)

        # conf_loss = self.cross_entropy_loss(conf_preds.view(-1, self.num_classes), \
        #                                     conf_targets)  #[batch_size*M,]

        # 预测的有前景、背景之分
        # 因为这个地方是one-hot编码，所以和cross-entropy-loss公式有出入的地方
        # cross-entropy-loss = \sum{pi}{log(qi/ \sum qi)} 
        batch_conf = conf_preds.view(-1, self.num_classes)   #[batch_size, M, num_class]
        conf_loss = self.log_sum_exp2(batch_conf) - batch_conf.gather(1, conf_targets.view(-1,1))
        logging.info('conf_loss after cross entropy loss')
        logging.info(conf_loss.size())
        logging.info('pos')
        logging.info(pos.size())
        
        # pos_temp = pos.view(-1,1)
        # logging.info('pos_temp')
        # logging.info(pos_temp.size())
        
        # conf_loss[pos_temp] = 0
        # conf_loss = conf_loss.view(batch_size, -1)
        # _, loss_idx = conf_loss.sort(1, descending=True)
        # _, idx_rank = loss_idx.sort(1)
        # logging.info('idx_rank')
        # logging.info(idx_rank.size())
        neg = self.hard_negative_mining(conf_loss, pos)  #[batch_size, M]

        # num_boxes = pos.size(1)
        # num_pos = pos.long().sum(1, keepdim=True)
        # num_neg = torch.clamp(3*num_pos, max=num_boxes-1)
        # logging.info('num_neg')
        # logging.info(num_neg.size())
        
        # neg = idx_rank < num_neg.expand_as(idx_rank)
        logging.info('\n\nget neg')
        logging.info(neg.size()) # [batch_size,M]

        pos_mask = pos.unsqueeze(2).expand_as(conf_preds)
        neg_mask = neg.unsqueeze(2).expand_as(conf_preds)

        logging.info('pos_mask')
        logging.info(pos_mask.size())
        logging.info(neg_mask.size())

        mask = (pos_mask+neg_mask).gt(0)

        logging.info('mask')
        logging.info(mask.size())
        logging.info('conf_preds')
        logging.info(conf_preds.size())

        preds = conf_preds[mask].view(-1, self.num_classes)
        # preds = conf_preds[mask]
        
        logging.info('preds')

        pos_and_neg = (pos+neg).gt(0)
        targets = conf_targets[pos_and_neg]
        logging.info('targets')

        conf_loss = F.cross_entropy(preds, targets, size_average=False)

        loc_loss /= num_matched_boxes
        conf_loss /= num_matched_boxes
        # print('%f %f' % (loc_loss.data[0], conf_loss.data[0]), end=' ')
        # print('%f, %f' % (loc_loss.data[0], conf_loss.data[0]))
        logging.warn(loc_loss+conf_loss)
        return loc_loss + conf_loss
