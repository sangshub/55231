#pragma once
#include "ui.h"

class CDiamond_Num :
	public CUI
{
public:
	CDiamond_Num(void);
	virtual ~CDiamond_Num(void);
public:
	virtual HRESULT Initialize(void);
	virtual const int Progress(void);
	virtual void Render(void);
private:
	virtual void Release(void);

public:
	void DrawNumber(int iNum, int iDigit);
	int NumberCheck(int iNum);

public:
	void SetDiamond_Num(int iNum);

private:
	int m_iDiaNum;



};
