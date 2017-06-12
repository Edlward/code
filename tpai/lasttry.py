# -*- coding: utf-8 -*-
"""
baseline 2: ad.csv (creativeID/adID/camgaignID/advertiserID/appID/appPlatform) + lr
"""
# 0.107
import zipfile
import pandas as pd
from scipy import sparse
from sklearn.preprocessing import OneHotEncoder
from sklearn.model_selection import train_test_split
import xgboost as xgb
import xgbfir
import scipy as sp



def logloss(act, pred):
  epsilon = 1e-15
  pred = sp.maximum(epsilon, pred)
  pred = sp.minimum(1-epsilon, pred)
  ll = sum(act*sp.log(pred) + sp.subtract(1,act)*sp.log(sp.subtract(1,pred)))
  ll = ll * -1.0/len(act)
  print ll
  return ll


global dfTrain
global dfTest

# load data
data_root = "~/codedata/tpai/pre"
dfTrain = pd.read_csv("%s/train.csv"%data_root)
dfTest = pd.read_csv("%s/test.csv"%data_root)
dfAd = pd.read_csv("%s/ad.csv"%data_root)
dfUser = pd.read_csv("%s/user.csv"%data_root)
dfPosition = pd.read_csv("%s/position.csv"%data_root)
dfAppcategory = pd.read_csv("%s/app_categories.csv"%data_root)

print('merge data')
# process data
dfTrain = pd.merge(dfTrain, dfAd, on="creativeID", how = 'left')
dfTest = pd.merge(dfTest, dfAd, on="creativeID", how = 'left')
dfTrain = pd.merge(dfTrain, dfUser, on = 'userID', how = 'left')
dfTest = pd.merge(dfTest, dfUser, on = 'userID', how = 'left')
dfTrain = pd.merge(dfTrain, dfPosition, on = 'positionID', how = 'left')
dfTest = pd.merge(dfTest, dfPosition, on = 'positionID', how = 'left')
dfTrain = pd.merge(dfTrain, dfAppcategory, on = 'appID', how = 'left')
dfTest = pd.merge(dfTest, dfAppcategory, on = 'appID', how = 'left')
dfTrain.drop('userID', axis=1, inplace=True)
dfTest.drop('userID', axis=1, inplace=True)
dfTrain.drop('conversionTime', axis=1, inplace=True)
# dfTest.drop('conversionTime', axis=1, inplace=True)
# dfTrain.drop('clickTime', axis=1, inplace=True)
# dfTest.drop('clickTime', axis=1, inplace=True)
del dfAd
del dfUser
del dfPosition
del dfAppcategory
print('merge done')

# 添加data属性
var = 'date'
dfTrain[var] = (dfTrain['clickTime'].values) / 10000

def process(var, var2):
    global dfTrain
    global dfTest
    
    count = dfTrain[var].value_counts()
    count_index = count.values < 5
    if count_index.any():
        dfTrain[var].replace(count[count_index].index, 10000, inplace=True) 
    
    global_mean = dfTrain['label'].groupby(dfTrain[var]).mean()
    date_mean = dfTrain['label'].groupby([dfTrain['date'], dfTrain[var]]).mean()
    dateType = date_mean.unstack()
    num = 0
    for flg in dfTrain[var].value_counts().index:
        num = num+1
        if dateType[flg].std() > 0.02:
            # 中位数填充
            count_proba = dateType[flg].median()
            dfTrain[var].replace(flg, count_proba, inplace=True)
            dfTest[var].replace(flg, count_proba, inplace=True)
            
            # myindex = dfTrain[var] == flg
            # dfTrain.loc[myindex, var2] = flg
            # myindex = dfTest[var] == flg
            # dfTest.loc[myindex,var2] = flg
            
            # print()
            # print(var, flg, dateType[flg].std())
            continue
        dfTrain[var].replace(flg, global_mean[flg], inplace=True) 
        dfTest[var].replace(flg, global_mean[flg], inplace=True)
        # print(num,'replace ', flg, 'with', global_mean[flg], 'while std() is', dateType[flg].std())

def emptyValue(var,var2):
    global dfTest
    check = dfTest[var] > 0.9
    myindexs = dfTest[check][var].value_counts().index
    for myindex in myindexs:
        # dfTest.loc[dfTest[var]==myindex, var2] = myindex
        dfTest[var].replace(myindex, 0, inplace=True)
        # print(myindex, 'is not in train, but in test')

def check(var):
    check = dfTest[var] > 0.9
    if check.any():
        myindexs = dfTest[check][var].value_counts().index
        for myindex in myindexs:
            dfTest[var].replace(myindex, 0, inplace=True)
            print(var, myindex, 'dfTest has, train not have')
    check = dfTrain[var] > 0.9
    if check.any():
        myindexs = dfTrain[check][var].value_counts().index
        for myindex in myindexs:
            dfTrain[var].replace(myindex, 0, inplace=True)
            print(var, myindex, 'dfTrain has something have not transform')

dfTrain['date'] = dfTrain['clickTime'].values / 10000


# 0-504
var = 'appCategory'
var2 = 'appCategory2'
process(var, var2)
# emptyValue(var, var2)
check(var)

var = 'marrige_appID'
dfTrain[var] = dfTrain['marriageStatus'] * 1000 + dfTrain['appID']
dfTest[var] = dfTest['marriageStatus'] * 1000 + dfTest['appID']
process(var, var2)
check(var)

var = 'haveBaye_appID'
dfTrain[var] = dfTrain['haveBaby'] * 1000 + dfTrain['appID']
dfTest[var] = dfTest['haveBaby'] * 1000 + dfTest['appID']
process(var, var2)
check(var)

var = 'gender_appID'
dfTrain[var] = dfTrain['gender'] * 1000 + dfTrain['appID']
dfTest[var] = dfTest['gender'] * 1000 + dfTest['appID']
process(var, var2)
check(var)


var = 'age_appID'
dfTrain[var] = dfTrain['age'] * 1000 + dfTrain['appID']
dfTest[var] = dfTest['age'] * 1000 + dfTest['appID']
process(var, var2)
check(var)

var = 'age_category'
dfTrain[var] = dfTrain['age'] * 1000 + dfTrain['appCategory']
dfTest[var] = dfTest['age'] * 1000 + dfTest['appCategory']
process(var, var2)
check(var)

# 500
var = 'appID'
process(var, var2)
check(var)


# 
# var = 'appID_category'
# dfTrain[var] = 0
# dfTest[var] = 0
# dfTrain[var] = dfTrain['appID'].values.round(2) * 10000 + dfTrain['appCategory'].values.round(3)*1000 
# dfTest[var] = dfTest['appID'].values.round(2) * 10000 + dfTest['appCategory'].values.round(3)*1000 
# process(var, var2)
# check(var)

# 0-100
var = 'advertiserID'
process(var, var2)
# emptyValue(var, var2)
check(var)

# 0-720
var = 'camgaignID'
process(var, var2)
# emptyValue(var, var2)
check(var)

# 6582
var = 'creativeID'
process(var, var2)
# emptyValue(var, var2)
check(var)

var = 'appPlatform'
process(var, var2)
check(var)



dfTrain['positionID_connectionType'] = dfTrain['connectionType'] * 10000 + dfTrain['positionID']
dfTest['positionID_connectionType'] = dfTest['connectionType'] * 10000 + dfTest['positionID']
var = 'positionID_connectionType'
process(var, var2)
check(var)

var = 'connectionType'
process(var, var2)
check(var)

var = 'positionID'
process(var, var2)
check(var)

# 0-3616
var = 'adID'
process(var, var2)
check(var)


var = 'positionType'
process(var, var2)
check(var)

dfTrain['age_gender'] = dfTrain['gender'] * 100 + dfTrain['age']
dfTest['age_gender'] = dfTrain['gender'] * 100 + dfTrain['age']
var = 'age_gender'
process(var, var2)
check(var)

var = 'age'
process(var, var2)
check(var)

var = 'gender'
process(var, var2)
check(var)

var = 'residence'
process(var, var2)
check(var)

# var = 'haveBaby'
# process(var, var2)
# check(var)

var = 'marriageStatus'
process(var, var2)
check(var)

var = 'hometown'
process(var, var2)
check(var)

var = 'education'
process(var, var2)
check(var)

var = 'sitesetID'
process(var, var2)
check(var)

var = 'telecomsOperator'
process(var, var2)
check(var)

# 模型训练，预测
feats = ['appCategory','appID', 'advertiserID', 'camgaignID', 'creativeID',
        'appPlatform', 'positionID', 'positionID_connectionType', 'adID',
        'positionType', 'age', 'gender', 'age_gender', 'residence',
        'marriageStatus', 'hometown', 'education', 'sitesetID', 'connectionType',
        'marrige_appID','haveBaye_appID','gender_appID','age_appID','age_category']


# feats = ['appCategory','appID', 'advertiserID']

X_test = pd.concat([dfTrain[dfTrain['date'] == 24][feats], dfTrain[dfTrain['date'] == 17][feats]]).values
Y_test = pd.concat([dfTrain[dfTrain['date'] == 24]['label'], dfTrain[dfTrain['date'] == 17]['label']]).values
# X_train = pd.concat([dfTrain[dfTrain['date'] != 24][feats], dfTrain[dfTrain['date'] != 17][feats]]).values
# Y_train = pd.concat([dfTrain[dfTrain['date'] != 24]['label'], dfTrain[dfTrain['date'] != 17]['label']]).values

X_train = dfTrain[feats].values
Y_train = dfTrain['label'].values


print('begin to train')
# xgbClassifier
# model = xgb.XGBClassifier(max_depth = 7, learning_rate=0.1, n_estimators=500,
#             silent=False, subsample=1, reg_lambda=10)
# model.fit(X_train, Y_train, eval_set=([X_train,Y_train], [X_test,Y_test]),
#                     eval_metric="logloss", early_stopping_rounds=2)
# Y_pred = model.predict_proba(X_test)[:,1]

# model = xgb.XGBClassifier(max_depth = 7, learning_rate=0.1, n_estimators=500,
#             silent=False, subsample=1, reg_lambda=10)
# model.fit(X_train, Y_train, eval_set=([X_test,Y_test]),
#                     eval_metric="logloss", early_stopping_rounds=2)
# Y_pred = model.predict_proba(X_test)[:,1]

# 
dtrain = xgb.DMatrix(X_train, label=Y_train)
dvalify = xgb.DMatrix(X_test, label=Y_test)
del X_train
del X_test

# specify parameters via map, definition are same as c++ version  
param = {'max_depth':7,
         'n_estimators':1000,
         'eta':0.05, 
         'silent':1, 
         'objective':'binary:logistic', 
         'eval_metric':'logloss',
         'reg_lambda':20 }  

num_round = 150
evallist = [(dvalify, 'eval'), (dtrain, 'train')]
bst = xgb.train(param, dtrain, num_round, evallist)
bst.save_model('xgboost.model')
Y_pred = bst.predict(dvalify)


print logloss(Y_test, Y_pred)


# mysubmission
df = pd.DataFrame({"instanceID": Y_test, "proba": Y_pred})
df.sort_values("instanceID", inplace=True)
df.to_csv("submissionx.csv", index=False)
with zipfile.ZipFile("submissionx.zip", "w") as fout:
    fout.write("submissionx.csv", compress_type=zipfile.ZIP_DEFLATED)

# Z_test = dfTest[feats].values
# Z_pred = model.predict_proba(Z_test)[:,1]

Z_test = xgb.DMatrix(dfTest[feats].values)
Z_pred = bst.predict(Z_test, ntree_limit=bst.best_ntree_limit)

# submission
df = pd.DataFrame({"instanceID": dfTest["instanceID"].values, "proba": Z_pred})
df.sort_values("instanceID", inplace=True)
df.to_csv("submission.csv", index=False)
with zipfile.ZipFile("submission.zip", "w") as fout:
    fout.write("submission.csv", compress_type=zipfile.ZIP_DEFLATED)

# 71 feats + feats2 validation_0-logloss:0.106431	validation_1-logloss:0.110743
# 72 feats validation_0-logloss:0.106431	validation_1-logloss:0.110744
# 59 feats2 validation_0-logloss:0.116213	validation_1-logloss:0.119058
# 66 feats	validation_0-logloss:0.106338	validation_1-logloss:0.110628
# 82 feats	validation_0-logloss:0.105197	validation_1-logloss:0.109576
# 166 feats	validation_0-logloss:0.101096	validation_1-logloss:0.105124
# [133]	validation_0-logloss:0.100174	validation_1-logloss:0.10421 中位数填充
# [124]	validation_0-logloss:0.100159	validation_1-logloss:0.104205
# [139]	validation_0-logloss:0.100071	validation_1-logloss:0.104137
#　[461]	validation_0-logloss:0.096659	validation_1-logloss:0.102084
# 100 0.1027
# 100 0.102788645829
# [99]	eval-logloss:0.102789	train-logloss:0.09784


print('save xgbfir')
# 下面这行会改变feats的内容所以要放到最后
featmp = feats
xgbfir.saveXgbFI(bst, feature_names=featmp, OutputXlsxFile = 'xgboost.xlsx')



