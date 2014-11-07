#pragma once
#include <vector>
#include <map>
#include <io.h>
#include "Global.h"

using namespace std;

class ICompany
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
	ICompany();
	~ICompany();
	/*��ȡ�ֲ��������ֲ�Ϊ0����ȥ��.��ֻ�����������)*/
	virtual void GetPos()=0;
	//��ȡ������,���ɷݴ�����ͬ�Ľ��׽��кϲ�.��ֻ����������У�
	virtual void GetTrade() = 0;
	//������ֽ���.
	virtual void GetCash(FILE *) = 0;
	//��ȡpnl��ֵ.
	virtual void GetPNL() = 0;
	//��ȡtcost��ֵ.

	/*�����⼸��Get��ͷ�Ķ���Ϊ���������*/
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

