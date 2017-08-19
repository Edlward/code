#!/bin/bash
cat /home/lxg/codedata/agegender/data/female.txt | while read line
do 
    echo $line
    cp $line /home/lxg/codedata/agegender/data/female/
done

cat /home/lxg/codedata/agegender/data/male.txt | while read line
do 
    echo $line
    cp $line /home/lxg/codedata/agegender/data/male/
done