@echo off
title MakeCMDhq

cd ..
rem ����ǰ �������ϴα�����ļ�
rd /q /s dist\CMDhq
rd /q /s build\CMDhq
rem ��ʱ3��
choice /t 3 /d y /n >null

rem ����CMDhq��ִ���ļ�
start pyinstaller CMDhq.py

rem ������ʱ10��
choice /t 10 /d y /n >null

rem ����Ҫ���Ľ�����
set "taskname=pyinstaller.exe"
rem �жϱ�������Ƿ����
tasklist|find /i "%taskname%"
rem  ���������̲�������  �����copy����
if ERRORLEVEL==1 goto copy  

:copy   
rem �������ļ��ͻ��������ļ�����������Ŀ¼
copy config.conf dist\CMDhq
echo "config.conf�����ɹ�"


echo "CMDhq.exe������ɣ��뵽distĿ¼�л�ȡ" & pause>null