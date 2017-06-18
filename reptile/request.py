#-*- coding:utf-8 -*-
import re
import requests

path = "/home/lxg/codedata/upperbody/"

def dowmloadPic(html,keyword,page):


    pic_url = re.findall('"objURL":"(.*?)",',html,re.S)
    i = 0
    print '找到关键词:'+keyword+'的图片，现在开始下载图片...'
    for each in pic_url:
        print '正在下载第'+str(i+1)+'张图片，图片地址:'+str(each)
        try:
            pic= requests.get(each, timeout=10)
        except requests.exceptions.ConnectionError:
            print '【错误】当前图片无法下载'
            continue
        string = path+"pedestrian"+'_'+str(i) + '.jpg'
        #resolve the problem of encode, make sure that chinese name could be store
        fp = open(string.decode('utf-8').encode('cp936'),'wb')
        fp.write(pic.content)
        fp.close()
        i += 1



if __name__ == '__main__':
    # word = raw_input("Input key word: ")
    word = "网红"
    for i in range(10):
        pn = str(i * 20)
        i = i + 1
        url = 'https://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word='+word+'&pn='+str(pn)+'&gsm=50&ct=&ic=0&lm=-1&width=0&height=0'
        result = requests.get(url)
        dowmloadPic(result.text, word, i)
# http://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word='+word+'pn='+str(pn)+'&gsm=8c&ct=&ic=0&lm=-1&width=0&height=0
# gsm 并没有什么用处，pn代表页数
# https://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word=%E8%A1%8C%E4%BA%BA&pn=0&gsm=8c&ct=&ic=0&lm=-1&width=0&height=0 140
# https://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word=%E8%A1%8C%E4%BA%BA&pn=20&gsm=3c&ct=&ic=0&lm=-1&width=0&height=0 60
# https://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word=%E8%A1%8C%E4%BA%BA&pn=40&gsm=50&ct=&ic=0&lm=-1&width=0&height=0 80
# https://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word=%E8%A1%8C%E4%BA%BA&pn=60&gsm=64&ct=&ic=0&lm=-1&width=0&height=0 100
# https://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word=%E8%A1%8C%E4%BA%BA&pn=80&gsm=78&ct=&ic=0&lm=-1&width=0&height=0 120
# https://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word=%E8%A1%8C%E4%BA%BA&pn=100&gsm=8c&ct=&ic=0&lm=-1&width=0&height=0 140
# https://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word=%E8%A1%8C%E4%BA%BA&pn=120&gsm=a0&ct=&ic=0&lm=-1&width=0&height=0 160
# https://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word=%E8%A1%8C%E4%BA%BA&pn=140&gsm=b4&ct=&ic=0&lm=-1&width=0&height=0 180
# https://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word=%E8%A1%8C%E4%BA%BA&pn=160&gsm=c8&ct=&ic=0&lm=-1&width=0&height=0 200
# https://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word=%E8%A1%8C%E4%BA%BA&pn=180&gsm=dc&ct=&ic=0&lm=-1&width=0&height=0 220
# https://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word=%E8%A1%8C%E4%BA%BA&pn=200&gsm=f0&ct=&ic=0&lm=-1&width=0&height=0 240 260