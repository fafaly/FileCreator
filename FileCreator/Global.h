#pragma once

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



