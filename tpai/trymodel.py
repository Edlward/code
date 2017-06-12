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
from xgboost import XGBRegressor
import scipy as sp
from sklearn.model_selection import cross_val_score, GridSearchCV
# from sklearn.grid_search import GridSearchCV
from sklearn.externals import joblib

def logloss(act, pred):
  epsilon = 1e-15
  pred = sp.maximum(epsilon, pred)
  pred = sp.minimum(1-epsilon, pred)
  ll = sum(act*sp.log(pred) + sp.subtract(1,act)*sp.log(sp.subtract(1,pred)))
  ll = ll * -1.0/len(act)
  return ll


# load data
data_root = "~/codedata/tpai/pre"
dfTrain = pd.read_csv("%s/train.csv"%data_root)
dfTest = pd.read_csv("%s/test.csv"%data_root)
dfAd = pd.read_csv("%s/ad.csv"%data_root)

# process data
dfTrain = pd.merge(dfTrain, dfAd, on="creativeID")
dfTest = pd.merge(dfTest, dfAd, on="creativeID")
y_train = dfTrain["label"].values

# feature engineering/encoding
enc = OneHotEncoder()
feats = ["creativeID", "adID", "camgaignID", "advertiserID", "appID", "appPlatform"]


print("data load done")

for i,feat in enumerate(feats):
    x_train = enc.fit_transform(dfTrain[feat].values.reshape(-1, 1))
    x_test = enc.transform(dfTest[feat].values.reshape(-1, 1))
    if i == 0:
        X_train, X_test = x_train, x_test
    else:
        X_train, X_test = sparse.hstack((X_train, x_train)), sparse.hstack((X_test, x_test))

# X_train, X_test, Y_train, Y_test = train_test_split(X_train, \
                            # y_train, train_size=0.8)


print('load model, make prediction')
gridgdb = joblib.load('xgboost.pkl') 
Y_pred = gridgdb.predict(X_train)

print logloss(y_train, Y_pred)

print('submission')
# submission
df = pd.DataFrame({"testlabel": y_train, "proba": Y_pred})
df.sort_values("testlabel", inplace=True)
df.to_csv("submission.csv", index=False)
with zipfile.ZipFile("submission.zip", "w") as fout:
    fout.write("submission.csv", compress_type=zipfile.ZIP_DEFLATED)

