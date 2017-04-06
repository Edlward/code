# -*- coding: utf-8 -*- 


import random
import numpy as np
import json
import sys

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
    
    def __init__(self):
        self.__name__ = "CrossEntropyCost"
        
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
        self.biases = [np.random.randn(y, 1) for y in self.sizes[1:]]
        self.weights = [np.random.randn(y, x) / np.sqrt(x) 
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
                """为什么要传入len(training_data)，正则化的系数要用到"""
                self.update_mini_batch(mini_batch, eta, lmbda, len(training_data))
            print "epoch %s training complete" % j
            if monitor_training_cost:
                cost = self.total_cost(training_data, lmbda)
                training_cost.append(cost)
                print "cost on training data: {}".format(cost)
            if monitor_training_accuracy:
                accuracy = self.accuracy(training_data, convert = True)
                training_accuracy.append(accuracy)
                print "accuracy on evalutaion data: {}".format(accuracy, n)
            if monitor_evaluation_cost:
                cost = self.total_cost(evaluation_data, lmbda, convert=True)
                evaluation_cost.append(cost)
                print "cost on evaluation data: {}".format(cost)
            if monitor_evaluation_accuracy:
                accuracy = self.accuracy(evaluation_data)
                evaluation_accuracy.append(accuracy)
                print "accuracy on evaluation data: {} / {}".format(
                        self.accuracy(evaluation_data), n_data)
            print
        return evaluation_cost, evaluation_accuracy, \
                training_cost, training_accuracy

    def update_mini_batch(self, mini_batch, eta, lmbda, n):
        """'n' is the total size of the training data set."""
        nabla_b = [np.zeros(b.shape) for b in self.biases]
        nabla_w = [np.zeros(w.shape) for w in self.weights]
        for x, y in mini_batch:
            delta_nabla_b, delta_nabla_w = self.backprop(x, y)
            # print('len(nabla_w)', len(nabla_w))
            # print('len(nabla_w[0])', len(nabla_w[0]))
            # print('len(nabla_w[1])', len(nabla_w[1]))
            # print('len(nabla_w[0][0])', len(nabla_w[0][0]))

            # print('len(delta_nabla_w)', len(delta_nabla_w))
            # print('len(delta_nabla_w[0])', len(delta_nabla_w[0]))
            # print('len(delta_nabla_w[1])', len(delta_nabla_w[1]))
            # print('len(delta_nabla_w[0][0])', len(delta_nabla_w[0][0]))
            # print('len(delta_nabla_w[1][0])', len(delta_nabla_w[1][0]))

            nabla_b = [nb+dnb for nb, dnb in zip(nabla_b, delta_nabla_b)]
            nabla_w = [nw+dnw for nw, dnw in zip(nabla_w, delta_nabla_w)]
        self.weights = [(1-eta*(lmbda/n)) * w - (eta/len(mini_batch)) * nw
                        for w, nw in zip(self.weights, nabla_w)]
        self.biases = [b-(eta/len(mini_batch))*nb
                        for b, nb in zip(self.biases, nabla_b)]

    def backprop(self, x, y):
        """return the gradient for the cost function"""
        nabla_b = [np.zeros(b.shape) for b in self.biases]
        nabla_w = [np.zeros(w.shape) for w in self.weights]
        # feedforward
        activation = x
        activations = [x]
        zs = []
        for b, w in zip(self.biases, self.weights):
            z = np.dot(w, activation) + b
            zs.append(z)
            activation = sigmoid(z)
            activations.append(activation)
        # backward pass, 梯度的后向传播迭代公式
        delta = (self.cost).delta(zs[-1], activations[-1], y)
        nabla_b[-1] = delta
        nabla_w[-1] = np.dot(delta, activations[-2].transpose())

        for l in xrange(2, self.num_layers):
            z = zs[-l]
            sp = sigmoid_prime(z)
            delta = np.dot(self.weights[-l+1].transpose(), delta) * sp
            nabla_b[-l] = delta
            #下面一行的-1写成了+1导致，矩阵元素不匹配
            nabla_w[-l] = np.dot(delta, activations[-l-1].transpose())
        return (nabla_b, nabla_w)
    
    def accuracy(self, data, convert=False):
        """根据训练好的神经网络参数，判断data数据内正确识别的数据"""
        if convert:
            results = [(np.argmax(self.feedforward(x)), np.argmax(y))
                        for x, y in data]
        else:
            results = [(np.argmax(self.feedforward(x)), y)
                        for x, y in data]
        return sum(int(x == y) for (x, y) in results)
    
    def total_cost(self, data, lmbda, convert=False):
        """规则化后的代价函数"""
        cost = 0.0
        for x, y in data:
            a = self.feedforward(x)
            if convert:
                y = vectorized_result(y)
            cost += self.cost.fn(a, y) / len(data)
        cost += 0.5*(lmbda/len(data)) * sum(
            np.linalg.norm(w)**2 for w in self.weights)
        return cost
    def save(self, filename):
        """save network parameter"""
        data = {"sizes": self.sizes,
                "weights": [w.tolist() for w in self.weights],
                "biases": [b.tolist() for b in self.biases],
                "cost": str(self.cost.__name__)}
        f = open(filename, "w")
        json.dump(data, f)
        f.close()

### loading a network
def load(filename):
    f = open(filename, "r")
    data = json.load(f)
    f.close()
    cost = getattr(sys.modules[__name__], data["cost"])
    net = Network(data["sizes"], cost=cost)
    net.weights = [np.array(w) for w in data["weights"]]
    net.biases = [np.array(b) for b in data["biases"]]
    return net

##杂项函数 
def vectorized_result(j):
    e = np.zeros((10, 1)) 
    e[j] = 1.0
    return e

def sigmoid(z):
    return 1.0/(1.0 + np.exp(-z))

def sigmoid_prime(z):
    return sigmoid(z) * (1 - sigmoid(z))
