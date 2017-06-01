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

## appCategory 关系比较大，缺失值可以用get_dummies补充掉
0      0.423841
2      0.015487
101    0.030474
104    0.034928
106    0.044643
108    0.006938
201    0.031498
203    0.096585
209    0.020328
301    0.028042
402    0.037568
407    0.203723
408    0.000000
503    0.031987

## appPlateform 关系不大
appPlatform
2    0.025866
1    0.024596