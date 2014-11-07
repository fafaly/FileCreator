#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <vector>
#include <io.h>
#include <map>
#include "strptime.h"
using namespace std;

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

char lastdate[30];//出文件昨天的日期
char fdate[8];//选择输出文件的日期.
vector<char*> tk_q;//ticker
vector<float> tpx_q;//today trade price

vector<int> trade_shr;//today
vector<float> commission;
vector<float> stamptax;
map<string, trade_tp> tt_map;//记录交易股票的起始时间
vector<float> dpx0_q, dpx1_q;
vector<int> shr0_q, shr1_q;
float total_com = 0;
float total_stamptax = 0;
int totalshr1 = 0;
int totalshr0 = 0;
int totaltrade = 0;
float tpx_avg = 0;

void GetPos();//获取持仓数，将持仓为0的行去掉.（只需给定的两列)
void GetTrade();//获取交易量,将股份代码相同的交易进行合并.（只需给定的三列）
void GetCash(FILE *);//获得现现金数.
void GetPNL();//获取pnl的值.
void GetTcost();//获取tcost的值.
void GetLastDate();
void GetDate(char *);
void GetAccount();
void Clearvector();
char* LocateColomn(int index);
int Time2Index(char *strtime);


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