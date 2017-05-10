# -*- coding: utf-8 -*-
import caffe
from caffe import layers as L,params as P,to_proto

path='/home/lxg/codedata/lenet/'                    #保存数据和配置文件的路径
train_lmdb=path+'mnist_train_lmdb'                  #训练数据LMDB文件的位置
test_lmdb=path+'mnist_test_lmdb'                      #验证数据LMDB文件的位置
# mean_file=path+'mean.binaryproto'                 #均值文件的位置
train_proto=path+'train.prototxt'           #生成的训练配置文件保存的位置
test_proto=path+'test.prototxt'             #生成的验证配置文件保存的位置
predict_proto = path+'predict.prototxt'      #预测没有标签的数据

NET_TYPE_TRAIN = 0
NET_TYPE_TEST = 1
NET_TYPE_VALIDATION = 2
NET_TYPE_PREDICT = 3

#编写一个函数，用于生成网络
def create_net(lmdb, batch_size, net_typeflg = 0):
    n = caffe.NetSpec()
    #创建第一层：数据层。向上传递两类数据：图片数据和对应的标签
    # data, label = L.Data(source=lmdb, backend=P.Data.LMDB, batch_size=batch_size, ntop=2,
    #     transform_param=dict(crop_size=40,mean_file=mean_file,mirror=True))
    if net_typeflg != NET_TYPE_PREDICT:
        n.data, n.label = L.Data(source=lmdb, backend=P.Data.LMDB, batch_size=batch_size, ntop=2
            ,transform_param=dict(scale=1./255))
        #创建第二屋：卷积层
        n.conv1=L.Convolution(n.data, kernel_size=5, stride=1, num_output=20, weight_filler=dict(type='xavier'))
    else:
        n.conv1=L.Convolution(bottom='data', kernel_size=5, stride=1, num_output=20,weight_filler=dict(type='xavier'))
    
    #创建激活函数层
    n.relu1=L.ReLU(n.conv1, in_place=True)
    #创建池化层
    n.pool1=L.Pooling(n.relu1, pool=P.Pooling.MAX, kernel_size=2, stride=2)
    

    n.conv2=L.Convolution(n.pool1, kernel_size=5, stride=1, num_output=50, weight_filler=dict(type='xavier'))
    n.relu2=L.ReLU(n.conv2, in_place=True)
    n.pool2=L.Pooling(n.relu2, pool=P.Pooling.MAX, kernel_size=2, stride=2)
    
    #创建一个全连接层
    n.fc3=L.InnerProduct(n.pool2, num_output=500, weight_filler=dict(type='xavier'))
    n.relu3=L.ReLU(n.fc3, in_place=True)
    #创建一个dropout层
    n.drop3 = L.Dropout(n.relu3, in_place=True)
    n.fc4 = L.InnerProduct(n.drop3, num_output=10, weight_filler=dict(type='xavier'))
    #创建一个softmax层
    if net_typeflg != NET_TYPE_PREDICT:
        n.loss = L.SoftmaxWithLoss(n.fc4, n.label)
    
    #在训练阶段，不需要accuracy层，但是在验证阶段，是需要的
    if net_typeflg == NET_TYPE_TRAIN:             
        return n.to_proto()
    elif net_typeflg == NET_TYPE_TEST:
        n.score = n.fc4
        # n.acc = L.Accuracy(n.fc4, n.label)
        return n.to_proto()
    elif net_typeflg == NET_TYPE_PREDICT:
        n.result = L.Softmax(n.fc4)
        return n.to_proto()
    
def write_net():
    #将以上的设置写入到prototxt文件
    with open(train_proto, 'w') as f:
        f.write(str(create_net(train_lmdb, batch_size=64, net_typeflg = NET_TYPE_TRAIN)))

    #写入配置文件    
    with open(test_proto, 'w') as f:
        f.write(str(create_net(test_lmdb, batch_size=32, net_typeflg = NET_TYPE_TEST)))

    #写入配置文件    
    with open(predict_proto, 'w') as f:
        f.write('name:"Lenet"\n')
        f.write('input:"data"\n')
        f.write('input_dim:1\n')
        f.write('input_dim:1\n')
        f.write('input_dim:28\n')
        f.write('input_dim:28\n')
        f.write(str(create_net(' ', batch_size = 1, net_typeflg = NET_TYPE_PREDICT)))
        
if __name__ == '__main__':
    write_net()