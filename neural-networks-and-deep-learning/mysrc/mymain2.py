import mymnist_loader as ml
import mynetwork2

train, validation, test = ml.load_data_wrapper()

net = mynetwork2.Network([784, 30, 10], cost=mynetwork2.CrossEntropyCost())
net.large_weight_initializer()
net.SGD(train, 30, 10, 0.1, lmbda = 5.0, \
        evaluation_data = validation,
        monitor_evaluation_accuracy = True)
net.save("network2.txt")