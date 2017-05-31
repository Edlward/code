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
from sklearn.externals import joblib
import scipy as sp
import zipfile

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


# 数据读取与融合 
ad = pd.read_csv(path + 'ad.csv')
appcategory = pd.read_csv(path + 'app_categories.csv')
user = pd.read_csv(path + 'user.csv')

# appId在creativeID的标号内
train = pd.merge(train, ad, on='creativeID',how='left')
train.drop('creativeID', axis=1,inplace=True)
ad = None

train = pd.merge(train, appcategory,on='appID',how='left')
train.drop('appID', axis=1,inplace=True)
appcategory = None

train = pd.merge(train, user, on = 'userID', how = 'left')
train.drop('userID', axis=1,inplace=True)
user = None



#
var = 'conversionTime'
train.drop(var, axis=1,inplace=True)

# 
var = 'clickTime'
train.drop(var, axis=1,inplace=True)

# 数据连接种类
var = 'connectionType'
con_columns = columns_tmp = ['ct0','ct1','ct2','ct3','ct4']
dummy = pd.get_dummies(train[var])
dummy.columns = columns_tmp
train = train.join(dummy)
train.drop(var, axis=1,inplace=True)

# 运营商种类
var = 'telecomsOperator'
tel_columns = columns_tmp = ['tc0','tc1','tc2','tc3']
dummy = pd.get_dummies(train[var])
dummy.columns = columns_tmp
train = train.join(dummy)
train.drop(var, axis=1,inplace=True)

# 种类数目未知，需要更改
var = 'appCategory'
app_columns = columns_tmp = ['ac0','ac1','ac2','ac3','ac4', 'ac5', 'ac6', 'ac7', 'ac8', 'ac9', 'ac10','ac11','ac12','ac13']
dummy = pd.get_dummies(train[var])
dummy.columns = columns_tmp
train = train.join(dummy)
train.drop(var, axis=1,inplace=True)


# 用户信息开始
# 使用userID获取到的用户信息， 
# 后面几类的概率基本相同，所以归为一类
var = 'haveBaby'
hab_columns = columns_tmp = ['hb0','hb1','hb2']
train.loc[train[var] > 2] = 2
dummy = pd.get_dummies(train[var])
dummy.columns = columns_tmp
train = train.join(dummy)
train.drop(var, axis=1,inplace=True)

# 使用userID获取的用户信息
var = 'gender'
gen_columns = columns_tmp = ['gen0','gen1','gen2']
dummy = pd.get_dummies(train[var])
dummy.columns = columns_tmp
train = train.join(dummy)
train.drop(var, axis=1,inplace=True)
# 用户信息结束


# 
var = 'positionID'
train.drop(var, axis=1,inplace=True)

# 系统类型无关紧要
var = 'appPlatform'
plat_columns = columns_tmp = ['android', 'ios']
dummy = pd.get_dummies(train[var])
dummy.columns = columns_tmp
train = train.join(dummy)
train.drop(var, axis=1,inplace=True)

# 
var = 'adID'
train.drop(var, axis=1,inplace=True)

# 
# var = 'campaignID'
var = 'camgaignID'
train.drop(var, axis=1,inplace=True)

# 
var = 'advertiserID'
train.drop(var, axis=1,inplace=True)

# enc = OneHotEncoder()
# feats = ['connectionType', 'telecomsOperator']
# for i,feat in enumerate(feats):
#     x_train = enc.fit_transform(train[feat].values.reshape(-1, 1))
#     if i == 0:
#         X_train = x_train
#     else:
#         X_train = sparse.hstack((X_train, x_train))

# 特征抽取 
X_columns = con_columns
X_columns.extend(tel_columns)
X_columns.extend(app_columns)
X_columns.extend(plat_columns)
X_columns.extend(hab_columns)
X_columns.extend(gen_columns)

X_train = train[X_columns]
print('prepare data')
print('train', train.shape)
print('X_train', X_train.shape)
print X_train.head()


train_data, varify_data, train_label, varify_label = train_test_split(X_train, \
                            train['label'].values, train_size=0.8)

print 'begin to train'
# 'connectionType', 'telecomsOperator' 0.115055742521
# connectionType telecomsOperator, appCatorgory 0.110719227512
# connectionType telecomsOperator, appCatorgory platform 0.109875571014
# connectionType telecomsOperator, appCatorgory platform haveBaby 0.733187823453
# connectionType telecomsOperator, appCatorgory platform haveBaby gender 31维度 0.669087918607, 0.720009529899,0.713121562789


lr = LogisticRegression()
lr.fit(train_data, train_label)
proba_varify = lr.predict_proba(varify_data)[:,1]
print logloss(varify_label,proba_varify)

# 保存模型到本地 
# joblib.dump(lr, 'train_model.m')

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
# connectionType是0.114453472694
# telecomsOperator 全部数据集，0.115715800111
# connectionType和telecomsOperator结果是0.114637306152
# connectionType和telecomsOperator和creativeID是0.236746484137
# random_forest = RandomForestRegressor(n_estimators=400)
# random_forest.fit(X_train, Y_train)
# Y_pred = random_forest.predict(X_test)
# # random_forest.score(X_train, Y_train)
# print logloss(Y_test, Y_pred)

# submission
df = pd.DataFrame({"testlabel": Y_test, "proba": proba_varify})
df.sort_values("testlabel", inplace=True)
df.to_csv("submission.csv", index=False)
with zipfile.ZipFile("submission.zip", "w") as fout:
    fout.write("submission.csv", compress_type=zipfile.ZIP_DEFLATED)
