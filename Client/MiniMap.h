#pragma once
#include "ui.h"

class CObjMgr;
class CMiniMap :
	public CUI
{
public:
	CMiniMap(void);
	virtual ~CMiniMap(void);

public:
	virtual HRESULT Initialize(void);
	virtual const int Progress(void);
	virtual void Render(void);

private:
	virtual void Release(void);
	void DrawPoint(int iImage, float fX, float fY);
	void DrawPoint2(int iImage, float fX, float fY);
private:
	CObjMgr*		m_pObjMgr;

	vector<CObj*>	m_vecTile;
	list<CObj*>*	m_pCubeList;
	list<CObj*>*	m_pItemList;
	list<CObj*>*	m_pMonsterList;
	list<CObj*>*	m_pEtcList;
	list<CObj*>*	m_pTrapList;
	CObj*			m_pPlayer;
	float			m_fMapSizeX;
	float			m_fMapSizeY;

	int				m_iRnd;

	DWORD			m_dwREF[14];

	bool			m_bOnOff;




};
