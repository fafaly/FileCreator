#pragma once
#include <vector>
#include <map>
#include <io.h>
#include "Global.h"

using namespace std;

class ICompany
{
public:
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
	ICompany();
	~ICompany();
	/*获取持仓数，将持仓为0的行去掉.（只需给定的两列)*/
	virtual void GetPos()=0;
	//获取交易量,将股份代码相同的交易进行合并.（只需给定的三列）
	virtual void GetTrade() = 0;
	//获得现现金数.
	virtual void GetCash(FILE *) = 0;
	//获取pnl的值.
	virtual void GetPNL() = 0;
	//获取tcost的值.

	/*下面这几个Get开头的都是为了填充数组*/
	virtual void GetTcost() = 0;
	virtual void GetLastDate() = 0;
	virtual void GetDate(char *) = 0;
	virtual void GetAccount() = 0;
	virtual char* LocateColomn(int index) = 0;
	virtual int Time2Index(char *strtime) = 0;
	virtual int CheckVector() = 0;
	virtual void FillTrade() = 0;
	virtual void FillDpx(int type, vector<float> &dpx_q, float *totaldpx) = 0;
	virtual void FillPos(int type, vector<int> &ve, int *totalshr) = 0;
	virtual void GetTimeRange() = 0;
	virtual void Clearvector() = 0;

};

