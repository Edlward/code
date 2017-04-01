# -*- coding: utf-8 -*- 


import random
import numpy as np

#object指QuadraticCost继承的类，所有的类最终都会继承object，所以可以直接写为object
class QuadraticCost(object):

    @staticmethod
    def fn(a, y):
        """返回代价函数的值， 已经是二范数了，再取平方"""
        return 0.5*np.linalg.norm(a - y) ** 2

    @staticmethod
    def delta(z, a, y):
        """返回输出层的误差"""
        return (a-y) * sigmoid_prime(z)

class CrossEntropyCost(object):
    
    @staticmethod
    def fn(a, y):
        """返回代价函数的值"""
        return np.sum(np.nan_to_num(-y*np.log(a) - (1-y)*np.log(1-a)))

    @staticmethod
    def delta(z, a, y):
        """返回输出层的误差"""
        return (a-y)


class Network(object):
    def __init__(self, sizes, cost=CrossEntropyCost):
        self.num_layers = len(sizes)
        self.sizes = sizes
        #这个是什么意思
        self.default_weight_initializer()
        self.cost = cost

    def default_weight_initializer(self):
        """系数除以了一个np.sqrt(x)"""
        self.biases = [np.random.randn(y, 1)] for y in self.sizes[1:]
        self.weights = [np.random.randn(y, x)/np.sqrt(x) 
                        for x, y in zip(self.sizes[:-1], self.sizes[1:])]

    def large_weight_initializer(self):
        """与上一个network的初始化函数相同"""
        self.biases = [np.random.randn(y, 1) for y in self.sizes[1:]]
        self.weights = [np.random.randn(y, x) 
                        for x, y in zip(self.sizes[:-1], self.sizes[1:])]
    
    def feedforward(self, a):
        for b, w in zip(self.biases, self.weights):
            a = sigmoid(np.dot(w, a) + b)
        return a

    def SGD(self, training_data, epochs, mini_batch_size, eta,
            lmbda = 0.0,
            evaluation_data = None,
            monitor_evaluation_cost = False,
            monitor_evaluation_accuracy = False,
            monitor_training_cost = False,
            monitor_training_accuracy = False):
            """梯度下降法"""
            if evaluation_data:
                n_data = len(evaluation_data)
            n = len(training_data)
            evaluation_cost, evaluation_accuracy = [], []
            training_cost, training_accuracy = [], []
            for j in xrange(epochs):
                random.shuffle(training_data)
                mini_batches = [training_data[k:k+mini_batch_size]
                                for k in xrange(0, n, mini_batch_size)]
                for mini_batch in mini_batches:
                    """为什么要传入len(training_data)"""
                    self.update_mini_batch(mini_batch, eta, lmbda, len(training_data))
                print "epoch %s training complete" % j
                if monitor_training_cost:
                    cost = self.total_cost(training_data, lmbda)
                    training_cost.append(cost)
                    print "cost on training data: {}".format(cost)

        def update_mini_batch(self, mini_batch, eta, lmbda, n):
            """'n' is the total size of the training data set."""
            nabla_b = [np.zeros(b.shape) for b in self.biases]
            nabla_w = [np.zeros(w.shape) for w in self.weights]
            for x, y in mini_batch:
                delta_nabla_b, delta_nabla_w = self.backprop(x, y)
                nabla_b = [nb+dnb for nb, dnb in zip(nabla_b, delta_nabla_b)]
                nabla_w = [nw+dnw for nw, dnw in zip(nabla_w, delta_nabla_w)]
            self.weights




### 杂项函数
def sigmoid(z):
    return 1.0/(1.0 + np.exp(-z))

def sigmoid_prime(z):
    return sigmoid(z) * (1 - sigmoid(z))
