# -*- coding: utf-8 -*-
import sys
from aip import AipSpeech
import urllib.parse
import urllib.request
import aiml
import os
import configparser
from Log.Log import Logger
from Snowboy.snowboy import Snowboy_JLing


class TuLing_Sdk:
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
    def start(self, message):
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


class BaiDu_Sdk:
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
    def SpeechRecognition(self):
        result = self.client.asr(self.get_file_content("input.wav"), 'wav', 16000, {
            'lan': 'zh',
        })
        # 转化成string,如果没有则返回error
        try:
            msg = result['result'][0]
        except:
            msg = 'error'
            return msg
        # 去除标点
        msg = msg[0:-1]
        return msg

    # 百度语音合成,message合成消息,VoicePath为合成语音路径,详情见BAIDU的REST API
    def MyTTS(self, message):
        result = self.client.synthesis(message, 'zh', 1, {
            'vol': 5, 'aue': 6, 'per': 4
        })
        # 识别正确返回语音二进制 错误则返回dict 参照下面错误码
        if not isinstance(result, dict):
            with open('output.wav', 'wb') as f:
                f.write(result)


