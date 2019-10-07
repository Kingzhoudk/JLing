# -*- coding: utf-8 -*-
import sys
from aip import AipSpeech
import urllib.parse
import urllib.request
import aiml
import os
import json
from vcad import Vcad


######判断是否形成大脑文件
import aiml
  
mybotChat = aiml.Kernel()
#自定义对话指令库
if os.path.isfile("./AIML/chat.brn"):
    mybotChat.bootstrap(brainFile="./AIML/chat.brn")
else:
    mybotChat.bootstrap(learnFiles="./AIML/chat.xml", commands="load aiml b")
    mybotChat.saveBrain("./AIML/chat.brn")
#########################

#百度url and post
APP_ID = '10469729'
API_KEY = 'orVUrM1igOq0ZB2wryx7dLkz'
SECRET_KEY = 'G6sfSlTOR2eqC0ldvyY0aMnPKnnA1ess'

#百度语音初始化AipSpeech对象
client = AipSpeech(APP_ID, API_KEY, SECRET_KEY)

#图灵机器人url and post
url = 'http://www.tuling123.com/openapi/api'
values = {
"key":"7172c50c2b5f48d08715bd0a9f9878c6",
"info":"hello",
"userid":"123456"
}

#文件读取
def get_file_content(filePath):
    with open(filePath, 'rb') as fp:
        return fp.read()
 

# 识别本地文件   
def VoiceTest():
    result= client.asr(get_file_content('input.wav'), 'wav', 16000, {
        'lan': 'zh',
        })
    #转化成string,如果没有则返回error
    try:
        str=result['result'][0]
    except:
        str='error'
    # print(str)
    #去除标点
    str=str[0:-1]
    return str

#百度语音合成
def MyTTS(message):
    result  = client.synthesis(message, 'zh', 1, {
    'vol': 5,
    })
    # 识别正确返回语音二进制 错误则返回dict 参照下面错误码
    # print(result)
    if not isinstance(result, dict):
        with open('output.mp3', 'wb') as f:
            f.write(result)
    

#图灵机器人
def Tuling(str):
    values['info']=str
    data = urllib.parse.urlencode(values)
    data = data.encode(encoding='gbk')
    req = urllib.request.Request(url, data)
    response = urllib.request.urlopen(req)
    the_page = response.read()
    t=eval(the_page.decode())
    ##转化成string
    output=t['text']
    #print(output)
    return output
    

#播放
def OutputVoice():
    os.system("mpg321 output.mp3")

#录音
def InputVoice():
    print("开始录音：")
    Vcad()


# 对话逻辑主函数
def JLing_speak():
    InputVoice()
    message=VoiceTest()
    print(message)
    if(message =='erro'):
        print("林")
        bot_response="请再说一遍，我刚刚没听清"
    else:
          #检测自己的自定义对话似是否存在
        bot_response=mybotChat.respond(message)
          #若指令都不存在，则连接图灵机器人
        if(bot_response==""):
            bot_response=Tuling(message)

    print(bot_response)
     #生成语音
    MyTTS(bot_response)
     #播放语音
    OutputVoice()

