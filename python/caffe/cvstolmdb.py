# -*- coding: utf-8 -*-

import caffe
import pandas as pd
import lmdb
import numpy as np
from sklearn.model_selection import train_test_split

train = pd.read_csv('data/csv/train.csv')
train = train.values    #返回ndarray数据

train_image = train[:, 1:]
train_label = train[:, :1]

#训练集、验证集分配
train_image, test_image, train_label, test_label = train_test_split(train_image, train_label, train_size = 0.8)

#要转换成uint8类型才会正常转换成字符串
train_image = train_image.astype(np.uint8)
train_image = train_image.reshape(train_image.shape[0], 1, 28, 28)
train_label = train_label.reshape(train_image.shape[0])
map_size = train_image.nbytes * 10

env = lmdb.open('data/csv2lmdb/train', map_size)
with env.begin(write = True) as txn:
    cursor = txn.cursor()
    for i in range(train_image.shape[0]):
        datum = caffe.proto.caffe_pb2.Datum()
        datum.channels = train_image.shape[1]
        datum.height = train_image.shape[2]
        datum.width = train_image.shape[3]
        datum.data = train_image[i].tobytes()
        datum.label = int(train_label[i])
        str_id = '{:08}'.format(i)
        txn.put(str_id, datum.SerializeToString())



#要转换成uint8类型才会正常转换成字符串
test_image = test_image.astype(np.uint8)
test_image = test_image.reshape(test_image.shape[0], 1, 28, 28)
test_label = test_label.reshape(test_image.shape[0])
map_size = test_image.nbytes * 10

env = lmdb.open('data/csv2lmdb/test', map_size)
with env.begin(write = True) as txn:
    cursor = txn.cursor()
    for i in range(test_image.shape[0]):
        datum = caffe.proto.caffe_pb2.Datum()
        datum.channels = test_image.shape[1]
        datum.height = test_image.shape[2]
        datum.width = test_image.shape[3]
        datum.data = test_image[i].tobytes()
        datum.label = int(test_label[i])
        str_id = '{:08}'.format(i)
        txn.put(str_id, datum.SerializeToString())
