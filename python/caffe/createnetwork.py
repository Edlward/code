import caffe 
from caffe import layers as L
from caffe import params as P

def mnistnet(lmdb, batch_size):
    n = caffe.NetSpec()
    # n.data, n.label = L.HDF5Data(batch_size = batch_size, source = hdf5, ntop = 2)
    n.data, n.label = L.Data(batch_size = batch_size, backend=P.Data.LMDB, source = lmdb,
                                transform_param=dict(scale=1./255),ntop=2)

    n.conv1 = L.Convolution(n.data, kernel_size = 5, num_output=20, weight_filler=dict(type='xavier'))
    n.pool1 = L.Pooling(n.conv1, kernel_size = 2, stride=2, pool=P.Pooling.MAX)
    n.conv2 = L.Convolution(n.pool1, kernel_size = 5, num_output=50, weight_filler=dict(type='xavier'))
    n.pool2 = L.Pooling(n.conv2, kernel_size = 2, stride=2, pool=P.Pooling.MAX)
    
    n.fc1 = L.InnerProduct(n.pool2, num_output=500, weight_filler=dict(type='xavier'))
    n.relu1 = L.ReLU(n.fc1, in_place=True)
    n.score = L.InnerProduct(n.relu1, num_output=10, weight_filler=dict(type = 'xavier'))
    n.loss = L.SoftmaxWithLoss(n.score, n.label)
    
    return n.to_proto()

netfolder = 'net/'
with open(netfolder + 'train.prototxt', 'w') as f:
    f.write(str(mnistnet('data/cvs2lmdb/train', 64)))
with open(netfolder + 'test.prototxt', 'w') as f:
    f.write(str(mnistnet('data/cvs2lmdb/test', 100)))