#include "stdafx.h"
#include "Item.h"
#include "Tile.h"
#include "ObjMgr.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Pipeline.h"
#include "Device.h"
#include "CamMgr.h"
#include "BeatMgr.h"
#include "ObjFactory.h"
#include "Player.h"
#include "Mini_Diamond.h"
#include "Num.h"

CItem::CItem()
: m_bMoving(false)
, m_bSelling(false)
, m_iPrice(0)
, m_iOriPrice(0)
, m_iRePrice(0)
, m_fTime(0.f)
, m_fCount(0.f)
, m_fScale(0.5f)
, m_dwType(0)
, vPlayerPos(D3DXVECTOR3(0.f, 0.f, 0.f))
, vSlotPos(D3DXVECTOR3(0.f, 0.f, 0.f))
{
}

CItem::~CItem()
{
	Release();
}

HRESULT CItem::Initialize()
{
	m_dwVtxCnt = 4;

	m_pVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pConvertVtxTex = new VTXTEX[m_dwVtxCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("UITex"), m_pVtxTex);

	m_dwIndexCnt = 2;

	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetIndex(TEXT("UITex"), m_pIndex);

	m_wstrStateKey = TEXT("Normal");

	m_eObjId = OBJ_ITEM;

	m_vecPrice.reserve(3);

	return S_OK;
}

const int CItem::Progress()
{
	SetMatrix(m_fScale);
	GetPlayerToDistance();
	StageEndScene();

	UsingOBB();//Test

	float fAnlge = D3DXToDegree(((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY());
	if((fAnlge > 260.f && 	fAnlge < 280.f) ||	(fAnlge < -80.f &&	fAnlge > -100.f) )
		m_iPrice = m_iOriPrice;
	else if ( (fAnlge > 80.f && fAnlge < 100.f) ||  (fAnlge < -260.f &&	fAnlge > -280.f))
		m_iPrice = m_iRePrice;
	else if ( (fAnlge > 170.f && fAnlge < 190.f) || (fAnlge < -170.f &&	fAnlge > -190.f) )
		m_iPrice = m_iRePrice;
	else
		m_iPrice = m_iOriPrice;

	if(!(m_dwType & ITEM_GOLD))
		Shake();

	if(m_bSelling)
	{
		size_t iSize = m_vecPrice.size();

		for(size_t i = 0; i < iSize; i++)
			m_vecPrice[i]->Progress();
	}

	if(m_bIsDelete)
		return 1;

	return 0;
}

void CItem::Render()
{
	m_pBufferMgr->SetVtxInfo(TEXT("UITex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey, m_wstrStateKey, TEXT(""), 0);

	if(pTexInfo == NULL)
		return;

	BeginRenderState();
	SetDark(m_iDarkness);

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);

	m_pBufferMgr->RenderBuffer(TEXT("UITex"));

	EndRenderState();

	if(m_bSelling 
		&& !(m_dwType & ITEM_GOLD) )
	{
		for(size_t i = 0; i < m_vecPrice.size(); i++)
		{
			m_vecPrice[i]->Render();
			m_vecPrice[i]->SetDarkness(m_iDarkness);
		}
	}
}

void CItem::Release()
{
	m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->ResetItem();

	for_each(m_vecPrice.begin(), m_vecPrice.end(), ::CDeleteObj());

	vector<CObj*>().swap(m_vecPrice);
}

void CItem::SetType(const DWORD dwType)
{
	m_dwType = dwType;
}

const DWORD CItem::GetType() const
{
	return m_dwType;
}

void CItem::SetMatrix(const float fScale)
{
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale, fScale, fScale), m_fAngle, m_tInfo.vPos);

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

void CItem::Shake()
{
	m_fTime += m_pTimeMgr->GetTime() / m_pBeatMgr->GetBeatMax();

	if(m_fTime < 0.5f)
		m_tInfo.vPos.y += m_pTimeMgr->GetTime();
	else if(m_fTime < 1.f)
	{
		m_tInfo.vPos.y -= m_pTimeMgr->GetTime();
		if(m_tInfo.vPos.y < 1.f)
			m_tInfo.vPos.y = 1.f;
	}
	else
		m_fTime = 0.f;	
}

void CItem::InitSetType()
{
	if(m_wstrObjKey == TEXT("Blunderbuss")
		|| m_wstrObjKey == TEXT("Broad_Sword")
		|| m_wstrObjKey == TEXT("Dagger")
		|| m_wstrObjKey == TEXT("Spear")
		|| m_wstrObjKey == TEXT("Whip"))
		m_dwType = ITEM_ATTACK;

	else if(m_wstrObjKey == TEXT("Leather_Armor")
		|| m_wstrObjKey == TEXT("Blood_Armor")
		|| m_wstrObjKey == TEXT("Grass_Armor")
		|| m_wstrObjKey == TEXT("Chain_Armor")
		|| m_wstrObjKey == TEXT("Golden_Armor")
		|| m_wstrObjKey == TEXT("Ki_Armor")
		|| m_wstrObjKey == TEXT("Plate_Armor"))
		m_dwType = ITEM_BODY;

	else if(m_wstrObjKey == TEXT("Bomb")
		|| m_wstrObjKey == TEXT("Bomb3"))
		m_dwType = ITEM_BOMB;	

	else if(m_wstrObjKey == TEXT("Boots_Explorers")
		|| m_wstrObjKey == TEXT("Red_Boots")
		|| m_wstrObjKey == TEXT("Plate_Boots"))
		m_dwType = ITEM_FEET;	

	else if(m_wstrObjKey == TEXT("Helmet")
		|| m_wstrObjKey == TEXT("Circlet")
		|| m_wstrObjKey == TEXT("Crown"))
		m_dwType = ITEM_HEAD;	

	else if(m_wstrObjKey == TEXT("Holster"))
		m_dwType = ITEM_HOLSTER;	

	else if(m_wstrObjKey == TEXT("Food_Apple")
		|| m_wstrObjKey == TEXT("Food_Cheese")
		|| m_wstrObjKey == TEXT("Food_Chicken")
		|| m_wstrObjKey == TEXT("Food_Meat"))
		m_dwType = ITEM_ITEM;	

	else if(m_wstrObjKey == TEXT("Key"))
		m_dwType = ITEM_KEY;	

	else if(m_wstrObjKey == TEXT("Ring_Luck")
		|| m_wstrObjKey == TEXT("Ring_Charisma")
		|| m_wstrObjKey == TEXT("Ring_Mighty")
		|| m_wstrObjKey == TEXT("Ring_Gold"))
		m_dwType = ITEM_MISC;	

	else if(m_wstrObjKey == TEXT("Pack"))
		m_dwType = ITEM_PACK;	

	else if(m_wstrObjKey == TEXT("Shovel_Basic"))
		m_dwType = ITEM_SHOVEL;

	else if(m_wstrObjKey == TEXT("Fireball")
		|| m_wstrObjKey == TEXT("Freeze")
		|| m_wstrObjKey == TEXT("SpellCool"))
		m_dwType = ITEM_SPELL;	

	else if(m_wstrObjKey == TEXT("Torch_1"))
		m_dwType = ITEM_TORCH;	
}

void CItem::SetPrice(const int iPrice, const bool bDiamond)
{
	m_bSelling = true;
	m_iPrice = iPrice;

	if(m_dwType & ITEM_GOLD)
	{
		m_iOriPrice = m_iPrice;
		m_iRePrice = m_iPrice;
	}

	if(bDiamond)
	{
		CObj* pDiamond = CObjFactory<CMini_Diamond>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, 1.f, m_tInfo.vPos.z));
		m_vecPrice.push_back(pDiamond);
	}
	
	CString temp = TEXT("");
	temp.Format(TEXT("%d"), iPrice);
	int len = temp.GetLength();

	if(len ==1)
	{
		CObj* pNum= CObjFactory<CNum>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, 1.f, m_tInfo.vPos.z - 1.f));
		pNum->SetStructFrame(FRAME((float)iPrice,0.f,0.f ));
		m_vecPrice.push_back(pNum);
	}
	else if(len == 2)
	{
		CObj* pNum= CObjFactory<CNum>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x - 0.25f, 1.f, m_tInfo.vPos.z - 1.f));
		pNum->SetStructFrame(FRAME(float(iPrice / 10) ,0.f,0.f ));
		m_vecPrice.push_back(pNum);

		pNum= CObjFactory<CNum>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x + 0.25f, 1.f, m_tInfo.vPos.z - 1.f));
		pNum->SetStructFrame(FRAME(float(iPrice % 10),0.f,0.f ));
		m_vecPrice.push_back(pNum);

	}
	else if(len == 3 )
	{
		CObj* pNum= CObjFactory<CNum>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x - 0.5f , 1.f, m_tInfo.vPos.z - 1.f));
		pNum->SetStructFrame(FRAME(float(iPrice / 100) ,0.f,0.f ));
		m_vecPrice.push_back(pNum);

		pNum= CObjFactory<CNum>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x , 1.f, m_tInfo.vPos.z - 1.f));
		pNum->SetStructFrame(FRAME(float((iPrice % 100)/10),0.f,0.f ));
		m_vecPrice.push_back(pNum);

		pNum= CObjFactory<CNum>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x + 0.5f, 1.f, m_tInfo.vPos.z - 1.f));
		pNum->SetStructFrame(FRAME(float((iPrice % 100)%10),0.f,0.f ));
		m_vecPrice.push_back(pNum);
	}
}

void CItem::SetPrice(void)
{
	CString temp = TEXT("");
	temp.Format(TEXT("%d"),m_iPrice);
	int len = temp.GetLength();

	if(len ==1)
	{
		CObj* pNum= CObjFactory<CNum>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, 1.f, m_tInfo.vPos.z - 1.f));
		pNum->SetStructFrame(FRAME((float)m_iPrice,0.f,0.f ));
		m_vecPrice.push_back(pNum);
	}
	else if(len == 2)
	{
		CObj* pNum= CObjFactory<CNum>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x - 0.25f, 1.f, m_tInfo.vPos.z - 1.f));
		pNum->SetStructFrame(FRAME(float(m_iPrice / 10) ,0.f,0.f ));
		m_vecPrice.push_back(pNum);

		pNum= CObjFactory<CNum>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x + 0.25f, 1.f, m_tInfo.vPos.z - 1.f));
		pNum->SetStructFrame(FRAME(float(m_iPrice % 10),0.f,0.f ));
		m_vecPrice.push_back(pNum);
	}
	else if(len == 3 )
	{
		CObj* pNum= CObjFactory<CNum>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x - 0.5f , 1.f, m_tInfo.vPos.z - 1.f));
		pNum->SetStructFrame(FRAME(float(m_iPrice / 100) ,0.f,0.f ));
		m_vecPrice.push_back(pNum);

		pNum= CObjFactory<CNum>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x , 1.f, m_tInfo.vPos.z - 1.f));
		pNum->SetStructFrame(FRAME(float((m_iPrice % 100)/10),0.f,0.f ));
		m_vecPrice.push_back(pNum);

		pNum= CObjFactory<CNum>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x + 0.5f, 1.f, m_tInfo.vPos.z - 1.f));
		pNum->SetStructFrame(FRAME(float((m_iPrice % 100)%10),0.f,0.f ));
		m_vecPrice.push_back(pNum);
	}

	m_iOriPrice = m_iPrice;

	temp.MakeReverse();
	m_iRePrice = _ttoi(temp);
}

void CItem::SetSelling(const bool bSelling)
{
	m_bSelling = bSelling;
}

const int CItem::GetPrice() const
{
	return m_iPrice;
}

void CItem::SetScale(const float fScale)
{
	m_fScale = fScale;
}

void CItem::SetSellPrice()
{
	if (m_wstrObjKey == TEXT("Blunderbuss"))
		m_iPrice = 500;
	else if (m_wstrObjKey == TEXT("Broad_Sword"))
		m_iPrice = 150;
	else if (m_wstrObjKey == TEXT("Dagger"))
		m_iPrice = 10;
	else if (m_wstrObjKey == TEXT("Spear"))
		m_iPrice = 200;
	else if (m_wstrObjKey == TEXT("Whip"))
		m_iPrice = 300;
	else if (m_wstrObjKey == TEXT("Leather_Armor"))
		m_iPrice = 50;
	else if (m_wstrObjKey == TEXT("Bomb"))
		m_iPrice = 20;
	else if (m_wstrObjKey == TEXT("Bomb3"))
		m_iPrice = 60;
	else if (m_wstrObjKey == TEXT("Boots_Explorers"))
		m_iPrice = 100;
	else if (m_wstrObjKey == TEXT("Helmet"))
		m_iPrice = 200;
	else if (m_wstrObjKey == TEXT("Holster"))
		m_iPrice = 50;
	else if (m_wstrObjKey == TEXT("Food_Apple"))
		m_iPrice = 20;
	else if (m_wstrObjKey == TEXT("Pack"))
		m_iPrice = 30;
	else if (m_wstrObjKey == TEXT("Ring_Luck"))
		m_iPrice = 50;
	else if (m_wstrObjKey == TEXT("Fireball"))
		m_iPrice = 50;
	else if (m_wstrObjKey == TEXT("Freeze"))
		m_iPrice = 30;
	else if (m_wstrObjKey == TEXT("Torch_1"))
		m_iPrice = 50;
	else if (m_wstrObjKey == TEXT("Blood_Armor"))
		m_iPrice = 230;
	else if (m_wstrObjKey == TEXT("Chain_Armor"))
		m_iPrice = 150;
	else if (m_wstrObjKey == TEXT("Golden_Armor"))
		m_iPrice = 250;
	else if (m_wstrObjKey == TEXT("Grass_Armor"))
		m_iPrice = 80;
	else if (m_wstrObjKey == TEXT("Ki_Armor"))
		m_iPrice = 350;
	else if (m_wstrObjKey == TEXT("Plate_Armor"))
		m_iPrice = 550;
	else if (m_wstrObjKey == TEXT("Plate_Boots"))
		m_iPrice = 50;
	else if (m_wstrObjKey == TEXT("Red_Boots"))
		m_iPrice = 80;
	else if (m_wstrObjKey == TEXT("Circlet"))
		m_iPrice = 20;
	else if (m_wstrObjKey == TEXT("Crown"))
		m_iPrice = 40;
	else if (m_wstrObjKey == TEXT("Food_Cheese"))
		m_iPrice = 50;
	else if (m_wstrObjKey == TEXT("Food_Chicken"))
		m_iPrice = 70;
	else if (m_wstrObjKey == TEXT("Food_Meat"))
		m_iPrice = 100;
	else if (m_wstrObjKey == TEXT("Ring_Charisma"))
		m_iPrice = 200;
	else if (m_wstrObjKey == TEXT("Ring_Mighty"))
		m_iPrice = 300;
	else if (m_wstrObjKey == TEXT("Ring_Gold"))
		m_iPrice = 150;
}

const bool CItem::GetisSelling() const
{
	return m_bSelling;
}