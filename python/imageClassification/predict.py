import torch
from resnet import *

model_path = "" #保存模型的路径
model = resnet18(pretrained=False)
checkpoint = torch.load(model_path)

model.load_state_dict(checkpoint['state_dict'])
result = [] #保存预测输出的结果

for images,labels in data_loader:
	images = Variable(images).cuda()
	labels = labels.cuda()
	outputs = model(images)
	_,pred = torch.max(outputs.data,1)
	result.append(pred)