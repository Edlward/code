#-*- coding:utf-8 -*-
import re
import requests

path = "/home/lxg/codedata/upperbody/360/"

word = ""
url = "http://image.so.com/i?q=%E5%A4%8F%E5%AD%A3%E8%B7%AF%E4%BA%BA&src=srp"
# url = "http://image.so.com/i?q=%E5%A4%8F%E5%AD%A3%E3%80%80%E8%A1%8C%E4%BA%BA&src=srp"
# url = "http://image.so.com/j?q=%E8%B7%AF%E4%BA%BA&src=srp&correct=%E8%B7%AF%E4%BA%BA&sn=61&pn=60&sid=5ac8eff753990714021361d385df5bad&ran=0&ras=0"
# url = "http://image.so.com/j?q=%E5%A4%8F%E5%AD%A3%E8%B7%AF%E4%BA%BA&src=srp&correct=%E5%A4%8F%E5%AD%A3%E8%B7%AF%E4%BA%BA&sn=122&pn=60&sid=20e5c6cc3e0b1f0c5b514fd86b3734fc&ran=0&ras=0"
# url = "http://image.baidu.com/search/index?tn=baiduimage&ps=1&ct=201326592&lm=-1&cl=2&nc=1&ie=utf-8&word=python%20re%E5%BA%93"
res = requests.get(url)
pic_url = re.findall('"img":"(.*?)",',res.text, re.S)
i = 0

for each in pic_url:
    a = str(each)
    b = a.replace('\\', '')
    print '正在下载第'+str(i+1)+'张图片，图片地址:'+str(b)
    try:
        pic= requests.get(b, timeout=10)
    # except requests.exceptions.ConnectionError:
    except Exception as e:
        print '【错误】当前图片无法下载'
        continue
    string = path + "pedestrian_360_" + str(i) + ".jpg"
    # string = path+"road"+'_'+str(i) + '.jpg'
    # string = path+"pedestrian_5"+'_'+str(i) + '.jpg'
    
    #resolve the problem of encode, make sure that chinese name could be store
    fp = open(string.decode('utf-8').encode('cp936'),'wb')
    fp.write(pic.content)
    fp.close()
    i += 1

