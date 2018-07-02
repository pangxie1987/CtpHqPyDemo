/********************************************************************
	created:	2011/11/16
	created:	16:11:2011   21:16
	filename: 	D:\projects\FIXII\2编码\CMDServer\buinc\MDBase.h
	file path:	D:\projects\FIXII\2编码\CMDServer\buinc
	file base:	MDBase
	file ext:	h
	author:		CHENYH
	
	purpose:	这是各类以结构化存取行情数据的基本结构
*********************************************************************/
#ifndef _MDBASE_H
#define _MDBASE_H

//#define MDP_TICK  "tick"   // 行情更新数据存放目录
#define MDF_VARI  "xtk"    // 行情变动数据存放目录
#define MDP_MIN   "mmd"    // 分钟数据存放目录
#define MDP_DAY   "dmd"
// #define MDP_WEEK  "wmd"
// #define MDP_MONTH "mon"


/* ****** Updated by CHENYH at 2012-1-14 11:56:54 ****** 
// 由于常规使用过程中，一般都以终止时间为基准，向前后退的K图单元数，或者以K图单元区间获取数据
// 而不是用时间区间；因此，不再采用这种数据结构
#define MAXKCDS   1460   // 24*60 为最大K图数据块单元数
struct T_KCINDEX
{
   unsigned int tm0;    // 本段数据的起始时间
   unsigned int tm1;    // 本段数据的结束时间
   unsigned int kcds;   // 本段K图数据块个数
   unsigned int next;   // 下一个索引块偏移位置
};
********************************************************/

enum BASE_KCTYPE
{
   KCT_MIN=0,
   KCT_DAY=1,
   KCT_WEEK=2,
   KCT_MONTH=3,
   KCT_YEAR=4
   // 其他，则kctype/10表示单位倍数；kctype%10即为K图基本单位，只能在上述这几个内，即 分钟、日、周、月、年这几个
};


#ifdef __cplusplus
extern "C" {
#endif

   /* 以下代码是用来字节对齐使用的 */
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
   
   //行情类型
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

   typedef int8 DONEVOLUME;   // 直接用int8, 但需要将各个历史文件的格式进行转换
   typedef unsigned int LVOLUNIT;   // 量的小单位，绝对不会超过42亿的
   typedef float PRICEUNIT;   // 为了能够减少存储和传输数据长度，并且实际行情中，有7位精度基本上已经足够了
   typedef double AMTUNIT;    // 金额单位


#define MDREALVERSION 20130921  // sizeof(_T_MDREAL)=414
typedef struct _T_MDREAL  // sizeof(_T_MDREAL)=414
{
   _T_MDREAL() { memset(this,0,sizeof(_T_MDREAL)); }
   // 除了MDS内部维护标注的字段外，其他都有MDUnit来提供
   int		hqtype;								//行情类型: 参考 HQTYPES
   unsigned int upd_serial;					//行情更新序号，对应：序号 由MDUnit维护
   int		upd_date;							//行情日期,格式：YYYYMMDD  由MDUnit更新 为行情原地日期
   unsigned int cmdstime;						//CMDS内部维护的更新的时间点time_t
   int		pre_upd_serial;					//CMDS内部维护的更新时的序号，这个才是CMDS内部的Tick更新序号
   int	   inttmus;		                  //内部用于记录时间原始发送方的时间点，用us为单位，用于性能检测
   
   AMTUNIT	floatissued;						//流通股本
   AMTUNIT  totalissued;						//总股本
   
   char	exchCode[MAX_EXCHCODE_LEN];			//交易所代码
   char	varity_code[MAX_VARI_LEN];			//品种代码
   char	varity_name[MAX_VARI_LEN];			//品种名称(证券名称)
   char	deliv_date[MAX_DATE_LEN];			//交割期
   char	chgStatus[MAX_QUOTA_STATUS_LEN];	//对应：状态
   //1-2bit表示：买入;3-4bit表示：卖出;
   //5-6bit表示：最新;7-8bit不用;
   //00->新行情    01->低于以前的行情
   //11->高于以前的行情    00->与以前相平
   
   PRICEUNIT	openPrice;					//开盘价
   PRICEUNIT	lastPrice;					//最新价
   PRICEUNIT	highestPrice;				//最高价
   PRICEUNIT	lowestPrice;				//最低价
   DONEVOLUME	doneVolume;					//成交量
   PRICEUNIT	chgPrice;					//涨跌
   PRICEUNIT	upperLimitPrice;			//涨停板
   PRICEUNIT	lowerLimitPrice;			//跌停板
   PRICEUNIT	hisHighestPrice;			//历史最高价
   PRICEUNIT	hisLowestPrice;				//历史最低价
   DONEVOLUME	openInterest;				//净持仓
   PRICEUNIT	preSettlePrice;				//昨日结算
   PRICEUNIT	preClosePrice;				//昨日收盘
   PRICEUNIT	settlePrice;				//今日结算
   AMTUNIT	turnover;					//成交金额
   DONEVOLUME	preOpenInterest;			//昨日持仓量
   PRICEUNIT	closePrice;					//今日收盘
   PRICEUNIT	preDelta;					//昨虚实度
   PRICEUNIT	currDelta;					//今虚实度
   
   unsigned int tickcount;						//累计成交笔数
   PRICEUNIT avgbuyprice;					//加权平均买价
   LVOLUNIT totlebuy;						//买总量
   PRICEUNIT avgsellprice;				//加权平均卖价
   LVOLUNIT totlesell;						//卖总量
   
   PRICEUNIT	bidPrice[MAX_LEVEL_HQ];					//买入价1-10
   LVOLUNIT	bidVolume[MAX_LEVEL_HQ];					//买入量1-10
   PRICEUNIT	askPrice[MAX_LEVEL_HQ];					//卖出价1-10
   LVOLUNIT	askVolume[MAX_LEVEL_HQ];					//卖出量1-10
   
   char cmbtype[MAX_HQTYPE_LEN];		//‘0’或'\0':普通行情;‘1’：组合套利行情
   LVOLUNIT derive_bidlot;					//买推导量
   LVOLUNIT derive_asklot;					//卖推导量
   PRICEUNIT syl;							//市盈率
   PRICEUNIT iopv;						//iopv值  ETF的基金份额参考净值
   int tpstatus;						//停牌标记(0 正常; 1 暂停交易; 2 停止交易)
} __PACKED__ T_MDREAL;   // 实时行情数据结构

#define GetSymbol(dest, phq)  {strcpy(dest,(phq)->varity_code); if ((phq)->deliv_date[0]) strcat(dest,(phq)->deliv_date); }  
//sprintf(dest, "%s%s", (phq)->varity_code,(phq)->deliv_date)


#define INVALIDTMSEC 0
#define MAXPRICE     9999999.99
//#define MINPRICE     -100.00 --2013/10/15 22:01:49
#define MINPRICE     0.0001

typedef struct _T_INDEXPOS
{
   unsigned short ups;  // 指数中的上涨数
   unsigned short dns;  // 指数中的下跌数
} __PACKED__ T_INDEXPOS;

typedef struct _T_MDVARI  // sizeof(T_MDVARI)=36
{
   unsigned int tmsec;  // 目前采用time(NULL)的值来表示即可，至少可以表示到2038年或者2100年
   PRICEUNIT lastPrice;  // 最新成交价，或指数，或...
   PRICEUNIT bidPrice;  // 买价1
   PRICEUNIT askPrice;  // 卖价1
   DONEVOLUME doneVolume;  // 累计成交量
   AMTUNIT turnover;  // 累计成交金额
   unsigned int  Position;  // 基金净持仓；股票市场的成交笔数   
} __PACKED__ T_MDVARI;  // 行情实时变动记录结构 36字节（若用double来代替float等，则似乎要56，且精度在实时走势中显得没有那么重要）

typedef struct _T_MDKCHART  // sizeof(T_MDKCHART)=44
{
   unsigned int tmsec;  // 目前采用time(NULL)的值来表示即可，至少可以表示到2038年或者2100年
   PRICEUNIT    openPrice;  // 开盘价   
   PRICEUNIT    closePrice; // 收盘价(最后价)
   PRICEUNIT    maxPrice;   // 最高价
   PRICEUNIT    minPrice;   // 最低价
   DONEVOLUME doneVolume;   // 成交量
   AMTUNIT turnover;  // 成交金额
   PRICEUNIT    prePrice; // 前收盘价 (或结算价)
   unsigned int  Position;  // 累计净持仓或当日开始的成交笔数
} __PACKED__ T_MDKCHART;   // 44字节 一个时间区间的行情数据统计，可以为1、5、15、30、60、日、周、月

typedef struct _T_MDTICK  // sizeof(T_MDTICK)=184
{
   unsigned int tmsec;  // 目前采用time(NULL)的值来表示即可，至少可以表示到2038年或者2100年
   PRICEUNIT	lastPrice;					//最新价
   DONEVOLUME	doneVolume;					//成交量
   AMTUNIT	turnover;					//成交金额
   PRICEUNIT	bidPrice[MAX_LEVEL_HQ];					//买入价1-10
   LVOLUNIT	bidVolume[MAX_LEVEL_HQ];					//买入量1-10
   PRICEUNIT	askPrice[MAX_LEVEL_HQ];					//卖出价1-10
   LVOLUNIT	askVolume[MAX_LEVEL_HQ];					//卖出量1-10
} __PACKED__ T_MDTICK;  // TICK - 每次行情关键数据（记录变更）

typedef struct _T_FMDVARI // sizeof(T_FMDVARI)=188
{
   unsigned int tmsec;  // 目前采用time(NULL)的值来表示即可，至少可以表示到2038年或者2100年
   PRICEUNIT	lastPrice;					//最新价 当doneVolume<=0时为preClosePrice / preSettlePrice
   DONEVOLUME	doneVolume;					//成交量
   AMTUNIT	turnover;					//成交金额
   PRICEUNIT	bidPrice[MAX_LEVEL_HQ];					//买入价1-10
   LVOLUNIT	bidVolume[MAX_LEVEL_HQ];					//买入量1-10
   PRICEUNIT	askPrice[MAX_LEVEL_HQ];					//卖出价1-10
   LVOLUNIT	askVolume[MAX_LEVEL_HQ];					//卖出量1-10
   unsigned int  Position;  // 基金净持仓；股票市场的成交笔数   
} __PACKED__ T_FMDVARI; // 存储成文件xtk文件的分笔成交数据



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

