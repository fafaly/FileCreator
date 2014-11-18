#include "stdafx.h"
#include "CiticsCompany.h"
#include <string.h>

/*pro_no ²úÆ·±àºÅ*/
CiticsCompany::CiticsCompany(int pro_no)
{
	log_no = pro_no;
	switch (pro_no)
	{
	case 1:
		strcpy(DPX_PATH, "Z:\\data\\WindTerminal\\dpx\\");
		strcpy(TRADE_PATH, "Z:\\data\\WindDB\\production\\trade\\");
		strcpy(POSITION_PATH, "Z:\\data\\WindDB\\production\\position\\");
		strcpy(IPX_PATH, "Z:\\data\\WindTerminal\\ipx\\");
		strcpy(POS_SYS_PATH, "production\\trade\\System\\");
		strcpy(TRADE_SYS_PATH, "production\\trade\\System\\");

		strcpy(OUT_ACCOUNT_PATH, "Z:\\data\\WindDB\\production\\Citics_dailyDetails\\our_account\\");
		strcpy(OUT_POSITION_PATH, "production\\out_position\\");
		strcpy(OUT_TRADE_PATH, "production\\out_trade\\");
		strcpy(OUT_POS_PATH, "production\\out_pos\\");
		strcpy(OUT_TCOST_PATH, "production\\out_tcost\\");
		strcpy(OUT_PNLPATH, "production\\out_pnl\\");
		break;
	case 2:
		strcpy(DPX_PATH, "Z:\\data\\WindTerminal\\dpx\\");
		strcpy(TRADE_PATH, "Z:\\data\\WindDB\\production2\\trade\\");
		strcpy(POSITION_PATH, "Z:\\data\\WindDB\\production2\\position\\");
		strcpy(IPX_PATH, "Z:\\data\\WindTerminal\\ipx\\");
		strcpy(POS_SYS_PATH, "production2\\trade\\System\\");
		strcpy(TRADE_SYS_PATH, "production2\\trade\\System\\");

		strcpy(OUT_ACCOUNT_PATH, "Z:\\data\\WindDB\\production2\\Citics_dailyDetails\\our_account\\");
		strcpy(OUT_POSITION_PATH, "production2\\out_position\\");
		strcpy(OUT_TRADE_PATH, "production2\\out_trade\\");
		strcpy(OUT_POS_PATH, "production2\\out_pos\\");
		strcpy(OUT_TCOST_PATH, "production2\\out_tcost\\");
		strcpy(OUT_PNLPATH, "production2\\out_pnl\\");
		break;
	}
}


CiticsCompany::~CiticsCompany()
{
}

