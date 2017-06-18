# -*- coding: utf-8 -*-
"""
baseline 2: ad.csv (creativeID/adID/camgaignID/advertiserID/appID/appPlatform) + lr
"""
# 0.107
import zipfile
import pandas as pd
from scipy import sparse
from sklearn.preprocessing import OneHotEncoder
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
import xgboost as xgb
import scipy as sp
from sklearn.model_selection import cross_val_score, GridSearchCV
from sklearn.metrics import make_scorer
import time
import datetime

def logloss(act, pred):
  epsilon = 1e-15
  pred = sp.maximum(epsilon, pred)
  pred = sp.minimum(1-epsilon, pred)
  ll = sum(act*sp.log(pred) + sp.subtract(1,act)*sp.log(sp.subtract(1,pred)))
  ll = ll * -1.0/len(act)
  print ll
  return ll

# load data
data_root = "~/codedata/tpai/pre"
dfTrain = pd.read_csv("%s/train.csv"%data_root)
dfTest = pd.read_csv("%s/test.csv"%data_root)
dfAd = pd.read_csv("%s/ad.csv"%data_root)
dfUser = pd.read_csv("%s/user.csv"%data_root)
dfPosition = pd.read_csv("%s/position.csv"%data_root)
print('merge data')
# process data
dfTrain = pd.merge(dfTrain, dfAd, on="creativeID", how = 'left')
dfTest = pd.merge(dfTest, dfAd, on="creativeID", how = 'left')
dfTrain = pd.merge(dfTrain, dfUser, on = 'userID', how = 'left')
dfTest = pd.merge(dfTest, dfUser, on = 'userID', how = 'left')
dfTrain = pd.merge(dfTrain, dfPosition, on = 'positionID', how = 'left')
dfTest = pd.merge(dfTest, dfPosition, on = 'positionID', how = 'left')
dfTrain.drop('userID', axis=1, inplace=True)
dfTest.drop('userID', axis=1, inplace=True)
dfTrain.drop('conversionTime', axis=1, inplace=True)
# dfTest.drop('conversionTime', axis=1, inplace=True)
dfTrain.drop('clickTime', axis=1, inplace=True)
dfTest.drop('clickTime', axis=1, inplace=True)
del dfAd
del dfUser
del dfPosition

user = None
y_train = dfTrain["label"].values


# var = 'campaignID' 分布在0~800
# 把点击次数少于200的ID归为一类
var = 'camgaignID'
count = dfTrain[var].value_counts(sort=True, ascending=False)
tmp1 = count.values < 200
dfTrain[var].replace(count[tmp1].index, 800, inplace = True)
dfTest[var].replace(count[tmp1].index, 800, inplace = True)

# 根据概率接近程度分类
means = dfTrain.groupby(var)['label'].mean()
del means[800]  # 800不做频数统计在分标签，所以要删掉
# tmp1 = means.values > 0.6
# dfTrain[var].replace(means[tmp1].index, 801, inplace = True)
# tmp1 = means.values < 0.601
# tmp2 = means.values > 0.399
# dfTrain[var].replace(means[tmp1 & tmp2].index, 802, inplace = True)
# tmp1 = means.values < 0.3991
# tmp2 = means.values > 0.35
# dfTrain[var].replace(means[tmp1 & tmp2].index, 803, inplace = True)
# tmp1 = means.values < 0.351
tmp1 = True
tmp2 = means.values > 0.299
dfTrain[var].replace(means[tmp1 & tmp2].index, 804, inplace = True)
dfTest[var].replace(means[tmp1 & tmp2].index, 804, inplace = True)
tmp1 = means.values < 0.2991
tmp2 = means.values > 0.20
dfTrain[var].replace(means[tmp1 & tmp2].index, 805, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, 805, inplace = True) 
tmp1 = means.values < 0.21
tmp2 = means.values > 0.18
dfTrain[var].replace(means[tmp1 & tmp2].index, 806, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, 806, inplace = True) 
tmp1 = means.values < 0.181
tmp2 = means.values > 0.16
dfTrain[var].replace(means[tmp1 & tmp2].index, 807, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, 807, inplace = True) 
tmp1 = means.values < 0.161
tmp2 = means.values > 0.14
dfTrain[var].replace(means[tmp1 & tmp2].index, 808, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, 808, inplace = True) 
tmp1 = means.values < 0.141
tmp2 = means.values > 0.12
dfTrain[var].replace(means[tmp1 & tmp2].index, 809, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, 809, inplace = True) 
tmp1 = means.values < 0.121
tmp2 = means.values > 0.10
dfTrain[var].replace(means[tmp1 & tmp2].index, 810, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, 810, inplace = True) 
tmp1 = means.values < 0.11
tmp2 = means.values > 0.08
dfTrain[var].replace(means[tmp1 & tmp2].index, 811, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, 811, inplace = True) 
tmp1 = means.values < 0.081
tmp2 = means.values > 0.06
dfTrain[var].replace(means[tmp1 & tmp2].index, 812, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, 812, inplace = True) 
tmp1 = means.values < 0.061
tmp2 = means.values > 0.04
dfTrain[var].replace(means[tmp1 & tmp2].index, 813, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, 813, inplace = True) 
tmp1 = means.values < 0.041
tmp2 = means.values > 0.02
dfTrain[var].replace(means[tmp1 & tmp2].index, 814, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, 814, inplace = True) 
tmp1 = means.values < 0.021
dfTrain[var].replace(means[tmp1].index, 815, inplace = True)
dfTest[var].replace(means[tmp1].index, 815, inplace = True)
print (var, dfTrain[var].value_counts(sort=False).index)



# residence 分布在0~450
var = 'residence'
label_min = 450
count = dfTrain[var].value_counts(sort=True, ascending=False)
tmp1 = count.values < 200
dfTrain[var].replace(count[tmp1].index, label_min, inplace = True)
dfTest[var].replace(count[tmp1].index, label_min, inplace = True)
dfTest[var].replace(3210, label_min, inplace = True)  #训练集内没有这个数据


means = dfTrain.groupby(var)['label'].mean()
del means[label_min]
# tmp1 = means.values > 0.06
# dfTrain[var].replace(means[tmp1].index, label_min+1, inplace = True)
# dfTest[var].replace(means[tmp1].index, label_min+1, inplace = True)
# tmp1 = means.values < 0.061
tmp1 = True
tmp2 = means.values > 0.04
dfTrain[var].replace(means[tmp1 & tmp2].index, label_min+2, inplace = True)
dfTest[var].replace(means[tmp1 & tmp2].index, label_min+2, inplace = True)
tmp1 = means.values < 0.041
tmp2 = means.values > 0.035
dfTrain[var].replace(means[tmp1 & tmp2].index, label_min+3, inplace = True)
dfTest[var].replace(means[tmp1 & tmp2].index, label_min+3, inplace = True)
tmp1 = means.values < 0.0351
tmp2 = means.values > 0.033
dfTrain[var].replace(means[tmp1 & tmp2].index, label_min+4, inplace = True)
dfTest[var].replace(means[tmp1 & tmp2].index, label_min+4, inplace = True)
tmp1 = means.values < 0.0331
tmp2 = means.values > 0.03
dfTrain[var].replace(means[tmp1 & tmp2].index, label_min+5, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, label_min+5, inplace = True) 
tmp1 = means.values < 0.031
tmp2 = means.values > 0.025
dfTrain[var].replace(means[tmp1 & tmp2].index, label_min+6, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, label_min+6, inplace = True) 
tmp1 = means.values < 0.0251
tmp2 = means.values > 0.023
dfTrain[var].replace(means[tmp1 & tmp2].index, label_min+7, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, label_min+7, inplace = True) 
tmp1 = means.values < 0.0231
tmp2 = means.values > 0.02
dfTrain[var].replace(means[tmp1 & tmp2].index, label_min+8, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, label_min+8, inplace = True) 
tmp1 = means.values < 0.021
tmp2 = means.values > 0.016
dfTrain[var].replace(means[tmp1 & tmp2].index, label_min+9, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, label_min+9, inplace = True) 
tmp1 = means.values < 0.0161
tmp2 = means.values > 0.013
dfTrain[var].replace(means[tmp1 & tmp2].index, label_min+10, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, label_min+10, inplace = True) 
tmp1 = means.values < 0.0131
tmp2 = means.values > 0.01
dfTrain[var].replace(means[tmp1 & tmp2].index, label_min+11, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, label_min+11, inplace = True) 
tmp1 = means.values < 0.011
tmp2 = means.values > 0.006
dfTrain[var].replace(means[tmp1 & tmp2].index, label_min+12, inplace = True) 
dfTest[var].replace(means[tmp1 & tmp2].index, label_min+12, inplace = True) 
tmp1 = means.values < 0.0061
# tmp2 = means.values > 0.004
# dfTrain[var].replace(means[tmp1 & tmp2].index, label_min+13, inplace = True) 
# dfTest[var].replace(means[tmp1 & tmp2].index, label_min+13, inplace = True) 
# tmp1 = means.values < 0.0041
dfTrain[var].replace(means[tmp1].index, label_min+14, inplace = True)
dfTest[var].replace(means[tmp1].index, label_min+14, inplace = True)
print (var, dfTrain[var].value_counts(sort=False).index)


# appID
var = 'appID'
label_min = 450
count = dfTrain[var].value_counts(sort=True, ascending=False)
tmp1 = count.values < 200
dfTrain[var].replace(count[tmp1].index, label_min, inplace = True)
dfTest[var].replace(count[tmp1].index, label_min, inplace = True)

means = dfTrain.groupby(var)['label'].mean()
# tmp1 = means.values > 0.3
# dfTrain[var].replace(means[tmp1].index, label_min+1, inplace = True)
# dfTest[var].replace(means[tmp1].index, label_min+1, inplace = True)
# tmp1 = means.values < 0.31
tmp1 = True
tmp2 = means.values > 0.17
dfTrain[var].replace(means[tmp1 & tmp2].index, label_min+2, inplace = True)
dfTest[var].replace(means[tmp1 & tmp2].index, label_min+2, inplace = True)
tmp1 = means.values < 0.171
tmp2 = means.values > 0.11
dfTrain[var].replace(means[tmp1 & tmp2].index, label_min+3, inplace = True)
dfTest[var].replace(means[tmp1 & tmp2].index, label_min+3, inplace = True)
tmp1 = means.values < 0.111
tmp2 = means.values > 0.09
dfTrain[var].replace(means[tmp1 & tmp2].index, label_min+4, inplace = True)
dfTest[var].replace(means[tmp1 & tmp2].index, label_min+4, inplace = True)
tmp1 = means.values < 0.091
tmp2 = means.values > 0.06
dfTrain[var].replace(means[tmp1 & tmp2].index, label_min+5, inplace = True)
dfTest[var].replace(means[tmp1 & tmp2].index, label_min+5, inplace = True)
tmp1 = means.values < 0.061
dfTrain[var].replace(means[tmp1].index, label_min+6, inplace = True)
dfTest[var].replace(means[tmp1].index, label_min+6, inplace = True)

print 'dfTrain'
print dfTrain.head()


# feature engineering/encoding
print("one hot encoding")
# feats = ["haveBaby", 'gender', 'marriageStatus', 'education']
# feats = ["creativeID", "adID", "camgaignID", "advertiserID", "appID", "appPlatform", "haveBaby", 'positionType']
feats = ["haveBaby", 'gender', 'marriageStatus', 'education', 'connectionType', 
        'telecomsOperator', 'positionType', 'sitesetID', 'camgaignID','residence',
        'appID']

enc = OneHotEncoder()
for i,feat in enumerate(feats):
    # values.reshape(), Gives a new shape to an array without changing its data.
    # x_train是sparse存储的可以直接进行/2运算
    print feat
    x_train = enc.fit_transform(dfTrain[feat].values.reshape(-1, 1))
    z_test = enc.transform(dfTest[feat].values.reshape(-1, 1))
    if i == 0:
        # 添加了/2
        X_train, Z_test = x_train / 2, z_test / 2
        print(feat, 'enc smale x', x_train.shape)
        print(feat, 'sparse big x', feat, X_train.shape)
    else:
        print(feat, 'enc smale x', x_train.shape)
        X_train, Z_test = sparse.hstack((X_train, x_train)), sparse.hstack((Z_test, z_test))
        print(feat, 'sparse big x', feat, X_train.shape)

print 'dfTrain before oneHotEncoder'

X_train, X_test, Y_train, Y_test = train_test_split(X_train, \
                            y_train, train_size=0.9)

print('size of one data', X_train.shape)    
print('\n')        
print('begin to train')
print datetime.datetime.now()
time1 = time.time()


# model training 要加正则项
# print('logist')
# lr = LogisticRegression(n_jobs=-1)
# lr.fit(X_train, Y_train)
# Y_pred = lr.predict_proba(X_test)[:,1]
# Z_pred = lr.predict_proba(Z_test)[:,1]
# print ('logloss', logloss(Y_test, Y_pred))


# max_depth
# booster='gbtree', gblinear, dart
# gamma=0:扩展一个叶子的最小损失下降幅度
# subsample=1:subsample ratio of training instance 
# scale_pos_weight:正负样本代价比例
# learning_rate
# n_estimators: tree number
# reg_alpha: L1 regulazition
# reg_lambda: L2 regulazition
# ('logloss', 0.11282494431200976)


# parameters = {'max_depth': [7],
#               'n_estimators': [1000],
#               'learning_rate': [0.05],
#               'reg_alpha':[5],
#               'reg_lambda':[10]}

# loss = make_scorer(logloss, greater_is_better = False)
# print('gridsearchCV')
# xgb_model = xgb.XGBRegressor()
# gridgdb = GridSearchCV(xgb_model, parameters, n_jobs = -1, scoring=loss)
# print('fit')
# gridgdb.fit(X_train, Y_train)
# print('train done')
# print()

# best_parameters, score, _ = max(gridgdb.grid_scores_, key=lambda x: x[1])
# print('Raw AUC score:', score)
# for param_name in sorted(best_parameters.keys()):
#     print("%s: %r" % (param_name, best_parameters[param_name]))

# print "best score:"
# print gridgdb.best_score_
# print "best estimator:"
# print gridgdb.best_estimator_
# print('\n')
# Y_pred = gridgdb.predict(X_test)
# print ('logloss', logloss(Y_test, Y_pred))
# # save model from gridsearchCV
# from sklearn.externals import joblib
# joblib.dump(gridgdb.best_estimator_, 'xgboost.pkl') 
# # clf = joblib.load('filename.pkl') 
# Z_pred = gridgdb.predict(Z_test)


print('submissionx')
# submission
df = pd.DataFrame({"testlabel": Y_test, "proba": Y_pred})
df.sort_values("testlabel", inplace=True)
df.to_csv("submissionx.csv", index=False)
with zipfile.ZipFile("submissionx.zip", "w") as fout:
    fout.write("submissionx.csv", compress_type=zipfile.ZIP_DEFLATED)

time2 = time.time()
delta = time2 - time1
delta = int(delta)
print("\n\nthe time used:", delta / 60, "min", delta % 60, 'seconds\n\n')



print('submission')
# submission
df = pd.DataFrame({"instanceID": dfTest["instanceID"].values, "proba": Z_pred})
df.sort_values("instanceID", inplace=True)
df.to_csv("submission.csv", index=False)
with zipfile.ZipFile("submission.zip", "w") as fout:
    fout.write("submission.csv", compress_type=zipfile.ZIP_DEFLATED)
