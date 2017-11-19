import os
import pandas as pd
import shutil
from skimage import io,color,transform
import numpy as np
import time
import cv2
import multiprocessing as mp

NCORE = 8
batch_size = 256
input_size = 254

train_images = pd.read_csv('../train_images.csv')
train_ids = list(train_images['product_id'])
train_idxs = list(train_images['img_idx'])
train_names = []

num_train = 9901753
length_train = num_train/batch_size + 1
for i in range(num_train):
    train_name = '{}_{}.jpg'.format(train_ids[i],train_idxs[i])
    train_names.append(train_name)

val_images = pd.read_csv('../val_images.csv')
val_ids = list(val_images['product_id'])

val_idxs = list(val_images['img_idx'])
val_names = []

num_val = 2469540
length_val = num_val/batch_size
for i in range(num_val):
    val_name = '{}_{}.jpg'.format(val_ids[i],val_idxs[i])
    val_names.append(val_name)


#train_images_df = pd.read_csv('train_example.csv')
def train_make_product_category():
    pro2cat = {}
    #cat2pro = {}
    for ir in train_images.itertuples():
        product_id = ir[-3]
        category_idx = ir[-2]
        pro2cat[product_id] = category_idx
        #cat2pro[category_idx] = pro2cat
    return pro2cat
train_pro2cat = train_make_product_category()
#print(pro2cat)

#val_images_df = pd.read_csv('val_images.csv')
def val_make_product_category():
    pro2cat = {}
    #cat2pro = {}
    for ir in val_images.itertuples():
        product_id = ir[-3]
        category_idx = ir[-2]
        pro2cat[product_id] = category_idx
        #cat2pro[category_idx] = pro2cat
    return pro2cat
val_pro2cat = val_make_product_category()


def train_generator():
    while True:
        for start in range(0, num_train, batch_size):
            x_batch = []
            y_batch = []
            end = min(start + batch_size, num_train)
            ids_train_batch = train_names[start:end]
            for id in ids_train_batch:
                img = cv2.imread('../train/{}'.format(id))
                img = img[10:-10,10:-10,:]
                #img = cv2.resize(img,(input_size,input_size))
                product_id = int(id.split('_')[0])
                category = train_pro2cat[product_id]
                x_batch.append(img)
                y_batch.append(category)
            x_batch = np.array(x_batch, np.float32) / 255
            y_batch = np.array(y_batch,np.int64)
            yield np.rollaxis(x_batch,3,1), y_batch


train = train_generator()
t0 = time.time()
next(train)
t1 = time.time()
print(t1-t0)


next(train)
t2 = time.time()
print(t2-t1)

next(train)
t3 = time.time()
print(t3-t2)

next(train)
t3 = time.time()
print(t3-t2)

next(train)
t3 = time.time()
print(t3-t2)

next(train)
t3 = time.time()
print(t3-t2)

next(train)
t3 = time.time()
print(t3-t2)

def val_generator():
    while True:
        for start in range(0, num_val, batch_size):
            x_batch = []
            y_batch = []
            end = min(start + batch_size, num_val)
            ids_val_batch = val_names[start:end]
            for id in ids_val_batch:
                img = cv2.imread('../val/{}'.format(id))
                img = img[10:-10,10:-10,:]
                #img = cv2.resize(img,(input_size,input_size))
                product_id = int(id.split('_')[0])
                category = val_pro2cat[product_id]
                x_batch.append(img)
                y_batch.append(category)
            x_batch = np.array(x_batch, np.float32) / 255
            y_batch = np.array(y_batch,np.int64)
            yield np.rollaxis(x_batch,3,1), y_batch





