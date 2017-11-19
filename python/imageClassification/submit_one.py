#encoding:utf-8

import numpy as np # linear algebra
import pandas as pd # data processing, CSV file I/O (e.g. pd.read_csv)
import bson,io
from random import randint
from skimage.data import imread
import torch
from torch.utils.data import Dataset, DataLoader
from torchvision import transforms
from torch.autograd import Variable
import torch.nn.functional as F
from tqdm import tqdm
import time
from scipy.sparse import coo_matrix
import cv2
import torch.nn as nn

from net.inceptionresnetv2 import inceptionresnetv2 as Net
data = bson.decode_file_iter(open('/home/we/devsda1/test.bson', 'rb'))

batch_size = 6
num_iter = 294697 #1768182/6
torch.cuda.set_device(1)

def test_iamge_generator(batch_size=6, start_image=0):
    count_product = 0
    
    while True:
        count = 0
        images_list = []
        _id = []
        for c, d in enumerate(data):
            product_id = d['_id']
            count += 1
            if count_product < start_image:
                count_product += 1
                continue
            for e, pic in enumerate(d['imgs']):
                picture = imread(io.BytesIO(pic['picture']))
                #picture = cv2.cvtColor(picture,cv2.COLOR_RGB2BGR)
                picture = picture[:,:,::-1]#bgr-->rgb
                images_list.append(picture)
                _id.append(product_id)
            if count == batch_size:
                images = (np.array(images_list)/255.0).astype('float32')
                images = images[:,10:-10,10:-10,:]
                images = np.rollaxis(images,3,1)
                images = torch.from_numpy(images)
                id_list = _id
                count = 0
                images_list = []
                _id = []
                yield images,id_list
                # just to be sure past batch are removed from the memory



categories_df = pd.read_csv("/home/we/devsda1/categories.csv", index_col="category_id")
def make_category_tables():
    cat2idx = {}
    idx2cat = {}
    for ir in categories_df.itertuples():
        category_id = ir[0]
        category_idx = ir[4]
        cat2idx[category_id] = category_idx
        idx2cat[category_idx] = category_id
    return cat2idx, idx2cat
#data_loader,product_ids= next(test_generator)
_,idx2cat = make_category_tables()


model_path = "model/resnetv2model_best.pth.tar" #保存模型的路径
model = Net(pretrained=False)
model.classif = nn.Linear(1536,5270)
print('ok')
checkpoint = torch.load(model_path)
print('ok')
model.load_state_dict(checkpoint['state_dict'])
model.cuda()
model.eval()
test_loader = test_iamge_generator(batch_size=batch_size)

category_id = [] #保存预测输出的结果
_id = []
prob_list =[]
count = 0
print('start test')
result_dict={}# {'id':prob_sparse_matrix}
for i in tqdm(range(294697)):
    category=[]
    images,product_ids=next(test_loader)
    product_ids_dup = list(set(product_ids))
    product_ids_dup.sort(key=product_ids.index)
    count += 1
    images = Variable(images,volatile=True).cuda()
    #print(images.size())
    logits = model(images)
    probs = F.softmax(logits)# (N,classes)
    probs = probs.cpu().data.numpy().reshape(-1,5270) #ensure shape[[]](num_id*num_id_pic,5270)

    '''probs_top5_index = np.argsort(probs,axis=1)[:,-6:]
    probs_top5 = np.sort(probs,axis=1)[:,-6:].reshape(-1)
    sparse_col = probs_top5_index.reshape(-1)
    sparse_row = np.arange(len(product_ids)).repeat(6)
    m = coo_matrix((probs_top5,(sparse_row,sparse_col)),shape=(len(product_ids),5270))
    result_dict[product_ids[0]] = m'''

    probs = np.concatenate([np.array(product_ids).reshape(-1,1),probs],1)  #(num_id*num_id_pic,1+classes)
    df_probs = pd.DataFrame(probs)
    df_probs = df_probs.groupby(0).mean()
    probs = df_probs.as_matrix() #(N,classes)
    prob = probs.max(1)
    pred = probs.argmax(1) # (N,) (N,)
    for temp in pred:
        category.append(idx2cat[temp])
    category_id += category
    _id += product_ids_dup
    prob_list += list(prob)
    #if count ==10000:
    #    break
#print(_id,category_id,prob_list)

df = pd.DataFrame({'_id':_id,'category_id':category_id,'prob':prob_list})
 #df = df.sort_values(by='prob',ascending=False)
 #df = df.drop_duplicates(subset=['_id'],keep = 'first')
df = df[['_id','category_id']]
df = df.sort_values(by='_id',ascending=True)
df.to_csv('resnetv2model_submission.csv.gz',index = False, compression = 'gzip')
 #df.to_csv('submission.csv',index = False,) 

#import cPickle as pickle
#from sklearn.externals import joblib
#pickle.dump(result_dict,open('submission_0.62_prob.pkl','wb'))
#joblib.dump(value=result_dict,filename='submission_0.62_prob.gz',compress=True)
