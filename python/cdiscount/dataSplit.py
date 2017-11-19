import os
import pandas as pd
from sklearn.model_selection import train_test_split

path = '/home/lxg/codedata/cdiscount'
# dataset = pd.read_csv(os.path.join(path, 'productCategoryImgsLevel1.csv'))
dataset = pd.read_csv(os.path.join(path, 'test.csv'))


trainset, testset =  train_test_split(dataset, test_size=0.01, random_state=2)

# trainset.to_csv(os.path.join(path, 'train.csv'), index=False)
testset.to_csv(os.path.join(path, 'test_small.csv'), index=False)