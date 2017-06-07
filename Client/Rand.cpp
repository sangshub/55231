#include "stdafx.h"
#include "Rand.h"

#define W 32
//#define R 16
#define P 0
#define M1 13
#define M2 9
#define M3 5

#define MAT0POS(t,v) (v^(v>>t))
#define MAT0NEG(t,v) (v^(v<<(-(t))))
#define MAT3NEG(t,v) (v<<(-(t)))
#define MAT4NEG(t,b,v) (v ^ ((v<<(-(t))) & b))

#define V0            STATE[state_i]
#define VM1           STATE[(state_i+M1) & 0x0000000fU]
#define VM2           STATE[(state_i+M2) & 0x0000000fU]
#define VM3           STATE[(state_i+M3) & 0x0000000fU]
#define VRm1          STATE[(state_i+15) & 0x0000000fU]
#define VRm2          STATE[(state_i+14) & 0x0000000fU]
#define newV0         STATE[(state_i+15) & 0x0000000fU]
#define newV1         STATE[state_i]
#define newVRm1       STATE[(state_i+14) & 0x0000000fU]

#define FACT 2.32830643653869628906e-10

unsigned int CRand::state_i = 0;
unsigned int CRand::STATE[R] = {0};
unsigned int CRand::z0 = 0;
unsigned int CRand::z1 = 0;
unsigned int CRand::z2 = 0;

CRand::CRand()
{
}

CRand::~CRand()
{
}

void CRand::InitWELLRNG512a(const unsigned int *init)
{
	state_i = 0;

	for(int j = 0; j < R; ++j)
	{
		STATE[j] = init[j];
	}
}

double CRand::WELLRNG512a()
{
	z0    = VRm1;
	z1    = MAT0NEG (-16,V0)    ^ MAT0NEG (-15, VM1);
	z2    = MAT0POS (11, VM2)  ;
	newV1 = z1                  ^ z2; 
	newV0 = MAT0NEG (-2,z0)     ^ MAT0NEG(-18,z1)    ^ MAT3NEG(-28,z2) ^ MAT4NEG(-5,0xda442d24U,newV1) ;
	state_i = (state_i + 15) & 0x0000000fU;

	return ((double) STATE[state_i]) * FACT;
}

int CRand::Random(const int iMinimum, const int iMaximum)
{
	return int(WELLRNG512a() * (iMaximum + 1 - iMinimum) + iMinimum);
}

bool CRand::Percentage(const int iPercent)
{
	double dRand = Random(0, 100);

	if(dRand <= iPercent)
		return true;

	return false;
}

// Rand 매니저 쓰는 방법
// CRand::Random(0, 100) : 0 ~ 100 사이의 랜덤 숫자(int)를 받음(0과 100 포함, 음수 가능)
// CRand::Percentage(70) : 70% 확률로 true 반환