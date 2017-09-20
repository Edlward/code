from __future__ import print_function

import tensorflow as tf
import os

# Dataset Parameters - CHANGE HERE
MODE = 'folder' # or 'file', if you choose a plain text file (see above).
DATASET_PATH = '/path/to/dataset/' # the dataset file or root folder path.

# Image Parameters
N_CLASSES = 2 # CHANGE HERE, total number of classes
IMG_HEIGHT = 64 # CHANGE HERE, the image height to be resized to
IMG_WIDTH = 64 # CHANGE HERE, the image width to be resized to
CHANNELS = 3 # The 3 color channels, change to 1 if grayscale


# Reading the dataset
# 2 modes: 'file' or 'folder'
def read_images(dataset_path, mode, batch_size):
    imagepaths, labels = list(), list()
    if mode == 'file':
        # Read dataset file
        data = open(dataset_path, 'r').read().splitlines()
        for d in data:
            imagepaths.append(d.split(' ')[0])
            labels.append(int(d.split(' ')[1]))
    elif mode == 'folder':
        # An ID will be affected to each sub-folders by alphabetical order
        label = 0
        # List the directory
        try:  # Python 2
            classes = sorted(os.walk(dataset_path).next()[1])
        except Exception:  # Python 3
            classes = sorted(os.walk(dataset_path).__next__()[1])
        # List each sub-directory (the classes)
        for c in classes:
            c_dir = os.path.join(dataset_path, c)
            try:  # Python 2
                walk = os.walk(c_dir).next()
            except Exception:  # Python 3
                walk = os.walk(c_dir).__next__()
            # Add each image to the training set
            for sample in walk[2]:
                # Only keeps jpeg images
                if sample.endswith('.jpg') or sample.endswith('.jpeg'):
                    imagepaths.append(os.path.join(c_dir, sample))
                    labels.append(label)
            label += 1
    else:
        raise Exception("Unknown mode.")

    # Convert to Tensor
    imagepaths = tf.convert_to_tensor(imagepaths, dtype=tf.string)
    labels = tf.convert_to_tensor(labels, dtype=tf.int32)
    # Build a TF Queue, shuffle data
    image, label = tf.train.slice_input_producer([imagepaths, labels],
                                                 shuffle=True)

    # Read images from disk
    image = tf.read_file(image)
    image = tf.image.decode_jpeg(image, channels=CHANNELS)

    # Resize images to a common size
    image = tf.image.resize_images(image, [IMG_HEIGHT, IMG_WIDTH])

    # Normalize
    image = image * 1.0/127.5 - 1.0

    # Create batches
    X, Y = tf.train.batch([image, label], batch_size=batch_size,
                          capacity=batch_size * 8,
                          num_threads=4)

    return X, Y


# -----------------------------------------------
# THIS IS A CLASSIC CNN (see examples, section 3)
# -----------------------------------------------
# Note that a few elements have changed (usage of queues).

# Parameters
learning_rate = 0.001
num_steps = 10000
batch_size = 128
display_step = 100

# Network Parameters
dropout = 0.75 # Dropout, probability to keep units

# Build the data input
X, Y = read_images(DATASET_PATH, MODE, batch_size)


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
        bias = tf.nn.bias_add(conv, biases)
        output_conv1_1 = tf.nn.relu(bias, name = scope)

    pool1 = pool_max(output_conv1_1)

    # conv2
    with tf.name_scope('conv1_2') as scope:
        kernel = weight_variable([3, 3, 16, 32])
        biases = bias_variable([512])
        # output_conv5_1 = tf.nn.relu(conv2d(pool4, kernel) + biases, name=scope)
        conv = conv2d(pool1, kernel)
        bias = tf.nn.bias_add(conv, biases)
        output_conv1_2 = tf.nn.relu(bias, name = scope)

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
        bias = tf.nn.bias_add(matmu, biases)
        output_fc1 = tf.nn.relu(bias, name = scope)
        
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