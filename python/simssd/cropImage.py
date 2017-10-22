import os
import os.path
import random
import matplotlib.pyplot as plt
import numpy as np

from PIL import Image, ImageDraw

def readList(file):
    ''' read file list and return
    Args:
        file: image file path
    Returns:
        fnames: image name
        boxes: label boxes
    '''
    fnames = []
    boxes = []
    with open(file) as f:
        lines = f.readlines()
    for line in lines:
        splited = line.strip().split()
        fnames.append(splited[0])
        num_objs = int(splited[1])
        box = []
        for i in range(num_objs):
            xmin = splited[2+4*i]
            ymin = splited[3+4*i]
            width = splited[4+4*i]
            height = splited[5+4*i]
            box.append([int(xmin), int(ymin), int(width)+int(xmin), int(height)+int(ymin)])
        boxes.append(box)
    return fnames, boxes            


root_path = "/home/lxg/codedata/headXml/srcImage/zqq201709080201/"
names, boxes = readList(root_path + 'label_dark.txt')
flabel_test = open(root_path + 'labelSizeCrop.txt', 'w')

target_size = 400.

for i in range(len(names)):
    name = names[i]
    box = boxes[i]
    file_path = root_path + name
    img = Image.open(file_path)
    imw, imh = img.size
    small_size = min(imw, imh)
    target_ratio = target_size / small_size

    selected_boxes = []
    print(name, box)

    while True:
        # random create a roi rectangele
        xmin = random.randrange(0, imw - small_size + 1)
        ymin = random.randrange(0, imh - small_size + 1)
        roi = [xmin, ymin, xmin + small_size, ymin + small_size]
        print('roi', roi)

        # test if any box is in this box, if is, 
        for item in box:
            # get item box and roi 's iou 
            xselect_left = max(item[0], roi[0])
            yselect_up = max(item[1], roi[1])
            xselect_right = min(item[2], roi[2])
            yselect_down = min(item[3], roi[3])
            select_width = xselect_right - xselect_left
            select_height = yselect_down - yselect_up
            if  select_height < 0 | select_width < 0 | select_width > 1.5*select_height | select_height > 1.5*select_width:
                continue

            select_area = select_height * select_width
            item_area = (item[3] - item[1]) * (item[2] - item[0])
            # if (item[0] >= roi[0]) & (item[1] >= roi[1]) & (item[2] <= roi[2]) & (item[3] <= roi[3]):
            if select_area * 1.0 / item_area > 0.7:
                print('item', item)
                selected_boxes.append([max(item[0]-xmin, 0), max(item[1]-ymin, 0), 
                                    min(item[2]-xmin, small_size), min(item[3]-ymin, small_size)])

        print('selected_boses:', len(selected_boxes))
        if len(selected_boxes) == 0:
            continue

        img_crop = img.crop((xmin, ymin, xmin+small_size, ymin+small_size))
        img_crop = img_crop.resize((int(target_size), int(target_size)))
        file_path_name = os.path.split(name)
        img_crop.save(os.path.join(root_path + 'videoImageCrop/' + file_path_name[1]))

        # draw = ImageDraw.Draw(img_crop)
        # for item in selected_boxes:
        #     draw.rectangle(item, outline=255)
        # plt.figure('head', figsize=(8,8))
        # plt.imshow(img_crop)
        # plt.show()

        flabel_test.write('videoImageCrop/' + file_path_name[1])
        flabel_test.write('\t')
        flabel_test.write(str(len(selected_boxes)))
        flabel_test.write('\t')
        for item in selected_boxes:
            itemx = int(item[0] * target_ratio)
            itemy = int(item[1] * target_ratio)
            item_width = int((item[2]-item[0]) * target_ratio)
            item_height = int((item[3]-item[1]) * target_ratio)
            strvalue = str(itemx) + '\t' + str(itemy) + '\t' + str(item_width) + '\t' + str(item_height) + '\t'            
            flabel_test.write(strvalue)
        flabel_test.write('\n')
        break
flabel_test.close()      
    

