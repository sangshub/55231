#pragma once

#include "Include.h"

class CObj;
class CTimeMgr;
class CMouse;
class CBufferMgr;
class CCubeTool;
class CObjTool;
class CItemTool;
class CEtcTool;
class CKeyMgr;
class CFrustum;

class CTileMap
{
public:
	DECLARE_SINGLETON(CTileMap)

private:
	CTileMap();

public:
	~CTileMap();

public:
	const int GetTileX() const;
	const int GetTileZ() const;
	const vector<TILE*>* GetTileInfo() const;
	vector<CObj*> GetFullTile() const;
	void SetTileInfo(const vector<TILE*>& vecTile);
	void SetDrawId(const int iDrawId);

public:
	HRESULT Initialize();
	void Progress();
	void Render();

private:
	void Release();

public:
	void CreateNewMap(const int iTileX, const int iTileZ, const int iDrawId);
	void InfoReset();
	void SetResetVtx();
	void SetCubeMode(bool _bCubeMode);
	void SetObjMode(bool _bObjMode);
	void SetItemMode(const bool bItemMode);
	void SetEtcMode(const bool bEtcMode);

private:
	void Twinkle();

private:
	bool	m_bTwinkle;
	bool	m_bCubeMode;
	bool	m_bObjMode;
	bool	m_bItemMode;
	bool	m_bEtcMode;
	int		m_iTileX;
	int		m_iTileZ;
	int		m_iDrawId;
	float	m_fTime;

	CTimeMgr*	m_pTimeMgr;
	CMouse*		m_pMouse;
	CBufferMgr* m_pBufferMgr;
	CCubeTool*	m_pCubeTool;
	CObjTool*	m_pObjTool;
	CItemTool*  m_pItemTool;
	CEtcTool*	m_pEtcTool;
	CKeyMgr*	m_pKeyMgr;
	CFrustum*	m_pFrustum;
	VTXTEX*		m_pVtxTex;
	VTXTEX*		m_pVtxTex2;

	vector<CObj*> m_vecTile;
	vector<CObj*> m_vecFullTile;
	vector<TILE*> m_vecTileInfo;
};