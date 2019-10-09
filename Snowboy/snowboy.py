import snowboydecoder
import sys
import signal
sys.path.append("..")
from Log import Logger


class Snowboy_JLing:
    def __init__(self, model, sensitivity):
        self.interrupted = False
        self.model = model
        self.sensitivity = sensitivity
        self.detector = snowboydecoder.HotwordDetector(self.model, sensitivity=self.sensitivity)

    def __del__(self):
        self.detector.terminate()

    def signal_handler(self):
        snowboydecoder.play_audio_file()
        self.interrupted = True

    def interrupt_callback(self):
        return self.interrupted

    def start(self):
        print('Listening... Press Ctrl+C to exit')
        self.detector.start(detected_callback=self.signal_handler,
                            interrupt_check=self.interrupt_callback,
                            sleep_time=0.03)
        self.interrupted = False
