#include "StdAfx.h"
#include "PlayerAttack.h"
#include "Pipeline.h"
#include "CamMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "TimeMgr.h"
#include "BeatMgr.h"

CPlayerAttack::CPlayerAttack(void)
{
}

CPlayerAttack::~CPlayerAttack(void)
{
}

HRESULT CPlayerAttack::Initialize()
{
	m_wstrObjKey = TEXT("PlayerAttack");
	m_wstrStateKey = TEXT("Normal");

	EffectInit();

	m_eObjId = OBJ_EFFECT;

	return S_OK;
}

const int CPlayerAttack::Progress()
{
	D3DXVECTOR3 vScale;

	m_tFrame.fFrame += m_tFrame.fCnt * GETTIME / m_pBeatMgr->GetBeatMax();

	SetEffactScale(vScale);
	SetPlayerMatrix(vScale);
	SetFrame(3.f);

	if(m_tFrame.fFrame >= m_tFrame.fMax)
		return 1;

	return 0;
}

void CPlayerAttack::Render()
{
	EffectRender();
}

void CPlayerAttack::SetPlayerMatrix(const D3DXVECTOR3& vScale)
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

void CPlayerAttack::SetEffactScale(D3DXVECTOR3& vScale)
{
	if(m_wstrStateKey == TEXT("Dagger"))
		vScale = D3DXVECTOR3(1.f, 1.f, 1.f);
	else if(m_wstrStateKey == TEXT("Broad"))
		vScale = D3DXVECTOR3(1.f, 1.f, 3.f);
	else if(m_wstrStateKey == TEXT("Spear"))
		vScale = D3DXVECTOR3(2.f, 1.f, 1.f);
	else if(m_wstrStateKey == TEXT("Whip_A")
		|| m_wstrStateKey == TEXT("Whip_B")
		|| m_wstrStateKey == TEXT("Whip_C")
		|| m_wstrStateKey == TEXT("Whip_D")
		|| m_wstrStateKey == TEXT("Whip_E"))
		vScale = D3DXVECTOR3(1.f, 1.f, 5.f);
	else if(m_wstrStateKey == TEXT("Blunderbuss"))
		vScale = D3DXVECTOR3(3.f, 1.f, 5.f);

	float fAngle = D3DXToDegree(((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY()) + 3.f;

	if(fAngle < 0)
		fAngle -= 6.f;

	if(int(fAngle/90)%2 == 1 ||
		int(fAngle/90)%2 == -1)
		vScale = D3DXVECTOR3(vScale.z, 1.f, vScale.x);
}