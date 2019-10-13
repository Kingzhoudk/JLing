# -*- coding: utf-8 -*-
import socket
import configparser
import os
from Log.Log import Logger
from Robot import Sdk
from Snowboy import snowboy


class JLing_Mand:
    def __init__(self):
        # 读取配置信息
        self.config = configparser.ConfigParser()
        self.config.read("config.ini")

        # log文件
        self.logger = Logger('./Log/JLing_Mand.txt', level='debug').logger

        # 播放音频
        self.Playwav = snowboy.PlayAudio_wav()

        # 输出音频的路径
        self.FilePath = os.path.dirname(os.path.abspath(__file__))
        self.OutPutFile = os.path.join(self.FilePath, "output.wav")

        # 注册Baidu的API
        self.Baidu = Sdk.BaiDu_Sdk(self.config.get("Baidu", "APP_ID"), self.config.get("Baidu", "API_KEY"),
                                   self.config.get("Baidu", "SECRET_KEY"))

        # 端口表
        self.JLingTalk_host = (self.config.get("JLing", "JLingIp"), self.config.get("JLing", "JLingTalk_port"))
        self.JLingAgora_host = (self.config.get("JLing", "JLingIp"), self.config.get("JLing", "JLingAgora_port"))
        self.JLingCommand_host = (self.config.get("JLing", "JLingIp"), self.config.get("JLing", "JLingCommand_port"))
        self.JLingRun_host = (self.config.get("JLing", "RunIp"), self.config.get("JLing", "Run_port"))

    # 数据的写入
    def writeTxT(self, path, msg):
        # 单引号转为双引号
        msg = msg.replace("\'", '\"')
        file = open("%s.txt" % path, "w")
        file.write(msg)

    # 数据的读取
    def readTxT(self, path):
        file = open("%s.txt" % path, "r")
        msg = file.read()
        return msg

    # 打开视频监控
    def openMonitor(self):
        os.system("")

    def Start(self):
        client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        client.bind(self.JLingCommand_host)
        while True:
            data, addr = client.recvfrom(1024)
            message = str(data, encoding='utf-8')
            self.logger.info("Mand_MSG:" + str(message) + "Addr:" + str(addr))
            # Mand终止
            if message == "JLing_exit":
                exit()
            # 播放指令
            if message.find('01') >= 0:
                msg = "小精灵已经把话说完了"
                self.Baidu.MyTTS(message[4:])
                self.Playwav.play_audio(self.OutPutFile)
                client.sendto(msg.encode(encoding="utf-8"), self.JLingAgora_host)
                self.logger.info("Mand send to agora :" + msg)
            # 打开视频监控
            if message.find('02') >= 0:
                msg = "已经打开了JLing的视频监控"
                os.system("")
                client.sendto(msg.encode(encoding="utf-8"), self.JLingAgora_host)
                self.logger.info("Mand send to agora :" + msg)
            # 消息的反馈
            if message.find('03') >= 0:
                msg = self.readTxT("JLing_date")
                client.sendto(msg.encode(encoding="utf-8"), self.JLingAgora_host)
                self.logger.info("Mand send to agora :" + msg)
            # 消息的更改
            if message.find('04') >= 0:
                msg = self.readTxT("JLing_date")
                dict = eval(msg)
                try:
                    dict[message[4:6]] = message[7:]
                    self.writeTxT("JLing_date", str(dict))
                    msg = "数据输入成功"
                    client.sendto(msg.encode(encoding="utf-8"), self.JLingAgora_host)
                    self.logger.info("Mand send to agora :" + msg)
                except:
                    msg = "数据输入失败，请按以下格式输入：0004D1：关闭"
                    client.sendto(msg.encode(encoding="utf-8"), self.JLingAgora_host)
                    self.logger.info("Mand send to agora :" + msg)
                # 消息的反馈
                msg = self.readTxT("JLing_date")
                client.sendto(msg.encode(encoding="utf-8"), self.JLingAgora_host)
                self.logger.info("Mand send to agora :" + msg)
            # 控制消息
            if message.find('05') >= 0:
                msg = message[4:]
                self.logger.info("Mand send to RUN :" + msg)
                client.sendto(msg.encode(encoding="utf-8"), self.JLingRun_host)


if __name__ == '__main__':
    Mand = JLing_Mand()
    Mand.Start()
