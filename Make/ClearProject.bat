@echo off
rem ���ô��ڱ���
title ClearProject
rem ������һ��Ŀ¼
cd ..

rem ����Ŀ¼�µ���־�ļ�
del *.log

rem ����������Ŀ
rd /s /q dist
rd /s /q build
del *.spec
echo "�������" & pause>null