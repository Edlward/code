from keras.preprocessing.image import Iterator
from keras.preprocessing.image import ImageDataGenerator
from keras import backend as K
from keras.preprocessing.image import load_img, img_to_array
import pandas as pd
import numpy as np
import bson
from io import BytesIO
import time 

#change target_size according to input size
'''class BSONIterator(Iterator):
    def __init__(self, bson_file, images_df, offsets_df, num_class,
                 image_data_generator, target_size=(224, 224), with_labels=True,
                 batch_size=32, shuffle=False, seed=None):

        self.file = bson_file
        self.images_df = images_df
        self.offsets_df = offsets_df
        self.with_labels = with_labels
        self.samples = len(images_df)
        self.num_class = num_class
        self.image_data_generator = image_data_generator
        self.target_size = tuple(target_size)
        self.image_shape = self.target_size + (3,)

        print("Found %d images belonging to %d classes." % (self.samples, self.num_class))

        super(BSONIterator, self).__init__(self.samples, batch_size, shuffle, seed)

    def _get_batches_of_transformed_samples(self, index_array):
        batch_x = np.zeros((len(index_array),) + self.image_shape, dtype=K.floatx())
        if self.with_labels:
            batch_y = np.zeros((len(batch_x)), dtype=K.floatx())

        ts = [0, 0, 0, 0, 0, 0]
        ts2 = [0,0]
        for (i, j) in enumerate(index_array):
            # Protect file and dataframe access with a lock.
            tmp = time.time()
            with self.lock:
                image_row = self.images_df.iloc[j]
                product_id = image_row["product_id"]
                offset_row = self.offsets_df.loc[product_id]
                t20 = time.time()
                ts2[0] += t20-tmp

                # Read this product's data from the BSON file.
                self.file.seek(offset_row["offset"])
                item_data = self.file.read(offset_row["length"])
                t21 = time.time()
                ts2[1] += t21-t20


            t0 = time.time()
            ts[0] += t0-tmp

            # Grab the image from the product.
            item = bson.BSON(item_data).decode()
            t1 = time.time()
            ts[1] += t1-t0

            img_idx = image_row["img_idx"]
            bson_img = item["imgs"][img_idx]["picture"]
            t2 = time.time()
            ts[2] += t2-t1

            # Preprocess the image.
            img = load_img(BytesIO(bson_img), target_size=self.target_size)
            t3 = time.time()
            ts[3] += t3-t2
    
            x = img_to_array(img)
            x = self.image_data_generator.random_transform(x)
            x = self.image_data_generator.standardize(x)


            t4 = time.time()
            ts[4] += t4-t3
            # Add the image and the label to the batch (one-hot encoded).
            batch_x[i] = x
            if self.with_labels:
                batch_y[i] = int(image_row["category_idx"])

            t5 = time.time()
            ts[5] += t5-t4

        print("Lock : [s] {:.5f} [0] {:.3f} [1] {:.3f} ".format(sum(ts2), ts2[0], ts2[1]))
        print("Total: [s] {:.5f} [0] {:.3f} [1] {:.3f} [2] {:.3f} [3] {:.3f} [4] {:.3f} [5] {:.3f} ".format(sum(ts), ts[0], ts[1], ts[2], ts[3], ts[4], ts[5]))

        if self.with_labels:
            return np.rollaxis(batch_x,3,1), batch_y.astype('int64')
        else:
            return np.rollaxis(batch_x,3,1)

    def next(self):
        with self.lock:
            index_array = next(self.index_generator)
        
        return self._get_batches_of_transformed_samples(index_array)



categories_df = pd.read_csv("../categories.csv", index_col=0)

def make_category_tables():
    cat2idx = {}
    idx2cat = {}
    for ir in categories_df.itertuples():
        category_id = ir[0]
        category_idx = ir[4]
        cat2idx[category_id] = category_idx
        idx2cat[category_idx] = category_id
    return cat2idx, idx2cat

cat2idx, idx2cat = make_category_tables()





train_offsets_df = pd.read_csv("../train_offsets.csv", index_col=0)
train_images_df = pd.read_csv("../train_images.csv", index_col=0)
val_images_df = pd.read_csv("../val_images.csv", index_col=0)

test_offsets_df = pd.read_csv("../test_offsets.csv", index_col=0)
test_images_df = pd.read_csv("../test_images.csv", index_col=0)

train_bson_file = open('../train.bson', "rb")

num_classes = 5270
num_train_images = len(train_images_df)
num_val_images = len(val_images_df)
batch_size = 1024
# Tip: use ImageDataGenerator for data augmentation and preprocessing.
train_datagen = ImageDataGenerator()
train_gen = BSONIterator(train_bson_file, train_images_df, train_offsets_df,
                         num_classes, train_datagen, batch_size=batch_size, shuffle=True)
import time,Queue
from multiprocessing import Pool




t0 = time.time()
print("=================[1]")
next(train_gen)
x ,y = next(train_gen)
x ,y = next(train_gen)
x ,y = next(train_gen)
x ,y = next(train_gen)
#print(y)
t1 = time.time()
print("=================[4]")
print (t1-t0)

#print(len(train_gen))'''

'''al_datagen = ImageDataGenerator()
val_gen = BSONIterator(train_bson_file, val_images_df, train_offsets_df,
                       num_classes, val_datagen, batch_size=batch_size)'''

'''test_bson_file = open('test.bson','rb')
test_datagen = ImageDataGenerator()
test_gen = BSONIterator(test_bson_file, test_images_df, test_offsets_df,
                        num_classes, test_datagen, batch_size=batch_size, 
                        with_labels=False, shuffle=False)'''



import bson
import base64
import numpy as np
import pandas as pd
import random
import struct
import time
import os
import cv2
import pickle
import gzip
import zlib
from bson.errors import InvalidBSON
from multiprocessing import Process, Manager, cpu_count

OUTPUT_PATH = '../hash'
THREADS = max(1, cpu_count())

categories_df = pd.read_csv("../categories.csv", index_col=0)

def make_category_tables():
    cat2idx = {}
    idx2cat = {}
    for ir in categories_df.itertuples():
        category_id = ir[0]
        category_idx = ir[4]
        cat2idx[category_id] = category_idx
        idx2cat[category_idx] = category_id
    return cat2idx, idx2cat

cat2idx, idx2cat = make_category_tables()

def save_in_file(arr, file_name):
    pickle.dump(arr, gzip.open(file_name, 'wb+', compresslevel=9))

def load_from_file(file_name):
    return pickle.load(gzip.open(file_name, 'rb'))

def show_image(im, name='image'):
    cv2.imshow(name, im.astype(np.uint8))
    cv2.waitKey(0)
    cv2.destroyAllWindows()

def count_file_documents(file_obj):
    """Counts how many documents provided BSON file contains"""
    cnt = 0
    while True:
        # Read size of next object.
        size_data = file_obj.read(4)
        if len(size_data) == 0:
            break  # Finished with file normaly.
        elif len(size_data) != 4:
            raise InvalidBSON("cut off in middle of objsize")
        obj_size = struct.Struct("<i").unpack(size_data)[0] - 4
        # Skip the next obj_size bytes
        file_obj.seek(obj_size, os.SEEK_CUR)
        cnt += 1
    return cnt


def get_hashes_train(part, total_parts, out_file):
    hashes = []
    ids = []
    category_list = []
    fraction = 1.0

    if 0:
        with open('../train.bson', 'rb') as fbson:
            total_count = count_file_documents(fbson)
            fbson.close()

    total_count = 7069896 #the number of product
    range_start = part * (total_count // total_parts)
    if part == total_parts - 1:
        range_end = total_count
    else:
        range_end = (part + 1) * (total_count // total_parts)
    print('Part: {} - {} from {}'.format(range_start, range_end, total_count))

    with open('../train.bson', 'rb') as fbson:
        data = bson.decode_file_iter(fbson)

        total = 0
        for c, d in enumerate(data):
            total += 1
            # This part probably can be made more elegant (we could read only needed data)
            if total < range_start + 1 or total >= range_end + 1:
                continue
            category = d['category_id']
            id = d['_id']
            for e, pic in enumerate(d['imgs']):
                img = cv2.imdecode(np.fromstring(pic['picture'], dtype=np.uint8), -1)
                #img = cv2.resize(img, (224, 224), cv2.INTER_LANCZOS4)
                hashes.append(img)
                ids.append(id)
                category_list.append(cat2idx[category])
            if total > fraction*(total_count // total_parts):
                break
            if total % 10000 == 0:
                print('Completed for part {}: {}'.format(part, total))
    save_in_file((hashes, ids, category_list), out_file)



def get_all_hashes_train():
    global THREADS
    start_time = time.time()

    # threads = 1
    print('Number of threads: {}'.format(THREADS))
    p = dict()
    for i in range(THREADS):
        pklz_file = OUTPUT_PATH + "ahash_results_train_{}.pklz".format(i)
        p[i] = Process(target=get_hashes_train, args=(i, THREADS, pklz_file))
        p[i].start()
    for i in range(THREADS):
        p[i].join()
    print('Total time for hash extraction: {} sec'.format(round(time.time() - start_time, 2)))


def batch_generator_train(images, labels):
    from keras.utils import np_utils
    random.seed(time.time())

    count = 0
    batch_size = 1000
    train_indexes = list(range(len(images)))
    random.shuffle(train_indexes)

    while True:
        batch_indexes = train_indexes[count:count + batch_size]
        image_list = images[batch_indexes].astype(np.float32).copy()
        batch_labels = labels[batch_indexes].copy()
        image_list /= 255.0
        labels_list = np_utils.to_categorical(batch_labels, 5270)

        count += batch_size
        if count + batch_size > len(train_indexes):
            random.shuffle(train_indexes)
            count = 0

        yield image_list, labels_list




def train_net():
    global THREADS

    if 1:
        in_file = OUTPUT_PATH + "ahash_results_train_.pklz"
        full_hashes, full_ids, full_categories = load_from_file(in_file)
    else:
        full_hashes = []
        full_ids = []
        full_categories = []
        for i in range(THREADS):
            pklz_file = OUTPUT_PATH + "ahash_results_train_{}.pklz".format(i)
            h, i, c = load_from_file(pklz_file)
            full_hashes += h
            full_ids += i
            full_categories += c
        full_hashes = np.array(full_hashes)
        full_ids = np.array(full_ids)
        full_categories = np.array(full_categories)

    
    split_point = len(full_hashes) // 2
    if 0:
        callbacks = [
            EarlyStopping(monitor='val_loss', patience=3, verbose=0),
            ModelCheckpoint(OUTPUT_PATH + 'simple_weights_temp.hdf5', monitor='val_loss', save_best_only=True, verbose=0),
        ]
        history = model.fit_generator(
            generator=batch_generator_train(full_hashes[:split_point], full_categories[:split_point]),
            nb_epoch=3,
            samples_per_epoch=len(full_hashes[:split_point]),
            validation_data=batch_generator_train(full_hashes[split_point:], full_categories[split_point:]),
            nb_val_samples=len(full_hashes[split_point:]),
            verbose=1,
            max_q_size=5,
            callbacks=callbacks)
        model.save_weights(OUTPUT_PATH + 'simple_weights.hdf5')
    else:
        model.load_weights(OUTPUT_PATH + 'simple_weights.hdf5')

    squeeze_weights(model)


get_all_hashes_train()
train_net()


