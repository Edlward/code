## 第三周周[冠军](https://mp.weixin.qq.com/s?__biz=MzIzMzgzOTUxNA==&mid=2247483744&idx=1&sn=9047363de25d34f68c73aec1b2d60152&chksm=e8fecf95df8946833f72cde2d77ea1d37cb61716b322d566bd35d3f6e6911b0501b49630aceb&scene=0&pass_ticket=Co8P5rzD4IiIM8T07ZtbM%2BPex1nHhxHlTWchrTphSYhSOct0S1YpEqViVqty9Mz2#rd)
## [gbdt造特征](http://breezedeus.github.io/2014/11/19/breezedeus-feature-mining-gbdt.html)
## [model_evaluation](http://scikit-learn.org/stable/modules/model_evaluation.html#scoring-parameter)
## [grid_searchcv](http://scikit-learn.org/stable/modules/generated/sklearn.model_selection.GridSearchCV.html#sklearn.model_selection.GridSearchCV)
## xgboost [api](http://xgboost.readthedocs.io/en/latest/python/python_api.html#module-xgboost.sklearn)
<!--获得对应矩阵-->
pd.as_matrix()

 <!--parameter-->
eta = 0.01 (small shrinkage)
max_depth = 9 (default max_depth=6 which is not deep enough)
sub_sample = 0.9 (giving some randomness for preventing overfitting)
num_rounds = 3000 (because of slow shrinkage, many trees are needed)


## 样本分布不均衡的解决方法
http://www.zhaokv.com/2016/01/learning-from-imbalanced-data.html

## 训练集数目
0    3656266
1      93262
```
在前面，我们使用准确度这个指标来评价分类质量，可以看出，在类别不均衡时，准确度这个评价指标并不能work。因为分类器将所有的样本都分类到大类下面时，该指标值仍然会很高。即，该分类器偏向了大类这个类别的数据。
```
## 博客摘抄
http://blog.csdn.net/heyongluoyao8/article/details/49408131
八大解决方法
1. 可以扩大数据集吗？ 
2. 尝试其它评价指标 
3. 对数据集进行重采样 
4. 尝试产生人工数据样本 
5. 尝试不同的分类算法 
    决策树往往在类别不均衡数据上表现不错
    adaboost
6. 尝试对模型进行惩罚 
    比如你的分类任务是识别那些小类，那么可以对分类器的小类样本数据增加权值，降低大类样本的权值（这种方法其实是产生了新的数据分布，即产生了新的数据集，译者注）
    代价敏感矩阵，对学习算法进行改造
7. 尝试一个新的角度理解问题 

# train.csv 3749528,8
instanceID，-1，clickTime，creativeID，userID，positionID，connectionType，telecomsOperator
connectionType是wifi的激活概率肯定大
clickTime的相关性应该是每天都一样的，把小时和分钟映射到秒上
# test.csv 338489,8

预测App广告点击后被激活的概率：pCVR=P(conversion=1 | Ad,User,Context)
## 广告特征
## 用户特征
## 上下文特征
positionID:      广告曝光的具体位置
sitesetID:       站点集合，多个广告位的集合，如QQ空间
positionType：   广告位类型
connectionType： 可以看做有序特征，可以根据不同类型统计的激活数目做一个字典映射
telecomsOperator：可以看做有序特征，可以根据不同类型统计的激活数目做一个字典映射


## pandas操作
.value_counts(): 按标签统计数目
.values(): 把Series,DataFrame转换成numpy array
.get_dummies(): 会把所有数类型看做连续的，不做编码，仅对字符串特征进行one_hot_encoder
.astype(str): 把数据转换成string类型

## [groupby](https://my.oschina.net/lionets/blog/280332)
GroupBy对象，支持迭代，里面是组名和组内数据

grouped.get_group(''): 选择一个分组
grouped.size():返回分组中各组数据的大小


## 计算平均值概率
<!--as_index=False?-->
embark_perc = titanic_df[["Embarked", "Survived"]].groupby(['Embarked'],as_index=False).mean()
<!--inplace 替换-->
test_df["Fare"].fillna(test_df["Fare"].median(), inplace=True)
<!--convert number type -->
titanic_df['Fare'] = titanic_df['Fare'].astype(int)
embark_dummies_titanic  = pd.get_dummies(titanic_df['Embarked'])
embark_dummies_titanic.drop(['S'], axis=1, inplace=True)
[kaggle参考](https://www.kaggle.com/lxghust/a-journey-through-titanic-de4d36-test/editnb)

<!--train-->
('positionID', 7219, 1.0, 0.097800108293958354)
!('connectionType', 5, 0.025336495627940228, 0.010308483261562163)
!('label', 2, 1, 0.70710678118654757)
('clickTiMin', 1440, 0.036250853630776236, 0.0048289329274051728)
('creativeID', 6315, 1.0, 0.071309827665513831)
!('telecomsOperator', 4, 0.010404207972994964, 0.0049696062876111828)
<!--user-->
!('gender', 3, 0.0089172307306685863, 0.0046607920728349225)
('age', 81, 0.057495012824166428, 0.006662250035074235)
!('residence', 399, 0.078651685393258425, 0.0075065408905755091)
!('haveBaby', 7, 0.013772943834932227, 0.0050835138043705281)
!('marriageStatus', 4, 0.0041022545913669932, 0.0016869540469555092)
('hometown', 365, 0.055743243243243243, 0.0057415423374340075)
!('education', 8, 0.0091586806707391159, 0.0030160253068509456)
<!--ad-->
('advertiserID', 89, 0.42384105960264901, 0.054924098627360564)
('adID', 3447, 1.0, 0.063169867995414214)
('appID', 50, 0.42384105960264901, 0.069580370430679106)
!('camgaignID', 677, 0.63636363636363635, 0.050796291439061121)
('appPlatform', 2, 0.0012702159343590154, 0.0008981783007564663)转换率不受日期影响，只有一个19号的有问题
<!--position-->
!('sitesetID', 3, 0.02636969602101254, 0.013700972348893198)
!('positionType', 6, 0.22270724758388133, 0.087460470534174911)
<!--appcategory-->
('plateform_category', 18, 0.42384105960264901, 0.10183784364878556)
('appCategory', 14, 0.42384105960264901, 0.11342039643011995)
('appPlatform', 2, 0.0012702159343590154, 0.0008981783007564663)


## 除去点击次数不少于5的
<!--train-->
@('positionID', 3847, 0.55555555555555558, 0.04342841722528238)
@('connectionType', 5, 0.025336495627940228, 0.010308483261562163)
('label', 2, 1, 0.70710678118654757)
('clickTiMin', 1440, 0.036250853630776236, 0.0048289329274051728)
('date', 14, 0.012684419215962309, 0.0028077837477102278)
@('creativeID', 5072, 0.63636363636363635, 0.042571135930954719)
@('telecomsOperator', 4, 0.010404207972994964, 0.0049696062876111837)
<!--user-->
@('gender', 3, 0.0089172307306685863, 0.0046607920728349225)
@('age', 81, 0.057495012824166428, 0.006662250035074235)
@('residence', 396, 0.078651685393258425, 0.0072174905596757589)
@('haveBaby', 7, 0.013772943834932227, 0.0050835138043705281)
@('marriageStatus', 4, 0.0041022545913669932, 0.0016869540469555094)
@('hometown', 365, 0.055743243243243243, 0.0057415423374340075)
@('education', 8, 0.0091586806707391159, 0.0030160253068509456)
<!--ad-->
@('advertiserID', 89, 0.42384105960264901, 0.05492409862736055)
@('adID', 3060, 0.63636363636363635, 0.041196377249297429)
@('appID', 50, 0.42384105960264901, 0.06958037043067912)
@('camgaignID', 635, 0.63636363636363635, 0.049115659693790684)
@('appPlatform', 2, 0.0012702159343590154, 0.0008981783007564663)
<!--position-->
@('sitesetID', 3, 0.02636969602101254, 0.013700972348893198)
@('positionType', 6, 0.22270724758388133, 0.087460470534174911)
<!--appCategory-->
<!--('plateform_category', 18, 0.42384105960264901, 0.10183784364878556)-->
@('appCategory', 14, 0.42384105960264901, 0.11342039643011995)
@('appPlatform', 2, 0.0012702159343590154, 0.0008981783007564663)

<!--camgaignID-->
推广计划是广告的集合，类似电脑文件夹功能。广告主可以将推广平台、预算限额、是否匀速投放等条件相同的广告放在同一个推广计划中，方便管理。


## 测试计划
1. Transforming infrequent features into a special tag. Conceptually, infrequent
features should only include very little or even no information, so it
should be very hard for a model to extract those information. In fact, these
features can be very noisy. We got significant improvement (more than 0.0005)
by transforming these features into a special tag.
加入现在的根据均值概率得到的camgaignID，进行测试，再除去点击次数少与500的重新统计并训练做对比
```python
count = train[var].value_counts(sort=True, ascending=False)
tmp1 = count.values < 200
train[var].replace(count[tmp1].index, 808, inplace = True)
```
在做实验
测试结果最终结果要好，gridsearchCV内的成绩差
2. Data normalization. According to our empirical experience, instance-wise
data normalization makes the optimization problem easier to be solved.
For example, for the following vector x
(0, 0, 1, 1, 0, 1, 0, 1, 0),
the standard data normalization divides each element of x by by the 2-norm
of x. The normalized x becomes
(0, 0, 0.5, 0.5, 0, 0.5, 0, 0.5, 0).
测试结果是做/2好一些

3. 加residense变量或者，重新做本次实验，因为camgaignID最后一个分类写错<0.21,这个实际上没有关系


0.00175000449262
('appCategory', 209)
0.00133002648494
('appCategory', 2)
0.0072945670025
('appCategory', 104)
0.0156567212625
('appCategory', 201)
0.00347001701443
('appCategory', 402)
0.00425759126643
('appCategory', 108)
0.0205059205763
('appCategory', 503)
0.004421117076
('appCategory', 301)
0.00888109216407
('appCategory', 407)
0.0210952693314
('appCategory', 101)
0.00863790561396
('appCategory', 203)
0.0182415676343
('appCategory', 106)
0.355030250904
('appCategory', 0)
0.0
('appCategory', 408)

