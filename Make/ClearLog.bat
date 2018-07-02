@echo off
cd ..
rem 设置窗口标题
title ClearLog
rem 清理目录下的日志文件
del *.log
echo "清理完成" & pause>null