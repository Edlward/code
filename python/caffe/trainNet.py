# -*- coding: utf-8 -*-
# 共70000条目

## kaggle
# train_size = 33600
# test_size = 8400
# 用于测试的size = 28000

## lenet
# train_size 60000
# test_size 10000

import caffe
import h5py
import numpy as np
from pylab import *

solver_config_path = '/home/lxg/codedata/lenet/solver.prototxt'

solver = None
solver = caffe.get_solver(solver_config_path)

niter = 525     #迭代次数
test_interval = niter / 10  #测试

train_loss = zeros(niter)
test_acc = zeros(int(np.ceil(niter / test_interval)) + 1)

for it in range(niter):
    solver.step(1)

    #记录损失
    train_loss[it] = solver.net.blobs['loss'].data

    if it % test_interval == 0:
        print 'Iteration', it, 'testing'
        correct = 0
        for test_it in range(100):
            solver.test_nets[0].forward()
            correct += sum(solver.test_nets[0].blobs['score'].data.argmax(1) 
                            == solver.test_nets[0].blobs['label'].data)
        test_acc[it // test_interval] = correct / 1e4 # test range = 100, batch_size = 100， 所以除以1e4=10000
        print(correct / 1e4)

_, ax1 = subplots()
ax2 = ax1.twinx()
ax1.plot(arange(niter), train_loss)
ax2.plot(test_interval * arange(len(test_acc)), test_acc, 'r')
ax1.set_xlabel('iteration')
ax1.set_ylabel('train loss')
ax2.set_ylabel('test accuracy')
ax2.set_title('Custom Test Accuracy: {:.2f}'.format(test_acc[-1]))
