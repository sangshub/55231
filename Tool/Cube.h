#pragma once

#include "Obj.h"

class CMainCamMgr;
class CCube
	:public CObj
{
public:
	CCube(void);
	~CCube(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void);
	virtual void Render(void);

public:
	void SetCubeInfo(const CUBE	_tCube);
	void SetCubeTexture(const int iDrawId);
	void SetToolViewSwitch(bool _bToolView);
	void SetAniViewSwitch(bool _bAniView);
	void SetWorldMatrix(const D3DXMATRIX& _matWorld);
	void SetCubeVertex(VTXCUBE* _pCubeTex);
	void SetParentsMatrix(D3DXMATRIX* _matParents);
	void SetChildMatrix(const D3DXMATRIX& _matWorld);
	void SetAniToolMode(bool _bAniTool);
	CUBE* GetCubeInfo(void);

private:
	virtual void SetMatrix(void);

private:
	bool			m_bAniTool;
	bool			m_bToolView;
	bool			m_bAniView;
	CMainCamMgr*	m_MainCamMgr;

private:
	CUBE					m_tCube; // 저장용도
	LPDIRECT3DCUBETEXTURE9	m_pCubeTexture;
};
