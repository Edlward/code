# -*- coding: utf-8 -*-
"""
baseline 2: ad.csv (creativeID/adID/camgaignID/advertiserID/appID/appPlatform) + lr
"""
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
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.feature_selection import SelectFromModel

print datetime.datetime.now()
time1 = time.time()

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

feats = ['appID', 'residence','camgaignID']
X_train = dfTrain[feats]
Y_train = dfTrain['label']

sfm = SelectFromModel(GradientBoostingClassifier(), threshold=0.01)
sfm.fit(X_train, Y_train)
n_features = sfm.transform(X_train).shape[1]

X_transform = []
while n_features > 2:
    sfm.threshold += 0.1
    X_transform = sfm.transform(X_train)
    n_features = X_transform.shape[1]

print 'one'
feature1 = X_transform[0:10, 0]
print feature1
print 'two'
feature2 = X_transform[0:10, 1]
print feature2
time2 = time.time()
delta = time2 - time1
delta = int(delta)
print("\n\nthe time used:", delta / 60, "min", delta % 60, 'seconds\n\n')