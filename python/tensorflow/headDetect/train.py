# coding: utf-8
import numpy as np
import os
import tensorflow as tf
import matplotlib.pyplot as plt

from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import tensorflow as tf
import os
import glob
from skimage import io, transform
from tensorflow.python.framework import graph_util
import collections


path = '/home/lxg/codedata/headDataset/testHair/'
w = 24
h = 24
c = 3


def read_img(path):
    cate   = [path + x for x in os.listdir(path) if os.path.isdir(path + x)]                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
    imgs   = []
    labels = []
    for idx, folder in enumerate(cate):
        for im in glob.glob(folder + '/*.jpg'):
            print('reading the image: %s' % (im))
            img = io.imread(im)
            img = transform.resize(img, (w, h, c))
            imgs.append(img)
            labels.append(idx)
    return np.asarray(imgs, np.float32), np.asarray(labels, np.int32)

data, label = read_img(path)
num_example = data.shape[0]
arr = np.arange(num_example)
np.random.shuffle(arr)
data = data[arr]
label = label[arr]

ratio = 0.8
s = np.int(num_example * ratio)
x_train = data[:s]
y_train = label[:s]
x_val   = data[s:]
y_val   = label[s:]


## loadData
train_dir = '/home/lxg/codedata/headDataset/trainHair/'

random_crop_x = 24
random_crop_y = 24
num_channels = 3

def get_files(file_dir):
    cats = []
    label_cats = []
    dogs = []
    label_dogs = []
    for file in os.listdir(file_dir+'/bg'):
            cats.append(file_dir +'/bg'+'/'+ file) 
            label_cats.append(0)
    for file in os.listdir(file_dir+'/headPos'):
            dogs.append(file_dir +'/headPos'+'/'+file)
            label_dogs.append(1)
    #把cat和dog合起来组成一个list（img和lab）
    image_list = np.hstack((cats, dogs))
    label_list = np.hstack((label_cats, label_dogs))
    #利用shuffle打乱顺序
    temp = np.array([image_list, label_list])
    temp = temp.transpose()
    np.random.shuffle(temp)
    #从打乱的temp中再取出list（img和lab）
    image_list = list(temp[:, 0])
    label_list = list(temp[:, 1])
    label_list = [int(i) for i in label_list]
    return image_list, label_list

def get_batch(image, label, image_W, image_H, batch_size, capacity):
    #转换类型
    image = tf.cast(image, tf.string)
    label = tf.cast(label, tf.int32)
    # make an input queue
    input_queue = tf.train.slice_input_producer([image, label])
    label = input_queue[1]
    image_contents = tf.read_file(input_queue[0]) #read img from a queue
    # 将图像解码，不同类型的图像不能混在一起，要么只用jpeg，要么只用png等。
    image = tf.image.decode_jpeg(image_contents, channels=3) 
    # image = tf.image.resize_image_with_crop_or_pad(image, image_W, image_H)
    image = tf.random_crop(image, size = [image_W, image_H, num_channels])

    # image = tf.image.per_image_standardization(image)
    image_batch, label_batch = tf.train.batch([image, label],
                                                batch_size= batch_size,
                                                num_threads= 3, 
                                                capacity = capacity)
    print(type(image_batch))
    print(image_batch.get_shape())   # batch_size                                             
    #重新排列label，行数为[batch_size]
    label_batch = tf.reshape(label_batch, [batch_size])
    # image_batch = tf.cast(image_batch, tf.float32)
    return image_batch, label_batch



def build_network(height, width, channel):
    x = tf.placeholder(tf.float32, shape=[None, height, width, channel], name='input')
    y = tf.placeholder(tf.int64, shape=[None, 2], name='labels_placeholder')

    def weight_variable(shape, name="weights"):
        initial = tf.truncated_normal(shape, dtype=tf.float32, stddev=0.1)
        return tf.Variable(initial, name=name)

    def bias_variable(shape, name="biases"):
        initial = tf.constant(0.1, dtype=tf.float32, shape=shape)
        return tf.Variable(initial, name=name)

    def conv2d(input, w):
        return tf.nn.conv2d(input, w, [1, 1, 1, 1], padding='SAME')

    def pool_max(input):
        return tf.nn.max_pool(input,
                               ksize=[1, 2, 2, 1],
                               strides=[1, 2, 2, 1],
                               padding='SAME',
                               name='pool1')

    def fc(input, w, b):
        # conv = tf.matmul(input, x)
        return tf.matmul(input, w) + b

    # conv1
    with tf.name_scope('conv1_1') as scope:
        kernel = weight_variable([3, 3, 3, 16])
        biases = bias_variable([16])
        # output_conv1_1 = tf.nn.relu(conv2d(x, kernel) + biases, name=scope)
        conv = conv2d(x, kernel)
        # bias = tf.nn.bias_add(conv, biases)
        output_conv1_1 = tf.nn.relu(conv, name = scope)

    pool1 = pool_max(output_conv1_1)

    # conv2
    with tf.name_scope('conv1_2') as scope:
        kernel = weight_variable([3, 3, 16, 32])
        biases = bias_variable([32])
        # output_conv5_1 = tf.nn.relu(conv2d(pool4, kernel) + biases, name=scope)
        conv = conv2d(pool1, kernel)
        # bias = tf.nn.bias_add(conv, biases)
        output_conv1_2 = tf.nn.relu(conv, name = scope)

    pool2 = pool_max(output_conv1_2)

    #fc1
    with tf.name_scope('fc1') as scope:
        
        shape = int(np.prod(pool2.get_shape()[1:]))
        print("fc1 shape")
        print(shape)
        print()
        kernel = weight_variable([shape, 2])
        # kernel = weight_variable([25088, 4096])
        biases = bias_variable([2])
        pool2_flat = tf.reshape(pool2, [-1, shape])
        # pool2_flat = tf.reshape(pool2, [-1, 25088])
        # output_fc6 = tf.nn.relu(fc(pool2_flat, kernel, biases), name=scope)
        matmu = tf.matmul(pool2_flat, kernel)
        # bias = tf.nn.bias_add(matmu, biases)
        output_fc1 = tf.nn.relu(matmu, name = scope)

    finaloutput = tf.nn.softmax(output_fc1, name="softmax")
    cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=finaloutput, labels=y))
    optimize = tf.train.AdamOptimizer(learning_rate=1e-4).minimize(cost)

    prediction_labels = tf.argmax(finaloutput, axis=1, name="output")
    read_labels = y

    correct_prediction = tf.equal(prediction_labels, read_labels)
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

    correct_times_in_batch = tf.reduce_sum(tf.cast(correct_prediction, tf.int32))

    return dict(
        x=x,
        y=y,
        optimize=optimize,
        correct_prediction=correct_prediction,
        correct_times_in_batch=correct_times_in_batch,
        cost=cost,
    )

BATCH_SIZE = 12
CAPACITY = 256
IMG_W = 24
IMG_H = 24
image_list, label_list = get_files(train_dir)
image_batch, label_batch = get_batch(image_list, label_list, IMG_W, IMG_H, BATCH_SIZE, CAPACITY)
length = len(image_list)

# graph is a dict here
def train_network(graph, batch_size, num_epochs, pb_file_path):
    init = tf.global_variables_initializer()
    with tf.Session() as sess:
        coord = tf.train.Coordinator()
        threads = tf.train.start_queue_runners(coord=coord)
        
        sess.run(init)
        epoch_delta = 2
        for epoch_index in range(num_epochs):
            for j in range(length / BATCH_SIZE):    
                img, label = sess.run([image_batch, label_batch])
                # print(type(img))    # numpy.ndarray
                # print(type(label))   # numpy.ndarray
                # print(img.shape)    # 2, 24, 24, 3
                for i in range(BATCH_SIZE):
                    sess.run([graph['optimize']], feed_dict={
                        graph['x']: np.reshape(img[i], (1, 24, 24, 3)),
                        graph['y']: ([[1, 0]] if label[i] == 0 else [[0, 1]]
                    )})
                # sess.run([graph['optimize']], feed_dict={   # 体会到了静态图的麻烦
                #     graph['x']: img,
                #     graph['y']: tf.one_hot(label)
                # })
            if epoch_index % epoch_delta == 0:
                total_batches_in_train_set = 0
                total_correct_times_in_train_set = 0
                total_cost_in_train_set = 0.
                for i in range(12):  # why 12
                    return_correct_times_in_batch = sess.run(graph['correct_times_in_batch'], feed_dict={
                        graph['x']: np.reshape(x_train[i], (1, 24, 24, 3)),
                        graph['y']: ([[1, 0]] if y_train[i] == 0 else [[0, 1]])
                    })
                    mean_cost_in_batch = sess.run(graph['cost'], feed_dict={
                        graph['x']: np.reshape(x_train[i], (1, 24, 24, 3)),
                        graph['y']: ([[1, 0]] if y_train[i] == 0 else [[0, 1]])
                    })
                    total_batches_in_train_set += 1
                    total_correct_times_in_train_set += return_correct_times_in_batch
                    total_cost_in_train_set += (mean_cost_in_batch * batch_size)


                total_batches_in_test_set = 0
                total_correct_times_in_test_set = 0
                total_cost_in_test_set = 0.
                for i in range(3):
                    return_correct_times_in_batch = sess.run(graph['correct_times_in_batch'], feed_dict={
                        graph['x']: np.reshape(x_val[i], (1, 24, 24, 3)),
                        graph['y']: ([[1, 0]] if y_val[i] == 0 else [[0, 1]])
                    })
                    mean_cost_in_batch = sess.run(graph['cost'], feed_dict={
                        graph['x']: np.reshape(x_val[i], (1, 24, 24, 3)),
                        graph['y']: ([[1, 0]] if y_val[i] == 0 else [[0, 1]])
                    })
                    total_batches_in_test_set += 1
                    total_correct_times_in_test_set += return_correct_times_in_batch
                    total_cost_in_test_set += (mean_cost_in_batch * batch_size)

                acy_on_test  = total_correct_times_in_test_set / float(total_batches_in_test_set * batch_size)
                acy_on_train = total_correct_times_in_train_set / float(total_batches_in_train_set * batch_size)
                print('Epoch - {:2d}, acy_on_test:{:6.2f}%({}/{}),loss_on_test:{:6.2f}, acy_on_train:{:6.2f}%({}/{}),loss_on_train:{:6.2f}'.format(
                                                epoch_index,  acy_on_test*100.0, total_correct_times_in_test_set,
                                                total_batches_in_test_set * batch_size,
                                                total_cost_in_test_set,
                                                acy_on_train * 100.0,
                                                total_correct_times_in_train_set,
                                                total_batches_in_train_set * batch_size,
                                                total_cost_in_train_set))

            writer = tf.summary.FileWriter("/home/lxg/codedata/tensorflow/log", tf.get_default_graph())

            constant_graph = graph_util.convert_variables_to_constants(sess, sess.graph_def, ["conv1_2"])
            # constant_graph = graph_util.convert_variables_to_constants(sess, sess.graph_def, ["fc6"])
            
            with tf.gfile.FastGFile(pb_file_path, mode='wb') as f:
                f.write(constant_graph.SerializeToString())


def main():
    batch_size = BATCH_SIZE
    num_epochs = 10

    pb_file_path = "vggs.pb"

    g = build_network(height=24, width=24, channel=3)
    train_network(g, batch_size, num_epochs, pb_file_path)

main()




