import caffe
import lmdb
import numpy as np
import pandas as pd
from pylab import *

path = '/home/lxg/codedata/agegender/'

gender_net_pretrained = path + 'gender_net.caffemodel'
gender_net_model_file = path + 'deploy_gender.prototxt'
gender_net = caffe.Classifier(gender_net_model_file, gender_net_pretrained,
                       mean=mean,
                       channel_swap=(2,1,0),
                       raw_scale=255,
                       image_dims=(256, 256))


gender_list=['Male','Female']
example_image = path + 'example_image.jpg'
input_image = caffe.io.load_image(example_image)
_ = plt.imshow(input_image)

prediction = gender_net.predict([input_image]) 

print 'predicted gender:', gender_list[prediction[0].argmax()]