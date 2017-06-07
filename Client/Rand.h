#pragma once

#define R 16

class CRand
{
public:
	CRand();
	~CRand();

private:
	static double WELLRNG512a();

public:
	static void InitWELLRNG512a(const unsigned int *init);
	static int Random(const int iMinimum, const int iMaximum);
	static bool Percentage(const int iPercent);

private:
	static unsigned int state_i;
	static unsigned int STATE[R];
	static unsigned int z0, z1, z2;
};