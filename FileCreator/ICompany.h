#pragma once
#include <vector>
#include <map>
#include <io.h>
#include "Global.h"

using namespace std;

class ICompany
{
private:
	
	map<string, trade_tp> tt_map;//��¼���׹�Ʊ����ʼʱ��

	float total_com = 0;
	float total_stamptax = 0;
	float total_transfer = 0;
	int totalshr1 = 0;
	int totalshr0 = 0;
	int totaltrade = 0;
	float totaltrans = 0;
	float tpx_avg = 0;

	void FillTrade(map<string, pnlst> *pnlmp);
	void FillDpx(int type, map<string, pnlst> *pnlmp, float *totaldpx);
	void FillPos(int type, map<string, pnlst> *pnlmp, int *totalshr);
	map<string, int> FillPosV2(vector<int> &ve, int type);
	map<string, trade_sys> FillTradeShr(char *);
public:
	ICompany();
	~ICompany();

	int log_no;

	char lastdate[30];//���ļ����������
	char fdate[10];//ѡ������ļ�������.

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

	/*��ȡ�ֲ��������ֲ�Ϊ0����ȥ��.��ֻ�����������)*/
	virtual void GetPos();
	//��ȡ������,���ɷݴ�����ͬ�Ľ��׽��кϲ�.��ֻ����������У�
	virtual void GetTrade();
	//������ֽ���.
	virtual void GetCash(FILE *);
	//��ȡpnl��ֵ.
	virtual void GetPNL();
	//��ȡtcost��ֵ.

	/*�����⼸��Get��ͷ�Ķ���Ϊ���������*/
	virtual void GetTcost();
	virtual void GetLastDate();
	virtual void GetDate(char *);
	//virtual void GetAccount();
	char* LocateColomn(int index);
	int Time2Index(char *strtime);
	//int CheckVector();

	void GetTimeRange();

	void CheckPos();
	map<string, int> FillPosV2(int);

};

