# coding: utf-8
import numpy as np
import os
import tensorflow as tf
import matplotlib.pyplot as plt

train_dir = '/home/lxg/codedata/tensorflow/train/'

cats = []
label_cats = []
dogs = []
label_dogs = []

random_crop_x = 24
random_crop_y = 24
num_channels = 3

def get_files(file_dir):
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
    image = tf.random_crop(image, size = [random_crop_x, random_crop_y, num_channels])
    # image = tf.image.per_image_standardization(image)
    image_batch, label_batch = tf.train.batch([image, label],
                                                batch_size= batch_size,
                                                num_threads= 3, 
                                                capacity = capacity)
    #重新排列label，行数为[batch_size]
    label_batch = tf.reshape(label_batch, [batch_size])
    # image_batch = tf.cast(image_batch, tf.float32)
    return image_batch, label_batch

BATCH_SIZE = 2
CAPACITY = 256
IMG_W = 208
IMG_H = 208
image_list, label_list = get_files(train_dir)
image_batch, label_batch = get_batch(image_list, label_list, IMG_W, IMG_H, BATCH_SIZE, CAPACITY)

with tf.Session() as sess:
    i = 0
    coord = tf.train.Coordinator()
    threads = tf.train.start_queue_runners(coord=coord)

    try:
        while not coord.should_stop() and i<2:

            img, label = sess.run([image_batch, label_batch])

            # just test one batch
            for j in np.arange(BATCH_SIZE):
                print('label: %d' %label[j])
                plt.imshow(img[j,:,:,:])
                plt.show()
            i+=1

    except tf.errors.OutOfRangeError:
        print('done!')
    finally:
        coord.request_stop()
    coord.join(threads)
