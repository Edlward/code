# /usr/bin/env sh
DATA=~/code/workpieceML/positive
echo "Create train.txt..."
rm -rf $DATA/train.txt
find $DATA -name 'positive*.bmp' | cut -d '/' -f7 | sed "s/$/ 1/">>$DATA/train.txt
find $DATA -name '*bike.jpg' | cut -d '/' -f7 | sed "s/$/ 2/">> $DATA/tmp.txt
cat $DATA/tmp.txt>>$DATA/train.txt
rm -rf $DATA/tmp.txt
echo "Done.."

#转换数据文件
# rm -r circlework
# ./convert_imageset -gray true ~/code/workpieceML/positive/ ~/code/workpieceML/positive/train.txt circlework

# #
# convert_imageset: Convert a set of images to the leveldb/lmdb
# format used as input for Caffe.
# Usage:
#     convert_imageset [FLAGS] ROOTFOLDER/ LISTFILE DB_NAME
# The ImageNet dataset for the training demo is at
#     http://www.image-net.org/download-images


#   Flags from tools/convert_imageset.cpp:
#     -backend (The backend {lmdb, leveldb} for storing the result) type: string
#       default: "lmdb"
#     -check_size (When this option is on, check that all the datum have the same
#       size) type: bool default: false
#     -encode_type (Optional: What type should we encode the image as
#       ('png','jpg',...).) type: string default: ""
#     -encoded (When this option is on, the encoded image will be save in datum)
#       type: bool default: false
#     -gray (When this option is on, treat images as grayscale ones) type: bool
#       default: false
#     -resize_height (Height images are resized to) type: int32 default: 0
#     -resize_width (Width images are resized to) type: int32 default: 0
#     -shuffle (Randomly shuffle the order of images and their labels) type: bool
#       default: false
