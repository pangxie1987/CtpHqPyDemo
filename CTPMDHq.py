#coding=utf-8
from ctypes import *
from Hqdtype import *
from  HqStruct import *
import HqStruct
from ctypes.wintypes import *
import ctypes
import sys
import time
import cx_Oracle
import configparser
import os

##################################
#连接CTP行情服务器的行情代码
##################################

#*************下面部分是初始服务器连接设置的函数*************
print('CTPMDClient演示代码python3.5版本启动')

#加载CTP行情 CTPMDClient.dll（依赖thostmduserapi.dll）
print('开始加载CTPMDClient.dll')
dll=ctypes.cdll.LoadLibrary('CTPMDClient.dll')
print('CTPMDClient.dll加载成功')


#初始化MDClient的接口，设置日志路径
logpath=sys.path[0].encode('utf-8')
if dll.MDCInitialize(logpath):
    dll.MDCSetDebug(1)
    print('行情日志打开成功')
else:
    print('行情日志打开失败')

#设置MDClient模式内部的调试模式
idebug=1
if dll.MDCSetDebug(idebug):
    print('MDClient模式内部的调试模式为：',idebug)
else:
    print('MDClient模式内部的调试模式失败')


#设置MDClient的SessionID
sessionid=b'LINK3-KS'
if dll.MDCSetSessionID(sessionid):
    print('sessionid设置成功',sessionid)
else:
    print('sessionid设置失败')

#设置DMClient在与服务器对接的时候，约定的心跳参数
hbt=7
hblatency=3
if dll.MDCSetHeartBtInt(hbt,hblatency):
    print('心跳参数设置为',hbt)
else:
    print('心跳设置失败')


#定义读取配置文件的方法，后续参数将从配置文件中获取
def getConfig(section,key):
    config=configparser.ConfigParser()
    path=('config.ini')
    config.read(path)
    return config.get(section,key)

#设置CTPMDClient的对接服务器端的连接参数(IP:Port)
#svrip_port=b'10.253.46.91:44001'
#从config.conf中获取行情连接地址
svrip_port=getConfig('CTPMD','ip2').encode('utf-8')
#print('svrip_port',svrip_port)
if dll.MDCSetServer(svrip_port):
    print('行情地址设置成功,ip地址为',svrip_port)
else:
    print('行情地址设置失败')

#设置登录的用户名和口令
#userpassword=b'ITSTest,123456'
userpassword=getConfig('CTPMD','pw2').encode('utf-8')
if dll.MDCSetLogonUser(userpassword):
    print('登录的用户名称密码为',userpassword)
else:
    print('用户名称密码错误')

'''
#设置行情接收模式
if dll.MDCSetSockMode(0,-1,0):
    print('设置行情接收模式为TCP')
'''

#开始连接服务器，若启动成功，则会启动后台数据接收线程
nresult=dll.MDCToConnectServer(svrip_port,userpassword)
time.sleep(5)
if nresult==1:
    print('登陆行情成功......')
else:
    print('登陆行情失败.返回值',nresult)


#获取与服务器的连接状态
mdCGetLinkStatus=dll.MDCGetLinkStatus()
if mdCGetLinkStatus==1:
    print('服务器的连接状态 1 - 连接链路已经建立，但还没有完成LOGN之前')
elif mdCGetLinkStatus==2:
    print('服务器的连接状态 2 - LOGON消息交换完毕，开始进行正式的MD数据交换状态')
elif mdCGetLinkStatus==0:
    print('服务器的连接状态 0 - 处于断开状态')
else:
    print('服务器的连接状态异常,请检查连接')

#获取是否启动后台数据接收线程状态
mdCGetBackRunStatus=dll.MDCGetBackRunStatus()
if mdCGetBackRunStatus==0:
    print('后台数据接收线程状态 0 - 停止状态')
elif mdCGetBackRunStatus==1:
    print('后台数据接收线程状态 1 - 启动状态')
else:
    print('后台数据接收线程状态异常，请检查')

#在登录成功时候的回调函数， difSec是本机和服务器之间的时间差，本机时间+difSec=近视的服务器时间
pFuncOnLogon=CFUNCTYPE(c_int)
def logdeftime(difSec):
    print('行情登录通知.本机与行情服务器时间差=',difSec)
logdeftimepb=pFuncOnLogon(logdeftime)
dll.MDCRegisterBCOnLogon(logdeftimepb)

#向服务器订阅实时行情数据; 或取消原订阅的请求 MDCBookRTMarketData
#行情订阅号
sMDReqID=b'123'

#订阅方式
cReqType=b'1'  #ASCII码相当于1，×××××注意×××××，否则订阅不成功
#'0' = Snapshot;  仅仅得到一次行情数据快照
#'1' = Snapshot + Updates (Subscribe);  快照和后续的更新(订阅)，直到取消或断开连接
#'2' = Disable previous Snapshot + Update Request (Unsubscribe) 取消之前的指定的订阅

iMDType=0
#0=仅仅为实时行情数据;
#1=除了实时行情数据，也包括实时分笔成交数据;

#建立订阅市场的合约列表
booklist=[]
booklist.append(b'CFFX,?;') #中金所
booklist.append(b'XSGE,?;')  #上期所
booklist.append(b'XDCE,?;')  #大商所
booklist.append(b'XCZE,?;')  #郑商所
booklist.append(b'XSHG,?;') #上交所
booklist.append(b'XSHE,?;') #深交所
#sylist=b'CFFX,IF1803'  #订阅单个个券的行情 ,CTPMDClient只能单个个券进行订阅
#从配置文件中读取订阅的个券
sylist=getConfig('CTPMkcode','ctpmk').encode('utf-8')
def show_bookresult(bookresult,mkkk):
    if (bookresult<0):
        if (-202==bookresult):
            print('错误的cReqType',bookresult)
        elif (-201==bookresult):
            print('订阅号重复',bookresult)
        else:
            print('订阅号发送失败',bookresult)
    else:
        print('订阅实时行情请求成功,',mkkk)
i=0
while (i<len(booklist)):
    #bookresult = dll.MDCBookRTMarketData(booklist[i], cReqType, iMDType, (booklist[i]))
    bookresult = dll.MDCBookRTMarketData(booklist[i], cReqType, iMDType, sylist)
    show_bookresult(bookresult, (booklist[i]))
    i=i+1
print('订阅实时全市场全品种行情数据完毕！')



#建立数据库连接，为后续数据插入做准备
try:
    conn = cx_Oracle.connect('core/core@127.0.0.1/xe')
    cursor = conn.cursor()
    print('oracle连接建立成功')
except:
    print('数据库连接失败')


#注册实时接收行情回调函数pBCOnMRTDReceived，接收订阅到的实时行情
pBCOnMRTDReceived=CFUNCTYPE(None,c_char_p,c_char_p,c_char_p,tT_MDREAL)
def bookqhrecord(MDReqID,excode,symbol,hqlist):
    mkcode=hqlist.contents.exchCode.decode('utf-8')
    stkcode=hqlist.contents.varity_code.decode('utf-8')
    stkname=hqlist.contents.varity_name.decode('utf-8')
    lastprice=round(hqlist.contents.lastPrice, 2)  # 最新价处理到两位
    donevolue=hqlist.contents.doneVolume
    upddate=hqlist.contents.upd_date
    #if (excode==b'CFFX'):  #设置行情接收的市场和合约条件
    #if (excode == b'CFFX' and symbol == b'IF1712'):  # 设置行情接收的市场和合约条件
    # print(10*'*'+'接收到实时行情数据'+10*'*'+'\n'+'市场号\t'+'合约代码\t'+'合约名称\t'+'最新价\t'+'成交量\t'+'行情日期'+'\n',
    #         mkcode,stkcode,stkname,lastprice,donevolue,upddate)

    print('行情订阅成功')
    print(hqlist.contents.varity_code.decode('utf-8'),
          hqlist.contents.varity_name.decode('gbk'),
          # hqlist.contents.cmdstime,
          hqlist.contents.openPrice,
          hqlist.contents.lastPrice,
          hqlist.contents.highestPrice,
          hqlist.contents.lowestPrice,
          hqlist.contents.doneVolume,  # 成交量
          round(hqlist.contents.chgPrice, 4), '\n',  # 涨跌
          round(hqlist.contents.upperLimitPrice, 4),  # 涨停板
          round(hqlist.contents.lowerLimitPrice, 4),  # 跌停板
          round(hqlist.contents.hisHighestPrice, 4),  # 历史最高价
          round(hqlist.contents.hisLowestPrice, 4), '\n',  # 历史最低价
          hqlist.contents.openInterest,  # 净持仓
          round(hqlist.contents.preSettlePrice, 4),  # 昨日结算
          round(hqlist.contents.preClosePrice, 4),  # 昨日收盘
          round(hqlist.contents.settlePrice, 4))  # 今日结算

        #接收到的行情数据写入本地文件
    with open('CTPhq.txt', 'a') as f:
                #list=[]
            f.write(str('市场号\t'+'合约代码\t'+'最新价\t'+'\n'))
            f.write(str(symbol)+'\t'+str(symbol)+'\t'+str(lastprice)+'\n'
                        )

            #orcinsert()
            #将行情数据插入到数据库
    sqlcomm="insert into CTPhqdata (excode,symbol,lastprice) values ('%s','%s','%d')"
    cursor.execute(sqlcomm % (mkcode,stkcode,lastprice))
    conn.commit()
    print('oracle数据插入成功')
        #cursor.close()
        #conn.close()

bookmkdata=pBCOnMRTDReceived(bookqhrecord)
dll.MDCRegisterBCOnMRTDReceived(bookmkdata)



#****************************以下为主函数****************************
if __name__=='__main__':
    input()



####################以下为同步函数##################################
#启动同步行情模式（必须在成功和CMDSIII连接成功才有可能成功）int ==0: 成功启动同步行情模式; <0: 同步行情启动失败
'''    
sExchCodeList='CFFX'
a=dll.MDCSyncModeSet(sExchCodeList)
if (a==0):
    print('启动同步行情模式成功')
else:
    print('同步行情模式启动失败')


#在同步模式下(即已经调用了MDCSyncModeSet后)，同步读取指定金融商品的行情信息数据 int 返回成功读取的记录数；<=0: 失败
hqTMDVARI=T_MDREAL()
sec_code=b'SR803'
b=dll.MDCSyncMarketData(sExchCodeList,sec_code,byref(hqTMDVARI),1)
if b<=0:
    print('同步行情查询失败')
else:
    print('同步行情查询成功,查询条数：',b)
    #price=float(hqTMDVARI.varity_code)
    print(sec_code,'的最新价：',hqTMDVARI.lastPrice)

#退出MDClient接口
if dll.MDCExit():
    print('退出MDClient接口')

#调用实时行情接收函数
showhqlist=bookqhrecord(MDReqID,excode,symbol,hqlist)
#print('最新行情价格',hqlist.lastPrice)

if dll.MDCRegisterBCOnMarketDataReject(bookmkdata):
    print('实时行情回调成功MDCRegisterBCOnMarketDataReject')
##########################以下为回调函数##################################

#为了从服务器获取其状态信息TAG10031，而发起的一个特殊请求
getstatus=dll.MDCRequestTag10031()
int(getstatus)
if getstatus>0:
    print('从服务器获取其状态信息TAG10031正常')
else:
    print('从服务器获取其状态信息TAG10031异常')

#批量注册回调函数


T_MDCCALLBACKFUNCS=CFUNCTYPE(None)
def RegisterCallBackFuncs():
    [('pFuncOnLogon', pBCOnLogon),
     ('pFuncOnHeartBeat', pBCOnHeartBeat),
     ('pFuncOnLogout', pBCOnLogout),
     ('pFuncOnMarketDataReject', pBCOnMarketDataReject),
     ('pFuncOnMRTDReceived', pBCOnMRTDReceived),
     ('pFuncOnEndOfMDRefush', pBCOnEndOfMDReflush),
     ('pFuncOnMRTDetailTrading', pBCOnMRTDetailTrading)]
    print('注册行情接口开始...')
Registerallfunction=T_MDCCALLBACKFUNCS(RegisterCallBackFuncs)
dll.MDCRegisterCallBackFunctions(Registerallfunction)
'''

