#include "stdafx.h"
#include "Tentacle.h"
#include "Cube.h"
#include "Device.h"
#include "ObjMgr.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "Pipeline.h"
#include "CamMgr.h"
#include "TimeMgr.h"
#include "BeatMgr.h"
#include "Player.h"

CTentacle::CTentacle(void)
: m_bShort(false)
{
}

CTentacle::~CTentacle(void)
{
	Release();
}

HRESULT CTentacle::Initialize(void)
{
	m_dwVtxCnt = 8;

	m_pCubeTex = new VTXCUBE[m_dwVtxCnt];
	m_pConvertCubeTex = new VTXCUBE[m_dwVtxCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("CubeTex"), m_pCubeTex);

	m_dwIndexCnt = 12;

	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetIndex(TEXT("CubeTex"), m_pIndex);

	m_eObjId = OBJ_EFFECT;

	LoadObject(TEXT("../Data/test.dat"));
	LoadAnimation(TEXT("../Data/Idle.dat"), STAT_IDLE);
	LoadAnimation(TEXT("../Data/Out.dat"), STAT_OUT);
	LoadAnimation(TEXT("../Data/Smash.dat"), STAT_ATT);
	LoadAnimation(TEXT("../Data/In.dat"), STAT_IN);
	LoadAnimation(TEXT("../Data/OutAttack.dat"), STAT_OUTATT);
	LoadAnimation(TEXT("../Data/Hide.dat"), STAT_HIDE);
	LoadAnimation(TEXT("../Data/DDR.dat"), STAT_DDR);

	m_eStatKey = STAT_IDLE;
	m_dwState = 0;

	return S_OK;
}

const int CTentacle::Progress(void)
{
	SetMatrix();
	BossAniPlay(m_eStatKey);

	for(int i = 0; i < m_iChildCnt; ++i)
	{
		m_vecChild[i]->SetAniStateKey(m_eStatKey);
		m_vecChild[i]->Progress();
	}

	if(m_bShort)
	{
		if(m_eStatKey == STAT_IDLE)
			return 1;
	}

	if(m_bIsDelete)
		return 1;

	return 0;
}

void CTentacle::Render(void)
{
	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pConvertCubeTex);

	m_pDevice->GetDevice()->SetTexture(0, m_pCubeTexture);

	BeginRenderState();

	m_pBufferMgr->RenderBuffer(TEXT("CubeTex"));

	for(int i = 0; i < m_iChildCnt; ++i)
		m_vecChild[i]->Render();

	EndRenderState();
}

void CTentacle::Release(void)
{
	for_each(m_vecChild.begin(), m_vecChild.end(), ::CDeleteObj());
	vector<CCube*>().swap(m_vecChild);

	for(int i = 0; i < STAT_END; ++i)
	{
		for_each(m_vecAni[i].begin(), m_vecAni[i].end(), ::CDeleteObj());
		vector<ANI*>().swap(m_vecAni[i]);
	}
}

void CTentacle::SetShort()
{
	m_bShort = true;
}

void CTentacle::SetMatrix(const float fScale)
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