# --coding:utf-8--

import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

path = '~/code/tpai/'
submission = pd.read_csv(path + 'submission.csv')

# 输出两种预测结果的平均值
print submission.groupby('testlabel')['proba'].mean()


# sns.distplot(submission[submission['testlabel'] == 0]['proba'])
# plt.show()

# fig = plt.figure()
# sns.distplot(submission[submission['testlabel'] == 1]['proba'])
# plt.show()