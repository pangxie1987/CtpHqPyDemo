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
   #define MDS_TCP   0       // ����֮ǰ��TCP����ģʽ
   #define MDS_UDP   1       // ����UDPģʽ�����չ㲥���� ��෢�ͺ���������
   #define MDS_MCB   2        // ���öಥģʽ������UDPģʽ
   #endif
   
   
   enum MDCBACKSTATUS
   {
      BS_STOP,
      BS_RUNNING
   };
   
   //////////////////////////////////////////////////////////////////////////
   // ����Ϊ����ص������Ľӿڶ����ע��:
   
   // �ڵ�¼�ɹ�ʱ��Ļص������� difSec�Ǳ����ͷ�����֮���ʱ������ʱ��+difSec=���ӵķ�����ʱ��
   typedef void (*pBCOnLogon)(int difSec);
   
   // ���յ�CMDS��������Ϣʱ��Ļص�������tag10031: <��ǰUserLinks����>,<��ǰ���߶��ĵ��������>,<��Ч�Ľ���������>,<(ExISOCode):(NoSym)>;...
   // ������Ϣֻ�����������������ұ����ɹ�����MDCRequestTag10031����֮�󣬲��п��ܱ����á�
   typedef void (*pBCOnHeartBeat)(const char *tag10031);
   
   // ���յ�CMDS��¼����Ϣʱ��Ļص�������info: ���Է�¼��ʱ�����Ϣ��
   typedef void (*pBCOnLogout)(const char *info);
   
   // ���յ�CMDS���г���������ܾ��Ļص�����
   typedef void (*pBCOnMarketDataReject)(const char *MDReqID, const char *MDReqRejReason, const char *text);
   
   // ���յ�CMDS��ʵʱ�������ݵĻص�����
   typedef void (*pBCOnMRTDReceived)(const char *MDReqID, const char *excode, const char *symbol, const T_MDREAL *pMDRecord);
   
   // ���յ�CMDS�����׶�������������ʱ��Ļص����� - �ڷ��������ø������û���Ҫ��EOM��ʱ����MDCBookRTMarketData�Ĳ�ѯ���ĵ�ʱ��ÿ��sSymbolList�н��������յ�һ��EOM��Ϣ
   // ����sSymbolList�е�symbol���֣�������'?'/'*'��ʼ�ģ���û�����EOM��Ϣ
   typedef void (*pBCOnEndOfMDReflush)(const char *MDReqID, const char *excode, const char *symbol, time_t tmServer);
   
   // ���յ���MDCBookRTMarketData���ʵʱ�ֱʳɽ�����ʱ��Ļص�����
   typedef void (*pBCOnMRTDetailTrading)(const char *MDReqID, const char *excode, const char *symbol, const T_MDVARI *pMDVari, int nRecords);
   
   
   // ��ѯ�����������ݵĻص�������ע��: 
   
   // ��MDCRequestDetailTrading���յ�ָ�����ڵĻص�����:
   typedef void (*pBCOnHISDetailTrading)(const char *excode, const char *symbol, const T_MDVARI *pMDVari, int nRecords);
   
   // ��MDCRequestKChartData���յ�Kͼ���ݵĻص�����
   typedef void (*pBCOnKChartData)(const char *excode, const char *symbol, int kctype, const T_MDKCHART *pKChart, int nRecords);
   
   ///  2012/12/6 21:53:02 ��汾֧��:
   // ��MDCRequestTickData���յ�TICK����ʱ�Ļص�����
   typedef void (*pBCOnTickData)(const char *excode, const char *symbol, const T_MDTICK *pTicks, int nrecords);
   
#define MAXKCITEMS 2000
#define MAXVITEMS 200000
   


//////////////////////////////////////////////////////////////////////////
// ���沿���ǳ�ʼ�������������õĺ���:

   // ������: MDCInitialize
   // ���  : ������ 2012-2-7 15:39:44
   // ����  : ��ʼ��MDClient�Ľӿڣ�������־·��
   // ����  : bool   
   // ����  : const char *logpath [IN]: ��־·�������Ҫ��Ŀ¼�ָ�������ΪNULL�����ڵ�ǰĿ¼��
   BUPUB_API bool MDCInitialize(const char *logpath=NULL);


   // ������: MDCExit
   // ���  : ������ 2012-2-7 15:42:50
   // ����  : �˳�MDClient�ӿ�
   // ����  : void 
   BUPUB_API void MDCExit();

   // ������: MDCSetDebug
   // ���  : ������ 2012-1-21 22:58:10
   // ����  : ����MDClientģʽ�ڲ��ĵ���ģʽ
   // ����  : void 
   // ����  : int idebug [IN]: 0 - �رգ�1 - �򿪣��Ὣ���ͽ��յ���Ϣ��¼����־�У�
   BUPUB_API void MDCSetDebug(int idebug);


// ������: MDCSetSessionID
// ���  : ������ 2012-1-15 17:01:38
// ����  : ����MDClient��SessionID
// ����  : int : 0-ʧ�ܣ�1-�ɹ�
// ����  : const char *sessionid [IN]: SessionID����<localid>-<serverid>��ʾ������: "LINK3-KS"
BUPUB_API int MDCSetSessionID(const char *sessionid);

// ������: MDCSetHeartBtInt
// ���  : ������ 2012-1-15 17:16:47
// ����  : ����DMClient����������Խӵ�ʱ��Լ������������
// ����  : int : 0-ʧ�ܣ�1-�ɹ�
// ����  : unsigned int hbt [IN]: �������ʱ�䣬�����
// ����  : unsigned int hblatency [IN]: �Է��������Է����ļ��ʱ�䣬�����
BUPUB_API int MDCSetHeartBtInt(unsigned int hbt,unsigned int hblatency);

// ������: MDCSetServer
// ���  : ������ 2012-1-15 17:13:59
// ����  : ����MDClient�ĶԽӷ������˵����Ӳ���(IP:Port)
// ����  : int : 0-ʧ�ܣ�1-�ɹ�
// ����  : const char *svrip_port [IN]: ���������������IP��Port����ϲ�������<IP>:<Port>��ʾ������: "127.0.0.1:11200"
BUPUB_API int MDCSetServer(const char *svrip_port);

// ������: MDCSetLogonUser
// ���  : ������ 2012-1-20 15:14:51
// ����  : ���õ�¼���û����Ϳ���
// ����  : void 
// ����  : const char *userpassword [IN]: <UserName>,<Password> - �ö��ŷָ��û����Ϳ���
BUPUB_API void MDCSetLogonUser(const char *userpassword);



// Function name: MDCSetSockMode
// Author: CHENYH @ 2014/3/3 23:08:24
// Description: �����������ģʽ
// Return type: int 
// Argument   : int sockmode [IN]: ʹ��������յ�ģʽ��MDS_TCP,MDS_UDP,MDS_MBC /*=MDS_TCP*/
// Argument   : int ttl [IN]: MDS_TCP��Ч /*=-1*/
// Argument   : int loop [IN]: MDS_TCP��Ч /*=-1*/
// Argument   : int bUseMDSet [IN]: MDS_TCP��Ч /*=0*/
BUPUB_API int MDCSetSockMode(int sockmode/*=MDS_TCP*/, int ttl/*=-1*/, int loop/*=-1*/, int bUseMDSet/*=0*/);

// ������: MDCToConnectServer
// ���  : ������ 2012-1-20 14:45:34
// ����  : ��ʼ���ӷ��������������ɹ������������̨���ݽ����̡߳�
// ����  : int : 1-��ʾ�ɹ���¼��������������Կ�ʼ���������ˣ�0 - ʧ�ܡ���ϸ�ɲο���־
// ����  : const char *svrip_port [IN]: ����ΪNULL�������Ѿ����õĲ���������Ϊ���������������IP��Port����ϲ�������<IP>:<Port>��ʾ������: "127.0.0.1:11200"
// ����  : const char *userpass [IN]:����ΪNULL�������Ѿ����õĲ���������Ҫ��Ϊ: <UserName>,<Password> - �ö��ŷָ��û����Ϳ���
BUPUB_API int MDCToConnectServer(const char *svrip_port, const char *userpass);


// ������: MDCToDisconnect
// ���  : ������ 2012-1-21 0:32:17
// ����  : 
// ����  : void 
BUPUB_API void MDCToDisconnect();

//////////////////////////////////////////////////////////////////////////
// ����Ϊ��ѯ��ģ���ڲ�״̬:

// ������: MDCGetLinkStatus
// ���  : ������ 2012-1-18 19:31:41
// ����  : ��ȡ�������������״̬
// ����  : int : 0 - ���ڶϿ�״̬��1 - ������·�Ѿ�����������û�����LOGN֮ǰ��2 - LOGON��Ϣ������ϣ���ʼ������ʽ��MD���ݽ���״̬
BUPUB_API int MDCGetLinkStatus();

// ������: MDCGetBackRunStatus
// ���  : ������ 2012-1-20 14:45:56
// ����  : ��ȡ�Ƿ�������̨���ݽ����߳�״̬
// ����  : int : 0 - ֹͣ״̬��1 - ����״̬
BUPUB_API int MDCGetBackRunStatus();

//////////////////////////////////////////////////////////////////////////
// ����Ϊʵ�ʵ���CMDS֮�����Ϣ�������Եõ������������:

// ������: MDCRequestTag10031
// ���  : ������ 2012-1-20 22:14:24
// ����  : Ϊ�˴ӷ�������ȡ��״̬��ϢTAG10031���������һ����������
// ����  : int >0: ��ʾ��������ɹ������򣬱�ʾʧ��
BUPUB_API int MDCRequestTag10031(); 

// ������: MDCBookRTMarketData
// ���  : ������ 2012-1-20 23:59:48
// ����  : �����������ʵʱ��������; ��ȡ��ԭ���ĵ�����
// ����  : int : -202: �����cReqType; -201: ���ĺ��ظ�; <=0: ������Ϣʧ�ܣ�����ɹ�
// ����  : const char *sMDReqID [IN]: ���鶩�ĺţ����ں��������ݷ��غ�����ȡ��
// ����  : char cReqType [IN]: 
//          '0' = Snapshot;  �����õ�һ���������ݿ���
//          '1' = Snapshot + Updates (Subscribe);  ���պͺ����ĸ���(����)��ֱ��ȡ����Ͽ�����
//          '2' = Disable previous Snapshot + Update Request (Unsubscribe) ȡ��֮ǰ��ָ���Ķ���
//                ��Ϊ '2' ��ʱ������Ҫ������Щ �� �ˡ�
// ����  : int iMDType [IN]: ������������
//                0=����Ϊʵʱ��������; 
//                1=����ʵʱ�������ݣ�Ҳ����ʵʱ�ֱʳɽ�����; 
// ����  : const char *sSymbolList [IN]: �����б�����ʽΪ: "<����������>,<���ڲ�Ʒ���봮>;...;<����������>,<���ڲ�Ʒ���봮>" 
//          �ڽ��ڲ�Ʒ���봮�У�'?': ��Ϊ�ý������µ�����Ʒ�֣�
//                              '*XXX': ����'*'Ϊ��ʼ�ַ�������Ϊָ���ý������£�����ΪXXX��ʼ��Ʒ�֣�
//                              ������Ϊ����ָ����Ʒ�ִ��봮
BUPUB_API int MDCBookRTMarketData(const char *sMDReqID, char cReqType, int iMDType, const char *sSymbolList);


// Function name: MDCGetMDReqIDs
// Author       : CHENYH 2012/5/4 15:46:04
// Description  : ��ȡ�ӿ��У��Ѿ���Ч���ĵĶ��ĺ��б��嵥
// Return type  : int ���ص�ǰ���ĸ���
// Argument : char *MDReqIDList [OUT]: ���ڴ�Ŷ��ĺ��嵥�Ļ��棬�䷵�ظ�ʽΪ:'<���ĺ�>;...' 
// Argument : unsigned int MDReqIDLength [IN]: MDReqIDList������ֽڳ���
BUPUB_API int MDCGetMDReqIDs(char *MDReqIDList, unsigned int MDReqIDLength);


// ������: MDCRequestDetailTrading
// ���  : ������ 2012-1-22 13:23:31
// ����  : ��CMDS��ѯָ�����ڵķֱʳɽ����ݣ�Ϊ�˽�����������������MDCRegisterHisDetailTradingע����Ӧ�Ļص�����:
//         ���������ݽ��գ����ÿ���յ�����(��ȫ��)��¼��ÿ����಻���� 8000/sizeof(T_MDVARI)��
//          ���һ����Ч��Ϣ�е����һ����¼��tmsecΪ0����ʾ����������ע����Ӧ�Ļص�������
//         ��������������ݺ󣬾ܾ�������յ�һ��������������ܾ�����Ϣ�����ص�֮ǰע���
//                   OnMarketDataReject�����е�MDReqID="", MDReqRejReason="DTR", �;���ܾ�ԭ��
// ����  : int: >0 �ύ����ɹ�, ׼����������; <=0 �ύ����ʧ�ܣ��������ӳ��ֹ���
// ����  : const char *sExchCode [IN]: ָ���Ľ���������
// ����  : const char *sSymbol [IN]: ��ȷ�ĸý�������Ʒ�ִ���
// ����  : int iYYYYMMDD [IN]: ָ����ȡ�ֱʳɽ�������
BUPUB_API int MDCRequestDetailTrading(const char *sExchCode, const char *sSymbol, int iYYYYMMDD);


// ������: MDCRequestKChartData
// ���  : ������ 2012-1-24 14:50:54
// ����  : ���������ѯ��ȡKChart���ݣ�Ϊ�˽���������������ע����Ӧ�Ļص�����:
//         ���������ݽ��գ����ÿ���յ�����(��ȫ��)��¼��ÿ����಻���� 8000/sizeof(T_MDKCHART)��
//          ���һ����Ч��Ϣ�е����һ����¼��tmsecΪ0����ʾ����������ע����Ӧ�Ļص�������
//         ��������������ݺ󣬾ܾ�������յ�һ��������������ܾ�����Ϣ�����ص�֮ǰע���
//                   OnMarketDataReject�����е�MDReqID="", MDReqRejReason="KCR", �;���ܾ�ԭ��
// ����  : int: >0 �ύ����ɹ�, ׼����������; <=0 �ύ����ʧ�ܣ��������ӳ��ֹ���
// ����  : const char *excode [IN]: ָ���Ľ���������
// ����  : const char *symbol [IN]: ��ȷ�ĸý�������Ʒ�ִ���
// ����  : int kctype [IN]: Kͼ���ݵ�����: (����BASE_KCTYPE����)+(����)*10
// ����  : unsigned int tmsec [IN]: �ο�ʱ��㣬������ kctype=KCT_DAY��KCT_MINʱ����Ч; ����Ϊ�ο�����¼�е�ʱ���
// ����  : int nitems [IN]: ������Kͼ���ݼ�¼����0 ���շ������ڶ������ֵ
//                         ��kctype=KCT_DAY��KCT_MINʱ��nitems<0��Ϊָ��ʱ���֮ǰ�ļ�¼���֣�
//                                               nitems>0��Ϊָ��ʱ���֮��ļ�¼���֣�
BUPUB_API int MDCRequestKChartData(const char *excode, const char *symbol, int kctype, unsigned int tmsec, int nitems);


// Function name: MDCRegisterDynaUser
// Author       : CHENYH 2012/12/16 23:26:06
// Description  : CMD-DNSר�ú��������ڿ�ָͨ���û�����������
// return   : int: >0 �ύ����ɹ�, ׼����������; <=0 �ύ����ʧ�ܣ��������ӳ��ֹ���
// Argument : const char *partnerid [IN]: ָ�����û�����ʹ�õ����Ӵ���
// Argument : const char *username [IN]: ָ�����û��ĵ�¼�û���
// Argument : const char *password [IN]: ָ�����û��ĵ�¼����
// Argument : int refint [IN]: ���û�����ˢ��Ƶ�� msΪ��λ
// Argument : int iEnd [IN]: ����������ˢ��ʱ���Ƿ���Ҫ��һ�����������ʱ���ṩEndOfReflush��Ϣ����ʾ 0 - ����Ҫ
BUPUB_API int MDCRegisterDynaUser(const char *partnerid, const char *username, const char *password, int refint, int iEnd);



// Function name: MDCRegisterBCOnLogon
// Author       : CHENYH 2012/4/30 22:46:27
// Description  : ���������յ�Logon��Ϣʱ��Ļص�����
// Return type  : void 
// Argument : pBCOnLogon pFuncOnLogon [IN] : �ص���������ΪNULL�Թرջص�
BUPUB_API void MDCRegisterBCOnLogon(pBCOnLogon pFuncOnLogon);

// Function name: MDCRegisterBCOnHeartBeat
// Author       : CHENYH 2012/4/30 22:48:21
// Description  : ���������յ�HeartBeat��Ϣʱ��Ļص�����
// Return type  : void 
// Argument : pBCOnHeartBeat pFuncOnHeartBeat[IN] : �ص���������ΪNULL�Թرջص�
BUPUB_API void MDCRegisterBCOnHeartBeat(pBCOnHeartBeat pFuncOnHeartBeat);


// Function name: MDCRegisterBCOnLogout
// Author       : CHENYH 2012/4/30 22:58:06
// Description  : ���������յ�Logout��Ϣʱ��Ļص�����
// Return type  : void 
// Argument : pBCOnLogout pFuncOnLogout [IN] : �ص���������ΪNULL�Թرջص�
BUPUB_API void MDCRegisterBCOnLogout(pBCOnLogout pFuncOnLogout);


// Function name: MDCRegisterBCOnMarketDataReject
// Author       : CHENYH 2012/4/30 22:58:09
// Description  : ���������յ�MarketDataReject��Ϣʱ��Ļص�����
// Return type  : void 
// Argument : pBCOnMarketDataReject pFuncOnMarketDataReject [IN] : �ص���������ΪNULL�Թرջص�
BUPUB_API void MDCRegisterBCOnMarketDataReject(pBCOnMarketDataReject pFuncOnMarketDataReject);


// Function name: MDCRegisterBCOnMRTDReceived
// Author       : CHENYH 2012/4/30 22:58:13
// Description  : ���������յ�MRTDReceived(ʵʱ��������)��Ϣʱ��Ļص�����
// Return type  : void 
// Argument : pBCOnMRTDReceived pFuncOnMRTDReceived [IN] : �ص���������ΪNULL�Թرջص�
BUPUB_API void MDCRegisterBCOnMRTDReceived(pBCOnMRTDReceived pFuncOnMRTDReceived);


// Function name: MDCRegisterBCOnEndOfMDReflush
// Author       : CHENYH 2012/4/30 22:58:17
// Description  : ���������յ�EndOfMDReflush(ͨ�䷽ʽ��ѯ�����������ݽ������)��Ϣʱ��Ļص�����
// Return type  : void 
// Argument : pBCOnEndOfMDReflush pFuncOnEndOfMDReflush [IN] : �ص���������ΪNULL�Թرջص�
BUPUB_API void MDCRegisterBCOnEndOfMDReflush(pBCOnEndOfMDReflush pFuncOnEndOfMDReflush);


// Function name: MDCRegisterBCOnMRTDetailTrading
// Author       : CHENYH 2012/5/1 21:21:34
// Description  : ���������յ����շֱʳɽ�����MRTDetailTrading��Ϣʱ��Ļص�����
// Return type  : void 
// Argument : pBCOnMRTDetailTrading pFuncOnMRTDetailTrading [IN] : �ص���������ΪNULL�Թرջص�
BUPUB_API void MDCRegisterBCOnMRTDetailTrading(pBCOnMRTDetailTrading pFuncOnMRTDetailTrading);

//////////////////////////////////////////////////////////////////////////
// ������һ����ע��������Щ�ص������Ľӿ�:
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


// ������: MDCRegisterCallBackFunctions
// ���  : ������ 2012-1-20 22:26:20
// ����  : ��һ����ע��һЩ�ص�����
// ����  : void 
// ����  : T_MDCCALLBACKFUNCS *pCallBackFunc [IN]: �ص������ļ��ϣ��Բ����ĵ��¼���������ΪNULL
BUPUB_API void MDCRegisterCallBackFunctions(T_MDCCALLBACKFUNCS *pCallBackFunc);
//////////////////////////////////////////////////////////////////////////


// ������: MDCRegisterHisDetailTrading
// ���  : ������ 2012-1-24 14:50:54
// ����  : ע��/ע�� ��ʷ�շֱʳɽ����ݻص�����
// ����  : void 
// ����  : pBCOnHISDetailTrading pFuncOnHisDetailTrading [IN]: ע��Ļص�������NULL: ��ע��
BUPUB_API void MDCRegisterHisDetailTrading(pBCOnHISDetailTrading pFuncOnHisDetailTrading);

// ������: MDCRegisterKChartData
// ���  : ������ 2012-1-24 14:50:54
// ����  : ע��/ע�� Kͼ���ݽ��ջص�����
// ����  : void 
// ����  : pBCOnKChartData pFuncOnKChartData [IN]: ע��Ļص�������NULL: ��ע��
BUPUB_API void MDCRegisterKChartData(pBCOnKChartData pFuncOnKChartData);


// Function name: MDCRegisterTickData
// Author       : CHENYH 2012/12/6 21:59:35
// Description  : ע��/ע�� TICK���ݽ��յĻص����������첽����ʽ��ʹ��
// Return type  : BUPUB_API void 
// Argument : pBCOnTickData pFuncOnTickData [IN]: ע��Ļص�������NULL: ��ע��
BUPUB_API void MDCRegisterTickData(pBCOnTickData pFuncOnTickData);
//////////////////////////////////////////////////////////////////////////
// ����Ϊͬ�����ù��ܺ���:
// Function name: MDCSyncModeSet
// Author       : CHENYH 2012/4/28 11:05:36
// Description  :  ����ͬ������ģʽ�������ڳɹ���CMDSIII���ӳɹ����п��ܳɹ���
// Return type		: int ==0: �ɹ�����ͬ������ģʽ; <0: ͬ����������ʧ��
// Argument         : const char *sExchCodeList[IN]: ��';'�ָ����г������б�: ��"XSHE;XSHG"
BUPUB_API int MDCSyncModeSet(const char *sExchCodeList);


// Function name: MDCSyncMarketData
// Author      : CHENYH 2012/4/28 16:27:08
// Description : ��ͬ��ģʽ��(���Ѿ�������MDCSyncModeSet��)��ͬ����ȡָ��������Ʒ��������Ϣ����
// Return type	: int ���سɹ���ȡ�ļ�¼����<=0: ʧ��
// Argument : const char *excode [IN]: ָ���Ľ��������룬�ο�MDCSyncModeSet�е�sExchCodeList������ֵ
// Argument : const char *symbol [IN]: ָ���Ľ�����Ʒ���룬NULL ��ȡȫ������������nMaxMDs�趨��
// Argument : T_MDREAL *pMDBuf [OUT]: ��ȡ�������ݵĴ�Ż���
// Argument : int nMaxMDs [IN]: pMDBuf���ɴ�ŵ������¼��
BUPUB_API int MDCSyncMarketData(const char *excode, const char *symbol, T_MDREAL *pMDBuf, int nMaxMDs);

// !!!! ��������ͬ�����ú�����ʹ���첽ģʽ��Ҳ��ʹ�ã������ڽ����д�������Ϣ���գ���Ӱ��֮ǰ���첽������Ϣ��ʱ��
// Function name: MDCSyncKChartData
// Author       : CHENYH 2012/4/29 21:49:06
// Description  : ���������ѯ��ȡKChart����
// ����  : int: >0 ���ر��β�ѯ���ļ�¼��; <=0 �ύ����ʧ�ܣ��������ӵȹ���
// ����  : const char *excode [IN]: ָ���Ľ���������
// ����  : const char *symbol [IN]: ��ȷ�ĸý�������Ʒ�ִ���
// ����  : int kctype [IN]: Kͼ���ݵ�����: (����BASE_KCTYPE����)+(����)*10
// ����  : unsigned int tmsec [IN]: �ο�ʱ��㣬������ kctype=KCT_DAYʱ����Ч; ����Ϊ�ο�����¼�е�ʱ���
// ����  : int nitems [IN]: ������Kͼ���ݼ�¼����
// Argument : T_MDKCHART *pKCItems [OUT]: ���ڴ�ŵ�Kͼ���ݼ�¼�Ļ��档
// Argument : int waitms [IN]: ���ȴ�ʱ��ms��<=0: һֱ�ȴ���ֱ�����ִ�������ݽ�������Ϊֹ
// Argument : char *errmsg256 [OUT]: ������<=0��ʱ���ṩ�Ĵ�����Ϣ����������Ҫ��256�ֽڳ��ȵĻ���
BUPUB_API int MDCSyncKChartData(const char *excode, const char *symbol, int kctype, unsigned int tmsec, 
                                int nitems, T_MDKCHART *pKCItems, int waitms, char *errmsg256);

// Function name: MDCSyncDetailTrading
// Author       : CHENYH 2012/4/30 9:11:05
// Description  : ��CMDS��ѯָ�����ڵķֱʳɽ�����
// ����  : int: >0 ����ɹ������ز�ѯ�������ݼ�¼��; <=0 ��ѯʧ�ܣ��������
// ����  : const char *sExchCode [IN]: ָ���Ľ���������
// ����  : const char *sSymbol [IN]: ��ȷ�ĸý�������Ʒ�ִ���
// ����  : int iYYYYMMDD [IN]: ָ����ȡ�ֱʳɽ�������
// Argument : DONEVOLUME fromDoneVolume [IN]: ��ȡָ�����ڵĳɽ������ڱ�ָ��ֵ������ʼΪ0
// Argument : int nitems [IN]: pItems���ɴ�ŷֱʳɽ����ݼ�¼��
// Argument : T_MDVARI *pItems [OUT]: ��ŷֱʳɽ����ݼ�¼�Ļ���
// Argument : int waitms [IN]: ���ȴ�ʱ��ms��<=0: һֱ�ȴ���ֱ�����ִ�������ݽ�������Ϊֹ
// Argument : char *errmsg256 [OUT]: ������<=0��ʱ���ṩ�Ĵ�����Ϣ����������Ҫ��256�ֽڳ��ȵĻ���
BUPUB_API int MDCSyncDetailTrading(const char *sExchCode, const char *sSymbol, int iYYYYMMDD, 
                         DONEVOLUME fromDoneVolume, int nitems, T_MDVARI *pItems, int waitms, char *errmsg256);



//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif

#undef BUPUB_API


#endif // MDC_h__