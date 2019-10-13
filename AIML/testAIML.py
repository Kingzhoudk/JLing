import aiml
import sys
import os

chat = aiml.Kernel()
if os.path.isfile("JLing_chat.brn"):
    chat.bootstrap(brainFile="JLing_chat.brn")
else:
    chat.bootstrap(learnFiles="JLing_chat.xml", commands="load aiml b")
    chat.saveBrain("JLing_chat.brn")

mand = aiml.Kernel()
if os.path.isfile("JLing_mand.brn"):
    mand.bootstrap(brainFile="JLing_mand.brn")
else:
    mand.bootstrap(learnFiles="JLing_mand.xml", commands="load aiml b")
    mand.saveBrain("JLing_mand.brn")


while True:
    print(chat.respond(input("Enter your message (chat)>> ")))
    print(mand.respond(input("Enter your message (mand)>> ")))
