#!/usr/bin/env python3
import os
import sys
import time
import collections
import pathlib

path = pathlib.Path(sys.path[0])
buildPath = path.parent.joinpath("build").joinpath("makelist")
path = path.joinpath("example")
os.chdir(path)

def makeTest(command, expectedOutput):
    os.system(str(buildPath) + " " + command)
    time.sleep(0.25)
    testFile = open("Список.txt", "r")
    fileLine = [line.strip() for line in testFile]
    fileLine.pop(0)
    if collections.Counter(fileLine) ==\
    collections.Counter(expectedOutput):
        print ("[+] Тест пройден. Команда задачи: " + command)
    else:
        print ("[-] Тест провален. Команда задачи: " + command)
    print ("    Ожидаемый вывод : " + str(expectedOutput))
    print ("    Полученный вывод: " + str(fileLine))
 
#test №1 начинается c aa и не заканчивается txt
command = "-s aa -c cc -ne .txt"
expectedOutput = ["aaBbcc(special).text", "aabbcc.text"]
makeTest(command, expectedOutput)

#test №2 фильтр списка
command = "-fe .txt -e txt"
expectedOutput = []
makeTest(command, expectedOutput)   

#test №3 содержит и удаляет
command = "-dc c.t"
expectedOutput = ["aabbcext", "aaBbcxt", "aBbcxt", "bbaacxt"]
makeTest(command, expectedOutput)   

#test №4 кириллица
command = '-s со -dr "(а).tx"'
expectedOutput = ["собак.txt", "Сойк.txt", "сорок.txt"]
makeTest(command, expectedOutput)   

#test №5 кириллица с учётом регистра
command = "-+s Список -Is со"
expectedOutput = ["собака.txt", "сорока.txt", "Список.txt"]
makeTest(command, expectedOutput)   

#test №6 регулярное выражение с удалением
command = '-dr "(\(special\))" -de .text'
expectedOutput = ["aaBbcc"]
makeTest(command, expectedOutput)   

#test №7
command = "-Ic aB"
expectedOutput = ["aaBbcc.txt", "aaBbcc(special).text", "aBbcc.txt"]
makeTest(command, expectedOutput)   
