# coding=-utf8
import os
import pandas as pd
import numpy as np

path_image = '/home/we/zsw/train'
path_csv = '/home/lxg/codedata/cdiscount'

category_name = pd.read_csv(os.path.join(path_csv, 'category_names.csv'))
#    category_id              category_level1     category_level2                  category_level3  
# 0   1000021794        ABONNEMENT / SERVICES      CARTE PREPAYEE           TE PREPAYEE MULTIMEDIA        

category_name.shape # (5270, 4)
category_name.groupby(['category_level1']).sum().shape  #(49, 1)
category_name.groupby(['category_level2']).sum().shape  #(483, 1)
category_name.groupby(['category_level3']).sum().shape  #(5263, 1) the name in level3 can be same, but means different class, because they belong to differnt level1 name

category_name['level1_sub_class'] = category_name.groupby(['category_level1']).cumcount()  # add level1_sub_class collumn


data = pd.read_csv(os.path.join(path_csv, 'productCategoryImgs.csv'))
'''
data.head
   _id  category_id  img_num
0    0   1000010653        1
1    1   1000010653        1
2    2   1000004079        1
'''

data.shape    # (7069896, 3)
data['img_num'].sum() # 12,371,293 image 

data = data.reindex(np.repeat(data.index.values, data['img_num']), method='ffill')  # add cum_sum collumn, img
data['cum_sum' ] = data.groupby(['_id']).cumcount()

# category_name = pd.read_csv(os.path.join(path_csv, 'category_names.csv'))
level1_class = category_name.drop_duplicates(['category_level1'])
level1_class['label'] = level1_class.reset_index().index  # reset_index, assure the index is from zero
level1_to_index = dict(zip(level1_class['category_level1'], level1_class['label']))
category_name['level1_nick'] = category_name['category_level1'].map(level1_to_index)

category_to_level1 = dict(zip(category_name['category_id'], category_name['level1_nick']))
data['level1'] = data['category_id'].map(category_to_level1)
category_name['category_simple'] = category_name.index
category_to_simple = dict(zip(category_name['category_id'], category_name['category_simple']))

data['category_simple_id'] = data['category_id'].map(category_to_simple)
# category_id to level1_sub_class 
category_to_level1_sub_class = dict(zip(category_name['category_id'], category_name['level1_sub_class']))
data['level1_sub_class'] = data['category_id'].map(category_to_level1_sub_class)
data.to_csv(os.path.join(path_csv, 'productCategoryImgsLevel1.csv'), index=False)
# productCategoryImgsLevel1.csv
#    _id  category_id  img_num  cum_sum  level1  category_simple_id   level1_sub_class 
# 0    0   1000010653        1        0      45                5055                 31  

# level1 12371293L
# 0          32
# 1        2906
# 2      127295
# 3          30
# 4      287488
# 5       65709
# 6     1193619
# 7      434675
# 8       16670
# 9      688243
# 10     620366
# 11     369325
# 12       2013
# 13      10004
# 14    1111509
# 15      38713
# 16      86739
# 17     378019
# 18      33592
# 19      37815
# 20        208
# 21     377255
# 22    1124907
# 23      40174
# 24     193652
# 25     551408
# 26      92438
# 27     863965
# 28      96358
# 29     167986
# 30       5174
# 31      16743
# 32      58221
# 33      36569
# 34     317519
# 35     373218
# 36      38896
# 37     273586
# 38       9231
# 39        138
# 40         27
# 41     130643
# 42      32576
# 43     434791
# 44      53438
# 45    1227001
# 46      22585
# 47     304563
# 48      23261
