#pragma once

int isPNG(const unsigned char* critical_fragment)
{
	if (critical_fragment[0] == 137 && critical_fragment[1] == 80 && critical_fragment[2] == 78 && critical_fragment[3] == 71 &&
		critical_fragment[4] == 13 && critical_fragment[5] == 10 && critical_fragment[6] == 26 && critical_fragment[7] == 10)
	{
		return 1;
	}
	return 0;
}

int isIHDR(const unsigned char* critical_fragment)
{
	if (critical_fragment[0] == 0 && critical_fragment[1] == 0 && critical_fragment[2] == 0 && critical_fragment[3] == 13 &&
		critical_fragment[4] == 73 && critical_fragment[5] == 72 && critical_fragment[6] == 68 && critical_fragment[7] == 82)
	{
		return 1;
	}
	return 0;
}

int isPLTE(const unsigned char* critical_fragment)
{
	if (critical_fragment[0] == 80 && critical_fragment[1] == 76 && critical_fragment[2] == 84 && critical_fragment[3] == 69)
	{
		return 1;
	}
	return 0;
}

int isIDAT(const unsigned char* critical_fragment)
{
	if (critical_fragment[0] == 73 && critical_fragment[1] == 68 && critical_fragment[2] == 65 && critical_fragment[3] == 84)
	{
		return 1;
	}
	return 0;
}

int isIEND(const unsigned char* critical_fragment)
{
	if (critical_fragment[0] == 73 && critical_fragment[1] == 69 && critical_fragment[2] == 78 && critical_fragment[3] == 68)
	{
		return 1;
	}
	return 0;
}
