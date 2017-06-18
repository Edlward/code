# gridsearchCV XGBoost
# haveBaby gender marriageStatus education 0.11461439330447962 18:min
# 不做除以2处理
parameters = {'max_depth': [7],
              'n_estimators': [100],
              'learning_rate': [0.05, 0.1],
              'reg_alpha':[0, 1],
              'reg_lambda':[0, 1]}
learning_rate: 0.1
max_depth: 7
n_estimators: 100
reg_alpha: 1
reg_lambda: 1
best score:
-0.114987063128
XGBRegressor(base_score=0.5, colsample_bylevel=1, colsample_bytree=1, gamma=0,
       learning_rate=0.1, max_delta_step=0, max_depth=7,
       min_child_weight=1, missing=None, n_estimators=100, nthread=-1,
       objective='reg:linear', reg_alpha=1, reg_lambda=1,
       scale_pos_weight=1, seed=0, silent=True, subsample=1)
('logloss', 0.1163700577624894)
submission
('\n\nthe time used:', 24, 'min', 8, 'seconds\n\n')

# 做除以2处理
learning_rate: 0.1
max_depth: 7
n_estimators: 100
reg_alpha: 1
reg_lambda: 0
best score:
-0.115989908962
best estimator:
XGBRegressor(base_score=0.5, colsample_bylevel=1, colsample_bytree=1, gamma=0,
       learning_rate=0.1, max_delta_step=0, max_depth=7,
       min_child_weight=1, missing=None, n_estimators=100, nthread=-1,
       objective='reg:linear', reg_alpha=1, reg_lambda=0,
       scale_pos_weight=1, seed=0, silent=True, subsample=1)
('logloss', 0.11569002230891422)
submission
('\n\nthe time used:', 23, 'min', 35, 'seconds\n\n')

# haveBaby gender marriageStatus education connectionType telecomsOperator 0.11351949059460317 40min
# haveBaby gender marriageStatus education connectionType telecomsOperator positionType sitesetID 0.11129409828423736 105:min
parameters = {'max_depth': [7],
              'n_estimators': [150, 200],
              'learning_rate': [0.05, 0.1],
              'reg_alpha':[1, 5],
              'reg_lambda':[0, 1]}
max_depth: 7
n_estimators: 150
reg_alpha: 5
reg_lambda: 0
best score:
-0.111002602677

XGBRegressor(base_score=0.5, colsample_bylevel=1, colsample_bytree=1, gamma=0,
       learning_rate=0.05, max_delta_step=0, max_depth=7,
       min_child_weight=1, missing=None, n_estimators=150, nthread=-1,
       objective='reg:linear', reg_alpha=5, reg_lambda=0,
       scale_pos_weight=1, seed=0, silent=True, subsample=1)

# haveBaby gender marriageStatus education connectionType telecomsOperator positionType sitesetID camgaignID
# by / 2， 做概率归类， 没有做把频数少的归为一类 55维
parameters = {'max_depth': [7],
              'n_estimators': [130, 150],
              'learning_rate': [0.05, 0.1],
              'reg_alpha':[5, 10],
              'reg_lambda':[0, 1]}
learning_rate: 0.05
max_depth: 7
n_estimators: 150
reg_alpha: 5
reg_lambda: 1
best score:
-0.105320888095
best estimator:
XGBRegressor(base_score=0.5, colsample_bylevel=1, colsample_bytree=1, gamma=0,
       learning_rate=0.05, max_delta_step=0, max_depth=7,
       min_child_weight=1, missing=None, n_estimators=150, nthread=-1,
       objective='reg:linear', reg_alpha=5, reg_lambda=1,
       scale_pos_weight=1, seed=0, silent=True, subsample=1)
('logloss', 0.10591721331135862)
('\n\nthe time used:', 82, 'min', 6, 'seconds\n\n')
# by / 2, 做概率归类，没有做把频数少的归为一类 55维
parameters = {'max_depth': [7],
              'n_estimators': [130, 150],
              'learning_rate': [0.05, 0.1],
              'reg_alpha':[5, 10],
              'reg_lambda':[0, 1]}
learning_rate: 0.05
max_depth: 7
n_estimators: 150
reg_alpha: 10
reg_lambda: 1
best score:
-0.105753775247
best estimator:
XGBRegressor(base_score=0.5, colsample_bylevel=1, colsample_bytree=1, gamma=0,
       learning_rate=0.05, max_delta_step=0, max_depth=7,
       min_child_weight=1, missing=None, n_estimators=150, nthread=-1,
       objective='reg:linear', reg_alpha=10, reg_lambda=1,
       scale_pos_weight=1, seed=0, silent=True, subsample=1)
('logloss', 0.10565753103547368)
# by / 2,做概率归类，把频数少的归为一类 53维
parameters = {'max_depth': [7],
              'n_estimators': [130, 150],
              'learning_rate': [0.05, 0.1],
              'reg_alpha':[5, 10],
              'reg_lambda':[0, 1]}
learning_rate: 0.05
max_depth: 7
n_estimators: 150
reg_alpha: 5
reg_lambda: 1
best score:
-0.106450874428
best estimator:
XGBRegressor(base_score=0.5, colsample_bylevel=1, colsample_bytree=1, gamma=0,
       learning_rate=0.05, max_delta_step=0, max_depth=7,
       min_child_weight=1, missing=None, n_estimators=150, nthread=-1,
       objective='reg:linear', reg_alpha=5, reg_lambda=1,
       scale_pos_weight=1, seed=0, silent=True, subsample=1)
('logloss', 0.10533820337029014)
submission
('\n\nthe time used:', 81, 'min', 14, 'seconds\n\n')
# haveBaby gender marriageStatus education connectionType telecomsOperator positionType sitesetID camgaignID residence
# by / 2,做概率归类，把频数少的归为一类 65维
parameters = {'max_depth': [7],
              'n_estimators': [130, 150],
              'learning_rate': [0.05, 0.1],
              'reg_alpha':[5, 10],
              'reg_lambda':[0, 1]}
('Raw AUC score:', -0.10564714729219096)
learning_rate: 0.05
max_depth: 7
n_estimators: 150
reg_alpha: 5
reg_lambda: 1
best score:
-0.105647147292
best estimator:
XGBRegressor(base_score=0.5, colsample_bylevel=1, colsample_bytree=1, gamma=0,
       learning_rate=0.05, max_delta_step=0, max_depth=7,
       min_child_weight=1, missing=None, n_estimators=150, nthread=-1,
       objective='reg:linear', reg_alpha=5, reg_lambda=1,
       scale_pos_weight=1, seed=0, silent=True, subsample=1)


('logloss', 0.10571396867788653)
submissionx
('\n\nthe time used:', 88, 'min', 58, 'seconds\n\n')
submission


# haveBaby gender marriageStatus education connectionType telecomsOperator positionType sitesetID camgaignID residence appID
parameters = {'max_depth': [7],
              'n_estimators': [150, 200],
              'learning_rate': [0.05],
              'reg_alpha':[1, 5],
              'reg_lambda':[0, 1]}
('Raw AUC score:', -0.10620989458011398)
learning_rate: 0.05
max_depth: 7
n_estimators: 1000
reg_alpha: 5
reg_lambda: 10
best score:
-0.10620989458
best estimator:
XGBRegressor(base_score=0.5, colsample_bylevel=1, colsample_bytree=1, gamma=0,
       learning_rate=0.05, max_delta_step=0, max_depth=7,
       min_child_weight=1, missing=None, n_estimators=1000, nthread=-1,
       objective='reg:linear', reg_alpha=5, reg_lambda=10,
       scale_pos_weight=1, seed=0, silent=True, subsample=1)


('logloss', 0.10711507170250818)
submissionx
('\n\nthe time used:', 112, 'min', 25, 'seconds\n\n')
submission

# 
parameters = {'max_depth': [7],
              'n_estimators': [500],
              'learning_rate': [0.05],
              'reg_alpha':[1, 5],
              'reg_lambda':[0, 1]}
# xgboost
# "creativeID", "adID", "camgaignID", "advertiserID", "appID", "appPlatform", max_depth = 5 n_estimators = 300 0.106978003831
# "creativeID", "adID", "camgaignID", "advertiserID", "appID", "appPlatform", max_depth = 5 n_estimators = 500 0.105873391877
# "creativeID", "adID", "camgaignID", "advertiserID", "appID", "appPlatform", "haveBaby" 0.105702103993
# "creativeID", "adID", "camgaignID", "advertiserID", "appID", "appPlatform", "haveBaby" 'marriageStatus' 0.10729924724
# "creativeID", "adID", "camgaignID", "advertiserID", "appID", "appPlatform", "haveBaby" "age" 0.107051819563
# "creativeID", "adID", "camgaignID", "advertiserID", "appID", "appPlatform", "haveBaby" "positionType" 0.106400307962
# "creativeID", "adID", "camgaignID", "advertiserID", "appID", "appPlatform", "haveBaby" max_depth = 7 n_estimators = 500 0.106032771155
# "creativeID", "adID", "camgaignID", "advertiserID", "appID", "appPlatform", "haveBaby" max_depth = 5 n_estimators = 800 0.105447070463
# xgb = XGBRegressor(max_depth=5, n_estimators=500, learning_rate=0.05).fit(X_train, Y_train)
# xgb.fit(X_train, Y_train)
# Y_pred= xgb.predict(X_test)
# print logloss(Y_test, Y_pred)