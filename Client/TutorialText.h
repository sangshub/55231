#pragma once

#include "Effect.h"

class CTutorialText : public CEffect
{
public:
	CTutorialText(void);
	~CTutorialText(void);

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

public:
	void  TextRender(const int iImageNum);
	void  TextSetMatrix(const float fX, const float fZ);

public:
	void SetTextImageNumSize(const int iImageNum, const float fSizeX, const float fSizeY);
public:
	int		m_iImageNum;
	float	m_fSizeX;
	float	m_fSizeY;
};