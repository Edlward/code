# --coding:utf-8--
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
# %matplotlib inline
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import OneHotEncoder
from scipy import sparse
from sklearn.linear_model import LogisticRegression
from sklearn.tree import DecisionTreeRegressor
from sklearn.ensemble import RandomForestRegressor
import scipy as sp

def logloss(act, pred):
  epsilon = 1e-15
  pred = sp.maximum(epsilon, pred)
  pred = sp.minimum(1-epsilon, pred)
  ll = sum(act*sp.log(pred) + sp.subtract(1,act)*sp.log(sp.subtract(1,pred)))
  ll = ll * -1.0/len(act)
  return ll


path = '~/codedata/tpai/pre/'
trainfile = 'train.csv'
# testfile = 'test.csv'
train = pd.read_csv(path + trainfile)
# test = pd.read_csv(path + testfile)
# position = pd.read_csv(path + 'position.csv')
# appcategory = pd.read_csv(path + 'app_categories.csv')

trainNum = 100000
train = train.sample(n=trainNum, replace = True)


enc = OneHotEncoder()
feats = ['connectionType', 'telecomsOperator']
for i,feat in enumerate(feats):
    x_train = enc.fit_transform(train[feat].values.reshape(-1, 1))
    if i == 0:
        X_train = x_train
    else:
        X_train = sparse.hstack((X_train, x_train))

train_data, varify_data, train_label, varify_label = train_test_split(X_train, \
                            train['label'].values, train_size=0.8)

# lr = LogisticRegression()
# lr.fit(train_data, train_label)
# proba_varify = lr.predict_proba(varify_data)[:,1]
# print logloss(varify_label,proba_varify)


# dtr = DecisionTreeRegressor()
# dtr.fit(train_data, train_label)
# proba_varify = dtr.predict(varify_data)
# print logloss(varify_label,proba_varify)

# 切换换数据名
X_train = train_data
Y_train = train_label
X_test = varify_data
Y_test = varify_label

# Random Forests 
# connectionType是0.115962438973
# telecomsOperator是0.123899858079
# connectionType和telecomsOperator结果是0.116582099373
# connectionType和telecomsOperator和creativeID是0.236746484137
random_forest = RandomForestRegressor(n_estimators=400)
random_forest.fit(X_train, Y_train)
Y_pred = random_forest.predict(X_test)
# random_forest.score(X_train, Y_train)
print logloss(Y_test, Y_pred)
