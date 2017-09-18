import tensorflow as tf

with tf.Graph().as_default():
    output_graph_def = tf.GraphDef()
    with open('minist.pb', "rb") as f:
    # with open('/home/lxg/codedata/tensorflow/frozen_inference_graph.pb', "rb") as f:
        output_graph_def.ParseFromString(f.read())
        _ = tf.import_graph_def(output_graph_def, name="")
        writer = tf.summary.FileWriter("/home/lxg/codedata/tensorflow/log", tf.get_default_graph())