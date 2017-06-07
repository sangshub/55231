#include "stdafx.h"
#include "Octoboss.h"
#include "Tentacle.h"
#include "Player.h"
#include "Tile.h"
#include "PushLeft.h"
#include "PushRight.h"
#include "PushDown.h"
#include "PushUp.h"
#include "Warning.h"
#include "BigDust.h"
#include "Arrow.h"
#include "Perfect.h"
#include "Missed.h"
#include "BossHp.h"
#include "Device.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "Pipeline.h"
#include "CamMgr.h"
#include "TimeMgr.h"
#include "BeatMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Rand.h"

struct DeleteObj
{
	void operator()(CObj* pObj)
	{
		pObj->SetDelete();
	}
};

COctoboss::COctoboss(void)
: m_bInit(false)
, m_iCount(0)
, m_iTileSize(0)
, m_iPattrenCount(0)
, m_iPattrenRand(0)
, m_iAppearType(0)
, m_iDamage(0)
, m_fShakeTime(0.f)
, m_dwWant(0)
, m_dwBossState(BOSS_IDLE)
{
	ZeroMemory(m_iRand, sizeof(int) * 4);
}

COctoboss::~COctoboss(void)
{
	Release();
}

HRESULT COctoboss::Initialize(void)
{
	m_dwVtxCnt = 4;

	m_pVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pConvertVtxTex = new VTXTEX[m_dwVtxCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("UITex"), m_pVtxTex);

	m_dwIndexCnt = 2;

	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetIndex(TEXT("UITex"), m_pIndex);

	m_wstrObjKey = TEXT("Octoboss");
	m_wstrStateKey = TEXT("Normal");

	m_eObjId = OBJ_MONSTER;

	m_vecFullTile = m_pObjMgr->GetFullTile();

	m_iTileSize = m_vecFullTile.size();

	m_vecHp.reserve(10);

	for(int i = 0; i < 10; ++i)
	{
		CBossHp* pHp = (CBossHp*)CObjFactory<CBossHp>::CreateObj(D3DXVECTOR3(-14.f + i * 3.f, -10.f, 0.f));
		m_pObjMgr->AddObj(OBJ_EFFECT, pHp);
		m_vecHp.push_back(pHp);
	}

	m_vecTentacle.reserve(8);
	m_vecTrap.reserve(20);

	m_pDevice->SoundPlay(TEXT("Boss_Init"), 0);
	m_pDevice->SoundPlay(TEXT("Boss_Welcome"), 0);
	
	return S_OK;
}

const int COctoboss::Progress(void)
{
	SetMatrix();

	if(!m_bInit)
	{
		InitTentacle();

		return 0;
	}

	BeatClock();

	if(m_dwBossState & BOSS_IDLE)
	{
		Shake();

		switch(m_iPattrenRand)
		{
		case 1:
			TentacleSmash();
			break;
		case 2:
			ChaseTentacle();
			break;
		case 3:
			DDR();
			break;
		}

		if(m_iPattrenCount == 0)
		{
			int iRand = CRand::Random(0, 1);

			switch(iRand)
			{
			case 0:
				m_pDevice->SoundPlay(TEXT("Boss_Pattren1"), 0);
				break;
			case 1:
				m_pDevice->SoundPlay(TEXT("Boss_Pattren2"), 0);
				break;
			}

			if(m_pBeatMgr->GetBeatBoomBoom())
			{
				m_iPattrenRand = CRand::Random(1, 3);

				m_iCount = 0;

				TrapRelease();

				if(m_iPattrenRand == 1)
				{
					for(int i = 0; i < m_iTileSize; ++i)
					{
						if(m_iAppearType == 0 || m_iAppearType == 2)
						{
							if(m_vecFullTile[i]->GetInfo()->vPos.x == 10.f)
							{
								CObj* pTrap = CObjFactory<CPushLeft>::CreateObj(m_vecFullTile[i]->GetInfo()->vPos + D3DXVECTOR3(0.f, 0.3f, 0.f));
								m_pObjMgr->AddObj(OBJ_TRAP, pTrap);
								m_vecTrap.push_back(pTrap);
							}
							if(m_vecFullTile[i]->GetInfo()->vPos.x == 12.f)
							{
								CObj* pTrap = CObjFactory<CPushRight>::CreateObj(m_vecFullTile[i]->GetInfo()->vPos + D3DXVECTOR3(0.f, 0.3f, 0.f));
								m_pObjMgr->AddObj(OBJ_TRAP, pTrap);
								m_vecTrap.push_back(pTrap);
							}
						}
						else
						{
							if(m_vecFullTile[i]->GetInfo()->vPos.z == 10.f)
							{
								CObj* pTrap = CObjFactory<CPushDown>::CreateObj(m_vecFullTile[i]->GetInfo()->vPos + D3DXVECTOR3(0.f, 0.3f, 0.f));
								m_pObjMgr->AddObj(OBJ_TRAP, pTrap);
								m_vecTrap.push_back(pTrap);
							}
							if(m_vecFullTile[i]->GetInfo()->vPos.z == 12.f)
							{
								CObj* pTrap = CObjFactory<CPushUp>::CreateObj(m_vecFullTile[i]->GetInfo()->vPos + D3DXVECTOR3(0.f, 0.3f, 0.f));
								m_pObjMgr->AddObj(OBJ_TRAP, pTrap);
								m_vecTrap.push_back(pTrap);
							}
						}
					}
				}
			}
		}
	}
	if(m_dwBossState & BOSS_HIDE)
	{
		TrapRelease();
		Hide();
	}
	if(m_dwBossState & BOSS_APPEAR)
		Appear();
	if(m_dwBossState & BOSS_DIE)
	{
		m_fAngle[ANGLE_Y] = D3DXToRadian(20.f) + ((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY();
		m_fAngle[ANGLE_X] = D3DXToRadian(20.f);

		m_pCamMgr->SetShake(0.1f, 0.1f);

		m_tInfo.vPos.y -= m_fShakeTime * m_pTimeMgr->GetTime() * 5.f;
		//m_tInfo.vPos.z += m_fShakeTime * m_pTimeMgr->GetTime() * 3.f;

		for(int i = 0; i < 8; ++i)
		{
			m_vecTentacle[i]->SetAniStateKey(STAT_IDLE);

			m_vecTentacle[i]->SetPos(D3DXVECTOR3(m_vecTentacle[i]->GetInfo()->vPos.x
				, m_vecTentacle[i]->GetInfo()->vPos.y - m_fShakeTime * m_pTimeMgr->GetTime() * 5.f
				, m_vecTentacle[i]->GetInfo()->vPos.z));// + m_fShakeTime * m_pTimeMgr->GetTime() * 3.f));
		}

		if(m_tInfo.vPos.y < -2.f)
			m_tInfo.vPos.y = -2.f;
	}

	if(m_bIsDelete)
		return 1;

	return 0;
}

void COctoboss::Render(void)
{
	m_pBufferMgr->SetVtxInfo(TEXT("UITex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey, m_wstrStateKey, TEXT(""), (int)m_tFrame.fFrame);

	if(pTexInfo == NULL)
		return;

	BeginRenderState();

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);

	m_pBufferMgr->RenderBuffer(TEXT("UITex"));

	EndRenderState();
}

void COctoboss::Release()
{
	for_each(m_vecTentacle.begin(), m_vecTentacle.end(), DeleteObj());
	for_each(m_vecTrap.begin(), m_vecTrap.end(), DeleteObj());
	for_each(m_vecHp.begin(), m_vecHp.end(), DeleteObj());

	vector<CTentacle*>().swap(m_vecTentacle);
	vector<CObj*>().swap(m_vecFullTile);
	vector<CObj*>().swap(m_vecTrap);
	vector<CBossHp*>().swap(m_vecHp);
}

void COctoboss::SetMatrix(const float fScale)
{
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale * 1.79f * 3.f, fScale * 2.79f * 3.f, fScale), m_fAngle, m_tInfo.vPos);

	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	m_pCamMgr->GetMatrix(D3DTS_VIEW, matView);
	m_pCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);

	memcpy(m_pConvertVtxTex, m_pVtxTex, sizeof(VTXTEX) * m_dwVtxCnt);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		//m_pConvertVtxTex[i] = m_pVtxTex[i];

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, m_tInfo.matWorld);
		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matView);

		if(m_pConvertVtxTex[i].vPos.z < 1.f)
			m_pConvertVtxTex[i].vPos.z = 1.f;

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matProj);
	}
}

const int COctoboss::GetBossDamage() const
{
	return m_iDamage;
}

void COctoboss::SetBossDamage()
{
	if(m_iDamage < 10)
		m_vecHp[m_iDamage]->SetDrawId(1);

	m_pCamMgr->SetShake(0.3f, 0.3f);

	++m_iDamage;

	m_pDevice->SoundPlay(TEXT("Boss_Hit"), 0);

	if(m_iDamage == 2 || m_iDamage == 4 || m_iDamage == 6 || m_iDamage == 9)
		m_dwBossState = BOSS_HIDE;

	if(m_iDamage >= 11)
	{
		m_dwBossState = BOSS_DIE;

		m_pDevice->SoundPlay(TEXT("Boss_Death"), 0);
	}
}

void COctoboss::InitTentacle()
{
	m_tInfo.vPos.y += m_pTimeMgr->GetTime() * 2.f;

	if(m_tInfo.vPos.y > 4.f)
	{
		m_pObjMgr->ResetCubeTex();

		for(int i = 0; i < 4; ++i)
		{
			CTentacle* pTentacle = (CTentacle*)CObjFactory<CTentacle>::CreateObj(D3DXVECTOR3(2.f + i * 2, 0.f, 24.f));
			m_pObjMgr->AddObj(OBJ_MONSTER, pTentacle);
			pTentacle->SetAniStateKey(STAT_OUT);
			m_vecTentacle.push_back(pTentacle);
		}
		for(int i = 0; i < 4; ++i)
		{
			CTentacle* pTentacle = (CTentacle*)CObjFactory<CTentacle>::CreateObj(D3DXVECTOR3(14.f + i * 2, 0.f, 24.f));
			m_pObjMgr->AddObj(OBJ_MONSTER, pTentacle);
			pTentacle->SetAniStateKey(STAT_OUT);
			m_vecTentacle.push_back(pTentacle);
		}
		m_bInit = true;
	}
}

void COctoboss::BeatClock()
{
	if(m_pBeatMgr->GetBeatBoomBoom())
		++m_iCount;

	if(m_iCount == 8)
		m_iCount = 0;
}

void COctoboss::Shake()
{
	m_fShakeTime += m_pTimeMgr->GetTime();

	float fBeatTime = 0.5f / m_pBeatMgr->GetBeatMax();

	if(m_fShakeTime < fBeatTime * 0.5f)
		m_tInfo.vPos.y += m_pTimeMgr->GetTime() * 2.f;
	else if(m_fShakeTime < fBeatTime)
	{
		m_tInfo.vPos.y -= m_pTimeMgr->GetTime() * 2.f;

		if(m_tInfo.vPos.y < 4.f)
			m_tInfo.vPos.y = 4.f;
	}
	else
	{
		m_tInfo.vPos.y = 4.f;
		m_fShakeTime = 0.f;
	}
}

void COctoboss::Hide()
{
	m_fShakeTime += m_pTimeMgr->GetTime();

	if(m_fShakeTime < 1.f)
		m_tInfo.vPos.y -= m_pTimeMgr->GetTime() * 20.f;
	else
	{
		m_tInfo.vPos.y = -10.f;
		m_fShakeTime = 0.f;

		m_iAppearType = CRand::Random(0, 3);

		m_dwBossState = BOSS_APPEAR;

		for(int i = 0; i < 8; ++i)
		{
			m_vecTentacle[i]->SetAniStateKey(STAT_HIDE);
		}
		m_pDevice->SoundPlay(TEXT("Boss_In"), 0);
	}
}

void COctoboss::Appear()
{
	switch(m_iAppearType)
	{
	case 0:
		m_tInfo.vPos.x = 11.f;
		m_tInfo.vPos.z = 24.f;
		m_fAngle[ANGLE_Y] = D3DXToRadian(0.f);
		break;
	case 1:
		m_tInfo.vPos.x = 24.f;
		m_tInfo.vPos.z = 11.f;
		m_fAngle[ANGLE_Y] = D3DXToRadian(90.f);
		break;
	case 2:
		m_tInfo.vPos.x = 11.f;
		m_tInfo.vPos.z = -2.f;
		m_fAngle[ANGLE_Y] = D3DXToRadian(180.f);
		break;
	case 3:
		m_tInfo.vPos.x = -2.f;
		m_tInfo.vPos.z = 11.f;
		m_fAngle[ANGLE_Y] = D3DXToRadian(270.f);
		break;
	}

	m_fShakeTime += m_pTimeMgr->GetTime();

	if(m_fShakeTime < 1.f)
	{
		m_tInfo.vPos.y += m_pTimeMgr->GetTime() * 20.f;

		if(m_tInfo.vPos.y >= 4.f)
			m_tInfo.vPos.y = 4.f;
	}
	else
	{
		m_tInfo.vPos.y = 4.f;
		m_fShakeTime = 0.f;

		m_dwBossState = BOSS_IDLE;

		for(int i = 0; i < 8; ++i)
		{
			m_vecTentacle[i]->SetAniStateKey(STAT_OUT);

			switch(m_iAppearType)
			{
			case 0:
				for(int i = 0; i < 4; ++i)
				{
					m_vecTentacle[i]->SetPos(D3DXVECTOR3(2.f + i * 2, 0.f, 24.f));
					m_vecTentacle[i]->SetAngle(D3DXToRadian(0.f));
				}
				for(int i = 4; i < 8; ++i)
				{
					m_vecTentacle[i]->SetPos(D3DXVECTOR3(6.f + i * 2, 0.f, 24.f));
					m_vecTentacle[i]->SetAngle(D3DXToRadian(0.f));
				}
				break;
			case 1:
				for(int i = 0; i < 4; ++i)
				{
					m_vecTentacle[i]->SetPos(D3DXVECTOR3(24.f, 0.f, 2.f + i * 2));
					m_vecTentacle[i]->SetAngle(D3DXToRadian(90.f));
				}
				for(int i = 4; i < 8; ++i)
				{
					m_vecTentacle[i]->SetPos(D3DXVECTOR3(24.f, 0.f, 6.f + i * 2));
					m_vecTentacle[i]->SetAngle(D3DXToRadian(90.f));
				}
				break;
			case 2:
				for(int i = 0; i < 4; ++i)
				{
					m_vecTentacle[i]->SetPos(D3DXVECTOR3(2.f + i * 2, 0.f, -2.f));
					m_vecTentacle[i]->SetAngle(D3DXToRadian(180.f));
				}
				for(int i = 4; i < 8; ++i)
				{
					m_vecTentacle[i]->SetPos(D3DXVECTOR3(6.f + i * 2, 0.f, -2.f));
					m_vecTentacle[i]->SetAngle(D3DXToRadian(180.f));
				}
				break;
			case 3:
				for(int i = 0; i < 4; ++i)
				{
					m_vecTentacle[i]->SetPos(D3DXVECTOR3(-2.f, 0.f, 2.f + i * 2));
					m_vecTentacle[i]->SetAngle(D3DXToRadian(270.f));
				}
				for(int i = 4; i < 8; ++i)
				{
					m_vecTentacle[i]->SetPos(D3DXVECTOR3(-2.f, 0.f, 6.f + i * 2));
					m_vecTentacle[i]->SetAngle(D3DXToRadian(270.f));
				}
				break;
			}
		}
		m_pDevice->SoundPlay(TEXT("Boss_Out"), 0);
	}
}

void COctoboss::TrapRelease()
{
	int iSize = m_vecTrap.size();

	for(int i = 0; i < iSize; ++i)
	{
		m_vecTrap[i]->SetDelete();
	}
	vector<CObj*>().swap(m_vecTrap);
}

void COctoboss::TentacleSmash()
{
	if(m_pBeatMgr->GetBeatBoomBoom())
	{
		++m_iPattrenCount;

		if(m_iCount == 1)
		{
			for(int i = 0; i < 4; ++i)
			{
				m_iRand[i] = CRand::Random(0, 7);
			}
			
			for(int i = 0; i < 4; ++i)
			{
				m_vecTentacle[m_iRand[i]]->SetAniStateKey(STAT_ATT);

				CWarning* pWarning = NULL;

				switch(m_iAppearType)
				{
				case 0:
					pWarning = (CWarning*)CObjFactory<CWarning>::CreateObj(m_vecTentacle[m_iRand[i]]->GetInfo()->vPos + D3DXVECTOR3(0.f, 0.f, -2.f));
					m_pObjMgr->AddObj(OBJ_EFFECT, pWarning);
					break;
				case 1:
					pWarning = (CWarning*)CObjFactory<CWarning>::CreateObj(m_vecTentacle[m_iRand[i]]->GetInfo()->vPos + D3DXVECTOR3(-2.f, 0.f, 0.f));
					m_pObjMgr->AddObj(OBJ_EFFECT, pWarning);
					break;
				case 2:
					pWarning = (CWarning*)CObjFactory<CWarning>::CreateObj(m_vecTentacle[m_iRand[i]]->GetInfo()->vPos + D3DXVECTOR3(0.f, 0.f, 2.f));
					m_pObjMgr->AddObj(OBJ_EFFECT, pWarning);
					break;
				case 3:
					pWarning = (CWarning*)CObjFactory<CWarning>::CreateObj(m_vecTentacle[m_iRand[i]]->GetInfo()->vPos + D3DXVECTOR3(2.f, 0.f, 0.f));
					m_pObjMgr->AddObj(OBJ_EFFECT, pWarning);
					break;
				}
				pWarning->SetType(m_iAppearType);
			}
		}
		if(m_iCount == 2)
		{
			for(int i = 0; i < m_iTileSize; ++i)
			{
				for(int j = 0; j < 4; ++j)
				{
					if(m_iAppearType == 0 || m_iAppearType == 2)
					{
						if(m_vecFullTile[i]->GetInfo()->vPos.x == m_vecTentacle[m_iRand[j]]->GetInfo()->vPos.x)
						{
							m_pObjMgr->AddObj(OBJ_EFFECT, CObjFactory<CBigDust>::CreateObj(m_vecFullTile[i]->GetInfo()->vPos));

							if(((CTile*)m_vecFullTile[i])->GetTileObjId() == OBJ_PLAYER)
								m_pObjMgr->GetPlayer()->SetDamage(1);
						}
					}
					else
					{
						if(m_vecFullTile[i]->GetInfo()->vPos.z == m_vecTentacle[m_iRand[j]]->GetInfo()->vPos.z)
						{
							m_pObjMgr->AddObj(OBJ_EFFECT, CObjFactory<CBigDust>::CreateObj(m_vecFullTile[i]->GetInfo()->vPos));

							if(((CTile*)m_vecFullTile[i])->GetTileObjId() == OBJ_PLAYER)
							{
								m_pObjMgr->GetPlayer()->SetDamage(1);

								m_pDevice->SoundPlay(TEXT("Boss_Attack1"), 0);
							}
						}
					}
				}
				m_pDevice->SoundPlay(TEXT("Boss_Smash"), 0);
			}
			m_pCamMgr->SetShake(0.3f, 0.3f);
			m_pObjMgr->SetJump(true);
			m_iCount = 0;
		}
		if(m_iPattrenCount == 8)
			m_iPattrenCount = 0;
	}
}

void COctoboss::ChaseTentacle()
{
	if(m_pBeatMgr->GetBeatBoomBoom())
	{
		++m_iPattrenCount;

		if(m_iCount == 1)
		{
			m_vecTentacle[CRand::Random(0, 7)]->SetAniStateKey(STAT_IN);

			m_vecOldPlayerPos = m_pObjMgr->GetPlayer()->GetInfo()->vPos;
			m_pObjMgr->AddObj(OBJ_EFFECT, CObjFactory<CBigDust>::CreateObj(m_vecOldPlayerPos));
		}
		if(m_iCount == 2)
		{
			m_pObjMgr->ResetCubeTex();

			CTentacle* pTentacle = (CTentacle*)CObjFactory<CTentacle>::CreateObj(m_vecOldPlayerPos);
			m_pObjMgr->AddObj(OBJ_MONSTER, pTentacle);
			pTentacle->SetAniStateKey(STAT_OUTATT);
			pTentacle->SetShort();

			if(((CTile*)m_vecFullTile[m_pObjMgr->GetIndex(m_vecOldPlayerPos)])->GetTileObjId() == OBJ_PLAYER)
			{
				m_pObjMgr->GetPlayer()->SetDamage(1);

				m_pDevice->SoundPlay(TEXT("Boss_Attack2"), 0);
			}

			m_iCount = 0;

			m_pDevice->SoundPlay(TEXT("Boss_Chase"), 0);
		}
		if(m_iPattrenCount == 8)
			m_iPattrenCount = 0;
	}
}

void COctoboss::DDR()
{
	if(m_pBeatMgr->GetBeatBoomBoom())
	{
		++m_iPattrenCount;

		if(m_iCount == 1)
		{
			for(int i = 0; i < 8; ++i)
			{
				m_vecTentacle[i]->SetAniStateKey(STAT_DDR);
			}

			int iRand = CRand::Random(0, 5);

			switch(iRand)
			{
			case 0:
				m_dwWant |= KEY_LEFT;
				break;
			case 1:
				m_dwWant |= KEY_RIGHT;
				break;
			case 2:
				m_dwWant |= KEY_UP;
				break;
			case 3:
				m_dwWant |= KEY_DOWN;
				break;
			case 4:
				m_dwWant |= KEY_LEFT;
				m_dwWant |= KEY_SPACE;
				break;
			case 5:
				m_dwWant |= KEY_RIGHT;
				m_dwWant |= KEY_SPACE;
				break;
			}
			CArrow* pArrow = (CArrow*)CObjFactory<CArrow>::CreateObj();
			m_pObjMgr->AddObj(OBJ_EFFECT, pArrow);
			pArrow->SetDrawId(iRand);
		}

		if(m_iCount == 3)
		{
			DWORD dwCommand = ((CPlayer*)m_pObjMgr->GetPlayer())->GetCommand();

			if(dwCommand != m_dwWant)
			{
				m_pObjMgr->GetPlayer()->SetDamage(1);

				m_pObjMgr->ResetCubeTex();

				CTentacle* pTentacle = (CTentacle*)CObjFactory<CTentacle>::CreateObj(m_pObjMgr->GetPlayer()->GetInfo()->vPos);
				m_pObjMgr->AddObj(OBJ_MONSTER, pTentacle);
				pTentacle->SetAniStateKey(STAT_OUTATT);
				pTentacle->SetShort();

				m_pObjMgr->AddObj(OBJ_EFFECT, CObjFactory<CMissed>::CreateObj());
				m_pCamMgr->SetShake(0.2f, 0.2f);

				m_pDevice->SoundPlay(TEXT("Boss_Fail"), 0);
				m_pDevice->SoundPlay(TEXT("Boss_Chase"), 0);
				m_pDevice->SoundPlay(TEXT("Boss_Attack2"), 0);
			}
			else
			{
				m_pObjMgr->AddObj(OBJ_EFFECT, CObjFactory<CPerfect>::CreateObj());
				m_pObjMgr->SetTwinkle(true);

				m_pDevice->SoundPlay(TEXT("Boss_Ok"), 0);
			}
			((CPlayer*)m_pObjMgr->GetPlayer())->SetCommand(0);
			m_dwWant = 0;
			m_iCount = 0;
		}
		if(m_iPattrenCount == 12)
		{
			for(int i = 0; i < 8; ++i)
			{
				m_vecTentacle[i]->SetAniStateKey(STAT_IDLE);
			}
			m_iPattrenCount = 0;
		}
	}
}