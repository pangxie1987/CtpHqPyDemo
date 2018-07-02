@echo off
rem 设置窗口标题
title ClearProject
rem 进入上一级目录
cd ..

rem 清理目录下的日志文件
del *.log

rem 清理编译的项目
rd /s /q dist
rd /s /q build
del *.spec
echo "清理完成" & pause>null