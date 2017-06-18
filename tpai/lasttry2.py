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
            dfTrain[var].replace(flg, 0, inplace=True)
            dfTest[var].replacce(flg, 0, inplace=True)

            print()
            print(var, flg, dateType[flg].std())
            continue
        dfTrain[var].replace(flg, global_mean[flg], inplace=True) 
        dfTest[var].replace(flg, global_mean[flg], inplace=True)
        print(num,'replace ', flg, 'with', global_mean[flg], 'while std() is', dateType[flg].std())


# 0-504
# var = 'appCategory'
# process(var)
# dfTrain[var].replace(0, 0, inplace=True) 
# dfTest[var].replace(0, 0, inplace=True)
# dfTrain[var].replace(101, 0.031879594759824458, inplace=True) 
# dfTest[var].replace(101, 0.031879594759824458, inplace=True)
# dfTrain[var].replace(503, 0.051995436834841556, inplace=True) 
# dfTest[var].replace(503, 0.051995436834841556, inplace=True)

var = 'appID'
var2 = 'appID2'
dfTrain[var2] = 0
process(var, var2)
# dfTrain[var].replace(146, 0.037239048419402135, inplace=True) 
# dfTest[var].replace(146, 0.037239048419402135, inplace=True)
# dfTrain[var].replace(218, 0.043062200956937802, inplace=True) 
# dfTest[var].replace(218, 0.043062200956937802, inplace=True)
# dfTrain[var].replace(271, 0.11100056379486413, inplace=True) 
# dfTest[var].replace(271, 0.11100056379486413, inplace=True)
# dfTrain[var].replace(105, 0.0, inplace=True) 
# dfTest[var].replace(105, 0.0, inplace=True)
# check = dfTest[var] > 0.9
if check.any():
    print('dfTest has, train not have')
check = dfTrain[var] > 0.9
if check.any():
    print('dfTrain has something have not transform')