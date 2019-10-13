# -*- coding: utf-8 -*-
import sys
from aip import AipSpeech
import urllib.parse
import urllib.request
import aiml
import os
import configparser
from Vcad import VCAD
from Log.Log import Logger
from Snowboy.snowboy import Snowboy_JLing
from Robot import Sdk
from Robot import Ai


class JLing_Speak:
    def __init__(self):
        # 调试消息
        self.logger = Logger('./Log/JLing_Speak.txt', level="info").logger

        # 读取配置信息
        self.config = configparser.ConfigParser()
        self.config.read("config.ini")

        # 输出音频的路径
        self.FilePath = os.path.dirname(os.path.abspath(__file__))
        self.OutPutFile = os.path.join(self.FilePath, "output.wav")

        # 注册Baidu的API
        self.Baidu = Sdk.BaiDu_Sdk(self.config.get("Baidu", "APP_ID"), self.config.get("Baidu", "API_KEY"),
                                   self.config.get("Baidu", "SECRET_KEY"))

        # VCAD功能
        self.Vcad = VCAD()

        # 注册snowboy功能
        self.Snowboy = Snowboy_JLing(self.config.get("Snowboy", "model"), self.config.get("Snowboy", "sensitivity"))

        # 生成JLing_Ai大脑
        self.JLing_Ai = Ai.JLingAi()

    def Speak(self):
        while True:
            try:
                self.Snowboy.start()
            except:
                exit()
            self.Vcad.start()
            message = self.Baidu.SpeechRecognition()
            bot_response = self.JLing_Ai.start(message)
            self.Baidu.MyTTS(bot_response)
            self.Snowboy.play_audio(self.OutPutFile)
