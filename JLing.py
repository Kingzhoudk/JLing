# -*- coding: utf-8 -*-
from Speak import JLing_Speak
import sys


if __name__ == '__main__':
    print('''
    ********************************************************
    *           JLing - 中文语音对话机器人                *
    *     (c) 2019 周定坤 <zhoudk@ccitrobot.com>        *
    ********************************************************

                如需退出，可以按 Ctrl-c 组合键。
    ''')
    try:
        mybot = JLing_Speak()
        mybot.Speak()
    except:
        exit()

