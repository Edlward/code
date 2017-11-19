import bson
import os
import pandas as pd

path = '/home/we/devsda1'

# path = '/home/lxg/codedata/cdiscount'

data = bson.decode_file_iter(open(os.path.join(path, 'train.bson')))
# data = bson.decode_file_iter(open(os.path.join(path, 'train_example.bson')))

product_category = dict()
product_imgs = dict()

for c,d in enumerate(data):
    product_id = d['_id']
    product_category[product_id] = d['category_id']
    product_imgs[product_id] = len(d['imgs'])
    if c % 10000 == 0:
        print('c:', c*10000)
    

product_category = pd.DataFrame.from_dict(product_category, orient='index')    
product_imgs = pd.DataFrame.from_dict(product_imgs, orient='index')

# product_category.to_csv(os.path.join(path, 'product_category.csv'), index=False)
# product_imgs.to_csv(os.path.join(path, 'product_imgs.csv'), index=False)

product_category_imgs = pd.merge(product_category, product_imgs, left_index=True, right_index=True)
product_category_imgs.index.name = '_id'
product_category_imgs.rename(columns={'0_x':'category_id', '0_y':'img_num'}, inplace=True)
product_category_imgs.to_csv('productCategoryImgs1.csv')