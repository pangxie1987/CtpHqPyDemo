#-*-coding:utf-8-*-
from  ctypes import *

def_macro={'MAX_QUOTA_STATUS_LEN':2,
            'MAX_DATE_LEN':9,
            'MAX_EXCHCODE_LEN':7,
            'MAX_VARI_LEN':31,
            'MAX_HQTYPE_LEN':2,
            'MAX_LEVEL_HQ':10
           }


def enum(**enums):
    return type('Enum',(),enums)

HqSourceType=enum(
    Exchange=0,
    Counter=1
)

BASE_KCTYPE=enum(
    KCT_MIN=c_int(0),
    KCT_DAY=c_int(1),
    KCT_WEEK=c_int(2),
    KCT_MONTH=c_int(3),
    KCT_YEAR=c_int(4)
)
#行情类型
HQTYPES=enum(
      INDEX=0,
      STOCK=1,
      FUND=2,
      BOND=3,
      OTHER_STOCK=4,
      OPTION=5,
      EXCHANGE=6,
      FUTURE=7,
      FTR_IDX=8,
      RGZ=9,
      ETF=10,
      LOF=11,
      COV_BOND=12,
      TRUST=13,
      WARRANT=14,
      REPO=15,
      COMM=16
)

PRICEUNIT=c_float
DONEVOLUME=c_int64
AMTUNIT=c_double
LVOLUNIT=c_uint
