#include "stdafx.h"
#include "ICompany.h"
#include "DataLog.h"
#include <stdio.h>
#include <time.h>
#include "strptime.h"
#include "GlobalFunc.h"
using namespace GlobalFunc;

ICompany::ICompany(){};
ICompany::~ICompany(){};

void ICompany::Clearvector()
{
	tk_q.clear();
	tpx_q.clear();//today trade price
	trade_shr.clear();//today
	commission.clear();
	stamptax.clear();
	tt_map.clear();//��¼���׹�Ʊ����ʼʱ��
	dpx0_q.clear();
	dpx1_q.clear();
	shr0_q.clear();
	shr1_q.clear();
	total_com = 0;
	total_stamptax = 0;
	totalshr0 = 0;
	totalshr1 = 0;
	totaltrade = 0;
	tpx_avg = 0;
}

//��0��ʼ����,ִ�д˺���ǰ����ʹ��һ��strtok
char* ICompany::LocateColomn(int index)
{
	int i = 0;
	for (; i < index - 1; i++)
	{
		strtok(NULL, ",");
	}
	return strtok(NULL, ",");
}

void ICompany::GetDate(char *date)
{
	if (strcmp(date,"")!=0)
	{
		strcpy(fdate, date);
		return;
	}
	struct tm *newtime;
	char tmpbuf[30];
	time_t lllt1 = time(NULL);
	newtime = localtime(&lllt1);
	strftime(fdate, 30, "%Y%m%d", newtime);
}

void ICompany::GetLastDate()
{
	struct tm *newtime = (struct tm*)malloc(sizeof(struct tm));//��ʼ��newtime
	memset(newtime, 0x00, sizeof(struct tm));
	char tmpbuf[100];
	strptime(fdate, "%Y%m%d", newtime);
	newtime->tm_year = newtime->tm_year + 70;
	time_t lllt1 = mktime(newtime);
	lllt1 -= 86400;
	newtime = localtime(&lllt1);
	sprintf(lastdate, "%.4d%.2d%.2d", 1900 + newtime->tm_year, newtime->tm_mon + 1, newtime->tm_mday);
	sprintf(tmpbuf, "%s%s.pos.csv", OUT_POSITION_PATH, lastdate);
	int i = 0;
	while (_access(tmpbuf, 0) == -1)
	{
		//��������ļ�û�����ҵ���һ���
		lllt1 -= 86400;
		newtime = localtime(&lllt1);
		sprintf(lastdate, "%.4d%.2d%.2d", 1900 + newtime->tm_year, newtime->tm_mon + 1, newtime->tm_mday);
		memset(tmpbuf, 0, sizeof(tmpbuf));
		sprintf(tmpbuf, "%s%s.pos.csv", OUT_POSITION_PATH, lastdate);
		i++;
		if (i > 10)
		{
			DataLog dl(0,log_no,fdate,2);
			dl.WriteLog("About 10 ten days that havn't got any files");//��Ϊ�ĳ���10��û������Ϊ���������
			exit(1);
		}
	}
}


/*ȥ���ֲ�Ϊ0����*/
void ICompany::GetPos()
{
	FILE *fp;
	char strline[100];
	char tmpbuf[100];
	map<string, int> mp;
	int *tshr;
	memset(tmpbuf, 0, sizeof(tmpbuf));
	sprintf(tmpbuf, "%s%s.PositionSystem.csv", POS_SYS_PATH, fdate);
	fp = F_OPEN(tmpbuf, "r", log_no, fdate);
	fgets(strline, 50, fp);//������ͷ
	int i = 0;
	while (1)
	{
		char *tk_tmp = new char[10];
		if (fgets(strline, 50, fp) == NULL)
		{
			delete tk_tmp;
			break;
		}
		strcpy(tk_tmp, strtok(strline, ","));
		char *shr = strtok(NULL, ",");
		mp[tk_tmp] = atoi(shr);
	}
	fclose(fp);

	char retfile[100];//����ļ���

	FILE *fp_ret;
	sprintf(retfile, "%s%s.pos.csv", OUT_POSITION_PATH, fdate);
	fp_ret = F_OPEN(retfile, "w", log_no, fdate);
	fprintf(fp_ret, "#tk,shares\n");
	GetCash(fp_ret);//�õ�CASHֵ
	map<string, int>::iterator it;
	char tk_it[10];
	for (it = mp.begin(); it != mp.end(); it++)
	{
		if (it->second != 0)
		{
			fprintf(fp_ret, "%s,%d\n", it->first.c_str(), it->second);
		}
	}
	fclose(fp_ret);
	char strtmp[100];
	sprintf(strtmp,"%s created successful!", retfile);
	DataLog dl(0, log_no, fdate, 1);
	dl.WriteLog(strtmp);
}

void ICompany::GetTrade()
{
	char tmpbuf[100];
	char retfile[100];
	/*���ļ�����ȡ��Ӧ��*/
	sprintf(tmpbuf, "%s%s.TradingSystem.csv", TRADE_SYS_PATH, fdate);
	FILE *fp = F_OPEN(tmpbuf, "r", log_no, fdate);
	map <string, trade_p>::iterator m_Iter;
	map<string, trade_p> t_map;
	memset(tmpbuf, 0, sizeof(tmpbuf));
	fgets(tmpbuf, 50, fp);//������ͷ
	while (1)
	{
		memset(tmpbuf, 0, sizeof(tmpbuf));
		if (fgets(tmpbuf, 50, fp) == 0)
			break;
		char *cur_tk = new char[10];
		strcpy(cur_tk, strtok(tmpbuf, ","));//��ǰ֤ȯ����
		//������ǹ�Ʊ�ͻ��������
		if (cur_tk[0] != '0' && cur_tk[0] != '3' && cur_tk[0] != '6' )
			continue;
		if (cur_tk[0] == '0' && cur_tk[1] != '0')
			continue;
		trade_p ts = (trade_p)malloc(sizeof(struct trade_sys));
		ts->shr = atoi(strtok(NULL, ","));
		ts->price = atof(strtok(NULL, ","));
		m_Iter = t_map.find(cur_tk);
		if (m_Iter == t_map.end())
		{
			t_map[cur_tk] = ts;
			t_map[cur_tk]->shr = ts->shr;
			t_map[cur_tk]->price = ts->shr*ts->price;
		}
		else
		{
			m_Iter->second->price += ts->price*ts->shr;
			m_Iter->second->shr += ts->shr;
			free(ts);
		}
	}
	fclose(fp);
	//д�����ļ�.
	memset(tmpbuf, 0, sizeof(tmpbuf));
	sprintf(retfile, "%s%s.trade.csv", OUT_TRADE_PATH, fdate);
	FILE *tfp = F_OPEN(retfile, "w", log_no, fdate);
	fprintf(tfp, "#ticker,shr,tpx\n");
	for (m_Iter = t_map.begin(); m_Iter != t_map.end(); m_Iter++)
	{
		fprintf(tfp, "%s,%d,%f\n", m_Iter->first.c_str(), m_Iter->second->shr, m_Iter->second->price / m_Iter->second->shr);
	}
	fclose(tfp);
	char strtmp[100];
	sprintf(strtmp, "%s created successful!", retfile);
	DataLog dl(0, log_no, fdate, 1);
	dl.WriteLog(strtmp);
}

/*
* �õ�pos�ļ��е�cash
* �õ�commission��stamptax��ֵ
* �Լ�trade��ֵ
*/
void ICompany::GetCash(FILE *fp)
{
	char tmpbuf[100];
	char retfile[30];//����ļ���.
	time_t lllt1;
	char *tmp = "#tk,shares\n";
	char shr[20];//�ֹ���.
	char sprice[20];//�ɽ��۸�.
	char strline[50];//��ȡÿһ�е�ֵ
	/*-------------------��ȡ�����CASH----------------------*/
	memset(tmpbuf, 0, sizeof(tmpbuf));
	sprintf(tmpbuf, "%s%s.pos.csv", OUT_POSITION_PATH, lastdate);
	FILE *lastpos_fp = F_OPEN(tmpbuf, "r", log_no, fdate);//�������pos.csv
	fgets(strline, 50, lastpos_fp);
	fgets(strline, 50, lastpos_fp);
	strtok(strline, ",");
	char lastcash_str[50];
	strcpy(lastcash_str, strtok(NULL, ","));
	fclose(lastpos_fp);
	/*----------------��ȡ��CASHֵ------------------------*/
	memset(tmpbuf, 0, sizeof(tmpbuf));
	sprintf(tmpbuf, "%s%s.trade.csv", OUT_TRADE_PATH, fdate);
	FILE *trade_fp = F_OPEN(tmpbuf, "r", log_no, fdate);//�򿪽����trade.csv
	if (trade_fp == NULL)
	{
		perror("trade.csv is not existed");
		exit(1);
	}
	fgets(strline, 50, trade_fp);//������ͷ
	float cash = 0;
	float total_tpx = 0;
	int i = 0;
	while (1)
	{
		if (fgets(strline, 50, trade_fp) == NULL)
			break;
		char t_str[20];
		strcpy(t_str, strtok(strline, ","));
		char shr_str[20];
		strcpy(shr_str, strtok(NULL, ","));
		char tpx_str[20];
		strcpy(tpx_str, strtok(NULL, ","));
		float tpx = atof(tpx_str);
		int shr = atoi(shr_str);
		total_tpx += tpx;
		totaltrade += shr;
		/*commission��stamptax�Ĵ���*/
		float cur_com = tpx * abs(shr) * 2.5 / 10000;//���֮2.5��Ӷ��
		float cur_stax = 0;
		float cur_trans = 0;
		if (shr < 0)//���Ļ�Ҫ��
			cur_stax = -(shr * tpx / 1000);//���֮ʮ
		if (t_str[6] == '6')
		{
			cur_trans = abs(shr) * 6 / 10000;//���������֮6
		}
		total_com += cur_com;
		total_stamptax += cur_stax;
		total_transfer += cur_trans;
		cash += -(shr * tpx);
		i++;
	}
	tpx_avg = total_tpx / i;//ȡ��ƽ��ֵ��pnlҪ�õ�
	cash += atof(lastcash_str) - total_com - total_stamptax - total_transfer;
	fclose(trade_fp);
	fprintf(fp, "CASH,%f\n", cash);
}

int ICompany::CheckVector()
{
	int ret = 0;
	DataLog dl(0, log_no, fdate, 2);
	if (tpx_q.size() != tk_q.size())
	{
		dl.WriteLog("tpx_q's size is incorrect,plesase check!");
		ret = -1;
	}
	if (dpx0_q.size() != tk_q.size())
	{
		dl.WriteLog("dpx0_q's size is incorrect");
		ret = -1;
	}
	if (dpx1_q.size() != tk_q.size())
	{
		dl.WriteLog("dpx_q's size is incorrect");
		ret = -1;
	}
	if (trade_shr.size() != tk_q.size())
	{
		dl.WriteLog("trade_shr's size is incorrect");
		ret = -1;
	}
	if (commission.size() != tk_q.size())
	{
		dl.WriteLog("commission's size is incorrect");
		ret = -1;
	}
	if (stamptax.size() != tk_q.size())
	{
		dl.WriteLog("stamptax's size is incorrect");
		ret = -1;
	}
	return ret;
}

void ICompany::FillTrade()
{
	FILE *trade_fp;
	char strline[100];
	char tmpbuf[100];
	tk_q.clear();
	/*��trade�ļ��Ĵ���*/
	sprintf(tmpbuf, "%s%s.trade.csv", OUT_TRADE_PATH, fdate);
	trade_fp = F_OPEN(tmpbuf, "r", log_no, fdate);//�򿪽����trade.csv
	fgets(strline, 50, trade_fp);//������ͷ
	int i = 0;
	float total_tpx = 0;
	tk_q.clear();
	commission.clear();
	stamptax.clear();
	total_com = 0;
	total_stamptax = 0;
	while (1)
	{
		if (fgets(strline, 50, trade_fp) == NULL)
			break;
		char *t_str = new char[20];
		strcpy(t_str, strtok(strline, ","));
		tk_q.push_back(t_str);
		char *shr_str = new char[20];
		strcpy(shr_str, strtok(NULL, ","));
		char *tpx_str = new char[10];
		strcpy(tpx_str, strtok(NULL, ","));
		float tpx = atof(tpx_str);
		tpx_q.push_back(tpx);
		int shr = atoi(shr_str);
		total_tpx += tpx;
		totaltrade += shr;
		trade_shr.push_back(shr);
		/*˰�ѵĴ���*/
		float cur_com = tpx * abs(shr) * 2.5 / 10000;//���֮2.5��Ӷ��
		float cur_stax = 0;
		float cur_trans = 0;
		if (shr < 0)//���Ļ�Ҫ��
			cur_stax = -(shr * tpx / 1000);//���֮ʮ
		if (t_str[0] == '6')
		{
			cur_trans = abs(shr) * 6 / 10000;//���������֮��
		}
		commission.push_back(cur_com);
		stamptax.push_back(cur_stax);
		transfer.push_back(cur_trans);
		total_com += cur_com;
		total_stamptax += cur_stax;
		totaltrans += cur_trans;
		i++;
	}
	tpx_avg = total_tpx / i;//ȡ��ƽ��ֵ��pnlҪ�õ�
	fclose(trade_fp);
}

/*
* ��tk_qΪ��λ���У�������Ŀ��tk_qһ��
* type:0��pos0,1Ϊpos1
*/
void ICompany::FillPos(int type, vector<int> &ve, int *totalshr)
{
	FILE *fp;
	int size = tk_q.size();
	char strline[100];
	char tmpbuf[100];
	int *tshr;
	memset(tmpbuf, 0, sizeof(tmpbuf));
	if (type == 0)
	{
		shr0_q.clear();
		sprintf(tmpbuf, "%s%s.pos.csv", OUT_POSITION_PATH, lastdate);
	}
	else
	{
		shr1_q.clear();
		sprintf(tmpbuf, "%s%s.pos.csv", OUT_POSITION_PATH, fdate);
	}
	fp = F_OPEN(tmpbuf, "r", log_no, fdate);
	fgets(strline, 50, fp);//������ͷ
	fgets(strline, 50, fp);//����CASH
	int i = 0;
	while (i < size)
	{
		char *tk_tmp = new char[20];
		if (fgets(strline, 50, fp) == NULL)
		{
			while (i < size)
			{
				ve.push_back(0);
				i++;
			}
			break;
		}
		strcpy(tk_tmp, strtok(strline, ","));
		char *b = new char[20];
		strcpy(b, strtok(NULL, ","));
		while (i<size && (atoi(tk_q[i]) < atoi(tk_tmp)))
		{
			ve.push_back(0);
			i++;
		}
		if (i<size && (atoi(tk_q[i]) == atoi(tk_tmp)))
		{
			*totalshr += atoi(b);
			ve.push_back(atoi(b));
			i++;
		}
	}
	fclose(fp);
}

/*
* ��tk_qΪ��λ���У�������Ŀ��tk_qһ��
* type:0��dpx0,1Ϊdpx1
*/
void ICompany::FillDpx(int type, vector<float> &dpx_q, float *totaldpx)
{
	FILE *fp;
	int size = tk_q.size();
	char strline[100];
	char tmpbuf[100];
	if (type == 0)
	{
		dpx0_q.clear();
		sprintf(tmpbuf, "%s%s.dpx.csv", DPX_PATH, lastdate);
	}
	else
	{
		dpx1_q.clear();
		sprintf(tmpbuf, "%s%s.dpx.csv", DPX_PATH, fdate);
	}
	fp = F_OPEN(tmpbuf, "r", log_no, fdate);
	fgets(strline, 100, fp);//������ͷ
	float dpx0_avg = 0;
	int i = 0;
	while (i < size)
	{
		if (fgets(strline, 100, fp) == NULL)
			break;
		char *tmp1 = new char[20];
		strcpy(tmp1, strtok(strline, ","));
		int bs = strncmp(tmp1, tk_q[i], 6);
		//������ǽ��׵Ĺ�Ʊ����һ��
		if (bs == 0)
		{
			strcpy(tmp1, LocateColomn(6));
			dpx_q.push_back(atof(tmp1));
			*totaldpx += atof(tmp1);
			i++;
		}
	}
	fclose(fp);
}

void ICompany::GetPNL()
{
	FILE *fp;
	char strline[100];
	char tmpbuf[100];
	float totaldpx1, totaldpx0;
	FillTrade();
	FillPos(0, shr0_q, &totalshr0);
	FillPos(1, shr1_q, &totalshr1);
	FillDpx(0, dpx0_q, &totaldpx0);
	FillDpx(1, dpx1_q, &totaldpx1);
	int size = tk_q.size();
	float dpx0_avg = totaldpx0 / size;
	float dpx1_avg = totaldpx1 / size;
	if (CheckVector() == -1)
	{
		exit(1);
	}
	/*����hold_pnl,trade_pnl*/
	vector<float> holdpnl_q;
	vector<float> tradpnl_q;
	float total_hpnl = 0;
	float total_tpnl = 0;
	int i = 0;
	for (i = 0; i < size; i++)
	{
		float cur_hpnl = shr0_q[i] * (dpx1_q[i] - dpx0_q[i]);
		holdpnl_q.push_back(cur_hpnl);
		total_hpnl += cur_hpnl;
		float cur_tpnl = (shr1_q[i] - shr0_q[i]) * (dpx1_q[i] - tpx_q[i]);
		tradpnl_q.push_back(cur_tpnl);
		total_tpnl += cur_tpnl;
	}
	/*д���µ�pnl�ļ�*/
	memset(tmpbuf, 0, sizeof(tmpbuf));
	sprintf(tmpbuf, "%s%s.pnl.csv", OUT_PNLPATH, fdate);
	fp = F_OPEN(tmpbuf, "w", log_no, fdate); // �½�pnl�ļ�
	fprintf(fp, "#tk,pos-1,pos0,trade,px-1,px0,tpx,hold_pnl,trade_pnl,commission,stamp_tax,transfer_tax\n");
	fprintf(fp, "#total,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f\n", totalshr0, totalshr1, totaltrade, dpx0_avg, dpx1_avg, tpx_avg, total_hpnl, total_tpnl, total_com, total_stamptax, totaltrans);
	for (i = 0; i < size; i++)
	{
		fprintf(fp, "%s,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f\n", tk_q[i], shr0_q[i], shr1_q[i], trade_shr[i], dpx0_q[i], dpx1_q[i], tpx_q[i], holdpnl_q[i], tradpnl_q[i], commission[i], stamptax[i],transfer[i]);
	}
	fclose(fp);
	char strtmp[100];
	sprintf(strtmp, "%s created successful! ", tmpbuf);
	DataLog dl(0, log_no, fdate, 1);
	dl.WriteLog(strtmp);
}

void ICompany::GetAccount()
{
	char tmpbuf[100];
	sprintf(tmpbuf, "%s%s.account.csv", OUT_ACCOUNT_PATH, fdate);
	FILE *fp = F_OPEN(tmpbuf, "w", log_no, fdate);
	fprintf(fp, "#tk,trade,rest_shr,tpx,happen_cash,commission,stamp_tax,transfer,clearing\n");
	int size = tk_q.size();
	int i = 0;
	while (i < size)
	{
		fprintf(fp, "%s,%d,%d,%f,%f,%f,%f,%f,%f\n", tk_q[i], trade_shr[i], shr1_q[i], tpx_q[i], -tpx_q[i] * trade_shr[i] - commission[i] - stamptax[i], commission[i], stamptax[i], 0, 0);//���ڽ��Ҫ������Ǯ�����ԣ����ϸ���
		i++;
	}
	fclose(fp);
	char strtmp[100];
	sprintf(strtmp, "%s created successful! ", tmpbuf);
	DataLog dl(0, log_no, fdate, 1);
	dl.WriteLog(strtmp);
}

//��tradingsystem�����ȡʱ��
void ICompany::GetTimeRange()
{
	//��TradingSystem
	char tmpbuf[100];
	sprintf(tmpbuf, "%s%s.TradingSystem.csv", TRADE_SYS_PATH, fdate);
	map <string, trade_tp>::iterator m_Iter;
	FILE *fp = F_OPEN(tmpbuf, "r", log_no, fdate);
	memset(tmpbuf, 0, sizeof(tmpbuf));
	fgets(tmpbuf, 100, fp);
	char strtime[8] = { 0 };
	char tmpstrtime[7] = { 0 };//����β�����Ǹ��ַ�
	while (1)
	{
		memset(tmpbuf, 0, sizeof(tmpbuf));
		if (fgets(tmpbuf, 50, fp) == 0)
			break;
		char *cur_tk = new char[10];
		strcpy(cur_tk, strtok(tmpbuf, ","));//��ǰ֤ȯ����
		//������ǹ�Ʊ�ͻ��������
		if (cur_tk[0] != '0' && cur_tk[0] != '3' && cur_tk[0] != '6' || cur_tk[1] != '0')
			continue;
		memset(strtime, 0, sizeof(strtime));
		memset(strtime, 0, sizeof(tmpstrtime));
		strncpy(tmpstrtime, LocateColomn(3), 6);//��ȡ����ʱ��
		if (tmpstrtime[5] == '\n')
		{
			strtime[0] = '0';
			strtime[1] = tmpstrtime[0];
			strtime[2] = tmpstrtime[1];
			strtime[3] = tmpstrtime[2];
			strtime[4] = tmpstrtime[3];
			strtime[5] = tmpstrtime[4];
			strtime[6] = '\0';
		}
		else
		{
			strncpy(strtime, tmpstrtime, 6);
		}
		trade_tp ttp = (trade_tp)malloc(sizeof(struct trade_time));
		m_Iter = tt_map.find(cur_tk);
		if (m_Iter == tt_map.end())
		{
			strncpy(ttp->st, strtime, 6);
			strncpy(ttp->et, strtime, 6);
			tt_map[cur_tk] = ttp;
		}
		else
		{
			if (strcmp(m_Iter->second->st, strtime) > 0)
				strncpy(m_Iter->second->st, strtime, 6);
			else if (strcmp(m_Iter->second->et, strtime)<0)
				strncpy(m_Iter->second->et, strtime, 6);
		}
	}
}

//��ʱ��ת����Ӧ��������
int ICompany::Time2Index(char *strtime)
{
	char stime[6];
	strncpy(stime, strtime, 6);
	tm *tm = (struct tm*)malloc(sizeof(struct tm));
	strptime(stime, "%H%M%S", tm);

	int index = 0;
	if (tm->tm_hour <= 11 && tm->tm_hour >= 9)
	{
		index = (tm->tm_hour - 9) * 60 - 30 + tm->tm_min;//��ǰ����
	}
	else if (tm->tm_hour >= 13 && tm->tm_hour <= 15)
	{
		index = (tm->tm_hour - 13) * 60 + tm->tm_min + 120;
	}
	else
	{
		DataLog dl(0, log_no, fdate, 1);
		dl.WriteLog("the time is out of range ");
		exit(1);
	}
	free(tm);
	return index % 2 == 0 ? index - 2 : index - 1;
}

void ICompany::GetTcost()
{
	vector<float> ipx0, aipx1, aipx2, amt, tcost1s, tcost1p, tcost2s, tcost2p;//�г���ʼ�ۣ��г�ƽ���ۣ��г���Ȩƽ���۸��ĸ�tcost�㷨
	float avgipx0 = 0, avgaipx1 = 0, avgaipx2 = 0, total_amt = 0, t_tcost1s = 0, t_tcost1p = 0, t_tcost2s = 0, t_tcost2p = 0;//total�е�ֵ
	int size = tk_q.size();
	int i = 0;
	GetTimeRange();//�Ȼ��ʱ�䷶Χ.
	char tmpbuf[5000];
	sprintf(tmpbuf, "%s%s.ipx.csv", IPX_PATH, fdate);
	FILE *fp = F_OPEN(tmpbuf, "r", log_no, fdate);
	fgets(tmpbuf, 5000, fp);//������ͷ
	char tk[10];
	while (i < size)
	{
		float avg = 0;
		float avgw = 0;
		float vol = 0;
		memset(tmpbuf, 0, strlen(tmpbuf));
		if (fgets(tmpbuf, 5000, fp) == NULL)
		{
			perror("some ticket is not in the ipx file");
			break;
		}
		strcpy(tk, strtok(tmpbuf, ","));//������һ��
		char tk1[10] = { 0 };
		strncpy(tk1, tk, strlen(tk) - strlen(strchr(tk, '.')));//ȥ��.SZ�����׺
		//������A�ͻ�A�ֱ����������е���֮��ύ��ipx�Ĵ���
		if (strcmp(tk1, tk_q[i]) != 0)
			continue;
		else
		{
			int st = Time2Index(tt_map[tk1]->st);//������ֹʱ��
			int et = Time2Index(tt_map[tk1]->et);
			LocateColomn(st);
			for (int j = st; j <= et; j += 2)
			{
				float ipxtmp = atof(strtok(NULL, ","));
				int voltmp = atoi(strtok(NULL, ","));
				avg += ipxtmp;
				avgw += ipxtmp * voltmp;
				vol += voltmp;
				if (j == st)
				{
					ipx0.push_back(ipxtmp);
				}
			}
			aipx1.push_back(avg / 120);
			aipx2.push_back(avgw / vol);
			amt.push_back(tpx_q[i] * (trade_shr[i] >= 0 ? trade_shr[i] : -1.0*trade_shr[i]));
			tcost1s.push_back(trade_shr[i] * (tpx_q[i] - aipx1[i]));
			tcost1p.push_back(tcost1s[i] / (tpx_q[i] * amt[i]));
			tcost2s.push_back(trade_shr[i] * (tpx_q[i] - aipx2[i]));
			tcost2p.push_back(tcost2s[i] / (tpx_q[i] * amt[i]));
			avgipx0 += ipx0[i];
			avgaipx1 += aipx1[i];
			avgaipx2 += aipx2[i];
			total_amt += amt[i];
			t_tcost1s += tcost1s[i];
			t_tcost1p += tcost1p[i];
			t_tcost2s += tcost2s[i];
			t_tcost2p += tcost2p[i];
			i++;
		}
	}
	avgipx0 = avgipx0 / 120;
	avgaipx1 = avgaipx1 / 120;
	avgaipx2 = avgaipx2 / 120;
	fclose(fp);
	sprintf(tmpbuf, "%s%s.tcost.csv", OUT_TCOST_PATH, fdate);
	fp = F_OPEN(tmpbuf,"w",log_no,fdate);
	//fp = fopen(tmpbuf, "w");
	fprintf(fp, "%s\n", "#tk,shr,ipx0,aipx1,aipx2,tpx,amt,tcost1s,tcost1p,tcost2s,tcost2p");
	fprintf(fp, "#total,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", totaltrade, avgipx0, avgaipx1, avgaipx2, tpx_avg, total_amt, t_tcost1s, t_tcost1p, t_tcost2s, t_tcost2p);
	i = 0;
	while (i < size)
	{
		fprintf(fp, "%s,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", tk_q[i], trade_shr[i], ipx0[i], aipx1[i], aipx2[i], tpx_q[i], amt[i], tcost1s[i], tcost1p[i], tcost2s[i], tcost2p[i]);
		i++;
	}
	fclose(fp);
	char strtmp[100];
	sprintf(strtmp, "%s.tcost.csv created successful! ", fdate);
	DataLog dl(0, log_no, fdate, 1);
	dl.WriteLog(strtmp);
}

/*
* ֱ���õ�����/�����pos����ֵ
* type = 0 ���� = 1 ����
*/
map<string, int>  ICompany::FillPosV2(int type)
{
	FILE *fp;
	char strline[100];
	char tmpbuf[100];
	map<string, int> mp;
	int *tshr;
	memset(tmpbuf, 0, sizeof(tmpbuf));
	if (type == 0)
	{
		sprintf(tmpbuf, "%s%s.pos.csv", OUT_POSITION_PATH, lastdate);
	}
	else
	{
		sprintf(tmpbuf, "%s%s.pos.csv", OUT_POSITION_PATH, fdate);
	}
	fp = F_OPEN(tmpbuf, "r", log_no, fdate);
	fgets(strline, 50, fp);//������ͷ
	fgets(strline, 50, fp);//����CASH
	int i = 0;
	while (1)
	{
		char *tk_tmp = new char[10];
		if (fgets(strline, 50, fp) == NULL)
		{
			delete tk_tmp;
			break;
		}
		strcpy(tk_tmp, strtok(strline, ","));
		char *shr = strtok(NULL, ",");
		mp[tk_tmp] = atoi(shr);
	}
	fclose(fp);
	return mp;
}

/*
* ��trade.csv�л�ȡ
* �õ�����tk����Ӧ��shr
*/
map<string, int>  ICompany::FillTradeShr()
{
	char logstr[100];
	FILE *fp;
	char strline[100];
	char tmpbuf[100];
	map<string, int> mp;
	int *tshr;
	memset(tmpbuf, 0, sizeof(tmpbuf));
	sprintf(tmpbuf, "%s%s.trade.csv", OUT_TRADE_PATH, fdate);
	fp = F_OPEN(tmpbuf,"r",log_no,fdate);
	fgets(strline, 50, fp);//������ͷ
	int i = 0;
	while (1)
	{
		char *tk_tmp = new char[10];
		if (fgets(strline, 50, fp) == NULL)
		{
			delete tk_tmp;
			break;
		}
		strcpy(tk_tmp, strtok(strline, ","));
		char *shr = strtok(NULL, ",");
		mp[tk_tmp] = atoi(shr);
	}
	fclose(fp);
	return mp;
}

void ICompany::CheckPos()
{
	map<string, int> trdshr = FillTradeShr();
	map<string, int> pos1 = FillPosV2(0);
	map<string, int> pos2 = FillPosV2(1);
	map<string, int> posnew;
	char logstr[200];
	int i = 0;
	map<string, int>::iterator it;
	/*�ô���trade�ļ���pos�ļ��Ĳ�������ȥ��0�ֲֵ���*/
	for (it = trdshr.begin(); it != trdshr.end(); it++)
	{
		if (pos1[it->first] + it->second == 0)
		{
			map<string, int>::iterator itmp = pos1.find(it->first);
			pos1.erase(itmp);
		}
		else
		{
			pos1[it->first] += it->second;

		}
	}
	/*�Ƚϼ��������pos����ʵpos*/
	if (pos1.size() != pos2.size())
	{
		DataLog dl(1, log_no, fdate, 2);
		memset(logstr,0,strlen(logstr));
		sprintf(logstr,"Size: pos-1 + trade0 = %d, pos0 = %d.Please Check", pos1.size(), pos2.size());
		dl.WriteLog(logstr);
		exit(1);
	}
	else
	{
		DataLog dl(1, log_no, fdate, 1);
		dl.WriteLog("The stock number of (pos-1+trade0) and pos0 is the same.");
	}
	int ret = 0;
	for (it = pos1.begin(); it != pos1.end(); it++)
	{
		if (pos2[it->first] != it->second)
		{
			DataLog dl(1, log_no, fdate, 2);
			memset(logstr, 0, strlen(logstr));
			sprintf(logstr," %s:pos-1+trade0 = %d, pos0 = %d", it->first, it->second, pos2[it->first]);
			dl.WriteLog(logstr);
			ret = 1;
		}
	}
	if (ret == 0)
	{
		DataLog dl(1, log_no, fdate, 1);
		dl.WriteLog("All the pos.csv data is correct.");
	}
	else
	{
		DataLog dl(1, log_no, fdate, 2);
		dl.WriteLog("Please Check.");
	}
}