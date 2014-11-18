#include "stdafx.h"
#include "DataLog.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace GlobalFunc
{
	FILE *F_OPEN(char *fname, char *mode, int log_no, char *fdate)
	{
		FILE *fp = fopen(fname, mode);
		char logstr[100];
		if (!fp)
		{
			DataLog dl(1, log_no, fdate, 3);
			sprintf(logstr, "%s is not existed.Check it please!", fname);
			dl.WriteLog(logstr);
			exit(1);
		}
		else
		{
			return fp;
		}
	}
}