import os
import torch
import pandas as pd
import numpy as np
import torchvision
from torch.autograd import Variable
from torchvision import transforms
from dataset import DataSet
from convNet import convNet
# from resNet import resnet18
# from fcNet import fcNet


use_cuda = True
path = '/home/lxg/codedata/ice/'

print('define model....')
model = convNet()
# model = torchvision.models.resnet18(); model.fc = torch.nn.Linear(512, 2)
if use_cuda:
    model.cuda()

print('loading model...')
model.load_state_dict(torch.load(os.path.join(path,'params1.pkl')))
model.eval()

def predict(data, batch_size=32):
    data_loader = torch.utils.data.DataLoader(data, 
                                    batch_size=batch_size,
                                    shuffle=False,
                                    num_workers=2)
    for batch_idx, (images, ids) in enumerate(data_loader):
        if use_cuda:
            images = images.cuda()
        images = Variable(images, requires_grad=False)
        outputs = model(images)
        out_array = outputs.data.cpu().numpy()
        probability = np.exp(out_array[:,1]) / np.exp(out_array).sum(1)
        return probability, ids

transform = transforms.Compose([
    transforms.ToTensor()
])

id_total = []
predicted_total = []

print('loading data...')
test_data = DataSet(path=path,
                    file='test1.json',
                    train=False,
                    transform=transform,
                    predicted=True)
print('predict.....')
prob, ids = predict(test_data)
predicted_total.extend(prob)
id_total.extend(ids)

print('loading data...')
test_data = DataSet(path=path,
                    file='test2.json',
                    train=False,
                    transform=transform,
                    predicted=True)
print('predict.....')
prob, ids = predict(test_data)
predicted_total.extend(prob)
id_total.extend(ids)

print('loading data...')
test_data = DataSet(path=path,
                    file='test3.json',
                    train=False,
                    transform=transform,
                    predicted=True)
print('predict.....')
prob, ids = predict(test_data)
predicted_total.extend(prob)
id_total.extend(ids)


predict_dict = {'id':id_total, 'is_iceberg':predicted_total}
predict_series = pd.DataFrame(predict_dict)
predict_series.to_csv(os.path.join(path, 'submit.csv'), index=False)
