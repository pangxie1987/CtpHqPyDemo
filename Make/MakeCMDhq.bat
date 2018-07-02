@echo off
title MakeCMDhq

cd ..
rem 编译前 先清理上次编译的文件
rd /q /s dist\CMDhq
rd /q /s build\CMDhq
rem 延时3秒
choice /t 3 /d y /n >null

rem 编译CMDhq可执行文件
start pyinstaller CMDhq.py

rem 设置延时10秒
choice /t 10 /d y /n >null

rem 设置要检测的进程名
set "taskname=pyinstaller.exe"
rem 判断编译进程是否存在
tasklist|find /i "%taskname%"
rem  如果编译进程不存在了  则进行copy动作
if ERRORLEVEL==1 goto copy  

:copy   
rem 将配置文件和基础数据文件拷贝到发布目录
copy config.conf dist\CMDhq
echo "config.conf拷贝成功"


echo "CMDhq.exe编译完成，请到dist目录中获取" & pause>null