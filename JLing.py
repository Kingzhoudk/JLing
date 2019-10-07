# -*- coding: utf-8 -*-
from Speak import JLing_speak
import sys
sys.path.append('./SNOWBOY')
import snowboydecoder
import signal
interrupted = False


def interrupted_True():
    global interrupted
    interrupted = True

def interrupted_False():
    global interrupted
    interrupted = False

def interrupt_callback():
    global interrupted
    return interrupted


def snowboy_jling():
    print("start snowboy")
    model = "SNOWBOY/jling.pmdl"

    # capture SIGINT signal, e.g., Ctrl+C
    signal.signal(signal.SIGINT, interrupted_True)

    detector = snowboydecoder.HotwordDetector(model, sensitivity=0.8)
    print('Listening... Press Ctrl+C to exit')

    # main loop
    detector.start(detected_callback=interrupted_True,
                   interrupt_check=interrupt_callback,
                   sleep_time=0.03)
    detector.terminate()

while True:
    print('''
********************************************************
*           JLing - 中文语音对话机器人                *
*     (c) 2019 周定坤 <zhoudk@ccitrobot.com>       *
********************************************************

            如需退出，可以按 Ctrl-c 组合键。

''')
    snowboy_jling()
    snowboydecoder.play_audio_file()
    JLing_speak()
    interrupted_False()