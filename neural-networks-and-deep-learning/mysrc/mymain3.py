# -*- coding: utf-8 -*- 

import mynetwork3

from mynetwork3 import Network
from mynetwork3 import ConvPoolLayer, FullyConnectedLayer, SoftmaxLayer

train, validation, test = mynetwork3.load_data_shared()
mini_batch_size = 18

# net = Network([FullyConnectedLayer(n_in = 784, n_out = 100),
#                 SoftmaxLayer(n_in = 100, n_out = 10)],
#                 mini_batch_size)
# net.SGD(train, 60, mini_batch_size, 0.1, validation, test)

net = Network([ConvPoolLayer(image_shape=(mini_batch_size, 1, 28, 28),
                            filter_shape=(20,1,5,5),
                            poolsize=(2,2)),
                #输入数据是 28 -4(卷积层) / 2(池化层是2*2内选最大值)
                FullyConnectedLayer(n_in = 20*12*12, n_out = 100),
                SoftmaxLayer(n_in = 100, n_out=10)], mini_batch_size)
net.SGD(train, 60, mini_batch_size, 0.1, validation, test)

                    