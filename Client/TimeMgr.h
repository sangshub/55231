#pragma once

#include "Include.h"

class CTimeMgr
{
public:
	DECLARE_SINGLETON(CTimeMgr)

private:
	CTimeMgr(void);

public:
	~CTimeMgr(void);

public:
	void InitTime(void);
	void SetTime(void);

public:
	const float GetTime(void) const;

private:
	float				m_fTime;

	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_CpuTick;
};