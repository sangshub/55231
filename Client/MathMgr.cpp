#include "StdAfx.h"
#include "MathMgr.h"

CMathMgr::CMathMgr(void)
{
}

CMathMgr::~CMathMgr(void)
{
}

void CMathMgr::MyRotationX(D3DXVECTOR3& vOut, const D3DXVECTOR3& vIn, const float fAngle)
{
	D3DXVECTOR3 vTemp = vIn;

	vOut.x = vTemp.x;
	vOut.y = vTemp.y * cosf(fAngle) - vTemp.z * sinf(fAngle);
	vOut.z = vTemp.y * sinf(fAngle) + vTemp.z * cosf(fAngle);
}

void CMathMgr::MyRotationY(D3DXVECTOR3& vOut, const D3DXVECTOR3& vIn, const float fAngle)
{
	D3DXVECTOR3 vTemp = vIn;

	vOut.x = vTemp.x * cosf(fAngle) + vTemp.z * sinf(fAngle);
	vOut.y = vTemp.y;
	vOut.z = vTemp.x * -sinf(fAngle) + vTemp.z * cosf(fAngle);
}

void CMathMgr::MyRotationZ(D3DXVECTOR3& vOut, const D3DXVECTOR3& vIn, const float fAngle)
{
	D3DXVECTOR3 vTemp = vIn;

	vOut.x = vTemp.x * cosf(fAngle) - vTemp.y * sinf(fAngle);
	vOut.y = vTemp.x * sinf(fAngle) + vTemp.y * cosf(fAngle);
	vOut.z = vTemp.z;
}
