#pragma once
#include "ICompany.h"

class CiticsCompany:public ICompany
{
private:
	const char * DPX_PATH = "Z:\\data\\WindDB\\dpx\\";
	//const char * TRADE_PATH = "Z:\\data\\WindDB\\production\\trade\\";
	//const char * POSITION_PATH = "Z:\\data\\WindDB\\production\\position\\";
	const char * TRADE_PATH = "trade\\";
	const char * POSITION_PATH = "position\\";
	const char * IPX_PATH = "Z:\\data\\WindTerminal\\ipx\\";
	const char * POS_SYS_PATH = "trade\\System\\";
	const char * TRADE_SYS_PATH = "trade\\System\\";

	const char * OUT_ACCOUNT_PATH = "Z:\\data\\WindDB\\production\\Citics_dailyDetails\\our_account\\";
	const char * OUT_POSITION_PATH = "out_position\\";
	const char * OUT_TRADE_PATH = "out_trade\\";
	const char * OUT_POS_PATH = "out_pos\\";
	const char * OUT_TCOST_PATH = "out_tcost\\";;
	const char * OUT_PNLPATH = "out_pnl\\";

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


	void FillTrade();
	void FillDpx(int type, vector<float> &dpx_q, float *totaldpx);
	void FillPos(int type, vector<int> &ve, int *totalshr);
	map<string,int> FillPosV2(vector<int> &ve, int type);
	map<string, int> FillTradeShr();
public:
	char lastdate[30];//出文件昨天的日期
	char fdate[8];//选择输出文件的日期.

	CiticsCompany();
	~CiticsCompany();
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

