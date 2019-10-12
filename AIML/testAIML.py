import aiml
import sys
import os

kernel = aiml.Kernel()
if os.path.isfile("JLing_chat.brn"):
    kernel.bootstrap(brainFile="JLing_chat.brn")
else:
    kernel.bootstrap(learnFiles="JLing.xml", commands="load aiml b")
    kernel.saveBrain("JLing.brn")

while True:
    print(kernel.respond(input("Enter your message >> ")))
