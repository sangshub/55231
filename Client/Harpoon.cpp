#include "stdafx.h"
#include "Harpoon.h"
#include "Cube.h"
#include "Octoboss.h"
#include "Device.h"
#include "ObjMgr.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "Pipeline.h"
#include "CamMgr.h"
#include "TimeMgr.h"
#include "BeatMgr.h"
#include "Player.h"
#include "BossStage.h"

CHarppon::CHarppon(void)
: m_bFire(false)
, m_Ending(false)
, m_iScreenMode(0)
, m_fTime(0.f)
, m_pBoss(NULL)
, m_pBossStage(NULL)
, m_vOriPos(D3DXVECTOR3(0.f, 0.f, 0.f))
{
}

CHarppon::~CHarppon(void)
{
	Release();
}

HRESULT CHarppon::Initialize(void)
{
	m_dwVtxCnt = 8;

	m_pCubeTex = new VTXCUBE[m_dwVtxCnt];
	m_pConvertCubeTex = new VTXCUBE[m_dwVtxCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("CubeTex"), m_pCubeTex);

	m_dwIndexCnt = 12;

	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetIndex(TEXT("CubeTex"), m_pIndex);

	m_eObjId = OBJ_EFFECT;

	LoadObject(TEXT("../Data/Harpoon.dat"));
	LoadAnimation(TEXT("../Data/Harpoon_Spin.dat"), STAT_IDLE);

	m_eStatKey = STAT_IDLE;
	m_dwState = 0;

	m_fAngle[ANGLE_Y] = D3DXToRadian(180.f);

	m_vOriPos = m_tInfo.vPos;

	return S_OK;
}

const int CHarppon::Progress(void)
{
	if(m_pBoss == NULL)
		return 1;

	SetMatrix();
	AniPlay(m_eStatKey);

	m_fAngle[ANGLE_Y] = ((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY() + D3DXToRadian(180.f);

	SetDirection();

	SetScreenMode();

	for(int i = 0; i < m_iChildCnt; ++i)
	{
		m_vecChild[i]->SetAniStateKey(m_eStatKey);
		m_vecChild[i]->Progress();
	}

	if(((CPlayer*)m_pObjMgr->GetPlayer())->GetState() & STATE_MAPJUMP)
	{
		if(!m_bFire)
			m_pDevice->SoundPlay(TEXT("Harpoon_Fire"), 0);

		m_bFire = true;
	}

	if(m_bFire)
	{
		m_tInfo.vPos -= m_tInfo.vDir * m_pTimeMgr->GetTime() * 30.f;

		m_fTime += m_pTimeMgr->GetTime();

		D3DXVECTOR3 vDir = D3DXVECTOR3(m_pBoss->GetInfo()->vPos.x, 0.f, m_pBoss->GetInfo()->vPos.z) - D3DXVECTOR3(m_tInfo.vPos.x, 0.f, m_tInfo.vPos.z);
		float fDistance = D3DXVec3Length(&vDir);

		if(m_fTime > 1.f && fDistance < 2.f)
		{
			m_pBoss->SetBossDamage();

			SetScreenMode();

			m_tInfo.vPos = m_vOriPos;
			m_bFire = false;
			m_fTime = 0.f;

			return 0;
		}

		if(m_fTime >= 3.f)
		{
			SetScreenMode();

			m_tInfo.vPos = m_vOriPos;
			m_bFire = false;
			m_fTime = 0.f;
		}

		if(!m_Ending && m_pBossStage && m_pBoss && m_pBoss->GetBossDamage() >= 10)
		{
			m_Ending = true;
 			m_pBossStage->EndingActionCamSetting(m_vOriPos);
		}
	}

	if(m_bIsDelete)
		return 1;

	return 0;
}

void CHarppon::Render(void)
{
	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pConvertCubeTex);

	m_pDevice->GetDevice()->SetTexture(0, m_pCubeTexture);

	BeginRenderState();

	m_pBufferMgr->RenderBuffer(TEXT("CubeTex"));

	for(int i = 0; i < m_iChildCnt; ++i)
		m_vecChild[i]->Render();

	EndRenderState();
}

void CHarppon::Release(void)
{
	for_each(m_vecChild.begin(), m_vecChild.end(), ::CDeleteObj());
	vector<CCube*>().swap(m_vecChild);

	for(int i = 0; i < STAT_END; ++i)
	{
		for_each(m_vecAni[i].begin(), m_vecAni[i].end(), ::CDeleteObj());
		vector<ANI*>().swap(m_vecAni[i]);
	}
}

const bool CHarppon::GetFire() const
{
	return m_bFire;
}

void CHarppon::SetBossStage(CBossStage* pBossStage)
{
	m_pBossStage = pBossStage;
}

void CHarppon::SetBoss(COctoboss* pBoss)
{
	m_pBoss = pBoss;
}

void CHarppon::SetMatrix(const float fScale)
{
	D3DXMATRIX matWorld;
	D3DXMATRIX matObjWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;

	CPipeline::MakeWorldMatrix(matWorld, D3DXVECTOR3(fScale, fScale, fScale), m_fAngle, m_tInfo.vPos);

	m_pCamMgr->GetMatrix(D3DTS_VIEW, matView);
	m_pCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);

	matObjWorld = m_matLocal * matWorld;
	m_tInfo.matWorld = m_matParents * matWorld;

	memcpy(m_pConvertCubeTex, m_pCubeTex, sizeof(VTXCUBE) * m_dwVtxCnt);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		//m_pConvertCubeTex[i] = m_pCubeTex[i];

		CPipeline::MyTransformCoord(m_pConvertCubeTex[i].vPos, m_pConvertCubeTex[i].vPos, matObjWorld);
		CPipeline::MyTransformCoord(m_pConvertCubeTex[i].vPos, m_pConvertCubeTex[i].vPos, matView);

		if(m_pConvertCubeTex[i].vPos.z < 1.f)
			m_pConvertCubeTex[i].vPos.z = 1.f;

		CPipeline::MyTransformCoord(m_pConvertCubeTex[i].vPos, m_pConvertCubeTex[i].vPos, matProj);
	}
}

void CHarppon::SetScreenMode()
{
	float fAngle = D3DXToDegree(((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY());

	if(fAngle >= 360.f)
	{
		int i = int(fAngle / 360.f);

		fAngle = fAngle - 360.f * i;
	}	

	if(fAngle < 0.f)
	{
		int i = int(-fAngle / 360.f);

		fAngle = 360.f * (i + 1) + fAngle;
	}

	if(fAngle == 360.f)
		fAngle = 0.f;

	if(fAngle >= 315.f || fAngle < 45.f)
	{
		m_iScreenMode = 0;
		m_vOriPos = D3DXVECTOR3(12.f, 6.f, -20.f);
	}
	else if(fAngle >= 45.f && fAngle < 135.f)
	{
		m_iScreenMode = 1;
		m_vOriPos = D3DXVECTOR3(-20.f, 6.f, 12.f);
	}
	else if(fAngle >= 135.f && fAngle < 225.f)
	{
		m_iScreenMode = 2;
		m_vOriPos = D3DXVECTOR3(12.f, 6.f, 46.f);
	}
	else if(fAngle >= 225.f && fAngle < 315.f)
	{
		m_iScreenMode = 3;
		m_vOriPos = D3DXVECTOR3(46.f, 6.f, 12.f);
	}
}