/********************************************************************
	created:	2011/11/16
	created:	16:11:2011   21:16
	filename: 	D:\projects\FIXII\2����\CMDServer\buinc\MDBase.h
	file path:	D:\projects\FIXII\2����\CMDServer\buinc
	file base:	MDBase
	file ext:	h
	author:		CHENYH
	
	purpose:	���Ǹ����Խṹ����ȡ�������ݵĻ����ṹ
*********************************************************************/
#ifndef _MDBASE_H
#define _MDBASE_H

//#define MDP_TICK  "tick"   // ����������ݴ��Ŀ¼
#define MDF_VARI  "xtk"    // ����䶯���ݴ��Ŀ¼
#define MDP_MIN   "mmd"    // �������ݴ��Ŀ¼
#define MDP_DAY   "dmd"
// #define MDP_WEEK  "wmd"
// #define MDP_MONTH "mon"


/* ****** Updated by CHENYH at 2012-1-14 11:56:54 ****** 
// ���ڳ���ʹ�ù����У�һ�㶼����ֹʱ��Ϊ��׼����ǰ���˵�Kͼ��Ԫ����������Kͼ��Ԫ�����ȡ����
// ��������ʱ�����䣻��ˣ����ٲ����������ݽṹ
#define MAXKCDS   1460   // 24*60 Ϊ���Kͼ���ݿ鵥Ԫ��
struct T_KCINDEX
{
   unsigned int tm0;    // �������ݵ���ʼʱ��
   unsigned int tm1;    // �������ݵĽ���ʱ��
   unsigned int kcds;   // ����Kͼ���ݿ����
   unsigned int next;   // ��һ��������ƫ��λ��
};
********************************************************/

enum BASE_KCTYPE
{
   KCT_MIN=0,
   KCT_DAY=1,
   KCT_WEEK=2,
   KCT_MONTH=3,
   KCT_YEAR=4
   // ��������kctype/10��ʾ��λ������kctype%10��ΪKͼ������λ��ֻ���������⼸���ڣ��� ���ӡ��ա��ܡ��¡����⼸��
};


#ifdef __cplusplus
extern "C" {
#endif

   /* ���´����������ֽڶ���ʹ�õ� */
#ifdef _MSC_VER
#define __PACKED__ 
#pragma pack(push,1)
#else
#ifdef __GNUC__
#define __PACKED__	__attribute__ ((packed))
   //#pragma pack(push,1)
#else
#ifdef HP_UX
#define __PACKED__
#pragma pack 1
#else
#define __PACKED__
#pragma options align=packed
#endif
#endif
#endif
   
#ifndef int8
#ifdef _MSC_VER
#define int8 __int64
#else 
#define int8 long long
#endif
#endif

#ifndef MDCONST_DEF
#define MDCONST_DEF
   const int MAX_QUOTA_STATUS_LEN = 2;
   const int MAX_DATE_LEN = 9;
   const int MAX_EXCHCODE_LEN = 7;
   const int MAX_VARI_LEN = 31;
   const int MAX_HQTYPE_LEN = 2;
   const int MAX_LEVEL_HQ = 10;
   
   //��������
   enum HQTYPES
   {
      INDEX=0,
      STOCK,
      FUND,
      BOND,
      OTHER_STOCK,
      OPTION,
      EXCHANGE,
      FUTURE,
      FTR_IDX,
      RGZ,
      ETF,
      LOF,
      COV_BOND,
      TRUST,
      WARRANT,
      REPO,
      COMM
   };
#endif

   typedef int8 DONEVOLUME;   // ֱ����int8, ����Ҫ��������ʷ�ļ��ĸ�ʽ����ת��
   typedef unsigned int LVOLUNIT;   // ����С��λ�����Բ��ᳬ��42�ڵ�
   typedef float PRICEUNIT;   // Ϊ���ܹ����ٴ洢�ʹ������ݳ��ȣ�����ʵ�������У���7λ���Ȼ������Ѿ��㹻��
   typedef double AMTUNIT;    // ��λ


#define MDREALVERSION 20130921  // sizeof(_T_MDREAL)=414
typedef struct _T_MDREAL  // sizeof(_T_MDREAL)=414
{
   _T_MDREAL() { memset(this,0,sizeof(_T_MDREAL)); }
   // ����MDS�ڲ�ά����ע���ֶ��⣬��������MDUnit���ṩ
   int		hqtype;								//��������: �ο� HQTYPES
   unsigned int upd_serial;					//���������ţ���Ӧ����� ��MDUnitά��
   int		upd_date;							//��������,��ʽ��YYYYMMDD  ��MDUnit���� Ϊ����ԭ������
   unsigned int cmdstime;						//CMDS�ڲ�ά���ĸ��µ�ʱ���time_t
   int		pre_upd_serial;					//CMDS�ڲ�ά���ĸ���ʱ����ţ��������CMDS�ڲ���Tick�������
   int	   inttmus;		                  //�ڲ����ڼ�¼ʱ��ԭʼ���ͷ���ʱ��㣬��usΪ��λ���������ܼ��
   
   AMTUNIT	floatissued;						//��ͨ�ɱ�
   AMTUNIT  totalissued;						//�ܹɱ�
   
   char	exchCode[MAX_EXCHCODE_LEN];			//����������
   char	varity_code[MAX_VARI_LEN];			//Ʒ�ִ���
   char	varity_name[MAX_VARI_LEN];			//Ʒ������(֤ȯ����)
   char	deliv_date[MAX_DATE_LEN];			//������
   char	chgStatus[MAX_QUOTA_STATUS_LEN];	//��Ӧ��״̬
   //1-2bit��ʾ������;3-4bit��ʾ������;
   //5-6bit��ʾ������;7-8bit����;
   //00->������    01->������ǰ������
   //11->������ǰ������    00->����ǰ��ƽ
   
   PRICEUNIT	openPrice;					//���̼�
   PRICEUNIT	lastPrice;					//���¼�
   PRICEUNIT	highestPrice;				//��߼�
   PRICEUNIT	lowestPrice;				//��ͼ�
   DONEVOLUME	doneVolume;					//�ɽ���
   PRICEUNIT	chgPrice;					//�ǵ�
   PRICEUNIT	upperLimitPrice;			//��ͣ��
   PRICEUNIT	lowerLimitPrice;			//��ͣ��
   PRICEUNIT	hisHighestPrice;			//��ʷ��߼�
   PRICEUNIT	hisLowestPrice;				//��ʷ��ͼ�
   DONEVOLUME	openInterest;				//���ֲ�
   PRICEUNIT	preSettlePrice;				//���ս���
   PRICEUNIT	preClosePrice;				//��������
   PRICEUNIT	settlePrice;				//���ս���
   AMTUNIT	turnover;					//�ɽ����
   DONEVOLUME	preOpenInterest;			//���ճֲ���
   PRICEUNIT	closePrice;					//��������
   PRICEUNIT	preDelta;					//����ʵ��
   PRICEUNIT	currDelta;					//����ʵ��
   
   unsigned int tickcount;						//�ۼƳɽ�����
   PRICEUNIT avgbuyprice;					//��Ȩƽ�����
   LVOLUNIT totlebuy;						//������
   PRICEUNIT avgsellprice;				//��Ȩƽ������
   LVOLUNIT totlesell;						//������
   
   PRICEUNIT	bidPrice[MAX_LEVEL_HQ];					//�����1-10
   LVOLUNIT	bidVolume[MAX_LEVEL_HQ];					//������1-10
   PRICEUNIT	askPrice[MAX_LEVEL_HQ];					//������1-10
   LVOLUNIT	askVolume[MAX_LEVEL_HQ];					//������1-10
   
   char cmbtype[MAX_HQTYPE_LEN];		//��0����'\0':��ͨ����;��1���������������
   LVOLUNIT derive_bidlot;					//���Ƶ���
   LVOLUNIT derive_asklot;					//���Ƶ���
   PRICEUNIT syl;							//��ӯ��
   PRICEUNIT iopv;						//iopvֵ  ETF�Ļ���ݶ�ο���ֵ
   int tpstatus;						//ͣ�Ʊ��(0 ����; 1 ��ͣ����; 2 ֹͣ����)
} __PACKED__ T_MDREAL;   // ʵʱ�������ݽṹ

#define GetSymbol(dest, phq)  {strcpy(dest,(phq)->varity_code); if ((phq)->deliv_date[0]) strcat(dest,(phq)->deliv_date); }  
//sprintf(dest, "%s%s", (phq)->varity_code,(phq)->deliv_date)


#define INVALIDTMSEC 0
#define MAXPRICE     9999999.99
//#define MINPRICE     -100.00 --2013/10/15 22:01:49
#define MINPRICE     0.0001

typedef struct _T_INDEXPOS
{
   unsigned short ups;  // ָ���е�������
   unsigned short dns;  // ָ���е��µ���
} __PACKED__ T_INDEXPOS;

typedef struct _T_MDVARI  // sizeof(T_MDVARI)=36
{
   unsigned int tmsec;  // Ŀǰ����time(NULL)��ֵ����ʾ���ɣ����ٿ��Ա�ʾ��2038�����2100��
   PRICEUNIT lastPrice;  // ���³ɽ��ۣ���ָ������...
   PRICEUNIT bidPrice;  // ���1
   PRICEUNIT askPrice;  // ����1
   DONEVOLUME doneVolume;  // �ۼƳɽ���
   AMTUNIT turnover;  // �ۼƳɽ����
   unsigned int  Position;  // ���𾻳ֲ֣���Ʊ�г��ĳɽ�����   
} __PACKED__ T_MDVARI;  // ����ʵʱ�䶯��¼�ṹ 36�ֽڣ�����double������float�ȣ����ƺ�Ҫ56���Ҿ�����ʵʱ�������Ե�û����ô��Ҫ��

typedef struct _T_MDKCHART  // sizeof(T_MDKCHART)=44
{
   unsigned int tmsec;  // Ŀǰ����time(NULL)��ֵ����ʾ���ɣ����ٿ��Ա�ʾ��2038�����2100��
   PRICEUNIT    openPrice;  // ���̼�   
   PRICEUNIT    closePrice; // ���̼�(����)
   PRICEUNIT    maxPrice;   // ��߼�
   PRICEUNIT    minPrice;   // ��ͼ�
   DONEVOLUME doneVolume;   // �ɽ���
   AMTUNIT turnover;  // �ɽ����
   PRICEUNIT    prePrice; // ǰ���̼� (������)
   unsigned int  Position;  // �ۼƾ��ֲֻ��տ�ʼ�ĳɽ�����
} __PACKED__ T_MDKCHART;   // 44�ֽ� һ��ʱ���������������ͳ�ƣ�����Ϊ1��5��15��30��60���ա��ܡ���

typedef struct _T_MDTICK  // sizeof(T_MDTICK)=184
{
   unsigned int tmsec;  // Ŀǰ����time(NULL)��ֵ����ʾ���ɣ����ٿ��Ա�ʾ��2038�����2100��
   PRICEUNIT	lastPrice;					//���¼�
   DONEVOLUME	doneVolume;					//�ɽ���
   AMTUNIT	turnover;					//�ɽ����
   PRICEUNIT	bidPrice[MAX_LEVEL_HQ];					//�����1-10
   LVOLUNIT	bidVolume[MAX_LEVEL_HQ];					//������1-10
   PRICEUNIT	askPrice[MAX_LEVEL_HQ];					//������1-10
   LVOLUNIT	askVolume[MAX_LEVEL_HQ];					//������1-10
} __PACKED__ T_MDTICK;  // TICK - ÿ������ؼ����ݣ���¼�����

typedef struct _T_FMDVARI // sizeof(T_FMDVARI)=188
{
   unsigned int tmsec;  // Ŀǰ����time(NULL)��ֵ����ʾ���ɣ����ٿ��Ա�ʾ��2038�����2100��
   PRICEUNIT	lastPrice;					//���¼� ��doneVolume<=0ʱΪpreClosePrice / preSettlePrice
   DONEVOLUME	doneVolume;					//�ɽ���
   AMTUNIT	turnover;					//�ɽ����
   PRICEUNIT	bidPrice[MAX_LEVEL_HQ];					//�����1-10
   LVOLUNIT	bidVolume[MAX_LEVEL_HQ];					//������1-10
   PRICEUNIT	askPrice[MAX_LEVEL_HQ];					//������1-10
   LVOLUNIT	askVolume[MAX_LEVEL_HQ];					//������1-10
   unsigned int  Position;  // ���𾻳ֲ֣���Ʊ�г��ĳɽ�����   
} __PACKED__ T_FMDVARI; // �洢���ļ�xtk�ļ��ķֱʳɽ�����



#ifdef _MSC_VER
#pragma pack(pop)
#else
#ifdef __GNUC__
#else
#ifdef HP_UX
#pragma pack 0
#else
#pragma options align=reset
#endif
#endif
#endif
#undef __PACKED__
   
#ifdef __cplusplus
}
#endif


#endif // MDBase_h

