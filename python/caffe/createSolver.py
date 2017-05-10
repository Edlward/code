from caffe.proto import caffe_pb2

path = '/home/lxg/codedata/lenet/'
train_net_path = path+'train.prototxt'
test_net_path = path+'test.prototxt'
solver_config_path = path+'solver.prototxt'

s = caffe_pb2.SolverParameter()

#set a seed for reproducible experiments
s.random_seed = 0xCAFFE

#specify locations of the train and test networks
s.train_net = train_net_path
s.test_net.append(test_net_path)
s.test_interval = 500
s.test_iter.append(100)

s.max_iter = 10000
s.type = 'SGD'
s.base_lr = 0.01
s.momentum = 0.9

s.weight_decay = 5e-4


s.lr_policy = 'inv'
s.gamma = 0.0001
s.power = 0.75
s.display = 100

s.snapshot = 500
s.snapshot_prefix = path+'net/lenet'

s.solver_mode = caffe_pb2.SolverParameter.CPU

with open(solver_config_path, 'w') as f:
    f.write(str(s))