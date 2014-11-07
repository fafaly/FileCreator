#pragma once
#include "ICompany.h"

class CiticsCompany:public ICompany
{
public:
	char lastdate[30];//���ļ����������
	char fdate[8];//ѡ������ļ�������.

	vector<char*> tk_q;//ticker
	vector<float> tpx_q;//today trade price
	vector<int> trade_shr;//today
	vector<float> commission;
	vector<float> stamptax;
	map<string, trade_tp> tt_map;//��¼���׹�Ʊ����ʼʱ��
	vector<float> dpx0_q, dpx1_q;
	vector<int> shr0_q, shr1_q;
	float total_com = 0;
	float total_stamptax = 0;
	int totalshr1 = 0;
	int totalshr0 = 0;
	int totaltrade = 0;
	float tpx_avg = 0;
	CiticsCompany();
	~CiticsCompany();
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
	virtual void GetAccount();
	char* LocateColomn(int index);
	int Time2Index(char *strtime);
	int CheckVector();
	void FillTrade();
	void FillDpx(int type, vector<float> &dpx_q, float *totaldpx);
	void FillPos(int type, vector<int> &ve, int *totalshr);
	void GetTimeRange();
	void Clearvector();
};

