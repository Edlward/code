import torch
import torchvision.utils as utils
import torchvision.transforms as transforms

from PIL import Image, ImageOps, ImageDraw
from simDatagen import ListDataset
from simEncoder import DataEncoder
import matplotlib.pyplot as plt

default_box_size = 60
image_idx = 80
# test ListDataset
transform = transforms.Compose([transforms.ToTensor()])
# trainset = ListDataset(root='/home/lxg/codedata/headXml/walmat/',
#                         list_file='/home/lxg/codedata/headXml/walmat/label_walmat_train.txt',
#                         train=True,
#                         transform=transform,
#                         enlargeWithBg=False)
trainset = ListDataset(root='/home/lxg/codedata/headXml/srcImage/',
                        list_file='/home/lxg/codedata/headXml/srcImage/train_label.txt',
                        train=True,
                        transform=transform,
                        enlargeWithBg=True)
dataencoder = DataEncoder()

# # test DataEncoder
# print('dataEncoder', dataencoder.default_boxes.size())
# pilimg = Image.open('img.jpg')
# draw = ImageDraw.Draw(pilimg)
# for item in dataencoder.default_boxes:
#     w,h = pilimg.size
#     item_box = item * torch.Tensor([w,h,w,h])
#     item_box[2] = item_box[0]+1
#     item_box[3] = item_box[1]+1
#     # print(item_box)
#     draw.rectangle(item_box.tolist())
# draw.rectangle([0,0,default_box_size,default_box_size])
# pilimg.save('pilimgdefault.jpg', 'jpeg')

# # draw label box
# img, boxes = trainset.idxTest(image_idx)
# utils.save_image(img, filename='labelimg.jpg')
# img = Image.open('labelimg.jpg')
# draw = ImageDraw.Draw(img)
# print(type(img))
# print(boxes)
# print(img.size)
# for item in boxes:
#     w,h = img.size
#     item_box = item * torch.Tensor([w,h,w,h])
#     # logging.info(item.tolist())
#     draw.rectangle(item_box.tolist())
#     # logging.info('boxes')
#     # logging.info(item)
# draw.rectangle([0,0,default_box_size,default_box_size])
# img.save('labelimg.jpg', 'jpeg')

# test iou
# box1 = trainset.boxes[0]
# box1[0] = torch.Tensor([0.,0.,0.01,0.01])

# default_boxes = dataencoder.default_boxes
# box2 = torch.cat([default_boxes[:3,:2] - default_boxes[:3,2:]/2,
#                 default_boxes[:3,:2] + default_boxes[:3,2:]/2], 1)

# print('box1', box1)
# print('box2', box2)

# N = box1.size(0)
# M = box2.size(0)
# lt = torch.max(
#     box1[:,:2].unsqueeze(1).expand(N,M,2),  # [N,2] -> [N,1,2] -> [N,M,2]
#     box2[:,:2].unsqueeze(0).expand(N,M,2),  # [M,2] -> [1,M,2] -> [N,M,2]
# )
# rb = torch.min(
#     box1[:,2:].unsqueeze(1).expand(N,M,2),  # [N,2] -> [N,1,2] -> [N,M,2]
#     box2[:,2:].unsqueeze(0).expand(N,M,2),  # [M,2] -> [1,M,2] -> [N,M,2]
# )

# wh = rb - lt # [N,M,2]
# wh[wh<0] = 0 # clip at 0
# inter = wh[:,:,0] * wh[:,:,1] # [N,M]
# area1 = (box1[:,2]-box1[:,0]) * (box1[:,3]-box1[:,1]) # [N,]
# area2 = (box2[:,2]-box2[:,0]) * (box2[:,3]-box2[:,1]) # [M,]
# area1 = area1.unsqueeze(1).expand_as(inter) # [N,] -> [N,1,]-> [N,M]
# area2 = area2.unsqueeze(0).expand_as(inter) # [M,] -> [1,M] -> [N,M]
# iou = inter / (area1 + area2 - inter)
# print('iou', iou)
# print('lt', lt)
# iou, max_idx = iou.max(0)  # [,M] pick the max iou label box for every default box 
# max_idx.squeeze_(0)
# iou.squeeze_(0)
# print('max_idx', type(max_idx), max_idx)
# print('iou', iou)
# print('box1', box1)
# box1 = box1[max_idx] # [M, 4] every default box is most jaccard_index to boxes_label
# print('box1', box1)

# variances = [0.1, 0.2]
# cxcy = (box1[:,:2] + box1[:,2:])/2 - box2[:3,:2]  # center distance
# cxcy /= variances[0] * box2[:3,2:]  # why
# print('cxcy', cxcy)
# print('box2', box2[:3,2:])

# wh = (box1[:,2:] - box1[:,:2]) / box2[:3, 2:] # size
# wh = torch.log(wh) / variances[1]
# loc = torch.cat([cxcy, wh], 1) # [M,4]
# print('loc', loc)
# # conf = 1 + classes[max_idx]
# # conf[iou<threshold] = 0
# print(iou<0.5)

# test data
def test_data(image_idx=image_idx):
    fname, img, boxes, conf_target = trainset.idxTest(image_idx)       
    # print('loc_target', type(loc_target), loc_target.size())
    # print('conf_target', type(conf_target), conf_target.size())
    pos = conf_target > 0
    num_matched_boxes = pos.long().sum()
    print('%s \t  matched default boxes num: %d' % (fname, num_matched_boxes))
    utils.save_image(img, filename='img.jpg')
    pilimg = Image.open('img.jpg')
    draw = ImageDraw.Draw(pilimg)
    draw.rectangle([0,0,default_box_size,default_box_size], outline=255)

    for box in boxes:
        w,h = pilimg.size
        box *= torch.Tensor([w,h,w,h])
        draw.rectangle(box.tolist())

    len = conf_target.size(0)
    for i in range(len):
        if(conf_target[i] > 0):
            w,h = pilimg.size
            item = dataencoder.default_boxes[i] * torch.Tensor([w,h,w,h])
            item[2] = item[0]+1
            item[3] = item[1]+1
            # print(item)
            draw.rectangle(item.tolist())
    # pilimg.save('pilimg.jpg', 'jpeg')
    plt.figure('head', figsize=(6,6))
    plt.imshow(pilimg)
    plt.show()

for i in range(len(trainset)):
    test_data(i)
        