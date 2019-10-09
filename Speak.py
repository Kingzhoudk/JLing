# -*- coding: utf-8 -*-
import sys
from aip import AipSpeech
import urllib.parse
import urllib.request
import aiml
import os
from Vcad import VCAD

class PlayVoice:
    def play_mpg321(self, VoicePath):
        os.system("mpg321 "+ VoicePath)

class TuLing_AI:
    # 注册参数key ,userid
    def __init__(self, key, userid):
        # 图灵机器人url and post
        self.url = 'http://www.tuling123.com/openapi/api'
        self.values = {
            "key": key,
            "info": "hello",
            "userid": userid
        }

    # message为对话的消息
    def Tuling(self,message):
        self.values['info'] = message
        data = urllib.parse.urlencode(self.values)
        data = data.encode(encoding='gbk')
        req = urllib.request.Request(self.url, data)
        response = urllib.request.urlopen(req)
        the_page = response.read()
        msg = eval(the_page.decode())
        # output=json.load(t)
        ##转化成string
        msg = msg['text']
        return msg


class BaiDu_AI:
    def __init__(self, APP_ID, API_KEY, SECRET_KEY):
        # 百度url and post
        self.APP_ID = APP_ID
        self.API_KEY = API_KEY
        self.SECRET_KEY = SECRET_KEY
        # 百度语音初始化AipSpeech对象
        self.client = AipSpeech(self.APP_ID, self.API_KEY, self.SECRET_KEY)

    # 文件读取
    def get_file_content(self, VoicePath):
        with open(VoicePath, 'rb') as fp:
            return fp.read()

    # 识别本地文件
    def VoiceTest(self, VoicePath):
        result = self.client.asr(self.get_file_content(VoicePath), 'wav', 16000, {
            'lan': 'zh',
        })
        # 转化成string,如果没有则返回error
        try:
            msg = result['result'][0]
        except:
            msg = 'error'
        # print(str)
        # 去除标点
        msg = msg[0:-1]
        return msg

    # 百度语音合成,message合成消息,VoicePath为合成语音路径
    def MyTTS(self, message, VoicePath):
        result = self.client.synthesis(message, 'zh', 1, {
            'vol': 5,
        })
        # 识别正确返回语音二进制 错误则返回dict 参照下面错误码
        if not isinstance(result, dict):
            with open(VoicePath, 'wb') as f:
                f.write(result)


class JLing_Speak:
    def __init__(self, config):
        # 判断是否形成大脑文件
        self.mybotChat = aiml.Kernel()
        # 自定义对话指令库
        if os.path.isfile("./AIML/JLing_chat.brn"):
            self.mybotChat.bootstrap(brainFile="./AIML/JLing_chat.brn")
        else:
            self.mybotChat.bootstrap(learnFiles="./AIML/JLing.xml", commands="load aiml b")
            self.mybotChat.saveBrain("./AIML/JLing_chat.brn")
        # JLing关闭
        self.exit=False

    def Speak(self):
        Tuling=TuLing_AI()
        while self.exit==False: