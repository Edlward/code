# -*- coding: utf-8 -*-

import caffe 
import numpy as np
import lmdb

filename = 'train2lmdb'
env = lmdb.open(filename)
txn = env.begin()
cursor = txn.cursor()
cursor.next()

datum = caffe.proto.caffe_pb2.Datum()

# 解析datum数据
datum = datum.FromString(cursor.value())

# 解析data数据
flat_x = np.fromstring(datum.data, dtype = np.uint8)

datum.channels
datum.width 
datum.height 
datum.label

flat_x
