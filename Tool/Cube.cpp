#include "StdAfx.h"
#include "Cube.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "CamMgr.h"
#include "Device.h"
#include "Pipeline.h"
#include "MainCamMgr.h"

CCube::CCube(void)
: m_MainCamMgr(CMainCamMgr::GetInstance())
, m_bToolView(false)
, m_bAniView(false)
, m_bAniTool(false)
{
}

CCube::~CCube(void)
{
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

	ZeroMemory(m_fAngle, sizeof(float)*3);
	ZeroMemory(&m_tCube, sizeof(m_tCube));

	return S_OK;
}

int CCube::Progress(void)
{
	AniPlay();
	SetMatrix();

	if(m_bIsDelete)
		return 1;

	return 0;
}

void CCube::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pDevice->GetDevice()->SetTexture(0, m_pCubeTexture);
	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pConvertCubeTex);
	m_pBufferMgr->RenderBuffer(TEXT("CubeTex"));
}

void CCube::SetMatrix(void)
{
	if(!m_bAniTool)
		CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(1.f, 1.f, 1.f), m_fAngle, m_tInfo.vPos);

	D3DXMATRIX	matWorld;
	if(m_matParents)
		matWorld = m_tInfo.matWorld * (*m_matParents);
	else
		matWorld = m_tInfo.matWorld;

	D3DXMATRIX matView;
	D3DXMATRIX matProj;

	if(m_bToolView)
	{
		m_pCamMgr->GetMatrix(D3DTS_VIEW, matView);
		m_pCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);
	}
	else if(m_bAniView)
	{
		m_pCamMgr->GetAniMatrix(D3DTS_VIEW, matView);
		m_pCamMgr->GetAniMatrix(D3DTS_PROJECTION, matProj);
	}
	else
	{
		m_MainCamMgr->GetMatrix(D3DTS_VIEW, matView);
		m_MainCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);
	}

	memcpy(m_pConvertCubeTex, m_pCubeTex, sizeof(VTXCUBE)*m_dwVtxCnt);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		CPipeline::MyTransformCoord(m_pConvertCubeTex[i].vPos, m_pConvertCubeTex[i].vPos, matWorld);
		CPipeline::MyTransformCoord(m_pConvertCubeTex[i].vPos, m_pConvertCubeTex[i].vPos, matView);

		if(m_pConvertCubeTex[i].vPos.z < 1.f)
			m_pConvertCubeTex[i].vPos.z = 1.f;

		CPipeline::MyTransformCoord(m_pConvertCubeTex[i].vPos, m_pConvertCubeTex[i].vPos, matProj);
	}
}

void CCube::SetCubeInfo(const CUBE _tCube)
{
	m_tCube.iDrawId = _tCube.iDrawId;
	m_tCube.dwType = _tCube.dwType;
	m_tCube.vPos = _tCube.vPos;
	m_tInfo.vPos = _tCube.vPos;

	SetCubeTexture(m_tCube.iDrawId);
}

void CCube::SetCubeTexture(const int iDrawId)
{
	m_pCubeTexture = m_pTextureMgr->GetCubeTex(iDrawId);
	m_tCube.iDrawId = iDrawId;
}

void CCube::SetToolViewSwitch(bool _bToolView)
{
	m_bToolView = _bToolView;
}

void CCube::SetAniViewSwitch(bool _bAniView)
{
	m_bAniView = _bAniView;
}

void CCube::SetWorldMatrix(const D3DXMATRIX& _matWorld)
{
	m_tInfo.matWorld = _matWorld;
}

void CCube::SetCubeVertex(VTXCUBE* _pCubeTex)
{
	memcpy(m_pCubeTex, _pCubeTex, sizeof(VTXCUBE)*8);
}

void CCube::SetParentsMatrix(D3DXMATRIX* _matParents)
{
	m_matParents = _matParents;
}

void CCube::SetAniToolMode(bool _bAniTool)
{
	m_bAniTool = _bAniTool;
}

void CCube::SetChildMatrix(const D3DXMATRIX& _matWorld)
{
	m_matChild = _matWorld;
}
