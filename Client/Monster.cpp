#include "stdafx.h"
#include "Monster.h"
#include "Player.h"
#include "Tile.h"
#include "EnemyAttack.h"
#include "Device.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "Pipeline.h"
#include "CamMgr.h"
#include "BeatMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "TimeMgr.h"
#include "SmallHeart.h"
#include "ComboMgr.h"
#include "ParticleMgr.h"
#include "ComboMgr.h"
#include "Rand.h"

struct SetDeleteObj
{
	void operator()(CObj* pObj)
	{
		pObj->SetDelete();
	}
};

CMonster::CMonster()
: m_bInit(false)
, m_bBeat(false)
, m_bLeft(true)
, m_bCubeBreak(false)
, m_bHide(false)
, m_bTrapMove(false)
, m_bIsFly(false)
, m_bJump(false)
, m_iCount(0)
, m_iOldIndex(0)
, m_iNewIndex(0)
, m_iAlpha(255)
, m_iSceenMode(0)
, m_fBaseY(1.f)
, m_fJumpTime(0.f)
, m_fJumpPow(0.6f)
, m_fScale(1.f)
, m_fWidth(1.f)
, m_fFrameSpeed(1.f)
, m_dwColor(0xff8A0707)
, m_strAttackSound(TEXT(""))
, m_strHitSound(TEXT(""))
, m_strDeathSound(TEXT(""))
, m_strMoveSound(TEXT(""))
, m_strInitSound(TEXT(""))
, m_vDamageDir(D3DXVECTOR3(0.f, 0.f, 0.f))
{
}

CMonster::~CMonster()
{
	Release();
}

void CMonster::Release()
{
	if(m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->GetObj() == this)
		m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->ResetObj();

	if(m_pObjMgr->GetTile(m_iNewIndex)->GetObj() == this)
		m_pObjMgr->GetTile(m_iNewIndex)->ResetObj();

	for_each(m_vecHp.begin(), m_vecHp.end(), ::SetDeleteObj());
	vector<CSmallHeart*>().swap(m_vecHp);
}

void CMonster::SetMatrix(const float fScale)
{
	if(m_bLeft)
		CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale, fScale, fScale), m_fAngle, m_tInfo.vPos);
	else
		SetDir(fScale);

	if(m_fWidth != 1.f)
	{
		if(m_bLeft)
		{
			switch(m_iSceenMode)
			{
			case 0:
				CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale * m_fWidth, fScale, fScale), m_fAngle, m_tInfo.vPos);
				break;
			case 1:
				CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale, fScale, fScale * m_fWidth), m_fAngle, m_tInfo.vPos);
				break;
			case 2:
				CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale * m_fWidth, fScale, fScale), m_fAngle, m_tInfo.vPos);
				break;
			case 3:
				CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale, fScale, fScale * m_fWidth), m_fAngle, m_tInfo.vPos);
				break;
			}
		}
		else
			SetDir(fScale);
	}

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

void CMonster::MonsterInit()
{
	m_dwVtxCnt = 4;

	m_pVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pConvertVtxTex = new VTXTEX[m_dwVtxCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("UITex"), m_pVtxTex);

	m_dwIndexCnt = 2;

	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetIndex(TEXT("UITex"), m_pIndex);

	m_wstrStateKey = TEXT("Normal");

	m_tFrame.fFrame = 0.f;

	m_eObjId = OBJ_MONSTER;

	m_vecHp.reserve(4);

	for(int i = 0; i < m_tUnitInfo.iMaxHp; ++i)
	{
		CSmallHeart* pHp = (CSmallHeart*)CObjFactory<CSmallHeart>::CreateObj();
		m_pObjMgr->AddObj(OBJ_EFFECT, pHp);
		m_vecHp.push_back(pHp);
	}
}

void CMonster::MonsterProgress()
{
	if(!m_bInit)
	{
		D3DXVECTOR3 m_vDir = m_pObjMgr->GetPlayer()->GetInfo()->vPos - m_tInfo.vPos;

		float fDistance = D3DXVec3Length(&m_vDir);

		if(fDistance > 14.f)
			return;

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

	SetMatrix(m_fScale);
	GetPlayerToDistance();
	FrameMove();
	SetFrame(m_fFrameSpeed);
	BeatClock();

	SetScreenMode();
	StageEndScene();

	HeartRender();

	SetFrozenTimer();

	if(m_bTrapMove)
		TrapMove(m_vTrapMove);

	if(m_bFreeze)
		m_iCount = 0;
}

void CMonster::MonsterRender()
{
	m_pBufferMgr->SetVtxInfo(TEXT("UITex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey, m_wstrStateKey, TEXT(""), (int)m_tFrame.fFrame);

	if(pTexInfo == NULL)
		return;

	BeginRenderState(m_iAlpha);
	if(m_iAlpha == 255)
		SetDark(m_iDarkness);
	ToShadow();

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);

	m_pBufferMgr->RenderBuffer(TEXT("UITex"));

	EndRenderState();
}

const int CMonster::GetNewIndex() const
{
	return m_iNewIndex;
}

const float CMonster::GetScale() const
{
	return m_fScale;
}

const DWORD CMonster::GetColor() const
{
	return m_dwColor;
}

const bool CMonster::GetHide() const
{
	return m_bHide;
}

const bool CMonster::GetIsFly() const
{
	return m_bIsFly;
}

void CMonster::SetOldIndex(const int iIndex)
{
	m_iOldIndex = iIndex;
}

void CMonster::BeatClock()
{
	if(m_pBeatMgr->GetBeatBoomBoom())
		++m_iCount;

	if(m_iCount == 8)
		m_iCount = 0;
}

void CMonster::Move(const D3DXVECTOR3& vPos)
{
	if(m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->GetObj() != NULL && 
		m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->GetTileObjId() == OBJ_MONSTER)
		m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->ResetObj();

	if(m_tUnitInfo.iHp > 0 && m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_vTarget))->GetTileObjId() != OBJ_PLAYER)
		m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_vTarget))->SetObj(this);

	D3DXVECTOR3 vDir = vPos - m_tInfo.vPos;
	float fDistance = D3DXVec3Length(&vDir);

	D3DXVec3Normalize(&vDir, &vDir);

	m_tInfo.vPos += vDir * m_pTimeMgr->GetTime() * 10.f;

	if(fDistance < 0.5f)
	{
		m_tInfo.vPos = vPos;
		m_bBeat = false;

		m_iOldIndex = m_iNewIndex;

		m_pDevice->SoundPlay(m_strMoveSound, 0);
	}
}

void CMonster::TrapMove(const D3DXVECTOR3& vPos)
{
	if(m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->GetObj() != NULL && 
		m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->GetTileObjId() == OBJ_MONSTER)
		m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->ResetObj();

	if(m_tUnitInfo.iHp > 0 && m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_vTarget))->GetTileObjId() != OBJ_PLAYER)
		m_pObjMgr->GetTile(m_pObjMgr->GetIndex(vPos))->SetObj(this);

	D3DXVECTOR3 vDir = vPos - m_tInfo.vPos;
	float fDistance = D3DXVec3Length(&vDir);

	D3DXVec3Normalize(&vDir, &vDir);

	m_tInfo.vPos += vDir * m_pTimeMgr->GetTime() * 20.f;

	if(fDistance < 0.5f)
	{
		m_tInfo.vPos = vPos;
		m_bBeat = false;

		m_tInfo.vPos.y = m_fBaseY;

		m_vTarget = vPos;

		m_pDevice->SoundPlay(m_strMoveSound, 0);

		m_bTrapMove = false;
	}
}

void CMonster::Attack()
{		
	if(m_tUnitInfo.iAtt == 0)
		return;

	if(m_pObjMgr->GetTile(m_iNewIndex)->GetObj() == NULL)
		return;

	OBJID e = m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId();

	if(m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() == OBJ_PLAYER)
	{
		D3DXVECTOR3 vPos = m_pObjMgr->GetTile(m_iNewIndex)->GetInfo()->vPos;
		vPos.y = 2.f;

		m_pObjMgr->AddObj(OBJ_EFFECT, CObjFactory<CEnemyAttack>::CreateObj(vPos + D3DXVECTOR3(0.f, 0.5f, 0.f)));

		m_pCamMgr->SetShake(0.3f, 0.1f);

		m_pObjMgr->GetPlayer()->SetDamage(m_tUnitInfo.iAtt);
		m_pComboMgr->SetCombo(0);

		m_pDevice->SoundPlay(m_strAttackSound, 0);

		if(m_wstrObjKey == TEXT("Light_Minotaur_Charge"))
			m_wstrObjKey = TEXT("Light_Minotaur_Stun");

		return;
	}
}

void CMonster::ToShadow()
{
	/*D3DXVECTOR3 vDist = m_pObjMgr->GetPlayer()->GetInfo()->vPos - m_tInfo.vPos;
	float fDistance = D3DXVec3Length(&vDist);*/

	BYTE byDarkness = m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->GetDarkness();

	if(byDarkness < 60 && byDarkness > 40)
	{
		m_iDarkness = 255;
		m_wstrStateKey = TEXT("Shadow");
	}
	else
	{
		m_iDarkness = byDarkness;
		m_wstrStateKey = TEXT("Normal");
	}
}

void CMonster::MoveLeft()
{
	m_iNewIndex = m_iOldIndex - m_pObjMgr->GetTileZ();

	if(m_iNewIndex < 0)
		m_iNewIndex = m_iOldIndex;

	for(list<CObj*>::iterator iter = m_pObjMgr->GetObjList(OBJ_MONSTER)->begin();
		iter != m_pObjMgr->GetObjList(OBJ_MONSTER)->end(); ++iter)
	{
		if((*iter) == this)
			continue;

		if(m_pObjMgr->GetTile(m_iNewIndex)->GetObj() != NULL
			&& m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() != OBJ_PLAYER)
		{
			if(m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() == OBJ_MONSTER
				|| m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() == OBJ_ETC)
			{
				if(m_wstrObjKey == TEXT("Light_Minotaur_Charge"))
				{
					m_wstrObjKey = TEXT("Light_Minotaur_Stun");

					m_pCamMgr->SetShake();

					m_pDevice->SoundPlay(TEXT("Minotaur_Stun"), 0);
				}
				return;
			}

			if(m_iNewIndex == ((CMonster*)(*iter))->GetNewIndex())
			{
				m_iNewIndex = m_iOldIndex;

				return;
			}
		}
	}

	if(m_iNewIndex < 0 || m_iNewIndex > m_pObjMgr->GetTileX() * m_pObjMgr->GetTileZ() - 1)
	{
		m_iNewIndex = m_iOldIndex;

		return;
	}

	switch(m_iSceenMode)
	{
	case 0:
		m_bLeft = true;
		break;
	case 2:
		m_bLeft = false;
		break;
	}

	Attack();

	if(m_wstrObjKey == TEXT("Golem"))
		m_fAngle[ANGLE_Y] = D3DXToRadian(90.f);

	if(m_pObjMgr->GetTile(m_iNewIndex)->GetObj() != NULL)
	{
		if(m_bCubeBreak && m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() == OBJ_CUBE)
		{
			if((m_pObjMgr->GetTile(m_iNewIndex)->GetObj())->GetCubeInfo().dwType & CUBE_NOTB)
			{
				if(m_wstrObjKey == TEXT("Light_Minotaur_Charge"))
				{
					m_wstrObjKey = TEXT("Light_Minotaur_Stun");

					m_pCamMgr->SetShake();

					m_pDevice->SoundPlay(TEXT("Minotaur_Stun"), 0);
				}

				return;
			}

			m_pObjMgr->GetTile(m_iNewIndex)->GetObj()->SetDelete();

			if(m_wstrObjKey == TEXT("Light_Minotaur_Charge"))
			{
				m_wstrObjKey = TEXT("Light_Minotaur_Stun");

				m_pCamMgr->SetShake();

				m_pDevice->SoundPlay(TEXT("Minotaur_Stun"), 0);
			}
		}

		return;
	}

	m_bBeat = true;
	m_vTarget = m_pObjMgr->GetTile(m_iNewIndex)->GetInfo()->vPos + D3DXVECTOR3(0.f, m_tInfo.vPos.y, 0.f);
}

void CMonster::MoveRight()
{
	m_iNewIndex = m_iOldIndex + m_pObjMgr->GetTileZ();

	if(m_iNewIndex < 0)
		m_iNewIndex = m_iOldIndex;

	for(list<CObj*>::iterator iter = m_pObjMgr->GetObjList(OBJ_MONSTER)->begin();
		iter != m_pObjMgr->GetObjList(OBJ_MONSTER)->end(); ++iter)
	{
		if((*iter) == this)
			continue;

		if(m_pObjMgr->GetTile(m_iNewIndex)->GetObj() != NULL
			&& m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() != OBJ_PLAYER)
		{
			if(m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() == OBJ_MONSTER
				|| m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() == OBJ_ETC)
			{
				if(m_wstrObjKey == TEXT("Light_Minotaur_Charge"))
				{
					m_wstrObjKey = TEXT("Light_Minotaur_Stun");

					m_pCamMgr->SetShake();

					m_pDevice->SoundPlay(TEXT("Minotaur_Stun"), 0);
				}

				return;
			}

			if(m_iNewIndex == ((CMonster*)(*iter))->GetNewIndex())
			{
				m_iNewIndex = m_iOldIndex;

				return;
			}
		}
	}

	if(m_iNewIndex < 0 || m_iNewIndex > m_pObjMgr->GetTileX() * m_pObjMgr->GetTileZ() - 1)
	{
		m_iNewIndex = m_iOldIndex;

		return;
	}

	switch(m_iSceenMode)
	{
	case 0:
		m_bLeft = false;
		break;
	case 2:
		m_bLeft = true;
		break;
	}

	Attack();

	if(m_wstrObjKey == TEXT("Golem"))
		m_fAngle[ANGLE_Y] = D3DXToRadian(270.f);

	if(m_pObjMgr->GetTile(m_iNewIndex)->GetObj() != NULL)
	{
		if(m_bCubeBreak && m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() == OBJ_CUBE)
		{
			if((m_pObjMgr->GetTile(m_iNewIndex)->GetObj())->GetCubeInfo().dwType & CUBE_NOTB)
			{
				if(m_wstrObjKey == TEXT("Light_Minotaur_Charge"))
				{
					m_wstrObjKey = TEXT("Light_Minotaur_Stun");

					m_pCamMgr->SetShake();

					m_pDevice->SoundPlay(TEXT("Minotaur_Stun"), 0);
				}

				return;
			}

			m_pObjMgr->GetTile(m_iNewIndex)->GetObj()->SetDelete();

			if(m_wstrObjKey == TEXT("Light_Minotaur_Charge"))
			{
				m_wstrObjKey = TEXT("Light_Minotaur_Stun");

				m_pCamMgr->SetShake();

				m_pDevice->SoundPlay(TEXT("Minotaur_Stun"), 0);
			}
		}

		return;
	}

	m_bBeat = true;
	m_vTarget = m_pObjMgr->GetTile(m_iNewIndex)->GetInfo()->vPos + D3DXVECTOR3(0.f, m_tInfo.vPos.y, 0.f);
}

void CMonster::MoveUp()
{
	m_iNewIndex = m_iOldIndex + 1;

	if(m_iNewIndex < 0)
		m_iNewIndex = m_iOldIndex;

	for(list<CObj*>::iterator iter = m_pObjMgr->GetObjList(OBJ_MONSTER)->begin();
		iter != m_pObjMgr->GetObjList(OBJ_MONSTER)->end(); ++iter)
	{
		if((*iter) == this)
			continue;

		if(m_pObjMgr->GetTile(m_iNewIndex)->GetObj() != NULL
			&& m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() != OBJ_PLAYER)
		{
			if(m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() == OBJ_MONSTER
				|| m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() == OBJ_ETC)
			{
				if(m_wstrObjKey == TEXT("Light_Minotaur_Charge"))
				{
					m_wstrObjKey = TEXT("Light_Minotaur_Stun");

					m_pCamMgr->SetShake();

					m_pDevice->SoundPlay(TEXT("Minotaur_Stun"), 0);
				}

				return;
			}

			if(m_iNewIndex == ((CMonster*)(*iter))->GetNewIndex())
			{
				m_iNewIndex = m_iOldIndex;

				return;
			}
		}
	}

	if(m_iNewIndex < 0 || m_iNewIndex > m_pObjMgr->GetTileX() * m_pObjMgr->GetTileZ() - 1)
	{
		m_iNewIndex = m_iOldIndex;

		return;
	}

	switch(m_iSceenMode)
	{
	case 1:
		m_bLeft = true;
		break;
	case 3:
		m_bLeft = false;
		break;
	}

	Attack();

	if(m_wstrObjKey == TEXT("Golem"))
		m_fAngle[ANGLE_Y] = D3DXToRadian(180.f);

	if(m_pObjMgr->GetTile(m_iNewIndex)->GetObj() != NULL)
	{
		if(m_bCubeBreak && m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() == OBJ_CUBE)
		{
			if((m_pObjMgr->GetTile(m_iNewIndex)->GetObj())->GetCubeInfo().dwType & CUBE_NOTB)
			{
				if(m_wstrObjKey == TEXT("Light_Minotaur_Charge"))
				{
					m_wstrObjKey = TEXT("Light_Minotaur_Stun");

					m_pCamMgr->SetShake();

					m_pDevice->SoundPlay(TEXT("Minotaur_Stun"), 0);
				}

				return;
			}

			m_pObjMgr->GetTile(m_iNewIndex)->GetObj()->SetDelete();

			if(m_wstrObjKey == TEXT("Light_Minotaur_Charge"))
			{
				m_wstrObjKey = TEXT("Light_Minotaur_Stun");

				m_pCamMgr->SetShake();

				m_pDevice->SoundPlay(TEXT("Minotaur_Stun"), 0);
			}
		}

		return;
	}

	m_bBeat = true;
	m_vTarget = m_pObjMgr->GetTile(m_iNewIndex)->GetInfo()->vPos + D3DXVECTOR3(0.f, m_tInfo.vPos.y, 0.f);
}

void CMonster::MoveDown()
{
	m_iNewIndex = m_iOldIndex - 1;

	if(m_iNewIndex < 0)
		m_iNewIndex = m_iOldIndex;

	for(list<CObj*>::iterator iter = m_pObjMgr->GetObjList(OBJ_MONSTER)->begin();
		iter != m_pObjMgr->GetObjList(OBJ_MONSTER)->end(); ++iter)
	{
		if((*iter) == this)
			continue;

		if(m_pObjMgr->GetTile(m_iNewIndex)->GetObj() != NULL
			&& m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() != OBJ_PLAYER)
		{
			if(m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() == OBJ_MONSTER
				|| m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() == OBJ_ETC)
			{
				if(m_wstrObjKey == TEXT("Light_Minotaur_Charge"))
				{
					m_wstrObjKey = TEXT("Light_Minotaur_Stun");

					m_pCamMgr->SetShake();

					m_pDevice->SoundPlay(TEXT("Minotaur_Stun"), 0);
				}

				return;
			}

			if(m_iNewIndex == ((CMonster*)(*iter))->GetNewIndex())
			{
				m_iNewIndex = m_iOldIndex;

				return;
			}
		}
	}

	if(m_iNewIndex < 0 || m_iNewIndex > m_pObjMgr->GetTileX() * m_pObjMgr->GetTileZ() - 1)
	{
		m_iNewIndex = m_iOldIndex;

		return;
	}

	switch(m_iSceenMode)
	{
	case 1:
		m_bLeft = false;
		break;
	case 3:
		m_bLeft = true;
		break;
	}

	Attack();

	if(m_wstrObjKey == TEXT("Golem"))
		m_fAngle[ANGLE_Y] = D3DXToRadian(0.f);

	if(m_pObjMgr->GetTile(m_iNewIndex)->GetObj() != NULL)
	{
		if(m_bCubeBreak && m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() == OBJ_CUBE)
		{
			if((m_pObjMgr->GetTile(m_iNewIndex)->GetObj())->GetCubeInfo().dwType & CUBE_NOTB)
			{
				if(m_wstrObjKey == TEXT("Light_Minotaur_Charge"))
				{
					m_wstrObjKey = TEXT("Light_Minotaur_Stun");

					m_pCamMgr->SetShake();

					m_pDevice->SoundPlay(TEXT("Minotaur_Stun"), 0);
				}

				return;
			}

			m_pObjMgr->GetTile(m_iNewIndex)->GetObj()->SetDelete();

			if(m_wstrObjKey == TEXT("Light_Minotaur_Charge"))
			{
				m_wstrObjKey = TEXT("Light_Minotaur_Stun");

				m_pCamMgr->SetShake();

				m_pDevice->SoundPlay(TEXT("Minotaur_Stun"), 0);
			}
		}

		return;
	}

	m_bBeat = true;
	m_vTarget = m_pObjMgr->GetTile(m_iNewIndex)->GetInfo()->vPos + D3DXVECTOR3(0.f, m_tInfo.vPos.y, 0.f);
}

void CMonster::MoveChase()
{
	D3DXVECTOR3 vPos = m_pObjMgr->GetPlayer()->GetInfo()->vPos - m_tInfo.vPos;

	int iRand = CRand::Random(0, 1);

	switch(iRand)
	{
	case 0:
		if(abs(vPos.x) >= abs(vPos.z))
		{
			if(m_pObjMgr->GetPlayer()->GetInfo()->vPos.x >= m_tInfo.vPos.x)
				MoveRight();
			else
				MoveLeft();
		}
		else
		{
			if(m_pObjMgr->GetPlayer()->GetInfo()->vPos.z >= m_tInfo.vPos.z)
				MoveUp();
			else
				MoveDown();
		}
		break;
	case 1:
		if(abs(vPos.x) <= abs(vPos.z))
		{
			if(m_pObjMgr->GetPlayer()->GetInfo()->vPos.z >= m_tInfo.vPos.z)
				MoveUp();
			else
				MoveDown();
		}
		else
		{
			if(m_pObjMgr->GetPlayer()->GetInfo()->vPos.x >= m_tInfo.vPos.x)
				MoveRight();
			else
				MoveLeft();
		}
		break;
	}
}

void CMonster::SetScreenMode()
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

	if(fAngle >= 0.f && fAngle < 90.f)
		m_iSceenMode = 0;
	else if(fAngle >= 90.f && fAngle < 180.f)
		m_iSceenMode = 1;
	else if(fAngle >= 180.f && fAngle < 270.f)
		m_iSceenMode = 2;
	else if(fAngle >= 270.f && fAngle < 360.f)
		m_iSceenMode = 3;
}

void CMonster::HeartRender()
{
	if(m_tUnitInfo.iHp == m_tUnitInfo.iMaxHp<<1)
		return;

	for(int i = 0; i < m_tUnitInfo.iMaxHp; ++i)
	{
		D3DXVECTOR3 vPos = D3DXVECTOR3(0.f, 0.f, 0.f);

		switch(m_iSceenMode)
		{
		case 0:
			vPos = D3DXVECTOR3(m_tInfo.vPos.x - 0.5f * m_fScale + i * 1.f, m_tInfo.vPos.y + 1.4f * m_fScale, m_tInfo.vPos.z + 1.f);
			break;
		case 1:
			vPos = D3DXVECTOR3(m_tInfo.vPos.x + 1.f, m_tInfo.vPos.y + 1.4f * m_fScale, m_tInfo.vPos.z + 0.5f * m_fScale - i * 1.f);
			break;
		case 2:
			vPos = D3DXVECTOR3(m_tInfo.vPos.x + 0.5f * m_fScale - i * 1.f, m_tInfo.vPos.y + 1.4f * m_fScale, m_tInfo.vPos.z - 1.f);
			break;
		case 3:
			vPos = D3DXVECTOR3(m_tInfo.vPos.x - 1.f, m_tInfo.vPos.y + 1.4f * m_fScale, m_tInfo.vPos.z - 0.5f * m_fScale + i * 1.f);
			break;
		}
		m_vecHp[i]->SetPos(vPos);
	}

	int iSize = ((m_tUnitInfo.iMaxHp<<1) - m_tUnitInfo.iHp)>>1;

	for(int i = 0; i < iSize; ++i)
	{
		m_vecHp[i]->SetDamage();
	}
}

void CMonster::SetDir(const float fScale)
{
	switch(m_iSceenMode)
	{
	case 0:
		CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(-fScale * m_fWidth, fScale, fScale), m_fAngle, m_tInfo.vPos);
		break;
	case 1:
		CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale, fScale, -fScale * m_fWidth), m_fAngle, m_tInfo.vPos);
		break;
	case 2:
		CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(-fScale * m_fWidth, fScale, fScale), m_fAngle, m_tInfo.vPos);
		break;
	case 3:
		CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale, fScale, -fScale * m_fWidth), m_fAngle, m_tInfo.vPos);
		break;
	}
}

void CMonster::SetHitSound()
{
	m_pDevice->SoundPlay(m_strHitSound, 0);
}

void CMonster::SetPosMon(const D3DXVECTOR3& vPos)
{
	m_tInfo.vPos.x = vPos.x;
	m_tInfo.vPos.z = vPos.z;
}

void CMonster::SetTrapMove(const D3DXVECTOR3& vPos)
{
	m_bTrapMove = true;
	m_vTrapMove = vPos;
}

void CMonster::Jump()
{
	if(!m_bJump)
		return;

	m_fJumpTime += m_pTimeMgr->GetTime();

	if(m_fJumpTime < m_fJumpPow * 0.5f)
		m_tInfo.vPos.y += m_pTimeMgr->GetTime() * 10.f;
	else if(m_fJumpTime < m_fJumpPow)
	{
		m_tInfo.vPos.y -= m_pTimeMgr->GetTime() * 10.f;
		if(m_tInfo.vPos.y < m_fBaseY)
			m_tInfo.vPos.y = m_fBaseY;
	}
	else
	{
		m_tInfo.vPos.y = m_fBaseY;
		m_fJumpTime = 0.f;
		m_bJump = false;
	}
}

void CMonster::SetDamageDirection(const D3DXVECTOR3& vDamageDir)
{
	m_vDamageDir = m_tInfo.vPos-vDamageDir;
	m_vDamageDir.y = 0.f;
	D3DXVec3Normalize(&m_vDamageDir, &m_vDamageDir);

	// ³Ë¹éÇÏ´Ù°¡ ¸¾ ¹ö±× ¼öÁ¤ ±ÍÂú
	/*m_bTrapMove = true;
	m_vTrapMove = m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->GetInfo()->vPos + D3DXVECTOR3(2.f, 0.f, 0.f);*/
}