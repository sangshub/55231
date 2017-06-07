#include "StdAfx.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr(void)
: m_fTime(0.f)
{
}

CTimeMgr::~CTimeMgr(void)
{
}

void CTimeMgr::InitTime(void)
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);

	QueryPerformanceFrequency(&m_CpuTick);
}

void CTimeMgr::SetTime(void)
{
	QueryPerformanceCounter(&m_FrameTime);

	if(m_FrameTime.QuadPart - m_LastTime.QuadPart > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	m_fTime = float(m_FrameTime.QuadPart - m_FixTime.QuadPart)/ float(m_CpuTick.QuadPart);

	m_FixTime = m_FrameTime;
}

const float CTimeMgr::GetTime(void) const
{
	return m_fTime;
}