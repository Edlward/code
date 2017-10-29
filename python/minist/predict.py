import os
import torch
import pandas as pd
from torch.autograd import Variable
from torchvision import transforms
from convNet import convNet
from resNet import resnet18
from fcNet import fcNet

use_cuda = True
path = '/home/lxg/codedata/minist/'
print('loading data...')
data = pd.read_csv(os.path.join(path, 'test.csv'))
data = data.as_matrix()
data_img = data.reshape(-1,28,28) # - 33.38  # substract mean value

transform = transforms.Compose([
    transforms.ToTensor()
])
# model = convNet()
model = resnet18()

if use_cuda:
    model.cuda()
print('loading model...')
model.load_state_dict(torch.load(os.path.join(path,'params.pkl')))
model.eval()

predict_label = []
for i in range(data.shape[0]):
# for i in range(100):
    image = data_img[i,:,:].reshape(28,28,-1)
    image = transform(image)
    if use_cuda:
        image = image.cuda()
    image = Variable(image[None,:,:,:], volatile=True)
    outputs = model(image)
    _, predicted = torch.max(outputs.data, 1)
    predict_label.extend(predicted)
    print('{0}/{1}'.format(i, data.shape[0]))

predict_dict = {'ImageId':range(1,len(predict_label)+1), 'Label':predict_label}
predict_series = pd.DataFrame(predict_dict)
predict_series.to_csv(os.path.join(path, 'submit.csv'), index=False)
