#pragma once

class CMathMgr
{
public:
	CMathMgr(void);
	~CMathMgr(void);

public:
	static void MyRotationX(D3DXVECTOR3& vOut, const D3DXVECTOR3& vIn, const float fAngle);
	static void MyRotationY(D3DXVECTOR3& vOut, const D3DXVECTOR3& vIn, const float fAngle);
	static void MyRotationZ(D3DXVECTOR3& vOut, const D3DXVECTOR3& vIn, const float fAngle);
};
