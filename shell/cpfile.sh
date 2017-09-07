#!/bin/bash
cat /home/lxg/codedata/gender/fold_frontal_0_data.txt_female.txt | while read line
do 
    echo $line
    cp $line /home/lxg/codedata/gender/fiveFolderGender/0/female/
done

cat /home/lxg/codedata/gender/fold_frontal_0_data.txt_male.txt | while read line
do 
    echo $line
    cp $line /home/lxg/codedata/gender/fiveFolderGender/0/male/
done



cat /home/lxg/codedata/gender/fold_frontal_1_data.txt_female.txt | while read line
do 
    echo $line
    cp $line /home/lxg/codedata/gender/fiveFolderGender/1/female/
done

cat /home/lxg/codedata/gender/fold_frontal_1_data.txt_male.txt | while read line
do 
    echo $line
    cp $line /home/lxg/codedata/gender/fiveFolderGender/1/male/
done



cat /home/lxg/codedata/gender/fold_frontal_2_data.txt_female.txt | while read line
do 
    echo $line
    cp $line /home/lxg/codedata/gender/fiveFolderGender/2/female/
done

cat /home/lxg/codedata/gender/fold_frontal_2_data.txt_male.txt | while read line
do 
    echo $line
    cp $line /home/lxg/codedata/gender/fiveFolderGender/2/male/
done


cat /home/lxg/codedata/gender/fold_frontal_3_data.txt_female.txt | while read line
do 
    echo $line
    cp $line /home/lxg/codedata/gender/fiveFolderGender/3/female/
done

cat /home/lxg/codedata/gender/fold_frontal_3_data.txt_male.txt | while read line
do 
    echo $line
    cp $line /home/lxg/codedata/gender/fiveFolderGender/3/male/
done


cat /home/lxg/codedata/gender/fold_frontal_4_data.txt_female.txt | while read line
do 
    echo $line
    cp $line /home/lxg/codedata/gender/fiveFolderGender/4/female/
done

cat /home/lxg/codedata/gender/fold_frontal_4_data.txt_male.txt | while read line
do 
    echo $line
    cp $line /home/lxg/codedata/gender/fiveFolderGender/4/male/
done