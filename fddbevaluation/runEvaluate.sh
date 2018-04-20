# ./evaluate -a test_predict_label.txt -d test_predict.txt -f 0 -i /home/lxg/codedata/headXml/srcImage/ -l test_label.txt -r roc.txt
# \Enter,反斜杠后面紧跟回车，表示下一行是这一行的续行
./evaluate -a /home/lxg/codedata/fddb/fddb.txt \
            -d /home/lxg/codedata/fddb/retinanet_fddb_predict.txt \
            -f 0 \
            -i /home/lxg/codedata/fddb/ \
            -l /home/lxg/codedata/fddb/fddblist.txt \
            -r /home/lxg/codedata/fddb/retinanet-lochead- \
#            -s

# ./evaluate [OPTIONS]
#    -h              : print usage
#    -a fileName     : file with face annotations (default: ellipseList.txt)
#    -d fileName     : file with detections (default: faceList.txt)
#    -f format       : representation of faces in the detection file (default: 0)
#                    : [ 0 (rectangle), 1 (ellipse) or  2 (pixels) ]
#    -i dirName      : directory where the original images are stored (default: ~/scratch/Data/facesInTheWild/)
#    -l fileName     : file with list of images to be evaluated (default: temp.txt)
#    -r fileName     : prefix for files to store the ROC curves (default: temp)
#    -s showMatchedImage 

#gnuplot /home/lxg/codedata/fddb/compareROC/discROC.p
