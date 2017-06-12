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

y_train = dfTrain["label"].values

# feats = ["advertiserID", "appID", "appPlatform"]  # 分类87 0.109318919187
# feats = ["creativeID", "adID", "camgaignID", "advertiserID", "appID", "appPlatform"]  # 回归0.105556
# feats = ["creativeID", "camgaignID", "advertiserID", "appID", "appPlatform"]  # 回归0.105707
# feats = ["creativeID", "camgaignID", "advertiserID", "appID", "appPlatform", 'positionID', 'connectionType']  # 回归67 0.103695978912 分类360 0.102159034421

dfTrain['positionID_connectionType'] = dfTrain['connectionType'] * 10000 + dfTrain['positionID']
dfTest['positionID_connectionType'] = dfTest['connectionType'] * 10000 + dfTest['positionID']
# 分类344 0.102020271947， 129 0.101822653311 
# 线上0.105
# feats = ["creativeID", "camgaignID", "advertiserID", "appID", "appPlatform", 'positionID', 'connectionType', 'positionID_connectionType'] 
feats = ["creativeID", "camgaignID", "advertiserID", "appID", "appPlatform", 'positionID', 'positionID_connectionType','appCategory'] #0.105262251052
# feats = ["creativeID", "camgaignID", "advertiserID", "appID", "appPlatform", 'positionID', 'positionID_connectionType','appCategory'] #0.105262251052


# positionID connectionType
# feature engineering/encoding
# enc = OneHotEncoder()
# for i,feat in enumerate(feats):
#     x_train = enc.fit_transform(dfTrain[feat].values.reshape(-1, 1))
#     x_test = enc.transform(dfTest[feat].values.reshape(-1, 1))
#     if i == 0:
#         X_train, X_test = x_train, x_test
#     else:
#         X_train, X_test = sparse.hstack((X_train, x_train)), sparse.hstack((X_test, x_test))

# model training
# lr = LogisticRegression()
# lr.fit(X_train, y_train)
# proba_test = lr.predict_proba(X_test)[:,1]
# 66 0.101; 30 0.101515; 0.1019
dfTrain['date'] = dfTrain['clickTime'].values / 10000
# X_train = dfTrain[dfTrain['date'] != 24][feats].values
# Y_train = dfTrain[dfTrain['date'] != 24]['label'].values
X_train = pd.concat([dfTrain[dfTrain['date'] != 24][feats], dfTrain[dfTrain['date'] != 17][feats]]).values
Y_train = pd.concat([dfTrain[dfTrain['date'] != 24]['label'], dfTrain[dfTrain['date'] != 17]['label']]).values
# X_train = dfTrain[feats].values
# Y_train = dfTrain['label'].values

# X_test = dfTrain[dfTrain['date'] == 24][feats].values
# Y_test = dfTrain[dfTrain['date'] == 24]['label'].values
X_test = pd.concat([dfTrain[dfTrain['date'] == 24][feats], dfTrain[dfTrain['date'] == 17][feats]]).values
Y_test = pd.concat([dfTrain[dfTrain['date'] == 24]['label'], dfTrain[dfTrain['date'] == 17]['label']]).values

# X_train = dfTrain[feats].values
# X_train, X_test, Y_train, Y_test = train_test_split(X_train, \
#                             y_train, train_size=0.7, random_state=80)

# xgbRegressor
# model = xgb.XGBRegressor(max_depth = 7, learning_rate=0.1, n_estimators=500,
#             silent=False)
# model.fit(X_train, Y_train, eval_set=([X_train,Y_train], [X_test,Y_test]),
#                     eval_metric="logloss", early_stopping_rounds=5)
# Y_pred = model.predict(X_test)

# xgbClassifier
model = xgb.XGBClassifier(max_depth = 7, learning_rate=0.1, n_estimators=1000,
            silent=False, subsample=1, reg_lambda=10)
model.fit(X_train, Y_train, eval_set=([X_train,Y_train], [X_test,Y_test]),
                    eval_metric="logloss", early_stopping_rounds=3)
Y_pred = model.predict_proba(X_test)[:,1]


print(X_train.shape)
print logloss(Y_test, Y_pred)

# mysubmission
df = pd.DataFrame({"instanceID": Y_test, "proba": Y_pred})
df.sort_values("instanceID", inplace=True)
df.to_csv("submissionx.csv", index=False)
with zipfile.ZipFile("submissionx.zip", "w") as fout:
    fout.write("submissionx.csv", compress_type=zipfile.ZIP_DEFLATED)

Z_test = dfTest[feats].values
Z_pred = model.predict_proba(Z_test)[:,1]

# submission
df = pd.DataFrame({"instanceID": dfTest["instanceID"].values, "proba": Z_pred})
df.sort_values("instanceID", inplace=True)
df.to_csv("submission.csv", index=False)
with zipfile.ZipFile("submission.zip", "w") as fout:
    fout.write("submission.csv", compress_type=zipfile.ZIP_DEFLATED)

print('save xgbfir')
# 下面这行会改变feats的内容所以要放到最后
xgbfir.saveXgbFI(model, feature_names=feats, OutputXlsxFile = 'xgboost.xlsx')
