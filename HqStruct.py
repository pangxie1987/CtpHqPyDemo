#-*-coding:utf-8-*-
from  ctypes import *
import Hqdtype
from Hqdtype import *

#实时行情数据结构体
class _T_MDREAL(Structure):
    _fields_ =[('hqtype',c_int),
               ('upd_serial',c_uint),
               ('upd_date',c_int),
               ('cmdstime',c_uint),
               ('pre_upd_serial',c_int),
               ('inttmus',c_int),
               ('floatissued',AMTUNIT),
               ('totalissued',AMTUNIT),
               ('exchCode',c_char*7),
               ('varity_code',c_char*31),
               ('varity_name',c_char*31),
               ('deliv_date',c_char*9),
               ('chgStatus',c_char*2),
               ('openPrice',PRICEUNIT),
               ('lastPrice',PRICEUNIT),
               ('highestPrice',PRICEUNIT),
               ('lowestPrice',PRICEUNIT),
               ('doneVolume',DONEVOLUME),
               ('chgPrice',PRICEUNIT),
               ('upperLimitPrice',PRICEUNIT),
               ('lowerLimitPrice',PRICEUNIT),
               ('hisHighestPrice',PRICEUNIT),
               ('hisLowestPrice',PRICEUNIT),
               ('openInterest',DONEVOLUME),
               ('preSettlePrice',PRICEUNIT),
               ('preClosePrice',PRICEUNIT),
               ('settlePrice',PRICEUNIT),
               ('turnover',AMTUNIT),
               ('preOpenInterest',DONEVOLUME),
               ('closePrice',PRICEUNIT),
               ('preDelta',PRICEUNIT),
               ('currDelta',PRICEUNIT),
               ('tickcount',c_uint),
               ( 'avgbuyprice',PRICEUNIT),
               ('totlebuy',LVOLUNIT),
               ('avgsellprice',PRICEUNIT),
               ('totlesell',LVOLUNIT),
               ('bidPrice',c_float*10),
               ('bidVolume',c_uint*10),
               ('askPrice',c_float*10),
               ('askVolume',c_uint*10),
               ('cmbtype',c_char*2),
               ('derive_bidlot',LVOLUNIT),
               ('derive_asklot',LVOLUNIT),
               ('syl',PRICEUNIT),
               ('iopv',PRICEUNIT),
               ('tpstatus',c_int)
               ]
    _pack_ = 1
tT_MDREAL=POINTER(_T_MDREAL)
T_MDREAL=_T_MDREAL

#
# #注册回调行数
# #下面是一次性注册上述这些回调函数的接口:
# pBCOnLogon=CFUNCTYPE(c_int)
# pBCOnLogout=CFUNCTYPE(c_char)
# pBCOnHeartBeat=CFUNCTYPE(c_int)
# pBCOnMarketDataReject=CFUNCTYPE(c_int)
# pBCOnMRTDReceived=CFUNCTYPE(c_int)
# pBCOnEndOfMDReflush=CFUNCTYPE(c_int)
# pBCOnMRTDetailTrading=CFUNCTYPE(c_int)
# #回调函数集合
# class _T_MDCCALLBACKFUNCS(Structure):
#     _fields_=[('pFuncOnLogon',pBCOnLogon),
#               ('pFuncOnHeartBeat',pBCOnHeartBeat),
#               ('pFuncOnLogout',pBCOnLogout),
#               ('pFuncOnMarketDataReject',pBCOnMarketDataReject),
#               ('pFuncOnMRTDReceived',pBCOnMRTDReceived),
#               ('pFuncOnEndOfMDRefush',pBCOnEndOfMDReflush),
#               ('pFuncOnMRTDetailTrading',pBCOnMRTDetailTrading)]
#     _pack_ = 4
# T_MDCCALLBACKFUNCS=_T_MDCCALLBACKFUNCS
#
