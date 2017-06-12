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
from sklearn.ensemble import GradientBoostingRegressor
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

print('merge data')
# 数据读取与融合 
ad = pd.read_csv(path + 'ad.csv')
appcategory = pd.read_csv(path + 'app_categories.csv')
user = pd.read_csv(path + 'user.csv')

# appId在creativeID的标号内
train = pd.merge(train, ad, on='creativeID',how='left')
train.drop('creativeID', axis=1,inplace=True)
ad = None

train = pd.merge(train, appcategory,on='appID',how='left')
appcategory = None

train = pd.merge(train, user, on = 'userID', how = 'left')
train.drop('userID', axis=1,inplace=True)
user = None

train = train.sample(n = 100000, replace = True)
print('get dummy data')

# appID
var = 'appID'
means = train.groupby(var)['label'].mean()
tmp1 = means.values > 0.3
train[var].replace(means[tmp1].index, -1, inplace = True)
tmp1 = means.values < 0.31
tmp2 = means.values > 0.17
train[var].replace(means[tmp1 & tmp2].index, -2, inplace = True)
tmp1 = means.values < 0.171
tmp2 = means.values > 0.11
train[var].replace(means[tmp1 & tmp2].index, -3, inplace = True)
tmp1 = means.values < 0.111
tmp2 = means.values > 0.09
train[var].replace(means[tmp1 & tmp2].index, -4, inplace = True)
tmp1 = means.values < 0.091
tmp2 = means.values > 0.06
train[var].replace(means[tmp1 & tmp2].index, -5, inplace = True)
tmp1 = means.values < 0.06
train[var].replace(means[tmp1].index, -6, inplace = True)
appid_columns = columns_tmp = ['appid0','appid1','appid2','appid3','appid4', 'appid5']
dummy = pd.get_dummies(train[var])
dummy.columns = columns_tmp
train = train.join(dummy)
train.drop(var, axis=1,inplace=True)

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
# train.loc[train[var] > 2] = 2
train[var][train[var] > 2] = 2
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

# hometown
var = 'hometown'
train.drop(var, axis=1, inplace=True)

# residence
var = 'residence'
train.drop(var, axis=1, inplace=True)
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
means = train.groupby(var)['label'].mean()
tmp1 = means.values > 0.9
train[var].replace(means[tmp1].index, -1, inplace = True)
tmp1 = means.values < 0.91
tmp2 = means.values > 0.6
train[var].replace(means[tmp1 & tmp2].index, -2, inplace = True)
tmp1 = means.values < 0.61
tmp2 = means.values > 0.42
train[var].replace(means[tmp1 & tmp2].index, -3, inplace = True)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
tmp1 = means.values < 0.421
tmp2 = means.values > 0.25
train[var].replace(means[tmp1 & tmp2].index, -4, inplace = True)
tmp1 = means.values < 0.251
tmp2 = means.values > 0.17
train[var].replace(means[tmp1 & tmp2].index, -5, inplace = True)                                                                                                                                        
tmp1 = means.values < 0.171
train[var].replace(means[tmp1].index, -6, inplace = True)
adid_columns = columns_tmp = ['adid0','adid1','adid2','adid3','adid4', 'adid5']
dummy = pd.get_dummies(train[var])
dummy.columns = columns_tmp
train = train.join(dummy)
train.drop(var, axis=1,inplace=True)

# 
# var = 'campaignID'
var = 'camgaignID'
means = train.groupby(var)['label'].mean()
tmp1 = means.values > 0.6
train[var].replace(means[tmp1].index, -1, inplace = True)
tmp1 = means.values < 0.61
tmp2 = means.values > 0.35
train[var].replace(means[tmp1 & tmp2].index, -2, inplace = True)
tmp1 = means.values < 0.351
tmp2 = means.values > 0.30
train[var].replace(means[tmp1 & tmp2].index, -3, inplace = True)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
tmp1 = means.values < 0.301
tmp2 = means.values > 0.15
train[var].replace(means[tmp1 & tmp2].index, -4, inplace = True)
tmp1 = means.values < 0.151
tmp2 = means.values > 0.08
train[var].replace(means[tmp1 & tmp2].index, -5, inplace = True)                                                                                                                                        
tmp1 = means.values < 0.081
train[var].replace(means[tmp1].index, -6, inplace = True)
camgid_columns = columns_tmp = ['camgid0','camgid1','camgid2','camgid3','camgid4', 'camgid5']
dummy = pd.get_dummies(train[var])
dummy.columns = columns_tmp
train = train.join(dummy)
train.drop(var, axis=1,inplace=True)

# 
var = 'advertiserID'
means = train.groupby(var)['label'].mean()
tmp1 = means.values > 0.3
train[var].replace(means[tmp1].index, -1, inplace = True)
tmp1 = means.values < 0.31
tmp2 = means.values > 0.17
train[var].replace(means[tmp1 & tmp2].index, -2, inplace = True)
tmp1 = means.values < 0.171
tmp2 = means.values > 0.13
train[var].replace(means[tmp1 & tmp2].index, -3, inplace = True)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
tmp1 = means.values < 0.131
tmp2 = means.values > 0.09
train[var].replace(means[tmp1 & tmp2].index, -4, inplace = True)
tmp1 = means.values < 0.091
tmp2 = means.values > 0.05
train[var].replace(means[tmp1 & tmp2].index, -5, inplace = True)                                                                                                                                        
tmp1 = means.values < 0.051
train[var].replace(means[tmp1].index, -6, inplace = True)
adver_columns = columns_tmp = ['adver0','adver1','adver2','adver3','adver4', 'adver5']
dummy = pd.get_dummies(train[var])
dummy.columns = columns_tmp
train = train.join(dummy)
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
X_columns.extend(appid_columns)
X_columns.extend(adver_columns)
X_columns.extend(adid_columns)
# X_columns.extend(camgid_columns)

X_traintmp = train[X_columns]
print('prepare data')
print('train', train.shape)
print('X_train', X_traintmp.shape)
print train.head()
print X_traintmp.head()


# train_data, varify_data, train_label, varify_label = train_test_split(X_train, \
#                             train['label'].values, train_size=0.8)

X_train, X_test, Y_train, Y_test = train_test_split(X_traintmp, \
                            train['label'].values, train_size=0.8)
# X_traintmp = None
# train = None
print 'begin to train'

# 权重换新
# weight = pd.Series(Y_train)
# cost = weight.value_counts()
# cost0 = cost[1] * 1. / cost[0]
# cost1 = 1
# weight[weight.values == 0] = cost0
# weight[weight.values == 1] = cost1

# 不权重更新
weight = None

# 逻辑回归
# 在一个二分类问题中，训练集中class 1的样本数比class 2的样本数是60:1。使用逻辑回归进行分类，最后结果是其忽略了class 2，即其将所有的训练样本都分类为class 1。
# 'connectionType', 'telecomsOperator' 0.115055742521
# connectionType telecomsOperator, appCatorgory 0.110719227512
# connectionType telecomsOperator, appCatorgory platform 25维 0.109875571014,0.109831643006 
# connectionType telecomsOperator, appCatorgory platform haveBaby 28维度 0.109320333473
# connectionType telecomsOperator, appCatorgory platform haveBaby gender 31维度  0.108233014599
# connectionType telecomsOperator, appCatorgory platform haveBaby gender appID advertiseID 43维 0.108259829247
# connectionType telecomsOperator, appCatorgory platform haveBaby gender appID advertiseID adID 49维 0.106808327986, 0.10775419771
# lr = LogisticRegression()
# lr.fit(X_train, Y_train, sample_weight = weight)
# Y_pred = lr.predict_proba(X_test)[:,1]
# print logloss(Y_test,Y_pred)

# 保存模型到本地 
# joblib.dump(lr, 'train_model.m')


# 决策树回归
# dtr = DecisionTreeRegressor()
# dtr.fit(train_data, train_label)
# proba_varify = dtr.predict(varify_data)
# print logloss(varify_label,proba_varify)


# Random Forests 
# connectionType是0.114453472694
# telecomsOperator 全部数据集，0.115715800111
# connectionType和telecomsOperator结果是0.114637306152
# connectionType和telecomsOperator和creativeID是0.236746484137
# connectionType telecomsOperator, appCatorgory platform haveBaby gender 31维度 0.109947102938
# random_forest = RandomForestRegressor(n_estimators=200)
# random_forest.fit(X_train, Y_train)
# Y_pred = random_forest.predict(X_test)
# # random_forest.score(X_train, Y_train)
# print logloss(Y_test, Y_pred)


# GradientBoostingRegressor 0.109029833025
# connectionType telecomsOperator, appCatorgory platform haveBaby gender 31维度 0.109135352106
# connectionType telecomsOperator, appCatorgory platform haveBaby gender 更改权重后 31维度 0.621507445525，0.620584289427
# 是不是可以基于结果做个分类把小于0.5的映射到更小，大于0.5的映射到更大  14.7160201231 , 更差了！！！！
# connectionType telecomsOperator, appCatorgory platform haveBaby gender appID 0.615182685684 
# connectionType telecomsOperator, appCatorgory platform haveBaby gender appID advertiseID 43维 0.610518742853， 0.611664226848, 0.109789711506

gradient_boost = GradientBoostingRegressor()
gradient_boost.fit(X_train, Y_train)
# gradient_boost.fit(X_train, Y_train, sample_weight=weight)
Y_pred = gradient_boost.predict(X_test)
print logloss(Y_test, Y_pred)


# submission
df = pd.DataFrame({"testlabel": Y_test, "proba": Y_pred})
df.sort_values("testlabel", inplace=True)
df.to_csv("submission.csv", index=False)
with zipfile.ZipFile("submission.zip", "w") as fout:
    fout.write("submission.csv", compress_type=zipfile.ZIP_DEFLATED)

print('write file')