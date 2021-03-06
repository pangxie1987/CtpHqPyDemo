#coding=utf-8
from ctypes import *
from Hqdtype import *
from  HqStruct import *
import HqStruct
import ctypes
import time
import data_type
import logger
import  threading
import random

##################################
#连接CMDS的行情代码，HqGUI创建UI界面进行展示
##################################

# *************下面部分是初始服务器连接设置的函数*************
print('CMDClient演示代码python3.4.3版本启动')

#对于tkgui中的CMDhqGUI.cmdgui=CmdGui()反注册
cmdgui=None

# 使用的CMDS行情CMDClient.dll
dll = ctypes.cdll.LoadLibrary('../CMDClient.dll')
logger.logging.info('CMDClient.dll加载成功')
print('CMDClient.dll加载成功')

# # 定义参数配置方法，参数将从配置文件config.conf中读取
# def getConfig(section, key):
#     config = configparser.ConfigParser()
#     path = 'config.ini'
#     config.read(path)
#     return config.get(section, key)

# # 建立数据库连接，为后续数据插入做准备
# dblink = getConfig('database', 'dbname')
# conn = cx_Oracle.connect('core/core@127.0.0.1/xe')
# 从config.conf中读取数据库配置
# conn = cx_Oracle.connect(dblink)
# cursor = conn.cursor()
global stkcodelist
stkcodelist=[]
global hqdata
hqdata=['0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0']
class Hqshow(object):   #行情回调
    #注册实时接收行情回调函数pBCOnMRTDReceived，接收订阅到的实时行情

    pBCOnMRTDReceived = CFUNCTYPE(None, c_char_p, c_char_p, c_char_p, tT_MDREAL)
    def bookqhrecord(MDReqID,excode,symbol,hqlist):

        hqdata[0]=(hqlist.contents.exchCode.decode('utf-8'))
        print('shichangdaima[0]',hqdata[0])
        hqdata[1] =(hqlist.contents.varity_code).decode('utf-8')
        #hqdata[2] =(hqlist.contents.varity_name).decode('utf-8')
        hqdata[2] =(hqlist.contents.chgStatus).decode('utf-8')
        hqdata[3] =(hqlist.contents.openPrice)
        hqdata[4] =(hqlist.contents.lastPrice)
        hqdata[5] =(hqlist.contents.highestPrice)
        hqdata[6] =(hqlist.contents.lowestPrice)
        hqdata[7] =(hqlist.contents.doneVolume)
        hqdata[8] =(hqlist.contents.chgPrice)
        hqdata[9] =(hqlist.contents.upperLimitPrice)
        hqdata[10] =(hqlist.contents.lowerLimitPrice)
        hqdata[11] =(hqlist.contents.openInterest)
        hqdata[12] =(hqlist.contents.preSettlePrice)
        hqdata[13] =(hqlist.contents.settlePrice)
        hqdata[14] =(hqlist.contents.preOpenInterest)
        hqdata[15] =(hqlist.contents.closePrice)
        hqdata[16] =(hqlist.contents.totlebuy)
        hqdata[17] =(hqlist.contents.totlesell)
        hqdata[18] =(hqlist.contents.turnover)


        # print(    hqlist.contents.varity_code.decode('utf-8'),
        #           hqlist.contents.varity_name.decode('gbk'),
        #           #hqlist.contents.cmdstime,
        #           hqlist.contents.openPrice,
        #           hqlist.contents.lastPrice,)
        #               # hqlist.contents.highestPrice,
        #               # hqlist.contents.lowestPrice,
        #               # hqlist.contents.doneVolume,  #成交量
        #               # round(hqlist.contents.chgPrice,4),'\n', #涨跌
        #               # round(hqlist.contents.upperLimitPrice ,4),    #涨停板
        #               # round(hqlist.contents.lowerLimitPrice,4), #跌停板
        #               # round(hqlist.contents.hisHighestPrice,4), #历史最高价
        #               # round(hqlist.contents.hisLowestPrice,4),'\n', #历史最低价
        #               # hqlist.contents.openInterest,  #净持仓
        #               # round(hqlist.contents.preSettlePrice,4), #昨日结算
        #               # round(hqlist.contents.preClosePrice,4),    #昨日收盘
        #               # round(hqlist.contents.settlePrice,4))  #今日结算

        if cmdgui==None:
            pass
        else:
            cmdgui.inserttreeview()
        logger.logging.info('行情变动推送成功，合约%s'%hqdata[1])

    bookmkdata=pBCOnMRTDReceived(bookqhrecord)
    dll.MDCRegisterBCOnMRTDReceived(bookmkdata)

def download(): #行情订阅
        # 订阅方式
    cReqType = 49  # ASCII码相当于1，×××××注意×××××，否则订阅不成功
    # '0' = Snapshot;  仅仅得到一次行情数据快照
    # '1' = Snapshot + Updates (Subscribe);  快照和后续的更新(订阅)，直到取消或断开连接
    # '2' = Disable previous Snapshot + Update Request (Unsubscribe) 取消之前的指定的订阅

    iMDType = 0
    # 0=仅仅为实时行情数据;
    # 1=除了实时行情数据，也包括实时分笔成交数据;

    # 建立订阅市场的合约列表

    booklist = []
    booklist.append(b'XSHG,?;')  # 上交所
    booklist.append(b'CFFX,?;')  # 中金所
    booklist.append(b'XSGE,?;')  # 上期所
    booklist.append(b'XDCE,?;')  # 大商所
    booklist.append(b'XCZE,?;')  # 郑商所
    booklist.append(b'XSHE,?;')  # 深交所

    # 向服务器订阅实时行情数据; 或取消原订阅的请求 MDCBookRTMarketData
    # 行情订阅号
    sMDReqID = random.choice(booklist)

    sSymbolList = data_type.datatype.stkcode_list.encode('utf-8')  # 从GUI中获取行情代码,根据这个合约订阅行情
    print('sSymbolList',sSymbolList)
    def show_bookresult(bookresult, mkkk):
        if (bookresult < 0):
            if (-202 == bookresult):
                print('错误的cReqType', bookresult)
                logger.logging.warning('错误的cReqType%s' % bookresult)
            elif (-201 == bookresult):
                print('订阅号重复', bookresult)
                logger.logging.warning('订阅号重复%s' % bookresult)

            else:
                print('订阅号发送失败', bookresult)
                logger.logging.warning('订阅号发送失败%s' % bookresult)
        else:
            print('订阅实时行情请求成功,', mkkk)
            logger.logging.info('订阅实时行情请求成功%s'%mkkk)

    i = 0
    while (i < len(booklist)):
        bookresult = dll.MDCBookRTMarketData(sSymbolList, cReqType, iMDType, sSymbolList)  # 订阅所有行情
        # bookresult = dll.MDCBookRTMarketData(booklist[i], cReqType, iMDType,sylist )#单个合约订阅
        show_bookresult(bookresult, (sSymbolList))
        i += 1
    print('订阅实时全市场全品种行情数据完毕！')
    time.sleep(1)

def loginCMDS():    #登录CMDS
    # 初始化MDClient的接口，设置日志路径
    logpath = ''
    print(logpath)
    if dll.MDCInitialize(logpath):
        dll.MDCSetDebug(1)
        print('行情日志打开成功')
    else:
        print('行情日志打开失败')

    # 退出MDClient接口
    if dll.MDCExit():
        print('退出MDClient接口')

    if dll.MDCToDisconnect():
        print('MDCToDisconnect')

    # 设置MDClient模式内部的调试模式
    idebug = 1
    if dll.MDCSetDebug(idebug):
        print('MDClient模式内部的调试模式为：', idebug)
    else:
        print('MDClient模式内部的调试模式失败')

    # 设置MDClient的SessionID
    sessionid = b'LINK3-KS'
    if dll.MDCSetSessionID(sessionid):
        print('sessionid设置成功', sessionid)
    else:
        print('sessionid设置失败')

    # 设置DMClient在与服务器对接的时候，约定的心跳参数
    hbt = '1'
    hblatency = '1'
    if dll.MDCSetHeartBtInt(hbt, hblatency):
        print('心跳参数设置为', hbt)
    else:
        print('心跳设置失败')

    # 从GUI中获取的ip地址
    svrip_port = data_type.datatype.hqip.encode('utf-8')
    #svrip_port=b'127.0.0.1:11200'
    if dll.MDCSetServer(svrip_port):
        print('行情地址设置成功,ip地址为', svrip_port)
        logger.logging.info('行情地址设置成功,ip地址为%s' % svrip_port)
    else:
        print('行情地址设置失败', svrip_port)
        logger.logging.warning('行情地址设置失败%s' % svrip_port)

    # 设置登录的用户名和口令(CMDS的用户名和密码都是固定的)
    userpassword = b'ITSTest,123456'

    if dll.MDCSetLogonUser(userpassword):
        print('登录的用户名称密码为', userpassword)
        logger.logging.info('登录的用户名称密码为%s' % userpassword)
    else:
        print('用户名称密码错误')
        logger.logging.warning('用户名称密码错误%s' % userpassword)
    '''
    #设置行情接收模式
    if dll.MDCSetSockMode(0,-1,0):
        print('设置行情接收模式为TCP')
    '''

    # 开始连接服务器，若启动成功，则会启动后台数据接收线程
    nresult = dll.MDCToConnectServer(svrip_port, userpassword)
    time.sleep(1)
    if nresult == 1:
        print('登陆行情成功......')
        logger.logging.info('登陆行情成功' )
    else:
        print('登陆行情失败.返回值', nresult)
        logger.logging.warning('登陆行情失败.返回值%s'%nresult)

    # 获取与服务器的连接状态
    mdCGetLinkStatus = dll.MDCGetLinkStatus()
    if mdCGetLinkStatus == 1:
        print('服务器的连接状态 \n1 - 连接链路已经建立，但还没有完成LOGN之前')
        logger.logging.warning('服务器的连接状态1 - 连接链路已经建立，但还没有完成LOGN之前')
    elif mdCGetLinkStatus == 2:
        print('服务器的连接状态 \n2 - LOGON消息交换完毕，开始进行正式的MD数据交换状态')
        logger.logging.warning('服务器的连接状态2- LOGON消息交换完毕，开始进行正式的MD数据交换状态')
    elif mdCGetLinkStatus == 0:
        print('服务器的连接状态 \n0 - 处于断开状态')
        logger.logging.warning('服务器的连接状态0--处于断开状态')
    else:
        print('服务器的连接状态异常,请检查连接')
        logger.logging.warning('服务器的连接状态异常,请检查连接')

    # 获取是否启动后台数据接收线程状态
    mdCGetBackRunStatus = dll.MDCGetBackRunStatus()
    if mdCGetBackRunStatus == 0:
        print('后台数据接收线程状态 0 - 停止状态')
        logger.logging.warning('后台数据接收线程状态 0 - 停止状态')
    elif mdCGetBackRunStatus == 1:
        print('后台数据接收线程状态 1 - 启动状态')
        logger.logging.warning('后台数据接收线程状态 1 - 启动状态')

        data_type.datatype.status='ok'
        if cmdgui==None:
            pass
        else:
            cmdgui.lable04update()

    else:
        print('后台数据接收线程状态异常，请检查')
        logger.logging.warning('后台数据接收线程状态异常，请检查')

    # 在登录成功时候的回调函数， difSec是本机和服务器之间的时间差，本机时间+difSec=近视的服务器时间
    pFuncOnLogon = CFUNCTYPE(c_int)

    def logdeftime(difSec):
        print('行情登录通知.本机与行情服务器时间差=', difSec)
        logger.logging.info('行情登录通知.本机与行情服务器时间差=%s'%difSec)

    logdeftimepb = pFuncOnLogon(logdeftime)
    dll.MDCRegisterBCOnLogon(logdeftimepb)


#****************************以下为主函数****************************
# def main():
#     #loginCMDS()
#     Hqshow()
# loginCMDS()
# download()