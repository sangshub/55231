#pragma once

#include "Include.h"

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9 pTexture;
	D3DXIMAGE_INFO ImgInfo;
}TEXINFO;

typedef struct tagVertexColor 
{
	D3DXVECTOR3 vPos;
	DWORD dwColor;
}VTXCOL;

const DWORD VTXFVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE;

typedef struct tagVertexTexture
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR2 vTex;
}VTXTEX;

const DWORD VTXFVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;

typedef struct tagVertexCube
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vTex;
}VTXCUBE;

const DWORD VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

typedef struct tagIndex
{
	DWORD _1;
	DWORD _2;
	DWORD _3;
}INDEX;

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXMATRIX	matWorld;
}INFO;

typedef struct tagTile // 타일 저장 정보
{
	int iDrawId;
	int iOption;
	int iCubeCnt;
	int iObjCnt;
	D3DXVECTOR3 vPos;
}TILE;

typedef struct tagItem // 아이템 저장 정보
{
	TCHAR szName[MIN_STR];
	D3DXVECTOR3 vPos;
	bool bSell;
}ITEMDATA;

typedef struct tagCubeInfo // 큐브 인포
{
	D3DXVECTOR3 vPos;
	DWORD		dwType; // 큐브의 타입
	int			iDrawId; // 텍스쳐키 값
}CUBE;

typedef struct tagMonsterInfo
{
	D3DXVECTOR3 vPos;
	TCHAR	szObjkey[MIN_STR];

}SLIME;

typedef struct tagImagePath
{
	int iCount;
	wstring wstrObjKey;
	wstring wstrStateKey;
	wstring wstrDirectionKey;
	wstring wstrPath;
}IMGPATH;

typedef struct tagCubePosData
{
	int			iImageNum;
	float		fTime;
	float		fPosX;
	float		fPosY;
	float		fPosZ;
	float		fScaleX;
	float		fScaleY;
	float		fScaleZ;
	float		fAngle[ANGLE_END];
}CUBEDATA;

typedef struct tagAniData
{
	float			fTime;
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vScale;
	D3DXQUATERNION	qRot;
}ANI;

typedef struct tagFrame
{
	float		fFrame;
	float		fCnt;
	float		fMax;

	tagFrame(void) {}
	tagFrame(float _frame, float _fcnt, float _fmax)
		: fFrame(_frame), fCnt(_fcnt), fMax(_fmax) {}
}FRAME;

typedef struct tagUnitInfo
{
	int iAtt;
	int iHp;
	int iMaxHp;
	int iDef;
	int	iMoney;
	int	iDiamond;

public:
	tagUnitInfo(){}
	tagUnitInfo(int _iAtt, int _iHp, int _iDef)
		:iAtt(_iAtt), iHp(_iHp), iMaxHp((_iHp+1)/2), iDef(_iDef) {}
}UNIT;

typedef struct tagConnect
{
	bool bIsConnect;
	CONNECTWHERE eDir;
	D3DXVECTOR3 vPos;
}CONNECT;

typedef struct tagMapInfo
{
	int iTileX;
	int iTileZ;
	float fModifyX;
	float fModifyZ;
	wstring wstrMapPath;
	vector<CONNECT*> vecConnect;
}MAP;

typedef struct tagOBB
{
	D3DXVECTOR3		vCenterPos;
	D3DXVECTOR3		vAxisDir[3];
	float			fAxisLen[3];
}OBB;

typedef struct tagActionCamera
{
	D3DXVECTOR3 vEye;
	D3DXVECTOR3 vAt;
	float		fTime;

public:
	tagActionCamera(void) {}
	tagActionCamera(const D3DXVECTOR3& _vEye, const D3DXVECTOR3& _vAt, const float _fTime)
		:vEye(_vEye), vAt(_vAt), fTime(_fTime)
	{}
	~tagActionCamera(void) {}
}ACTION;