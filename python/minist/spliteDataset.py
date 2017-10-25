import os
import pandas as pd
from sklearn.model_selection import train_test_split

path = '/home/lxg/codedata/minist'
dataset = pd.read_csv(os.path.join(path, 'train.csv'))

trainset, testset =  train_test_split(dataset, test_size=0.2, random_state=2)

trainset.to_csv(os.path.join(path, 'train_train.csv'), index=False)
testset.to_csv(os.path.join(path, 'train_test.csv'), index=False)