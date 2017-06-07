#pragma once
#include "UI.h"

class CCoin_Num :
	public CUI
{
public:
	CCoin_Num(void);
	virtual ~CCoin_Num(void);

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
	void SetCoin_Num(int iNum);

private:
	int m_iCoinNum;

};
