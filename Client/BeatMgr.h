#pragma once

#include "Include.h"

class CBeatMgr
{
public:
	DECLARE_SINGLETON(CBeatMgr)

private:
	CBeatMgr(void);

public:
	~CBeatMgr(void);

public:
	void Initialize(void);
	bool DropTheBeat();

public:
	const float GetBeatMax(void) const;
	void SetBeatMax(const float fCnt);
	const bool GetBeatBoomBoom(void) const;
	void SetBeatFrame(const FRAME& tBeat);
	void SetBeatFrame(const BEATSTAGE& eBeat);

private:
	bool	m_bBeat;
	bool	m_bRest;
	int		m_iRestTiming;
	BEATSTAGE m_eBeatType;
	FRAME	m_tBeat;
};