# -*- coding: utf-8 -*-
import os
import configparser
import socket
import aiml
from Vcad import VCAD
from Log.Log import Logger
from Snowboy import snowboy
from Robot import Sdk


class JLing_Talk:
    def __init__(self):
        # 调试消息
        self.logger = Logger('./Log/JLing_Talk.txt', level="info").logger

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
        self.Snowboy = snowboy.Snowboy_JLing(self.config.get("Snowboy", "model"),
                                             self.config.get("Snowboy", "sensitivity"))

        # 注册播放功能
        self.Playwav = snowboy.PlayAudio_wav()

        # 端口表
        self.JLingTalk_host = (str(self.config.get("JLing", "JLingIp")), int(self.config.get("JLing", "JLingTalk_port")))
        self.JLingAgora_host = (str(self.config.get("JLing", "JLingIp")), int(self.config.get("JLing", "JLingAgora_port")))
        self.JLingCommand_host = (str(self.config.get("JLing", "JLingIp")), int(self.config.get("JLing", "JLingCommand_port")))
        self.JLingRun_host = (str(self.config.get("JLing", "RunIp")), int(self.config.get("JLing", "Run_port")))
        self.logger.info("端口表配置成功")

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

    def Start(self):
        client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        client.bind(self.JLingTalk_host)
        data = ""
        addr = ""
        while True:
            try:
                data, addr = client.recvfrom(1024)
            except:
                msg = "JLing_exit"
                client.sendto(msg.encode(encoding="utf-8"), self.JLingCommand_host)
                client.sendto(msg.encode(encoding="utf-8"), self.JLingAgora_host)
                exit()
            # 清洗数据
            message = str(data, encoding='utf-8')
            self.logger.info("msg to Agora:" + message + " ,Addr:" + str(addr))
            # 命令发送给Mand进程
            if message.find("000") >= 0:
                client.sendto(message.encode(encoding="utf-8"), self.JLingCommand_host)
            else:
                # 检测自己的指令库是否有对应指令
                bot_response = self.mybotMand.respond(message)
                if bot_response == "":
                    # 检测自己的自定义对话是否存在
                    bot_response = self.mybotChat.respond(message)
                    # 若指令都不存在，则连接图灵机器人
                    if bot_response == "":
                        bot_response = self.Tuling.start(message)
                        self.logger.info("TuLing:" + bot_response)
                    else:
                        self.logger.info("mybotChat:" + bot_response)
                    client.sendto(message.encode(encoding="utf-8"), self.JLingAgora_host)
                else:
                    # 自己的指令库的指令发给mand
                    client.sendto(message.encode(encoding="utf-8"), self.JLingCommand_host)
                    self.logger.info("mybotMand:" + bot_response)
