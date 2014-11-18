#pragma once
#include <vector>
#include <map>
#include <io.h>
#include "Global.h"

using namespace std;

class ICompany
{
private:
	vector<char*> tk_q;//ticker
	vector<float> tpx_q;//today trade price
	vector<int> trade_shr;//today
	vector<float> commission;
	vector<float> stamptax;
	vector<float> transfer;
	map<string, trade_tp> tt_map;//记录交易股票的起始时间
	vector<float> dpx0_q, dpx1_q;
	vector<int> shr0_q, shr1_q;
	float total_com = 0;
	float total_stamptax = 0;
	int totalshr1 = 0;
	int totalshr0 = 0;
	int totaltrade = 0;
	float totaltrans = 0;
	float tpx_avg = 0;

	void FillTrade();
	void FillDpx(int type, vector<float> &dpx_q, float *totaldpx);
	void FillPos(int type, vector<int> &ve, int *totalshr);
	map<string, int> FillPosV2(vector<int> &ve, int type);
	map<string, int> FillTradeShr();
public:
	ICompany();
	~ICompany();

	int log_no;

	char lastdate[30];//出文件昨天的日期
	char fdate[10];//选择输出文件的日期.

	char DPX_PATH[100];
	char TRADE_PATH[100];
	char POSITION_PATH[100];
	//const char * TRADE_PATH = "trade\\";
	//const char * POSITION_PATH = "position\\";
	char IPX_PATH[100];
	char POS_SYS_PATH[100];
	char TRADE_SYS_PATH[100];

	char OUT_ACCOUNT_PATH[100];
	char OUT_POSITION_PATH[100];
	char OUT_TRADE_PATH[100];
	char OUT_POS_PATH[100];
	char OUT_TCOST_PATH[100];
	char OUT_PNLPATH[100];

	/*获取持仓数，将持仓为0的行去掉.（只需给定的两列)*/
	virtual void GetPos();
	//获取交易量,将股份代码相同的交易进行合并.（只需给定的三列）
	virtual void GetTrade();
	//获得现现金数.
	virtual void GetCash(FILE *);
	//获取pnl的值.
	virtual void GetPNL();
	//获取tcost的值.

	/*下面这几个Get开头的都是为了填充数组*/
	virtual void GetTcost();
	virtual void GetLastDate();
	virtual void GetDate(char *);
	virtual void GetAccount();
	char* LocateColomn(int index);
	int Time2Index(char *strtime);
	int CheckVector();

	void GetTimeRange();
	void Clearvector();
	void CheckPos();
	map<string, int> FillPosV2(int);

};

