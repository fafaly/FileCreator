#pragma once
#include <stdio.h>

namespace GlobalFunc
{
	FILE *F_OPEN(char *fname, char *mode, int log_no, char *fdate);
}