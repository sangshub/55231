#include "stdafx.h"
#include "Golem.h"
#include "Tile.h"
#include "Cube.h"
#include "SmallHeart.h"
#include "Device.h"
#include "BufferMgr.h"
#include "BeatMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Pipeline.h"
#include "CamMgr.h"
#include "ParticleMgr.h"
#include "ComboMgr.h"

CGolem::CGolem()
{
}

CGolem::~CGolem()
{
	Release();
}

HRESULT CGolem::Initialize()
{
	m_tUnitInfo = UNIT(4, 10, 0);

	m_dwVtxCnt = 8;

	m_pCubeTex = new VTXCUBE[m_dwVtxCnt];
	m_pConvertCubeTex = new VTXCUBE[m_dwVtxCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("CubeTex"), m_pCubeTex);

	m_dwIndexCnt = 12;

	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetIndex(TEXT("CubeTex"), m_pIndex);

	m_eObjId = OBJ_MONSTER;

	LoadObject(TEXT("../Data/Golem.dat"));
	LoadAnimation(TEXT("../Data/Golem_Idle.dat"), STAT_IDLE);

	m_eStatKey = STAT_IDLE;
	m_dwState = 0;

	for(int i = 0; i < m_tUnitInfo.iMaxHp; ++i)
	{
		CSmallHeart* pHp = (CSmallHeart*)CObjFactory<CSmallHeart>::CreateObj();
		m_pObjMgr->AddObj(OBJ_EFFECT, pHp);
		m_vecHp.push_back(pHp);
	}

	m_bCubeBreak = true;
	m_bIsFly = true;

	m_fScale = 1.5f;

	/*m_fBaseY = 2.5f;
	m_fMegaY = 2.5f;*/
	m_fJumpPow = 0.3f;

	m_strAttackSound = TEXT("Golem_Attack");
	m_strHitSound = TEXT("Golem_Hit");
	m_strDeathSound = TEXT("Golem_Death");
	m_strMoveSound = TEXT("Golem_Move");

	for(int i = 0; i < m_iChildCnt; ++i)
	{
		m_vecChild[i]->SetIsMon();
	}

	return S_OK;
}

const int CGolem::Progress()
{
	SetMatrix(m_fScale);
	AniPlay(m_eStatKey);

	if(!m_bInit)
	{
		D3DXVECTOR3 m_vDir = m_pObjMgr->GetPlayer()->GetInfo()->vPos - m_tInfo.vPos;

		float fDistance = D3DXVec3Length(&m_vDir);

		if(fDistance > 14.f)
			return 0;

		int iIndex = m_pObjMgr->GetIndex(m_tInfo.vPos);

		m_iOldIndex = iIndex;

		m_bInit = true;

		m_pDevice->SoundPlay(m_strInitSound, 0);
	}

	if(m_tUnitInfo.iHp <= 0)
	{
		m_pDevice->SoundPlay(m_strDeathSound, 0);

		m_pObjMgr->ResetRcCol();
		m_pPaticleMgr->CtreateParticle(m_tInfo.vPos, 12, TEXT("RcCol"), 0.4f*(int)m_fScale, 20*(int)m_fScale, 50*(int)m_fScale, 0.0f, 1.5f*m_fScale, 0.7f, m_dwColor, m_vDamageDir, true, true);

		if(m_wstrObjKey != TEXT("Monkey"))
			m_pObjMgr->GoldDrop(m_tInfo.vPos);
		if(GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN)	|| GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(VK_LEFT))
			m_pComboMgr->AddCombo();

		m_bIsDelete = true;
	}

	m_iDarkness = m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->GetDarkness();

	BeatClock();

	SetScreenMode();
	StageEndScene();

	m_fScale = 3.f;

	HeartRender();

	m_fScale = 1.5f;

	SetFrozenTimer();

	if(m_bFreeze)
		m_iCount = 0;

	for(int i = 0; i < m_iChildCnt; ++i)
	{
		m_vecChild[i]->SetAniStateKey(m_eStatKey);
		m_vecChild[i]->Progress();
		m_vecChild[i]->SetDarkness(m_iDarkness);
	}

	if(m_bIsDelete)
		return 1;

	if(m_bFreeze)
		return 0;

	Pattren();
	Jump();
	MegaJump();

	if(m_bBeat)
		Move(m_vTarget);

	return 0;
}

void CGolem::Render()
{
	//MonsterRender();

	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pConvertCubeTex);

	m_pDevice->GetDevice()->SetTexture(0, m_pCubeTexture);

	BeginRenderState();
	SetDark(m_iDarkness);

	m_pBufferMgr->RenderBuffer(TEXT("CubeTex"));

	for(int i = 0; i < m_iChildCnt; ++i)
		m_vecChild[i]->Render();

	EndRenderState();
}

void CGolem::Pattren()
{
	if(m_pBeatMgr->GetBeatBoomBoom())
	{
		if(m_iCount == 4)
		{
			m_bJump = true;

			MoveChase();

			m_iCount = 0;
		}
	}
}

void CGolem::Release()
{
	for_each(m_vecChild.begin(), m_vecChild.end(), ::CDeleteObj());
	vector<CCube*>().swap(m_vecChild);

	for(int i = 0; i < STAT_END; ++i)
	{
		for_each(m_vecAni[i].begin(), m_vecAni[i].end(), ::CDeleteObj());
		vector<ANI*>().swap(m_vecAni[i]);
	}
}

void CGolem::SetMatrix(const float fScale)
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