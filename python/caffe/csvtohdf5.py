import pandas as pd
from sklearn.model_selection import train_test_split
import h5py
import numpy as np

dirname = 'data/'
foldername = 'hdf5/'


data = pd.read_csv( dirname + 'train.csv')
# labels = data['label']
labels = data.iloc[0:500, :1]
images = data.iloc[0:500, 1:]

train_image, test_image, train_label, test_label = train_test_split(images, labels, train_size = 0.8, random_state=0)
train_image *= 1./255
test_image *= 1./255

with h5py.File(dirname + foldername + 'train.h5', 'w') as f:
    f['data'] = train_image
    f['label'] = labels.astype(np.float32)
with open(dirname + foldername + 'train.txt', 'w') as f:
    f.write(dirname + foldername + 'train.h5\n')


test_filename = 'test.h5'
with h5py.File(dirname + foldername + test_filename, 'w') as f:
    f['data'] = test_image
    f['label'] = test_label
with open(dirname + foldername + 'test.txt', 'w') as f:
    f.write(dirname + foldername + test_filename + '\n')