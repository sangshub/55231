#pragma once

#include "UI.h"

class CBar : public CUI
{
public:
	CBar(void);
	virtual ~CBar(void);

public:
	virtual HRESULT Initialize(void);
	virtual const int Progress(void);
	virtual void Render(void);

private:
	virtual void Release(void);

public:
	void MoveDir(void);
	void SetBeatDir(BEATDIR _eBeatDir);
	void SetVtxTex(VTXTEX* pVtxTex);
	void SetProgress(int iProgress);
	int	 GetProgress(void);
	BEATDIR GetBeatDir(void);
	int EraseBar();


private:
	BEATDIR		m_eBeatDir;
	int			m_iProgress;
	int			m_iAlpha;
	bool		m_bRender;



};