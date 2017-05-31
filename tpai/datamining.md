[分析数据的例子](https://www.kaggle.com/pmarcelino/comprehensive-data-exploration-with-python)
[tpai比赛资料  干货】Kaggle 数据挖掘比赛经验分享](https://mp.weixin.qq.com/s?__biz=MzIzMzgzOTUxNA==&mid=2247483678&idx=1&sn=5f044dabfaa726e292686287a1dd5ca4&pass_ticket=4UOhRb2j3wjSghc071hfIkbB5KFW0mpttMZSpN7b6sVLQ1qJj2jTZFYK1XBh7T2K)
[干货 sklern数据挖掘](http://www.cnblogs.com/jasonfreak/p/5448385.html)
[python 机器学习中文笔记](https://ljalphabeta.gitbooks.io/python-/content/categorical_data.html)

""有一种说法是，特征决定了效果的上限，而不同模型只是以不同的方式或不同的程度来逼近这个上限""

## analysing data

##　处理missing data
统计缺失数据的数目、百分比

## outliers

cording to Hair et al. (2013), four assumptions should be tested:
使用log变换可以纠正数据的偏度(skewness)
## 流程
- 数据分析  
    * 分析特征变量的分布  
        特征变量为连续值：如果为长尾分布并且考虑使用线性模型，可以对变量进行幂变换或者对数变换  
        特征变量为离散值：观察频率分布，对于频次较低的特征，可以统一编码为”其他“特征
    * 分析目标变量的分布  
        目标变量为连续值：查看目标变量分布范围是否较大，如果较大，可以考虑对数变换，Box-Cox变换？  
        目标变量为离散值：如果数据分布不均衡，可以上采样、下采样、分层采样
    * 分析变量之间两两的分布和相关度  
        可用于发现高相关和共线性的特征  
- 数据清洗
    * 数据拼接  
        数据分布在多个文件，根据键值进行拼接  
    * 特征缺失值处理  
        特征值为连续值：按不同分布类型对缺失值进行补全：偏正态分布，使用均值代替，可以保持数据的均值；偏长尾分布，使用中值代替，避免受outlier的影响  
        特征值为离散值：使用众数代替
    * 文本数据的清洗  
- 特征工程  ""特征决定了效果的上限，不同模型只是以不同方式或不同程度逼近这个上限"" 
    * 特征变换
        幂变换、对数变换
    * 特征编码 
        离散特征值：LableEncoder、OneHotEncoder
- 模型训练
    * 模型选择
    * 调参和模型验证
    * 模型集成
        averaging、voting
        stacking
        blending
        bagging ensemble selection
    * 自动化框架[github分享](https://github.com/ChenglongChen/Kaggle_CrowdFlower)
        模块化特征方程
        自动化模型调参和验证：参数空间调优，grid search，hyperopt
        自动化模型集成
- 模型验证
- 模型预测
## 特征工程
[特征工程](http://breezedeus.github.io/2014/11/15/breezedeus-feature-processing.html)
[美团技术点评特征工程](http://tech.meituan.com/machinelearning-data-feature-process.html)
- 连续特征
- 无序类别特征
    转成one-hot，再聚类、降维    
    独热编码、LableEncoder
- 有序类别特征
    做字典映射

## terminology
脱敏特征：特征的含义不清楚
public leaderboard(LB): 用于计算排名的测试集，总体来说算是验证集
private leaderboard(LB): 最终排名计算的测试机