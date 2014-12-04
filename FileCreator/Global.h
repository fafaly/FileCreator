#pragma once

typedef struct trade_sys * trade_p;
typedef struct trade_time * trade_tp;

struct trade_sys
{
	float price;
	int shr;
};

struct trade_time
{
	char st[10];
	char et[10];
};

struct pnlst
{
	int pos_1;
	int pos0;
	int trade;
	float px0;
	float px_1;
	float tpx;
	float hold_pnl;
	float trade_pnl;
	float commission;
	float stamp;
	float transfer;
};



