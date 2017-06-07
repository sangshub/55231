#include "stdafx.h"
#include "Heart.h"

#include "BeatMgr.h"
#include "Bar.h"
#include "ObjMgr.h"
CHeart::CHeart(void)
:m_iPlaying(0)
{

}

CHeart::~CHeart(void)
{
	Release();
}

HRESULT CHeart::Initialize(void)
{
	m_dwVtxCnt = 4;
	m_pVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pConvertVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pBufferMgr->GetVtxInfo(TEXT("UITex"), m_pVtxTex);

	m_dwIndexCnt = 2;
	m_pIndex = new INDEX[m_dwIndexCnt];
	m_pBufferMgr->GetIndex(TEXT("UITex"), m_pIndex);

	m_wstrObjKey = TEXT("Heart");
	m_wstrStateKey = TEXT("Normal");

	m_tInfo.vPos = D3DXVECTOR3(0.f, -5.f, 0.f);

	m_fSprCX = 22;	m_fSprCY = 13;

	MakeBeatBar();

	return S_OK;
}

const int CHeart::Progress(void)
{
	SetMatrix();
	SetFrame();
	FrameMove();

	//if(CBeatMgr::GetInstance()->DropTheBeat())
	//{
	//	MakeBeatBar();
	//	m_iPlaying +=1;
	//}

	if(m_pBeatMgr->DropTheBeat())
	{
		MakeBeatBar();
		if(m_pDevice->SoundPlaying(TEXT("Zone1_1")))
			m_iPlaying +=1;
		else
			m_iPlaying = 0;
		
	}

	return 0;
}

void CHeart::Render(void)
{
	m_pBufferMgr->SetVtxInfo(TEXT("UITex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey,m_wstrStateKey,TEXT(""),(int)m_tFrame.fFrame);

	if(pTexInfo == NULL)
		return;

	BeginRenderState();

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);

	//m_pDevice->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pDevice->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pBufferMgr->RenderBuffer(TEXT("UITex"));
	
	EndRenderState();
}

void CHeart::Release(void)
{

}

void CHeart::MakeBeatBar()
{
	

	CObj* pObj = new CBar;
	((CBar*)pObj)->SetBeatDir(BEAT_LEFT);
	((CBar*)pObj)->SetVtxTex(m_OriVtxCnt);
	pObj->Initialize();
	if(m_iPlaying >170)
 		pObj->SetStateKey(TEXT("Red"));
	CObjMgr::GetInstance()->AddObj(OBJ_UI, pObj);

	pObj = new CBar;
	((CBar*)pObj)->SetBeatDir(BEAT_RIGHT);
	((CBar*)pObj)->SetVtxTex(m_OriVtxCnt);
	pObj->Initialize();
	if(m_iPlaying >170)
		pObj->SetStateKey(TEXT("Red"));
	CObjMgr::GetInstance()->AddObj(OBJ_UI, pObj);
}
