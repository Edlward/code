import cv2
import numpy as np
import gzip
import cPickle

f = gzip.open('../data/mnist.pkl.gz', 'rb')
training, validation, test = cPickle.load(f)
f.close()

im = np.zeros((28,28,1), np.uint8)

print('len(train[0][0])', len(training[0][0]))

im = np.reshape(training[0][0], (28, 28))

cv2.imshow('test', im)
cv2.waitKey(0)
cv2.destroyAllWindows()




