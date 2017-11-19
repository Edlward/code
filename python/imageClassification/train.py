#encoding:utf-8
#from net.excited_resnet50 import SEResNet50 as Net
import os
#os.environ['CUDA_VISIBLE_DEVICES']='0,1'
#NUM_CUDA_DEVICES = len(os.environ['CUDA_VISIBLE_DEVICES'].split(','))
from tool import *
#from new_generator import *
from xiongDataset import *

import torch
import torch.nn as nn
from torchvision import models
from torch.autograd import Variable
import time

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import shutil
import numpy as np
#from net.se_inception import SEInception3 as Net
#from net.torchvision.models.inception import inception_v3 as Net
#from net.resnet import resnet50 as Net
from net.inceptionresnetv2 import inceptionresnetv2 as Net


use_gpu = torch.cuda.is_available()
#torch.cuda.set_device(1)

def check_acc(cnn,criterion,data_loader):
	t0 = time.time()
	num_correct,num_sample ,count= 0.0, 0.0,0.0
	loss = 0.0
	cnn.eval()
	data_iter = iter(data_loader)
	for i in range(len(data_loader)):
		images,labels = next(data_iter)
		#images,labels = torch.from_numpy(images),labels#torch.from_numpy(labels)
		images = Variable(images,volatile=True).cuda()
		labels = Variable(labels,volatile=True).cuda()
		outputs = cnn(images)
		loss += criterion(outputs,labels).data[0]
		_,pred = torch.max(outputs.data,1)
		num_sample += labels.size(0)
		count += 1
		num_correct += (pred == labels.data).sum()
		#print('loss is {},accuracy is {}'.format(loss/count,float(num_correct)/num_sample))
	t1 = time.time()
	cnn.train()
	print('check validation take {} s'.format(t1-t0))
	return num_correct/num_sample , loss/count
def plot_performance_curves(train_acc_history,val_acc_history,train_loss_history,val_loss_history, epoch_history):
	plt.figure()
	plt.plot(np.array(epoch_history),np.array(train_acc_history),label = 'Training accuracy')
	plt.plot(np.array(epoch_history),np.array(val_acc_history),label = 'Validation accuracy')
	plt.title('Accuracy on training and validation')
	plt.ylabel('Accuracy')
	plt.xlabel('Number of epochs')
	plt.legend()
	plt.savefig('accuracy.png')

	plt.figure()
	plt.plot(np.array(epoch_history),np.array(train_loss_history),label = 'Training loss')
	plt.plot(np.array(epoch_history),np.array(val_loss_history),label = 'Validation loss')
	plt.title('loss on training and validation')
	plt.ylabel('loss')
	plt.xlabel('Number of epochs')
	plt.legend()
	plt.savefig('loss.png')

def save_checkpoint(state,is_best,file_name = 'model/resnetv2checkpoint.pth.tar'):
	torch.save(state,file_name)
	if is_best:
		shutil.copyfile(file_name,'model/resnetv2model_best.pth.tar')

def load_state_dict(ourmodel, oldmodel):
	import re
	from collections import OrderedDict
	own_state = ourmodel.state_dict()
	state_dict = oldmodel.state_dict()

	for name, param in own_state.items():
		if name not in state_dict:
			continue
		param = state_dict[name]
		own_state[name].copy_(param)

#data set
train_data = train_dataloader
val_data = val_dataloader

train_iter = iter(train_data)
val_iter = iter(val_data)

num_iter = len(train_data)
print("num_iter is {}".format(num_iter))
	

#original_model = models.resnet50(pretrained = True)
#original_model = models.inception_v3(pretrained = True)
#model = Net(num_classes = 5270,aux_logits=False) # 1000->num_classes
model = Net(pretrained=True)
model.classif = nn.Linear(1536,5270)
print('change classif')
#model = nn.DataParallel(model,device_ids=[0,1])
#load_state_dict(model,original_model)

#load trained model checkpoint
num_epoch_has_trained=1
num_iter_has_trained=0
initinal_checkpoint = 'model/resnetv2checkpoint.pth.tar'
#initinal_checkpoint = None
if initinal_checkpoint is not None:
	print('load trained model')
	checkpoint = torch.load(initinal_checkpoint)
	model.load_state_dict(checkpoint['state_dict'])
	num_epoch_has_trained = checkpoint['epoch']
	num_iter_has_trained = checkpoint['iter']
	print('num_iter_has_trained {}'.format(num_iter_has_trained))


if use_gpu:
	model.cuda()
model.train()
criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.SGD(model.parameters(),lr=0.01,momentum=0.9,weight_decay=0.001)
#optimizer = torch.optim.Adam(model.parameters(),weight_decay=0.001)

num_epochs = 9
train_acc_history = []
val_acc_history = []
train_loss_history=[]
val_loss_history=[]
epoch_history = []
iter_history = []
learning_rate = 0.01
best_val_acc = 0.0
iter_valid  = list(range(10000,num_iter,10000)) + [num_iter-1]

#step = 1 #look validation

log = Logger()
log.open("log1.txt",'a')
log.write(u'** resnet network **\n')
log.write(u'iter_k  |  rate  |  val_loss  |  val_acc  \n')

#num_iter_has_trained = 0
for epoch in range(num_epoch_has_trained-1,num_epochs):
		num_correct_total = 0.0
		loss_total = 0.0
		if epoch>1:
			learning_rate=0.001
		if epoch > 3:
			learning_rate=0.0001
		#if epoch > 5:
			#learning_rate = 0.00005
		if epoch >5:
			learning_rate = 0.00001
		if epoch > 6:
			learning_rate = 0.000001
		

		
		optimizer = torch.optim.SGD(model.parameters(),lr=learning_rate,momentum=0.9,weight_decay=0.001)
		num_correct=0
		print('Starting epoch %d / %d' % (epoch + 1, num_epochs))
		print('Learning Rate for this epoch: {}'.format(learning_rate))
		iter_count=0
		for i,(images,labels) in enumerate(train_data):
			#print('------------{}----------'.format(i))
			#t0 = time.time()
			#images,labels = next(train_iter)
			#t1 = time.time()
			#print('load data time {}'.format(t1-t0))
			#t0 = time.time()
			#images,labels = torch.from_numpy(images),labels#torch.from_numpy(labels)
			#optimizer.zero_grad()
			if i == len(train_data)-num_iter_has_trained:
				break
			i = i+num_iter_has_trained
			iter_count += 1
			images = Variable(images)
			labels = Variable(labels)
			if use_gpu:
				images,labels = images.cuda(),labels.cuda()
			
			#pred_labels = torch.nn.DataParallel(model)(images)
			pred_labels = model(images)
			loss = criterion(pred_labels,labels)
			_,pred = torch.max(pred_labels.data,1)
			num_correct = (pred == labels.data).sum()
			num_correct_total += num_correct
			loss_total += loss.data[0]
			#optimizer.zero_grad()
			loss.backward()
			#optimizer.zero_grad()
			if i%4 == 0 or i == len(train_data)-1:
				optimizer.step()
				optimizer.zero_grad()
			
			#t1 = time.time()
			#print('training time {}'.format(t1-t0))

			if (i) % 5 == 0:
				print ('Epoch [%d/%d], Iter [%d/%d] Loss: %.4f Accuracy: %.4f' 
					%(epoch+1, num_epochs, i+1, len(train_data), loss_total/(iter_count), num_correct_total/((iter_count)*batch_size)))

			if i in iter_valid:
				val_acc,val_loss = check_acc(model,criterion,val_data)
				val_acc_history.append(val_acc)
				val_loss_history.append(val_loss)
				print('Validation accuracy for iter {} : {} '.format(epoch*len(train_data)+(i+1),val_acc))
				print('Validation loss for iter {} : {} '.format(epoch*len(train_data)+(i+1),val_loss))
				iter_history.append((epoch*len(train_data)+(i+1))/1000)
				#plot_performance_curves(train_acc_history,val_acc_history,train_loss_history,val_loss_history,iter_history)
				log.write(u'%5.1f      %1.6f      %1.6f      %1.6f\n' % ((epoch*len(train_data)+(i+1))/1000.0,learning_rate,val_loss,val_acc))
				is_best = val_acc > best_val_acc
				best_val_acc = max(val_acc,best_val_acc)
				save_checkpoint(
					{'epoch':epoch+1,
					'iter':i+1,
					'state_dict':model.state_dict(),
					'best_val_acc':best_val_acc,
					'optimizer':optimizer.state_dict()},is_best)
		num_iter_has_trained = 0
		'''if epoch % step ==0 or epoch == num_epochs-1:
			learning_rate = learning_rate * 0.9

			train_acc,train_loss = check_acc(model,criterion,train_data)
			train_acc_history.append(train_acc)
			train_loss_history.append(train_loss)
			print('Train accuracy for epoch {}: {} '.format(epoch + 1,train_acc))
			print('Train loss for epoch {}: {} '.format(epoch + 1,train_loss))
			val_acc,val_loss = check_acc(model,criterion,val_data)
			val_acc_history.append(val_acc)
			val_loss_history.append(val_loss)
			print('Validation accuracy for epoch {} : {} '.format(epoch + 1,val_acc))
			print('Validation loss for epoch {} : {} '.format(epoch + 1,val_loss))
			epoch_history.append(epoch+1)
			plot_performance_curves(train_acc_history,val_acc_history,train_loss_history,val_loss_history,epoch_history)
			log.write(u'%3d      %1.6f      %1.6f      %1.6f\n' % (epoch+1,learning_rate,train_acc,val_acc))
			is_best = val_acc > best_val_acc
			best_val_acc = max(val_acc,best_val_acc)
			save_checkpoint(
				{'epoch':epoch+1,
				'state_dict':model.state_dict(),
				'best_val_acc':best_val_acc,
				'optimizer':optimizer.state_dict()},is_best)'''