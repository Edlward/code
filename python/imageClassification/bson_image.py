import bson
import numpy as np
import pandas as pd
import os
#from tqdm import tqdm_notebook

out_folder = '../train'


# Create output folder
'''if not os.path.exists(out_folder):
    os.makedirs(out_folder)'''

# Create categories folders
#categories = pd.read_csv('../category_names.csv', index_col='category_id')

'''for category in tqdm_notebook(categories.index):
    os.mkdir(os.path.join(out_folder, str(category))'''

#num_products = 7069896 # 7069896 for train and 1768182 for test

#bar = tqdm_notebook(total=num_products)
'''with open('../train.bson', 'rb') as fbson:

    data = bson.decode_file_iter(fbson)
    
    for c, d in enumerate(data):
        _id = d['_id']
        for e, pic in enumerate(d['imgs']):
            fname = os.path.join(out_folder,  '{}_{}.jpg'.format(_id, e))
            with open(fname, 'wb') as f:
                f.write(pic['picture'])'''

        #bar.update()
 
#print(len(os.listdir(out_folder)))


train = pd.read_csv('../train_images.csv')
dict = {}
cate_idx = list(train['category_idx'])

for i in cate_idx:
    dict[i] = 0
for j in cate_idx:
    dict[j] += 1
print(dict) 