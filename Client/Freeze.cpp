#include "StdAfx.h"
#include "Freeze.h"
#include "Player.h"
#include "Tile.h"
#include "Pipeline.h"
#include "CamMgr.h"
#include "ObjMgr.h"
#include "TimeMgr.h"
#include "BeatMgr.h"
#include "MathMgr.h"

CFreeze::CFreeze(void)
{
}

CFreeze::~CFreeze(void)
{
}

HRESULT CFreeze::Initialize()
{
	m_wstrObjKey = TEXT("FreezeSpell");
	m_wstrStateKey = TEXT("Normal");

	EffectInit();

	m_eObjId = OBJ_EFFECT;

	int iIndex = 0;

	for(int i = 1; i < 4; ++i)
	{
		for(int j = 0; j < (i<<1) - 1; ++j)
		{
			m_vDamagePos[iIndex] = D3DXVECTOR3((i - 1) * 2.f - (j * 2.f), 0.f, i * 2.f);

			++iIndex;
		}
	}

	m_vScale = D3DXVECTOR3(3.f, 1.f, 5.f);

	float fAngle = D3DXToDegree(((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY()) + 3;

	if(int(fAngle/90)%2 == 1 ||
		int(fAngle/90)%2 == -1)
		m_vScale = D3DXVECTOR3(m_vScale.z, 1.f, m_vScale.x);

	return S_OK;
}

const int CFreeze::Progress()
{
	m_tFrame.fFrame += m_tFrame.fCnt * GETTIME / m_pBeatMgr->GetBeatMax();

	SetPlayerMatrix(m_vScale);
	SetFrame(3.f);

	if(m_tFrame.fFrame >= m_tFrame.fMax)
		return 1;

	return 0;
}

void CFreeze::Render()
{
	EffectRender();
}

void CFreeze::SetPlayerMatrix(const D3DXVECTOR3& vScale)
{
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, vScale, m_fAngle, m_tInfo.vPos);

	SetBillBoard();

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

void CFreeze::SetDamageInfo(const D3DXVECTOR3& vPlayerPos, const float& fAngle)
{
	m_pCamMgr->SetShake(0.3f);

	m_vPlayerPos = vPlayerPos;

	D3DXVECTOR3 vDamagePos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vTile = D3DXVECTOR3(0.f, 0.f, 0.f);
	CTile* pTile = NULL;

	for(size_t i = 0; i < 9; ++i)
	{
		CMathMgr::MyRotationY(vDamagePos, m_vDamagePos[i], fAngle);

		vTile = vDamagePos + vPlayerPos; // 목표 타일 포스
		pTile = m_pObjMgr->GetTile(m_pObjMgr->GetIndex(vTile));

		if(pTile->GetTileObjId() == OBJ_MONSTER) // 목표 타일의 포인터
		{
			//pTile->GetObj()->SetDamage(0);
			pTile->GetObj()->SetFrozen(true);
		}
	}
}