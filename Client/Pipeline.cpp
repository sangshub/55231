#include "StdAfx.h"
#include "Pipeline.h"
#include "MathMgr.h"
#include "Include.h"

CPipeline::CPipeline(void)
{
}

CPipeline::~CPipeline(void)
{
}

void CPipeline::MakeWorldMatrix(D3DXMATRIX& matOut
								, const D3DXVECTOR3& vScale 
								, const float* pAngle
								, const D3DXVECTOR3& vTempPos)
{
	D3DXMatrixIdentity(&matOut);

	D3DXVECTOR3	vRight		= D3DXVECTOR3(1.f, 0.f, 0.f);
	D3DXVECTOR3	vUp			= D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXVECTOR3	vLook		= D3DXVECTOR3(0.f, 0.f, 1.f);
	D3DXVECTOR3	vPos		= D3DXVECTOR3(0.f, 0.f, 0.f);

	vRight	*= vScale.x;
	vUp		*= vScale.y;
	vLook	*= vScale.z;

	CMathMgr::MyRotationX(vUp, vUp, pAngle[ANGLE_X]);
	CMathMgr::MyRotationX(vLook, vLook, pAngle[ANGLE_X]);

	CMathMgr::MyRotationY(vRight, vRight, pAngle[ANGLE_Y]);
	CMathMgr::MyRotationY(vLook, vLook, pAngle[ANGLE_Y]);

	CMathMgr::MyRotationZ(vRight, vRight, pAngle[ANGLE_Z]);
	CMathMgr::MyRotationZ(vUp, vUp, pAngle[ANGLE_Z]);

	vPos = vTempPos;

	MakeTransformMatrix(matOut, vRight, vUp, vLook, vPos);
}

void CPipeline::MakeViewSpaceMatrix(D3DXMATRIX& matOut , const D3DXVECTOR3& vEye , const D3DXVECTOR3& vAt , const D3DXVECTOR3& vTempUp)
{
	D3DXMatrixIdentity(&matOut);

	D3DXVECTOR3	vRight	= D3DXVECTOR3(1.f, 0.f, 0.f);
	D3DXVECTOR3	vUp		= D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXVECTOR3	vLook	= D3DXVECTOR3(0.f, 0.f, 1.f);
	D3DXVECTOR3	vPos	= D3DXVECTOR3(0.f, 0.f, 0.f);

	vLook = vAt - vEye;
	D3DXVec3Normalize(&vLook, &vLook);

	D3DXVec3Cross(&vRight, &vTempUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	vPos = vEye;

	MakeTransformMatrix(matOut, vRight, vUp, vLook, vPos);

	D3DXMatrixInverse(&matOut, 0, &matOut);
}

void CPipeline::MakeProjectionMatrix(D3DXMATRIX& matOut , const float& fFovY , const float& fAspect , const float& fNear , const float& fFar)
{
	D3DXMatrixIdentity(&matOut);

	matOut._11 = (1.f / tanf(fFovY * 0.5f))/ fAspect;
	matOut._22 = 1.f / tanf(fFovY * 0.5f);
	matOut._33 = fFar / (fFar - fNear);
	matOut._43 = -fNear * fFar / (fFar - fNear);

	matOut._34 = 1.f;
	matOut._44 = 0.f;
}

void CPipeline::MyTransformCoord(D3DXVECTOR3& vOut, const D3DXVECTOR3& vTemp , const D3DXMATRIX& matMatrix)
{
	D3DXVECTOR4		vVector(vTemp.x, vTemp.y, vTemp.z, 1.f);

	vOut.x = vVector.x * matMatrix._11 
		+ vVector.y * matMatrix._21
		+ vVector.z * matMatrix._31
		+ vVector.w * matMatrix._41;

	vOut.y = vVector.x * matMatrix._12 
		+ vVector.y * matMatrix._22
		+ vVector.z * matMatrix._32
		+ vVector.w * matMatrix._42;

	vOut.z = vVector.x * matMatrix._13
		+ vVector.y * matMatrix._23
		+ vVector.z * matMatrix._33
		+ vVector.w * matMatrix._43;

	float	fZ = vVector.x * matMatrix._14
		+ vVector.y * matMatrix._24
		+ vVector.z * matMatrix._34
		+ vVector.w * matMatrix._44;

	vOut.x /= fZ;
	vOut.y /= fZ;
	vOut.z /= fZ;
}

void CPipeline::MakeTransformMatrix(D3DXMATRIX& matOut , const D3DXVECTOR3& vRight , const D3DXVECTOR3& vUp , const D3DXVECTOR3& vLook , const D3DXVECTOR3& vPos)
{
	memcpy(&matOut.m[0][0], vRight, sizeof(float) * 3);
	memcpy(&matOut.m[1][0], vUp,	sizeof(float) * 3);
	memcpy(&matOut.m[2][0], vLook,	sizeof(float) * 3);
	memcpy(&matOut.m[3][0], vPos,	sizeof(float) * 3);
}