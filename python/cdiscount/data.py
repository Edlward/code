import bson
import io
import os
import matplotlib.pyplot as plt

from skimage.data import imread

path = '/home/lxg/codedata/cdiscount'
data = bson.decode_file_iter(open(os.path.join(path, 'train_example.bson')))
prod_to_category = dict()

for c, d in enumerate(data):
    product_id = d['_id']
    category_id = d['category_id']
    prod_to_category[product_id] = category_id

    for e, pic in enumerate(d['imgs']):
        picture = imread(io.BytesIO(pic['picture']))
        plt.figure('picture')
        plt.imshow(picture)
        plt.show()
        print('product_id:',product_id, '\tcategory_id:',category_id, 'img:', e)
