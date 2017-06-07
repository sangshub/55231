#pragma once

#include "Obj.h"

class CEtc : public CObj
{
public:
	CEtc();
	virtual ~CEtc();

public:
	const bool GetIsItem() const;

public:
	virtual HRESULT Initialize() PURE;
	virtual const int Progress() PURE;
	virtual void Render() PURE;

protected:
	virtual void SetMatrix(const float fScale = 1.f);
	void EtcInit();
	void EtcProgress();
	void EtcRender();

private:
	void ToShadow();

protected:
	bool m_bSprite;
	bool m_bGetItem;
	float m_fScale;
	float m_fFrameSpeed;
};