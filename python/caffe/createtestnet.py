# -*- coding: utf-8 -*-

import caffe 
from caffe import layers as L
from caffe import params as P

def create_deploy():
    n = caffe.NetSpec()
    # n.data, n.label = L.HDF5Data(batch_size = batch_size, source = hdf5, ntop = 2)
    # n.data, n.label = L.MemoryData(batch_size = batch_size, channels = 1, height = 28, width = 28, ntop = 2,
    #                         transform_param=dict(scale=1./255))
    # 直接少了data层，
    # n.data = L.Input(shape = 1)
    n.conv1 = L.Convolution(bottom = 'data', kernel_size = 5, num_output=20, weight_filler=dict(type='xavier'))
    n.pool1 = L.Pooling(n.conv1, kernel_size = 2, stride=2, pool=P.Pooling.MAX)
    n.conv2 = L.Convolution(n.pool1, kernel_size = 5, num_output=50, weight_filler=dict(type='xavier'))
    n.pool2 = L.Pooling(n.conv2, kernel_size = 2, stride=2, pool=P.Pooling.MAX)
    
    n.fc1 = L.InnerProduct(n.pool2, num_output=500, weight_filler=dict(type='xavier'))
    n.relu1 = L.ReLU(n.fc1, in_place=True)
    n.score = L.InnerProduct(n.relu1, num_output=10, weight_filler=dict(type = 'xavier'))
    n.loss = L.Softmax(n.score)
    
    return n.to_proto()

netfolder = 'net/'
with open(netfolder + 'testnotval.prototxt', 'w') as f:
    # f.write('name:"Lenet"\n')
    # f.write('input:"data"\n')
    # f.write('input_dim:1\n')
    # f.write('input_dim:1\n')
    # f.write('input_dim:28\n')
    # f.write('input_dim:28\n')
    f.write(str(create_deploy()))


