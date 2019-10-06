import aiml
import sys
import os 

kernel = aiml.Kernel() 
if os.path.isfile("chat.brn"): 
	kernel1.bootstrap(brainFile = "JLing.brn")
else: 
	kernel1.bootstrap(learnFiles = "JLing.xml", commands = "load aiml b")
	kernel1.saveBrain("JLing.brn") 

while True: 
	print(kernel.respond(input("Enter your message >> ")))
