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

//struct trade_sys
//{
//	float price;
//	int shr;
//};
//
//struct trade_time
//{
//	char st[6];
//	char et[6];
//};

//void GetDate(char *date);

char lastdate[30];//���ļ����������
char fdate[8];//ѡ������ļ�������.

#define DPX_PATH "Z:\\data\\WindDB\\dpx\\"
#define TRADE_PATH "Z:\\data\\WindDB\\production\\trade\\"
#define POSITION_PATH "Z:\\data\\WindDB\\production\\position\\"
#define OUT_ACCOUNT_PATH "Z:\\data\\WindDB\\production\\Citics_dailyDetails\\our_account\\"
#define IPX_PATH "Z:\\data\\WindTerminal\\ipx\\"


#define POS_SYS_PATH "trade\\System\\"
#define TRADE_SYS_PATH "trade\\System\\"

#define OUT_POSITION_PATH "out_position\\"
#define OUT_TRADE_PATH "Z:\\data\\WindDB\\production\\trade\\auto\\"
#define OUT_POS_PATH "Z:\\data\\WindDB\\production\\postion\\auto\\"
#define OUT_TCOST_PATH "Z:\\data\\WindDB\\production\\tcost\\auto\\"
#define OUT_PNLPATH "Z:\\data\\WindDB\\production\\pnl\\auto\\\\"