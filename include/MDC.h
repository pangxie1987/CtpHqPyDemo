#ifndef MDC_h__
#define MDC_h__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MDBase.h"

#ifndef BUDLL
#define BUPUB_API 
#else
#ifdef BUPUB_EXPORTS
#define BUPUB_API __declspec(dllexport)
#else
#ifdef _MSC_VER
#define BUPUB_API __declspec(dllimport)
#else
#define BUPUB_API 
#endif
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif
   #ifndef MDS_TCP
   #define MDS_TCP   0       // 采用之前的TCP连接模式
   #define MDS_UDP   1       // 采用UDP模式，接收广播行情 许多发送函数不可用
   #define MDS_MCB   2        // 采用多播模式，类似UDP模式
   #endif
   
   
   enum MDCBACKSTATUS
   {
      BS_STOP,
      BS_RUNNING
   };
   
   //////////////////////////////////////////////////////////////////////////
   // 以下为各类回调函数的接口定义和注册:
   
   // 在登录成功时候的回调函数， difSec是本机和服务器之间的时间差，本机时间+difSec=近视的服务器时间
   typedef void (*pBCOnLogon)(int difSec);
   
   // 在收到CMDS的心跳消息时候的回调函数，tag10031: <当前UserLinks个数>,<当前在线订阅的行情个数>,<有效的交易所个数>,<(ExISOCode):(NoSym)>;...
   // 但本消息只有在连接正常，并且本方成功调用MDCRequestTag10031函数之后，才有可能被调用。
   typedef void (*pBCOnHeartBeat)(const char *tag10031);
   
   // 在收到CMDS的录出消息时候的回调函数，info: 即对方录出时候的信息串
   typedef void (*pBCOnLogout)(const char *info);
   
   // 在收到CMDS的市场数据请求拒绝的回调函数
   typedef void (*pBCOnMarketDataReject)(const char *MDReqID, const char *MDReqRejReason, const char *text);
   
   // 在收到CMDS的实时行情数据的回调函数
   typedef void (*pBCOnMRTDReceived)(const char *MDReqID, const char *excode, const char *symbol, const T_MDREAL *pMDRecord);
   
   // 在收到CMDS的整套订阅行情接收完毕时候的回调函数 - 在服务器设置该连接用户需要有EOM的时候，在MDCBookRTMarketData的查询或订阅的时候，每段sSymbolList中结束，即收到一个EOM消息
   // 但若sSymbolList中的symbol部分，不是以'?'/'*'开始的，则没有这个EOM消息
   typedef void (*pBCOnEndOfMDReflush)(const char *MDReqID, const char *excode, const char *symbol, time_t tmServer);
   
   // 在收到用MDCBookRTMarketData后的实时分笔成交数据时候的回调函数
   typedef void (*pBCOnMRTDetailTrading)(const char *MDReqID, const char *excode, const char *symbol, const T_MDVARI *pMDVari, int nRecords);
   
   
   // 查询批量行情数据的回调函数的注册: 
   
   // 在MDCRequestDetailTrading后收到指定日期的回调函数:
   typedef void (*pBCOnHISDetailTrading)(const char *excode, const char *symbol, const T_MDVARI *pMDVari, int nRecords);
   
   // 在MDCRequestKChartData后收到K图数据的回调函数
   typedef void (*pBCOnKChartData)(const char *excode, const char *symbol, int kctype, const T_MDKCHART *pKChart, int nRecords);
   
   ///  2012/12/6 21:53:02 后版本支持:
   // 在MDCRequestTickData后收到TICK数据时的回调函数
   typedef void (*pBCOnTickData)(const char *excode, const char *symbol, const T_MDTICK *pTicks, int nrecords);
   
#define MAXKCITEMS 2000
#define MAXVITEMS 200000
   


//////////////////////////////////////////////////////////////////////////
// 下面部分是初始服务器连接设置的函数:

   // 函数名: MDCInitialize
   // 编程  : 陈永华 2012-2-7 15:39:44
   // 描述  : 初始化MDClient的接口，设置日志路径
   // 返回  : bool   
   // 参数  : const char *logpath [IN]: 日志路径，最后不要加目录分隔符；可为NULL，即在当前目录下
   BUPUB_API bool MDCInitialize(const char *logpath=NULL);


   // 函数名: MDCExit
   // 编程  : 陈永华 2012-2-7 15:42:50
   // 描述  : 退出MDClient接口
   // 返回  : void 
   BUPUB_API void MDCExit();

   // 函数名: MDCSetDebug
   // 编程  : 陈永华 2012-1-21 22:58:10
   // 描述  : 设置MDClient模式内部的调试模式
   // 返回  : void 
   // 参数  : int idebug [IN]: 0 - 关闭；1 - 打开（会将发送接收的消息记录到日志中）
   BUPUB_API void MDCSetDebug(int idebug);


// 函数名: MDCSetSessionID
// 编程  : 陈永华 2012-1-15 17:01:38
// 描述  : 设置MDClient的SessionID
// 返回  : int : 0-失败；1-成功
// 参数  : const char *sessionid [IN]: SessionID，用<localid>-<serverid>表示，例如: "LINK3-KS"
BUPUB_API int MDCSetSessionID(const char *sessionid);

// 函数名: MDCSetHeartBtInt
// 编程  : 陈永华 2012-1-15 17:16:47
// 描述  : 设置DMClient在与服务器对接的时候，约定的心跳参数
// 返回  : int : 0-失败；1-成功
// 参数  : unsigned int hbt [IN]: 心跳间隔时间，用秒计
// 参数  : unsigned int hblatency [IN]: 对方心跳测试反馈的间隔时间，用秒计
BUPUB_API int MDCSetHeartBtInt(unsigned int hbt,unsigned int hblatency);

// 函数名: MDCSetServer
// 编程  : 陈永华 2012-1-15 17:13:59
// 描述  : 设置MDClient的对接服务器端的连接参数(IP:Port)
// 返回  : int : 0-失败；1-成功
// 参数  : const char *svrip_port [IN]: 行情服务器的连接IP和Port的组合参数，用<IP>:<Port>表示，例如: "127.0.0.1:11200"
BUPUB_API int MDCSetServer(const char *svrip_port);

// 函数名: MDCSetLogonUser
// 编程  : 陈永华 2012-1-20 15:14:51
// 描述  : 设置登录的用户名和口令
// 返回  : void 
// 参数  : const char *userpassword [IN]: <UserName>,<Password> - 用逗号分隔用户名和口令
BUPUB_API void MDCSetLogonUser(const char *userpassword);



// Function name: MDCSetSockMode
// Author: CHENYH @ 2014/3/3 23:08:24
// Description: 设置行情接收模式
// Return type: int 
// Argument   : int sockmode [IN]: 使用行情接收的模式，MDS_TCP,MDS_UDP,MDS_MBC /*=MDS_TCP*/
// Argument   : int ttl [IN]: MDS_TCP无效 /*=-1*/
// Argument   : int loop [IN]: MDS_TCP无效 /*=-1*/
// Argument   : int bUseMDSet [IN]: MDS_TCP无效 /*=0*/
BUPUB_API int MDCSetSockMode(int sockmode/*=MDS_TCP*/, int ttl/*=-1*/, int loop/*=-1*/, int bUseMDSet/*=0*/);

// 函数名: MDCToConnectServer
// 编程  : 陈永华 2012-1-20 14:45:34
// 描述  : 开始连接服务器，若启动成功，则会启动后台数据接收线程。
// 返回  : int : 1-表示成功登录行情服务器，可以开始正常服务了；0 - 失败。详细可参考日志
// 参数  : const char *svrip_port [IN]: 可以为NULL，即用已经设置的参数；否则为行情服务器的连接IP和Port的组合参数，用<IP>:<Port>表示，例如: "127.0.0.1:11200"
// 参数  : const char *userpass [IN]:可以为NULL，即用已经设置的参数；否则即要求为: <UserName>,<Password> - 用逗号分隔用户名和口令
BUPUB_API int MDCToConnectServer(const char *svrip_port, const char *userpass);


// 函数名: MDCToDisconnect
// 编程  : 陈永华 2012-1-21 0:32:17
// 描述  : 
// 返回  : void 
BUPUB_API void MDCToDisconnect();

//////////////////////////////////////////////////////////////////////////
// 下面为查询本模块内部状态:

// 函数名: MDCGetLinkStatus
// 编程  : 陈永华 2012-1-18 19:31:41
// 描述  : 获取与服务器的连接状态
// 返回  : int : 0 - 处于断开状态；1 - 连接链路已经建立，但还没有完成LOGN之前；2 - LOGON消息交换完毕，开始进行正式的MD数据交换状态
BUPUB_API int MDCGetLinkStatus();

// 函数名: MDCGetBackRunStatus
// 编程  : 陈永华 2012-1-20 14:45:56
// 描述  : 获取是否启动后台数据接收线程状态
// 返回  : int : 0 - 停止状态；1 - 启动状态
BUPUB_API int MDCGetBackRunStatus();

//////////////////////////////////////////////////////////////////////////
// 以下为实际的与CMDS之间的消息交换，以得到相关行情数据:

// 函数名: MDCRequestTag10031
// 编程  : 陈永华 2012-1-20 22:14:24
// 描述  : 为了从服务器获取其状态信息TAG10031，而发起的一个特殊请求
// 返回  : int >0: 表示发送请求成功；否则，表示失败
BUPUB_API int MDCRequestTag10031(); 

// 函数名: MDCBookRTMarketData
// 编程  : 陈永华 2012-1-20 23:59:48
// 描述  : 向服务器订阅实时行情数据; 或取消原订阅的请求
// 返回  : int : -202: 错误的cReqType; -201: 订阅号重复; <=0: 发送消息失败；否则成功
// 参数  : const char *sMDReqID [IN]: 行情订阅号，用于后续的数据返回和请求取消
// 参数  : char cReqType [IN]: 
//          '0' = Snapshot;  仅仅得到一次行情数据快照
//          '1' = Snapshot + Updates (Subscribe);  快照和后续的更新(订阅)，直到取消或断开连接
//          '2' = Disable previous Snapshot + Update Request (Unsubscribe) 取消之前的指定的订阅
//                当为 '2' 的时候，则不需要下面这些 域 了。
// 参数  : int iMDType [IN]: 行情数据内容
//                0=仅仅为实时行情数据; 
//                1=除了实时行情数据，也包括实时分笔成交数据; 
// 参数  : const char *sSymbolList [IN]: 代码列表串，格式为: "<交易所代码>,<金融产品代码串>;...;<交易所代码>,<金融产品代码串>" 
//          在金融产品代码串中，'?': 即为该交易所下的所有品种；
//                              '*XXX': 即以'*'为开始字符，后面为指定该交易所下，代码为XXX开始的品种；
//                              其他则为具体指定的品种代码串
BUPUB_API int MDCBookRTMarketData(const char *sMDReqID, char cReqType, int iMDType, const char *sSymbolList);


// Function name: MDCGetMDReqIDs
// Author       : CHENYH 2012/5/4 15:46:04
// Description  : 提取接口中，已经有效订阅的订阅号列表清单
// Return type  : int 返回当前订阅个数
// Argument : char *MDReqIDList [OUT]: 用于存放订阅号清单的缓存，其返回格式为:'<订阅号>;...' 
// Argument : unsigned int MDReqIDLength [IN]: MDReqIDList缓存的字节长度
BUPUB_API int MDCGetMDReqIDs(char *MDReqIDList, unsigned int MDReqIDLength);


// 函数名: MDCRequestDetailTrading
// 编程  : 陈永华 2012-1-22 13:23:31
// 描述  : 向CMDS查询指定日期的分笔成交数据，为了接收正常，必须先用MDCRegisterHisDetailTrading注册相应的回调函数:
//         若正常数据接收，则会每次收到部分(或全部)记录（每次最多不超过 8000/sizeof(T_MDVARI)）
//          最后一个有效消息中的最后一条记录的tmsec为0，表示结束，可以注销相应的回调函数；
//         若服务器检查数据后，拒绝，则会收到一条行情数据请求拒绝的消息，即回调之前注册的
//                   OnMarketDataReject，其中的MDReqID="", MDReqRejReason="DTR", 和具体拒绝原因
// 返回  : int: >0 提交请求成功, 准备接收数据; <=0 提交请求失败，可能连接出现故障
// 参数  : const char *sExchCode [IN]: 指定的交易所代码
// 参数  : const char *sSymbol [IN]: 明确的该交易所的品种代码
// 参数  : int iYYYYMMDD [IN]: 指定获取分笔成交的日期
BUPUB_API int MDCRequestDetailTrading(const char *sExchCode, const char *sSymbol, int iYYYYMMDD);


// 函数名: MDCRequestKChartData
// 编程  : 陈永华 2012-1-24 14:50:54
// 描述  : 向服务器查询获取KChart数据，为了接收正常，必须先注册相应的回调函数:
//         若正常数据接收，则会每次收到部分(或全部)记录（每次最多不超过 8000/sizeof(T_MDKCHART)）
//          最后一个有效消息中的最后一条记录的tmsec为0，表示结束，可以注销相应的回调函数；
//         若服务器检查数据后，拒绝，则会收到一条行情数据请求拒绝的消息，即回调之前注册的
//                   OnMarketDataReject，其中的MDReqID="", MDReqRejReason="KCR", 和具体拒绝原因
// 返回  : int: >0 提交请求成功, 准备接收数据; <=0 提交请求失败，可能连接出现故障
// 参数  : const char *excode [IN]: 指定的交易所代码
// 参数  : const char *symbol [IN]: 明确的该交易所的品种代码
// 参数  : int kctype [IN]: K图数据的类型: (基本BASE_KCTYPE定义)+(倍数)*10
// 参数  : unsigned int tmsec [IN]: 参考时间点，仅仅在 kctype=KCT_DAY或KCT_MIN时候有效; 其他为参考最后记录中的时间点
// 参数  : int nitems [IN]: 最大接收K图数据记录数；0 则按照服务器内定的最大值
//                         当kctype=KCT_DAY或KCT_MIN时候，nitems<0即为指定时间点之前的记录部分；
//                                               nitems>0即为指定时间点之后的记录部分；
BUPUB_API int MDCRequestKChartData(const char *excode, const char *symbol, int kctype, unsigned int tmsec, int nitems);


// Function name: MDCRegisterDynaUser
// Author       : CHENYH 2012/12/16 23:26:06
// Description  : CMD-DNS专用函数，用于开通指定用户的行情连接
// return   : int: >0 提交请求成功, 准备接收数据; <=0 提交请求失败，可能连接出现故障
// Argument : const char *partnerid [IN]: 指定该用户连接使用的连接串号
// Argument : const char *username [IN]: 指定该用户的登录用户名
// Argument : const char *password [IN]: 指定该用户的登录密码
// Argument : int refint [IN]: 该用户行情刷新频率 ms为单位
// Argument : int iEnd [IN]: 在批量行情刷新时候，是否需要在一批行情结束的时候，提供EndOfReflush消息以提示 0 - 不需要
BUPUB_API int MDCRegisterDynaUser(const char *partnerid, const char *username, const char *password, int refint, int iEnd);



// Function name: MDCRegisterBCOnLogon
// Author       : CHENYH 2012/4/30 22:46:27
// Description  : 独立设置收到Logon消息时候的回调函数
// Return type  : void 
// Argument : pBCOnLogon pFuncOnLogon [IN] : 回调函数，可为NULL以关闭回调
BUPUB_API void MDCRegisterBCOnLogon(pBCOnLogon pFuncOnLogon);

// Function name: MDCRegisterBCOnHeartBeat
// Author       : CHENYH 2012/4/30 22:48:21
// Description  : 独立设置收到HeartBeat消息时候的回调函数
// Return type  : void 
// Argument : pBCOnHeartBeat pFuncOnHeartBeat[IN] : 回调函数，可为NULL以关闭回调
BUPUB_API void MDCRegisterBCOnHeartBeat(pBCOnHeartBeat pFuncOnHeartBeat);


// Function name: MDCRegisterBCOnLogout
// Author       : CHENYH 2012/4/30 22:58:06
// Description  : 独立设置收到Logout消息时候的回调函数
// Return type  : void 
// Argument : pBCOnLogout pFuncOnLogout [IN] : 回调函数，可为NULL以关闭回调
BUPUB_API void MDCRegisterBCOnLogout(pBCOnLogout pFuncOnLogout);


// Function name: MDCRegisterBCOnMarketDataReject
// Author       : CHENYH 2012/4/30 22:58:09
// Description  : 独立设置收到MarketDataReject消息时候的回调函数
// Return type  : void 
// Argument : pBCOnMarketDataReject pFuncOnMarketDataReject [IN] : 回调函数，可为NULL以关闭回调
BUPUB_API void MDCRegisterBCOnMarketDataReject(pBCOnMarketDataReject pFuncOnMarketDataReject);


// Function name: MDCRegisterBCOnMRTDReceived
// Author       : CHENYH 2012/4/30 22:58:13
// Description  : 独立设置收到MRTDReceived(实时行情数据)消息时候的回调函数
// Return type  : void 
// Argument : pBCOnMRTDReceived pFuncOnMRTDReceived [IN] : 回调函数，可为NULL以关闭回调
BUPUB_API void MDCRegisterBCOnMRTDReceived(pBCOnMRTDReceived pFuncOnMRTDReceived);


// Function name: MDCRegisterBCOnEndOfMDReflush
// Author       : CHENYH 2012/4/30 22:58:17
// Description  : 独立设置收到EndOfMDReflush(通配方式查询订阅行情数据接收完毕)消息时候的回调函数
// Return type  : void 
// Argument : pBCOnEndOfMDReflush pFuncOnEndOfMDReflush [IN] : 回调函数，可为NULL以关闭回调
BUPUB_API void MDCRegisterBCOnEndOfMDReflush(pBCOnEndOfMDReflush pFuncOnEndOfMDReflush);


// Function name: MDCRegisterBCOnMRTDetailTrading
// Author       : CHENYH 2012/5/1 21:21:34
// Description  : 独立设置收到当日分笔成交数据MRTDetailTrading消息时候的回调函数
// Return type  : void 
// Argument : pBCOnMRTDetailTrading pFuncOnMRTDetailTrading [IN] : 回调函数，可为NULL以关闭回调
BUPUB_API void MDCRegisterBCOnMRTDetailTrading(pBCOnMRTDetailTrading pFuncOnMRTDetailTrading);

//////////////////////////////////////////////////////////////////////////
// 下面是一次性注册上述这些回调函数的接口:
typedef struct _T_MDCCALLBACKFUNCS
{
   pBCOnLogon pFuncOnLogon;
   pBCOnHeartBeat pFuncOnHeartBeat;
   pBCOnLogout pFuncOnLogout;
   pBCOnMarketDataReject pFuncOnMarketDataReject;
   pBCOnMRTDReceived pFuncOnMRTDReceived;
   pBCOnEndOfMDReflush pFuncOnEndOfMDRefush;
   pBCOnMRTDetailTrading pFuncOnMRTDetailTrading;
} T_MDCCALLBACKFUNCS;


// 函数名: MDCRegisterCallBackFunctions
// 编程  : 陈永华 2012-1-20 22:26:20
// 描述  : 可一次性注册一些回调函数
// 返回  : void 
// 参数  : T_MDCCALLBACKFUNCS *pCallBackFunc [IN]: 回调函数的集合，对不关心的事件可以设置为NULL
BUPUB_API void MDCRegisterCallBackFunctions(T_MDCCALLBACKFUNCS *pCallBackFunc);
//////////////////////////////////////////////////////////////////////////


// 函数名: MDCRegisterHisDetailTrading
// 编程  : 陈永华 2012-1-24 14:50:54
// 描述  : 注册/注销 历史日分笔成交数据回调函数
// 返回  : void 
// 参数  : pBCOnHISDetailTrading pFuncOnHisDetailTrading [IN]: 注册的回调函数；NULL: 即注销
BUPUB_API void MDCRegisterHisDetailTrading(pBCOnHISDetailTrading pFuncOnHisDetailTrading);

// 函数名: MDCRegisterKChartData
// 编程  : 陈永华 2012-1-24 14:50:54
// 描述  : 注册/注销 K图数据接收回调函数
// 返回  : void 
// 参数  : pBCOnKChartData pFuncOnKChartData [IN]: 注册的回调函数；NULL: 即注销
BUPUB_API void MDCRegisterKChartData(pBCOnKChartData pFuncOnKChartData);


// Function name: MDCRegisterTickData
// Author       : CHENYH 2012/12/6 21:59:35
// Description  : 注册/注销 TICK数据接收的回调函数，在异步处理方式下使用
// Return type  : BUPUB_API void 
// Argument : pBCOnTickData pFuncOnTickData [IN]: 注册的回调函数；NULL: 即注销
BUPUB_API void MDCRegisterTickData(pBCOnTickData pFuncOnTickData);
//////////////////////////////////////////////////////////////////////////
// 以下为同步调用功能函数:
// Function name: MDCSyncModeSet
// Author       : CHENYH 2012/4/28 11:05:36
// Description  :  启动同步行情模式（必须在成功和CMDSIII连接成功才有可能成功）
// Return type		: int ==0: 成功启动同步行情模式; <0: 同步行情启动失败
// Argument         : const char *sExchCodeList[IN]: 用';'分隔的市场代码列表: 如"XSHE;XSHG"
BUPUB_API int MDCSyncModeSet(const char *sExchCodeList);


// Function name: MDCSyncMarketData
// Author      : CHENYH 2012/4/28 16:27:08
// Description : 在同步模式下(即已经调用了MDCSyncModeSet后)，同步读取指定金融商品的行情信息数据
// Return type	: int 返回成功读取的记录数；<=0: 失败
// Argument : const char *excode [IN]: 指定的交易所代码，参考MDCSyncModeSet中的sExchCodeList中设置值
// Argument : const char *symbol [IN]: 指定的金融商品代码，NULL 则取全部（最大个数由nMaxMDs设定）
// Argument : T_MDREAL *pMDBuf [OUT]: 读取行情数据的存放缓存
// Argument : int nMaxMDs [IN]: pMDBuf最大可存放的行情记录数
BUPUB_API int MDCSyncMarketData(const char *excode, const char *symbol, T_MDREAL *pMDBuf, int nMaxMDs);

// !!!! 以下两个同步调用函数即使在异步模式下也可使用，但由于将会有大量的消息接收，会影响之前的异步订阅消息及时性
// Function name: MDCSyncKChartData
// Author       : CHENYH 2012/4/29 21:49:06
// Description  : 向服务器查询获取KChart数据
// 返回  : int: >0 返回本次查询到的记录数; <=0 提交请求失败，可能连接等故障
// 参数  : const char *excode [IN]: 指定的交易所代码
// 参数  : const char *symbol [IN]: 明确的该交易所的品种代码
// 参数  : int kctype [IN]: K图数据的类型: (基本BASE_KCTYPE定义)+(倍数)*10
// 参数  : unsigned int tmsec [IN]: 参考时间点，仅仅在 kctype=KCT_DAY时候有效; 其他为参考最后记录中的时间点
// 参数  : int nitems [IN]: 最大接收K图数据记录数；
// Argument : T_MDKCHART *pKCItems [OUT]: 用于存放的K图数据记录的缓存。
// Argument : int waitms [IN]: 最大等待时间ms。<=0: 一直等待，直到出现错误或数据接收完整为止
// Argument : char *errmsg256 [OUT]: 当返回<=0的时候，提供的错误信息描述，必须要有256字节长度的缓存
BUPUB_API int MDCSyncKChartData(const char *excode, const char *symbol, int kctype, unsigned int tmsec, 
                                int nitems, T_MDKCHART *pKCItems, int waitms, char *errmsg256);

// Function name: MDCSyncDetailTrading
// Author       : CHENYH 2012/4/30 9:11:05
// Description  : 向CMDS查询指定日期的分笔成交数据
// 返回  : int: >0 请求成功，返回查询到的数据记录数; <=0 查询失败，各类错误
// 参数  : const char *sExchCode [IN]: 指定的交易所代码
// 参数  : const char *sSymbol [IN]: 明确的该交易所的品种代码
// 参数  : int iYYYYMMDD [IN]: 指定获取分笔成交的日期
// Argument : DONEVOLUME fromDoneVolume [IN]: 提取指定日期的成交量大于本指定值，即开始为0
// Argument : int nitems [IN]: pItems最大可存放分笔成交数据记录数
// Argument : T_MDVARI *pItems [OUT]: 存放分笔成交数据记录的缓存
// Argument : int waitms [IN]: 最大等待时间ms。<=0: 一直等待，直到出现错误或数据接收完整为止
// Argument : char *errmsg256 [OUT]: 当返回<=0的时候，提供的错误信息描述，必须要有256字节长度的缓存
BUPUB_API int MDCSyncDetailTrading(const char *sExchCode, const char *sSymbol, int iYYYYMMDD, 
                         DONEVOLUME fromDoneVolume, int nitems, T_MDVARI *pItems, int waitms, char *errmsg256);



//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif

#undef BUPUB_API


#endif // MDC_h__