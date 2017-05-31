import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
%matplotlib inline
path = '~/codedata/tpai/pre/'

trainfile = 'train.csv'
testfile = 'test.csv'
train = pd.read_csv(path + trainfile)
test = pd.read_csv(path + testfile)

position = pd.read_csv(path + 'position.csv')

appcategory = pd.read_csv(path + 'app_categories.csv')
