#include "stdafx.h"
#include "CiticsCompany.h"
#include <stdio.h>
#include <time.h>
#include "strptime.h"

CiticsCompany::CiticsCompany()
{
}


CiticsCompany::~CiticsCompany()
{
}

void CiticsCompany::Clearvector()
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
char* CiticsCompany::LocateColomn(int index)
{
	int i = 0;
	for (; i < index - 1; i++)
	{
		strtok(NULL, ",");
	}
	return strtok(NULL, ",");
}

void CiticsCompany::GetDate(char *date)
{
	if (date != NULL)
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

void CiticsCompany::GetLastDate()
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
	sprintf(tmpbuf, "%s%s.pos.csv", POSITION_PATH, lastdate);
	int i = 0;
	while (_access(tmpbuf, 0) == -1)
	{
		//��������ļ�û�����ҵ���һ���
		lllt1 -= 86400;
		newtime = localtime(&lllt1);
		sprintf(lastdate, "%.4d%.2d%.2d", 1900 + newtime->tm_year, newtime->tm_mon + 1, newtime->tm_mday);
		memset(tmpbuf, 0, sizeof(tmpbuf));
		sprintf(tmpbuf, "%s%s.pos.csv", POSITION_PATH, lastdate);
		i++;
		if (i > 10)
		{
			printf("About 10 ten days that havn't got any files\n");//��Ϊ�ĳ���10��û������Ϊ���������
			exit(1);
		}
	}
}


/*ȥ���ֲ�Ϊ0����*/
void CiticsCompany::GetPos()
{
	char retfile[30];//����ļ���
	FILE *fp;
	FILE *fp_ret;
	char *tmp = "#tk,shares\n";
	char tmpbuf[100];
	sprintf(tmpbuf, "%s%s.PositionSystem.csv", POS_SYS_PATH, fdate);
	fp = fopen(tmpbuf, "r");
	if (fp == NULL)
	{
		perror("PositionSystem.csv not exist");
		exit(1);
	}
	sprintf(retfile, "%s%s.pos.csv", POSITION_PATH, fdate);
	fp_ret = fopen(retfile, "w");
	fwrite(tmp, 1, strlen(tmp), fp_ret);//��ͷ
	GetCash(fp_ret);//�õ�CASHֵ
	char strline[50];
	fgets(strline, 50, fp);//������ͷ
	while (1)
	{
		char strshares[20];
		memset(strline, 0, strlen(strline));
		if (fgets(strline, 50, fp) == NULL)
			break;
		char *tk_tmp = strtok(strline, ",");
		strcpy(strshares, strtok(NULL, ","));
		char *b = new char[20];
		strcpy(b, strshares);
		if (strcmp(b, "0\n") != 0)
		{
			fprintf(fp_ret, "%s\n", strline);
		}
	}
	fclose(fp);
	fclose(fp_ret);
	printf("%s created successful!\n", retfile);
}

void CiticsCompany::GetTrade()
{
	char tmpbuf[100];
	/*���ļ�����ȡ��Ӧ��*/
	sprintf(tmpbuf, "%s%s.TradingSystem.csv", TRADE_SYS_PATH, fdate);
	FILE *fp = fopen(tmpbuf, "r");
	if (fp == NULL)
	{
		perror("TradingSystem.csv not exist");
		exit(1);
	}
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
		if (cur_tk[0] != '0' && cur_tk[0] != '3' && cur_tk[0] != '6' || cur_tk[1] != '0')
			continue;
		trade_p ts = (trade_p)malloc(sizeof(struct trade_sys));
		ts->trade_sum = atoi(strtok(NULL, ","));
		ts->price_sum = atof(strtok(NULL, ","));
		m_Iter = t_map.find(cur_tk);
		ts->index = 0;
		if (m_Iter == t_map.end())
		{
			ts->index++;
			t_map[cur_tk] = ts;
		}
		else
		{
			m_Iter->second->index++;
			m_Iter->second->price_sum += ts->price_sum;
			m_Iter->second->trade_sum += ts->trade_sum;
			free(ts);
		}
	}
	fclose(fp);
	//д�����ļ�.
	memset(tmpbuf, 0, sizeof(tmpbuf));
	sprintf(tmpbuf, "%s%s.trade.csv", TRADE_PATH, fdate);
	FILE *tfp = fopen(tmpbuf, "w");
	fprintf(tfp, "#ticker,shr,tpx\n");
	for (m_Iter = t_map.begin(); m_Iter != t_map.end(); m_Iter++)
	{
		fprintf(tfp, "%s,%d,%f\n", m_Iter->first.c_str(), m_Iter->second->trade_sum, m_Iter->second->price_sum / m_Iter->second->index);
	}
	fclose(tfp);
}

/*
* �õ�pos�ļ��е�cash
* �õ�commission��stamptax��ֵ
* �Լ�trade��ֵ
*/
void CiticsCompany::GetCash(FILE *fp)
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
	sprintf(tmpbuf, "%s%s.pos.csv", POSITION_PATH, lastdate);
	FILE *lastpos_fp = fopen(tmpbuf, "r");//�������pos.csv
	if (lastpos_fp == NULL)
	{
		printf("%s.PositionSystem.csv not exist", lastdate);
		exit(1);
	}
	fgets(strline, 50, lastpos_fp);
	fgets(strline, 50, lastpos_fp);
	strtok(strline, ",");
	char lastcash_str[20];
	strcpy(lastcash_str, strtok(NULL, ","));
	fclose(lastpos_fp);
	/*----------------��ȡ��CASHֵ------------------------*/
	memset(tmpbuf, 0, sizeof(tmpbuf));
	sprintf(tmpbuf, "%s%s.trade.csv", TRADE_PATH, fdate);
	FILE *trade_fp = fopen(tmpbuf, "r");//�򿪽����trade.csv
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
		/*commission��stamptax�Ĵ���*/
		float cur_com = tpx * abs(shr) * 2.5 / 10000;//���֮2.5��Ӷ��
		float cur_stax = 0;
		if (shr < 0)//���Ļ�Ҫ��
			cur_stax = -(shr * tpx / 1000);//���֮ʮ
		commission.push_back(cur_com);
		stamptax.push_back(cur_stax);
		total_com += cur_com;
		total_stamptax += cur_stax;
		cash += -(shr * tpx);
		i++;
	}
	tpx_avg = total_tpx / i;//ȡ��ƽ��ֵ��pnlҪ�õ�
	cash += atof(lastcash_str) + total_com + total_stamptax;
	fclose(trade_fp);
	fprintf(fp, "CASH,%f\n", cash);
}

int CiticsCompany::CheckVector()
{
	int ret = 0;
	if (tpx_q.size() != tk_q.size())
	{
		printf("tpx_q's size is incorrect,plesase check!\n");
		ret = -1;
	}
	if (dpx0_q.size() != tk_q.size())
	{
		printf("dpx0_q's size is incorrect\n");
		ret = -1;
	}
	if (dpx1_q.size() != tk_q.size())
	{
		printf("dpx_q's size is incorrect\n");
		ret = -1;
	}
	if (trade_shr.size() != tk_q.size())
	{
		printf("trade_shr's size is incorrect\n");
		ret = -1;
	}
	if (commission.size() != tk_q.size())
	{
		printf("commission's size is incorrect\n");
		ret = -1;
	}
	if (stamptax.size() != tk_q.size())
	{
		printf("stamptax's size is incorrect\n");
		ret = -1;
	}
	return ret;
}

void CiticsCompany::FillTrade()
{
	FILE *trade_fp;
	char strline[100];
	char tmpbuf[100];
	/*��trade�ļ��Ĵ���*/
	sprintf(tmpbuf, "%s%s.trade.csv", TRADE_PATH, fdate);
	trade_fp = fopen(tmpbuf, "r");//�򿪽����trade.csv
	if (trade_fp == NULL)
	{
		perror("trade.csv is not existed");
		exit(1);
	}
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
		/*commission��stamptax�Ĵ���*/
		float cur_com = tpx * abs(shr) * 2.5 / 10000;//���֮2.5��Ӷ��
		float cur_stax = 0;
		if (shr < 0)//���Ļ�Ҫ��
			cur_stax = -(shr * tpx / 1000);//���֮ʮ
		commission.push_back(cur_com);
		stamptax.push_back(cur_stax);
		total_com += cur_com;
		total_stamptax += cur_stax;
		i++;
	}
	tpx_avg = total_tpx / i;//ȡ��ƽ��ֵ��pnlҪ�õ�
	fclose(trade_fp);
}

/*type:0��pos0,1Ϊpos1*/
void CiticsCompany::FillPos(int type, vector<int> &ve, int *totalshr)
{
	FILE *fp;
	int size = tk_q.size();
	char strline[100];
	char tmpbuf[100];
	int *tshr;
	/*��pos0�ļ��Ĵ���*/
	memset(tmpbuf, 0, sizeof(tmpbuf));
	if (type == 0)
		sprintf(tmpbuf, "%s%s.pos.csv", POSITION_PATH, lastdate);
	else
		sprintf(tmpbuf, "%s%s.pos.csv", POSITION_PATH, fdate);
	fp = fopen(tmpbuf, "r");
	fgets(strline, 50, fp);//������ͷ
	fgets(strline, 50, fp);//����CASH
	int i = 0;
	while (i < size)
	{
		char *tk_tmp = new char[10];
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
		char *b = new char[10];
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

/*type:0��dpx0,1Ϊdpx1*/
void CiticsCompany::FillDpx(int type, vector<float> &dpx_q, float *totaldpx)
{
	FILE *fp;
	int size = tk_q.size();
	char strline[100];
	char tmpbuf[100];
	if (type == 0)
		sprintf(tmpbuf, "%s%s.dpx.csv", DPX_PATH, lastdate);
	else
		sprintf(tmpbuf, "%s%s.dpx.csv", DPX_PATH, fdate);
	fp = fopen(tmpbuf, "r");
	if (fp == NULL)
	{
		printf("%s fopen failed", tmpbuf);
		exit(1);
	}
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

void CiticsCompany::GetPNL()
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
	fp = fopen(tmpbuf, "w");//�½�pnl�ļ�
	if (fp == NULL)
	{
		perror("fopen");
		exit(1);
	}
	fprintf(fp, "#tk,pos-1,pos0,trade,px-1,px0,tpx,hold_pnl,trade_pnl,commission,stamp_tax\n");
	fprintf(fp, "#total,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f\n", totalshr0, totalshr1, totaltrade, dpx0_avg, dpx1_avg, tpx_avg, total_hpnl, total_tpnl, total_com, total_stamptax);
	for (i = 0; i < size; i++)
	{
		fprintf(fp, "%s,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f\n", tk_q[i], shr0_q[i], shr1_q[i], trade_shr[i], dpx0_q[i], dpx1_q[i], tpx_q[i], holdpnl_q[i], tradpnl_q[i], commission[i], stamptax[i]);
	}
	fclose(fp);

	printf("%s created successful!\n", tmpbuf);
}

void CiticsCompany::GetAccount()
{
	char tmpbuf[100];
	sprintf(tmpbuf, "%s%s.account.csv", OUT_ACCOUNT_PATH, fdate);
	FILE *fp = fopen(tmpbuf, "w");
	fprintf(fp, "#tk,trade,rest_shr,tpx,happen_cash,commission,stamp_tax,transfer,clearing\n");
	int size = tk_q.size();
	int i = 0;
	while (i < size)
	{
		fprintf(fp, "%s,%d,%d,%f,%f,%f,%f,%f,%f\n", tk_q[i], trade_shr[i], shr1_q[i], tpx_q[i], -tpx_q[i] * trade_shr[i] - commission[i] - stamptax[i], commission[i], stamptax[i], 0, 0);//���ڽ��Ҫ������Ǯ�����ԣ����ϸ���
		i++;
	}
	fclose(fp);
	printf("%s created successful!\n", tmpbuf);
}

//��tradingsystem�����ȡʱ��
void CiticsCompany::GetTimeRange()
{
	//��TradingSystem
	char tmpbuf[100];
	sprintf(tmpbuf, "%s%s.TradingSystem.csv", TRADE_SYS_PATH, fdate);
	map <string, trade_tp>::iterator m_Iter;
	FILE *fp = fopen(tmpbuf, "r");
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
int CiticsCompany::Time2Index(char *strtime)
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
		printf("the time is out of range\n");
		exit(1);
	}
	free(tm);
	return index % 2 == 0 ? index - 2 : index - 1;
}

void CiticsCompany::GetTcost()
{
	vector<float> ipx0, aipx1, aipx2, amt, tcost1s, tcost1p, tcost2s, tcost2p;//�г���ʼ�ۣ��г�ƽ���ۣ��г���Ȩƽ���۸��ĸ�tcost�㷨
	float avgipx0 = 0, avgaipx1 = 0, avgaipx2 = 0, total_amt = 0, t_tcost1s = 0, t_tcost1p = 0, t_tcost2s = 0, t_tcost2p = 0;//total�е�ֵ
	int size = tk_q.size();
	int i = 0;
	GetTimeRange();//�Ȼ��ʱ�䷶Χ.
	char tmpbuf[5000];
	sprintf(tmpbuf, "%s%s.ipx.csv", IPX_PATH, fdate);
	FILE *fp = fopen(tmpbuf, "r");
	if (fp == NULL)
	{
		perror("The tcost file is not existed");
		exit(1);
	}
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
		//������A�ͻ�A�ֱ����������е���֮��ύ����ipx�Ĵ���
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
	fp = fopen(tmpbuf, "w");
	fprintf(fp, "%s\n", "#tk,shr,ipx0,aipx1,aipx2,tpx,amt,tcost1s,tcost1p,tcost2s,tcost2p");
	fprintf(fp, "#total,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", totaltrade, avgipx0, avgaipx1, avgaipx2, tpx_avg, total_amt, t_tcost1s, t_tcost1p, t_tcost2s, t_tcost2p);
	i = 0;
	while (i < size)
	{
		fprintf(fp, "%s,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", tk_q[i], trade_shr[i], ipx0[i], aipx1[i], aipx2[i], tpx_q[i], amt[i], tcost1s[i], tcost1p[i], tcost2s[i], tcost2p[i]);
		i++;
	}
	fclose(fp);
	printf("%s.tcost.csv created successful!\n", fdate);
}