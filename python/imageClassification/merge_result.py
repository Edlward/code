#encoding:utf-8
# load pickle file, then merge result form different model
# pickle dict{key:id,value:sparse matrix}
import cPickle as pickle
from sklearn.externals import joblib
import pandas as pd
from tqdm import tqdm
import numpy as np


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

category_id = [] #保存预测输出的结果
_id = []

result = joblib.load('submission_0.62_prob.gz')

for id,probs in tqdm(result.items()):
	probs = probs.toarray()
	probs = np.mean(probs,axis=0)
	pred = probs.argmax() # one number
	_id.append(id)
	category_id.append(idx2cat[pred])

df = pd.DataFrame({'_id':_id,'category_id':category_id})
df = df.sort_values(by='_id',ascending=True)
df.to_csv('merge_submission_0.62_aaa.csv.gz',index = False, compression = 'gzip')