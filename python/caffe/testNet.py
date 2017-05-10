import caffe
import lmdb
import numpy as np
import pandas as pd
from pylab import *

path = '/home/lxg/codedata/lenet/'
model_define = path+'predict.prototxt'
model_file = path+'net/lenet_iter_500.caffemodel'
net = caffe.Classifier(model_define, model_file)
# image = pd.read_csv('data/csv/test.csv')
# image = image.values
# image = image.reshape(image.shape[0],1,28,28)
# image = image * 1. / 255


## lmdb
filename = path+'mnist_train_lmdb'
env = lmdb.open(filename)
txn = env.begin()
cursor = txn.cursor()
cursor.next()
datum = caffe.proto.caffe_pb2.Datum()
# 解析datum数据
datum = datum.FromString(cursor.value())
# 解析data数据
image = np.fromstring(datum.data, dtype = np.uint8)
image = image.reshape(1,1,28,28)
image = image * 1. / 255
imshow(image[0, 0])
show()


net.blobs['data'].data[0] = image[0]
out = net.forward()
