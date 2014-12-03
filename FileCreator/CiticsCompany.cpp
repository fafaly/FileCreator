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
		strcpy(TRADE_PATH, "Z:\\data\\WindDB\\production\\trade\\auto\\");
		strcpy(POSITION_PATH, "Z:\\data\\WindDB\\production\\position\\auto\\");
		strcpy(IPX_PATH, "Z:\\data\\WindTerminal\\ipx\\");
		strcpy(POS_SYS_PATH, "Z:\\data\\WindDB\\production\\trade\\System\\auto\\");
		strcpy(TRADE_SYS_PATH, "Z:\\data\\WindDB\\production\\trade\\System\\auto\\");

		strcpy(OUT_ACCOUNT_PATH, "Z:\\data\\WindDB\\production\\Citics_dailyDetails\\our_account\\");
		strcpy(OUT_POSITION_PATH, "Z:\\data\\WindDB\\production\\position\\auto\\");
		strcpy(OUT_TRADE_PATH, "Z:\\data\\WindDB\\production\\trade\\auto\\");
		strcpy(OUT_TCOST_PATH, "Z:\\data\\WindDB\\production\\tcost\\");
		strcpy(OUT_PNLPATH, "Z:\\data\\WindDB\\production\\pnl\\");
		break;
	case 2:
		strcpy(DPX_PATH, "Z:\\data\\WindTerminal\\dpx\\");
		strcpy(TRADE_PATH, "Z:\\data\\WindDB\\production2\\trade\\");
		strcpy(POSITION_PATH, "Z:\\data\\WindDB\\production2\\position\\");
		strcpy(IPX_PATH, "Z:\\data\\WindTerminal\\ipx\\");
		strcpy(POS_SYS_PATH, "Z:\\data\\WindDB\\production2\\trade\\System\\auto\\");
		strcpy(TRADE_SYS_PATH, "Z:\\data\\WindDB\\production2\\trade\\System\\auto\\");

		strcpy(OUT_ACCOUNT_PATH, "Z:\\data\\WindDB\\production2\\Citics_dailyDetails\\our_account\\");
		strcpy(OUT_POSITION_PATH, "Z:\\data\\WindDB\\production2\\position\\auto\\");
		strcpy(OUT_TRADE_PATH, "Z:\\data\\WindDB\\production2\\trade\\auto\\");
		strcpy(OUT_TCOST_PATH, "Z:\\data\\WindDB\\production2\\tcost\\");
		strcpy(OUT_PNLPATH, "Z:\\data\\WindDB\\production2\\pnl\\");
		break;
	}
}


CiticsCompany::~CiticsCompany()
{
}

