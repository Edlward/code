#-*-coding:utf-8-*-  

fid_a = open('/home/lxg/codedata/aflw/data/facerect.txt')    
fid_b = open('/home/lxg/codedata/aflw/data/faceid.txt')    
fid_c = open('/home/lxg/codedata/aflw/data/cc.txt','w')    
lines_a = fid_a.readlines()    
lines_b = fid_b.readlines()    
lines_c = []    
lines_c.append("123")
num = []  
#去重  
lines_a_no_repeat=[]#没重复的  
for line_a in lines_a:  
    if line_a not in lines_a_no_repeat:  
        lines_a_no_repeat.append(line_a)  
        num.append(lines_a.count(line_a))  
#取编号  
ii=0  
for tmp in lines_a_no_repeat:  
    j = 0  
    for i in range(0,len(lines_a)):  
        if tmp==lines_a[i]:  
            if j == 0:  
                fid_c.writelines(lines_a[i].strip('\n\r')+' '+str(num[ii])+'\n')  
                j = j + 1  
            fid_c.writelines(lines_b[i])  
    ii = ii + 1  