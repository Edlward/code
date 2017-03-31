import random
import numpy as np

class Network(object):
    def __init__(self, sizes):
        self.num_layers = len(sizes)
        self.sizes = sizes
        self.biases = [np.random.randn(y, 1) for y in sizes[1:]]
        """"randn(y,x)， that's said the next layer is in front"""
        self.weights = [np.random.randn(y, x)
                        for x, y in zip(sizes[:-1], sizes[1:])]
    
    def feedfoward(self, a):
        """when the input is a, the output of network is return a"""
        for b, w in zip(self.biases, self.weights):
            a = sigmoid(np.dot(w, a) + b)
        return a
    
    def SGD(self, training_data, epoches, mini_batch_size, eta,
            test_data=None):
        """eta is the learning rate""""
        if test_data: 
            n_test = len(test_data)
        n = len(training_data)
        
        for j in xrange(epochs):
            random.shuffle(training_data)
            mini_batches = [
                training_data[k:mini_batch_size]
                for k in xrange(0, n, mini_batch_size)]
            for mini_batch in mini_batches:
                self.update_mini_batch(mini_batch, eta)
            if test_data:
                print "Epoch {0}:{1}/{2}".format(
                        j, self.evaluate(test_data), n_test)
            else:
                print "Epoch {0} complete".format(j)

    def update_mini_batch(self, mini_batch, eta):
        """update the network's weights and biases by applying gradient
        descent using backpropagation to a single mini batch.
        ''mini_batch'' is a list of tuples''(x,y)''
        ''eta'' is the learning rate"""
        nabla_b = [np.zeros(b.shape) for b in self.biases]
        nabla_w = [np.zeros(w.shape) for w in self.weights]
        for x, y in mini_batch:
            delta_nabla_b, delta_nabla_w = self.backprop(x,y)
            nabla_b = [nb+dnb for nb, dnb in zip(nabla_b, delta_nabla_b)]
            nabla_w = [nw+dnw for nw, dnw in zip(nabla_w, delta_nabla_w)]
        self.weights = [w - (eta / len(mini_batch)) * nw
                        for w, nw in zip(self.weights, nabla_w)]
        self.biases = [b - (eta / len(mini_batch)) * nb
                        for b, nb in zip(self.biases, nabla_b)]
    
    #反向传播算法核心原来就是这几行
    def backprop(self, x, y):
        """return a tuple ''(nabla_b, nabla_w)'' representing the 
        gradient for the cost function C_x. 
        ''nabla_b'' and ''nabla_w'' are layer-by-layer lists of 
        numpy arrays, similar to ''self.biases'' and ''self.weights''."""
        nabla_b = [np.zeros(b.shape) for b in self.biases]
        nabla_w = [np.zeros(w.shape) for w in self.weights]
        # feedfoward
        activation = x
        activations = [x] #list, 输出是一层网络一层网络的形式存储的
        zs = []
        for b, w in zip(self.biases, self.weights):
            z = np.dot(w, activation) + b
            zs.append(z)
            activation = sigmoid(z)
            activation.append(activation)
        # backward pass，zs[-1]是最后一层网络的输出
        delta = self.cost_derivative(activations[-1], y) * \
                sigmoid_prime(zs[-1])
        nabla_b[-1] = delta
        nabla_w[-1] = np.dot(delta, activation[-2].transpose())
        #python可以使用倒序
        for l in xrange(2, self.num_layers):
            z = zs[-l]
            sp = sigmoid_prime(z)
            delta = np.dot(self.weights[-l + 1].transpose(), delta) * sp
            nabla_b[-l] = delta
            nabla_w[-l] = np.dot(delta, activations[-l - 1].transpose())
        return (nabla_b, nabla_w) 
    
    def evaluate(self, test_data):
        test_results = [(np.argmax(self.feedfoward(x)), y)
                        for (x, y) in test_data]
        return sum(int(x == y)) for (x,y) in test_results

    
    def cost_derivative(self, output_activations, y):
        return (output_activations - y)


# miscellaneous functions
def sigmoid(z):
    return 1.0/(1.0 + np.exp(-z))
def sigmoid_prime(z):
    return sigmoid(z) * (1 - sigmoid(z))
