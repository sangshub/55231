#pragma once

#include "UI.h"

class CHeart :	public CUI
{
public:
	CHeart(void);
	virtual ~CHeart(void);

public:
	virtual HRESULT Initialize(void);
	virtual const int Progress(void);
	virtual void Render(void);

private:
	virtual void Release(void);

public:
	void MakeBeatBar();

private:
	unsigned int m_iPlaying;

};