#pragma once
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <vector>
#include <io.h>
#include <map>
#include "strptime.h"

typedef struct trade_sys * trade_p;
typedef struct trade_time * trade_tp;

struct trade_sys
{
	int index;
	float price_sum;
	int trade_sum;
};

struct trade_time
{
	char st[6];
	char et[6];
};

//void GetDate(char *date);

char lastdate[30];//出文件昨天的日期
char fdate[8];//选择输出文件的日期.

#define DPX_PATH "Z:\\data\\WindDB\\dpx\\"
#define TRADE_PATH "Z:\\data\\WindDB\\production\\trade\\"
#define POSITION_PATH "Z:\\data\\WindDB\\production\\position\\"
#define OUT_ACCOUNT_PATH "Z:\\data\\WindDB\\production\\Citics_dailyDetails\\our_account\\"
#define IPX_PATH "Z:\\data\\WindTerminal\\ipx\\"


#define POS_SYS_PATH "trade\\System\\"
#define TRADE_SYS_PATH "trade\\System\\"

#define OUT_POSITION_PATH "out_position\\"
#define OUT_TRADE_PATH "out_trade\\"
#define OUT_POS_PATH "out_pos\\"
#define OUT_TCOST_PATH "out_tcost\\"
#define OUT_PNLPATH "out_pnl\\"