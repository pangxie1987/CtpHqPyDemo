1、CTPMDHq.py为调用CTP行情客户端CTPMDClient.dll接收CTP柜台的行情；（依赖thostmduserapi.dll）
2、CMDHq.py为调用CTP行情客户端CMDClient.dll接收CMS的行情行情；
3、Make中的MakeCMDhq.bat和MakeCTPMDhq.bat为编译脚本，其他为日志清理脚本；
4、config.conf为程序的配置文件，运行程序时要依赖这个文件；
5、手动编译项目,进入到项目文件目录下：
	pyinstaller -F CTPMDHq.py