import caffe
import pandas as pd

image = pd.read_csv('data/csv/test.csv')
image = image.values
image = image.reshape(image.shape[0],1,28,28)
image = image * 1. / 255

model_define = 'net/testnotval.prototxt'
model_file = 'net/lenet_iter_500.caffemodel'

net = caffe.Classifier(model_define, model_file)

print_interval = image.shape[0] / 20

result = []
for i in range(image.shape[0]):
    net.blobs['data'].data[0] = image[i]
    out = net.forward()
    result.append(out['loss'].argmax())
    if i % print_interval == 0:
        print('has down ', i)

ImageId = range(image.shape[0]+1)
ImageId = ImageId[1:]
submit_pd = pd.DataFrame({'ImageId':ImageId, 'Label':result})
submit_pd.to_csv('data/csv/result.csv', index=False)