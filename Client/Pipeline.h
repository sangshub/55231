#pragma once

class CPipeline
{
public:
	CPipeline(void);
	~CPipeline(void);

public:
	static void MakeWorldMatrix(D3DXMATRIX& matOut, const D3DXVECTOR3& vScale
		, const float* pAngle, const D3DXVECTOR3& vTempPos);

	static void MakeViewSpaceMatrix(D3DXMATRIX& matOut
		, const D3DXVECTOR3& vEye
		, const D3DXVECTOR3& vAt
		, const D3DXVECTOR3& vTempUp);

	static void MakeProjectionMatrix(D3DXMATRIX& matOut
		, const float& fFovY
		, const float& fAspect
		, const float& fNear
		, const float& fFar);

	static void MyTransformCoord(D3DXVECTOR3& vOut, const D3DXVECTOR3& vVector
		, const D3DXMATRIX& matMatrix);

	static void MakeTransformMatrix(D3DXMATRIX& matOut
		, const D3DXVECTOR3& vRight
		, const D3DXVECTOR3& vUp
		, const D3DXVECTOR3& vLook
		, const D3DXVECTOR3& vPos);
};