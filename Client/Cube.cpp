#include "stdafx.h"
#include "Cube.h"
#include "Tile.h"
#include "Player.h"
#include "Device.h"
#include "ObjMgr.h"
#include "CamMgr.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "Pipeline.h"
#include "ParticleMgr.h"
#include "Rand.h"

CCube::CCube(void)
: m_bIsMon(false)
{
}

CCube::~CCube(void)
{
	Release();
}

HRESULT CCube::Initialize(void)
{
	m_dwVtxCnt = 8;
	m_dwIndexCnt = 12;

	m_pCubeTex = new VTXCUBE[m_dwVtxCnt];
	m_pConvertCubeTex = new VTXCUBE[m_dwVtxCnt];
	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("CubeTex"), m_pCubeTex);
	m_pBufferMgr->GetIndex(TEXT("CubeTex"), m_pIndex);

	ZeroMemory(m_fAngle, sizeof(float) * 3);
	ZeroMemory(&m_tCube, sizeof(m_tCube));

	m_eObjId = OBJ_CUBE;

	return S_OK;
}

const int CCube::Progress(void)
{
	if(m_pmatParents)
		AniPlay(m_eStatKey);
	else
		MegaJump();

	SetMatrix();
	GetPlayerToDistance();
	StageEndScene(); //엔딩
	
	if(m_bIsDelete)
	{
		m_pObjMgr->ResetCubeTex();

		if(!m_bStageEnd)
			m_pPaticleMgr->CtreateParticle(m_tInfo.vPos, 8, TEXT("CubeTex"), 0.2f, 10, 38, 0.6f, 1.5f, 0.7f, m_tCube.iDrawId, D3DXVECTOR3(0.f, 0.f,0.f), true);
		else if(CRand::Random(0, 2))
		{
			int iSound = 0;
			TCHAR szSoundName[MIN_STR] = TEXT("");
			iSound = CRand::Random(0, 1);
			wsprintf(szSoundName, TEXT("StageEnd%d"), iSound);
			m_pDevice->SoundPlay(szSoundName, 0);
		}

		if(!m_bStageEnd 
			&& m_tCube.dwType & CUBE_EVEN1) // 골드 드랍
			m_pObjMgr->GoldDrop(m_tInfo.vPos, 10);
		return 1;
	}
	return 0;
}

void CCube::Render(void)
{
	/*m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);*/

	BeginRenderState();
	SetDark(m_iDarkness);

	if(m_pmatParents != NULL && !m_bIsMon)
		SetDark(255);

	m_pDevice->GetDevice()->SetTexture(0, m_pCubeTexture);
	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pConvertCubeTex);
	m_pBufferMgr->RenderBuffer(TEXT("CubeTex"));

	EndRenderState();
}

void CCube::SetMatrix(const float fScale)
{
	D3DXMATRIX matView;
	D3DXMATRIX matProj;

	if(m_pmatParents == NULL)
		CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale, fScale, fScale), m_fAngle, m_tInfo.vPos);
	else
		m_tInfo.matWorld = m_matLocal * (*m_pmatParents);

	m_pCamMgr->GetMatrix(D3DTS_VIEW, matView);
	m_pCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);
	
	memcpy(m_pConvertCubeTex, m_pCubeTex, sizeof(VTXCUBE) * m_dwVtxCnt);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		CPipeline::MyTransformCoord(m_pConvertCubeTex[i].vPos, m_pConvertCubeTex[i].vPos, m_tInfo.matWorld);
		CPipeline::MyTransformCoord(m_pConvertCubeTex[i].vPos, m_pConvertCubeTex[i].vPos, matView);

		if(m_pConvertCubeTex[i].vPos.z < 1.f)
			m_pConvertCubeTex[i].vPos.z = 1.f;

		CPipeline::MyTransformCoord(m_pConvertCubeTex[i].vPos, m_pConvertCubeTex[i].vPos, matProj);
	}
}

void CCube::SetCubeInfo(const CUBE& _tCube)
{
	m_tCube.iDrawId = _tCube.iDrawId;
	m_tCube.dwType = _tCube.dwType;
	m_tCube.vPos = _tCube.vPos;
	m_tInfo.vPos = _tCube.vPos;

	m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->SetObj(this);

	SetCubeTexture(m_tCube.iDrawId);

	if(m_tCube.dwType & CUBE_THRO)
		m_pvCamDir = ((CPlayer*)m_pObjMgr->GetPlayer())->GetCamDirection();
}

void CCube::Release(void)
{
	for(int i = 0; i < STAT_END; ++i)
	{
		for_each(m_vecAni[i].begin(), m_vecAni[i].end(), CDeleteObj());
		vector<ANI*>().swap(m_vecAni[i]);
	}

	m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->ResetObj();
}

void CCube::SetIsMon()
{
	m_bIsMon = true;
}