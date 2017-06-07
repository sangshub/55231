#include "StdAfx.h"
#include "BeatMgr.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CBeatMgr)

CBeatMgr::CBeatMgr(void)
: m_bRest(false)
, m_bBeat(false)
, m_iRestTiming(1)
{
	ZeroMemory(&m_tBeat, sizeof(FRAME));
}

CBeatMgr::~CBeatMgr(void)
{
}

void CBeatMgr::Initialize(void)
{
	//m_tBeat.fCnt = 1.f;
	m_tBeat.fFrame = 0.f;
	m_tBeat.fMax= 0.51f;		//zone1_1;
	//m_tBeat.fMax= 0.6f;		//튜토리얼;
}

bool CBeatMgr::DropTheBeat()
{
	m_tBeat.fFrame += /*m_tBeat.fCnt */ GETTIME;

	if(m_tBeat.fFrame > m_tBeat.fMax)
	{
		m_tBeat.fFrame = 0.f;

		if(m_bRest)
		{
			if(m_iRestTiming == 7)
			{
				m_iRestTiming = 0;
				m_bBeat = false;

				return m_bBeat;
			}
			else
				++m_iRestTiming;	
		}	
		m_bBeat = true;

		return m_bBeat;
	}
	m_bBeat = false;

	return m_bBeat;
}

void CBeatMgr::SetBeatFrame(const FRAME& tBeat)
{
	m_tBeat = tBeat;
}

void CBeatMgr::SetBeatFrame(const BEATSTAGE& eBeat)
{
	switch(eBeat)
	{
	case BEAT_TUTORIAL:
		m_tBeat.fMax = 0.595f;	//Tuto	
		m_bRest = 0;
		m_iRestTiming = 1;
		break;
	case BEAT_ZONE1_1:
		m_tBeat.fMax = 0.512f;	//zone1_1;
		m_bRest = 0;
		m_iRestTiming = 1;
		break;
	case BEAT_ZONE2_1:
		m_tBeat.fMax = 0.437f;	//zone2_1;
		m_bRest = 0;
		m_iRestTiming = 1;
		break;
	case BEAT_ZONE3_1:
		m_tBeat.fMax = 0.377f;	//zone3_1;
		m_bRest = 0;
		m_iRestTiming = 1;
		break;
	case BEAT_CONGA:
		m_tBeat.fMax = 0.487f;		//콩가(7박하고 쉬고) ;
		m_bRest = 1;
		m_iRestTiming = 4;
		break;
	}
}

const float CBeatMgr::GetBeatMax(void) const
{
	return m_tBeat.fMax;
}

void CBeatMgr::SetBeatMax(const float fMax)
{
	m_tBeat.fMax = fMax;
}

const bool CBeatMgr::GetBeatBoomBoom(void) const
{
	return m_bBeat;
}