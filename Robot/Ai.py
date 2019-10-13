# -*- coding: utf-8 -*-
import aiml
import os
import configparser
from Log.Log import Logger
from . import Sdk


class JLingAi:
    def __init__(self):
        # 调试消息
        self.logger = Logger('./Log/JLing_Ai.txt', level="info").logger

        # 读取配置信息
        self.config = configparser.ConfigParser()
        self.config.read("config.ini")
        self.logger.info("读取配置文件config.ini成功")

        # 判断是否形成大脑文件,生成mybotchat自定义聊天库，mybotmand自定义指令库
        self.mybotChat = aiml.Kernel()
        self.mybotMand = aiml.Kernel()
        # 自定义对话指令库
        if os.path.isfile("./AIML/JLing_chat.brn"):
            self.mybotChat.bootstrap(brainFile="./AIML/JLing_chat.brn")
            self.logger.info("mybotchat读取自定义对话指令库成功")
        else:
            self.mybotChat.bootstrap(learnFiles="./AIML/JLing_chat.xml", commands="load aiml b")
            self.mybotChat.saveBrain("./AIML/JLing_chat.brn")
            self.logger.info("mybotchat重新生成自定义对话指令库")
        # 自定义控制指令库
        if os.path.isfile("./AIML/JLing_mand.brn"):
            self.mybotMand.bootstrap(brainFile="./AIML/JLing_mand.brn")
            self.logger.info("mybotmand读取自定义控制指令库成功")
        else:
            self.mybotMand.bootstrap(learnFiles="./AIML/JLing_mand.xml", commands="load aiml b")
            self.mybotMand.saveBrain("./AIML/JLing_mand.brn")
            self.logger.info("mybotmand重新生成自定义控制指令库")

        # 注册图灵机器人
        self.Tuling = Sdk.TuLing_Sdk(self.config.get("Tuling", "key"), self.config.get("Tuling", "userid"))

    def start(self, message):
        self.logger.info("问题：" + message)
        if message == 'error' or message == '':
            bot_response = "请在说一遍，我刚刚走神了。"
            return bot_response
        else:
            # 判断自己的本地对话词料库是否存在指令
            bot_response = self.mybotChat.respond(message)
            if bot_response == "":
                bot_response = self.Tuling.start(message)
                self.logger.info("Tuling回答：" + bot_response)
            else:
                self.logger.info("MybotChat回答：" + bot_response)
            return bot_response
