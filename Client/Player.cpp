#include "stdafx.h"
#include "Player.h"
#include "Device.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "BeatMgr.h"
#include "TimeMgr.h"
#include "ParticleMgr.h"
#include "CamMgr.h"
#include "ObjMgr.h"
#include "MathMgr.h"
#include "Cube.h"
#include "Pipeline.h"
#include "Frustum.h"
#include "KeyMgr.h"
#include "ItemTableMgr.h"
//#include "TextMgr.h"

#include "Slot.h"
#include "UI.h"
#include "Bar.h"
#include "Hpbar.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "BeatMiss.h"
#include "Item.h"
#include "Tile.h"
#include "PlayerAttack.h"
#include "Monkey.h"
#include "Freeze.h"
#include "KaBomb.h"
#include "FireBall.h"
#include "ComboMgr.h"
#include "Etc.h"
#include "Monster.h"

CPlayer::CPlayer()
: m_fCamDistance(35.f)
, m_pFrustum(CFrustum::GetInstance())
, m_pMonkey(NULL)
, m_bJumpSetting(false)
, m_bJumpSwap(false)
, m_bActionCam(false)
{
	ZeroMemory(&m_tUnitInfo, sizeof(m_tUnitInfo));
}

CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize(void)
{
	m_tUnitInfo = UNIT(2, 4, 0);
	m_tUnitInfo.iDiamond = 0;
	m_tUnitInfo.iMoney = 140;

	m_dwVtxCnt = 8;
	m_dwIndexCnt = 12;

	m_pCubeTex = new VTXCUBE[m_dwVtxCnt];
	m_pConvertCubeTex = new VTXCUBE[m_dwVtxCnt];
	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pSlot = ((CSlot*)CObjFactory<CSlot>::CreateObj());
	CItemTableMgr::GetInstance()->SetSlot(m_pSlot);
	m_pObjMgr->AddObj(OBJ_UI, m_pSlot);
	m_pHpBar = ((CHpbar*)CObjFactory<CHpbar>::CreateObj());
	m_pObjMgr->AddObj(OBJ_UI, m_pHpBar);

	m_pSlot->AddSlot(ITEM_SHOVEL, TEXT("Shovel_Basic"),0,1);
	m_pSlot->AddSlot(ITEM_ATTACK, TEXT("Dagger"),0,1);
	m_pSlot->AddSlot(ITEM_BOMB, TEXT("Bomb"),0,1);

	m_pBufferMgr->GetVtxInfo(TEXT("CubeTex"), m_pCubeTex);
	m_pBufferMgr->GetIndex(TEXT("CubeTex"), m_pIndex);

	ZeroMemory(m_fAngle, sizeof(float)*3);
	ZeroMemory(&m_tCube, sizeof(m_tCube));

	m_dwState = 0;
	m_iCommenCnt = 0;
	m_iAttCombo = 0;
	m_Commend = 0;
	m_fSpeed = 2.f;
	m_fMoveDis = 0.f;
	m_fRotAngle = 0.f;
	m_fRotPlayer = 0.f;
	m_fTotalAngle = 0.f;
	m_fMJTime = 0.f;
	m_fMegaTime = 0.f;
	m_fMegaY = 0.f;
	m_vCamDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_fCamAngle[ANGLE_X] = D3DXToRadian(60.f);
	m_fCamAngle[ANGLE_Y] = 0.f; 
	m_fCamAngle[ANGLE_Z] = 0.f;
	m_iIndex = m_pObjMgr->GetIndex(m_tInfo.vPos);

	m_eObjId = OBJ_PLAYER;

	SetCubeTexture(0);
	SetWeaponPos(); // 무기 범위 셋팅

	LoadObject(TEXT("../Data/Player/PlayerObject.dat"));
	LoadAnimation(TEXT("../Data/Player/PlayerIdle.dat"), STAT_IDLE);
	LoadAnimation(TEXT("../Data/Player/PlayerJump.dat"), STAT_MOVE);

	return S_OK;
}

void CPlayer::SetWeaponPos() // 무기 범위 좌표 셋팅
{
	m_vecWeapon[WP_DAGGER].push_back(D3DXVECTOR3(0.f, 0.f, 2.f));
	m_vecWeapon[WP_BROAD].push_back(D3DXVECTOR3(0.f, 0.f, 2.f));
	m_vecWeapon[WP_BROAD].push_back(D3DXVECTOR3(2.f, 0.f, 2.f));
	m_vecWeapon[WP_BROAD].push_back(D3DXVECTOR3(-2.f, 0.f, 2.f));
	m_vecWeapon[WP_SPEAR].push_back(D3DXVECTOR3(0.f, 0.f, 2.f));
	m_vecWeapon[WP_SPEAR].push_back(D3DXVECTOR3(0.f, 0.f, 4.f));
	m_vecWeapon[WP_WHIP].push_back(D3DXVECTOR3(0.f, 0.f, 2.f));
	m_vecWeapon[WP_WHIP].push_back(D3DXVECTOR3(-2.f, 0.f, 2.f));
	m_vecWeapon[WP_WHIP].push_back(D3DXVECTOR3(2.f, 0.f, 2.f));
	m_vecWeapon[WP_WHIP].push_back(D3DXVECTOR3(-4.f, 0.f, 2.f));
	m_vecWeapon[WP_WHIP].push_back(D3DXVECTOR3(4.f, 0.f, 2.f));

	for(int i = 1; i < 4; ++i)
	{
		for(int j = 0; j < i*2-1; ++j)
			m_vecWeapon[WP_BLUNDERBUSS].push_back(D3DXVECTOR3((i-1)*2.f-(j*2.f), 0.f, i*2.f));
	}
}

const int CPlayer::Progress(void)
{
	D3DXVECTOR3 vTemp = m_tInfo.vPos;
	KeyInput();
	AniPlay(m_eStatKey);
	SetCamera();
	SetMatrix(1.3f);
	SetPlayerDirection();
	PlayerState(); // 플레이어의 상태 키값의 따라 행동 함수

	UsingOBB();//Test

	m_pHpBar->RefreshHp(m_tUnitInfo.iHp, m_tUnitInfo.iMaxHp);

	for(int i = 0; i < m_iChildCnt; ++i)
	{
		m_vecChild[i]->SetAniStateKey(m_eStatKey);
		m_vecChild[i]->Progress();
	}

	MegaJump(); // 점프Y값수정

	return 0;
}

void CPlayer::Render(void)
{
	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pConvertCubeTex);

	m_pDevice->GetDevice()->SetTexture(0, m_pCubeTexture);
	//m_pDevice->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pDevice->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pBufferMgr->RenderBuffer(TEXT("CubeTex"));

	for(int i = 0; i < m_iChildCnt; ++i)
		m_vecChild[i]->Render();
}

void CPlayer::SetMatrix(const float fScale)
{
	D3DXMATRIX matWorld;
	D3DXMATRIX matObjWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	float	   fAngle[ANGLE_END];

	fAngle[ANGLE_X] = m_fAngle[ANGLE_X];
	fAngle[ANGLE_Y] = m_fAngle[ANGLE_Y] + m_fCamAngle[ANGLE_Y];
	fAngle[ANGLE_Z] = m_fAngle[ANGLE_Z];

	CPipeline::MakeWorldMatrix(matWorld, D3DXVECTOR3(fScale, fScale, fScale), fAngle, m_tInfo.vPos);

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

void CPlayer::SetCamera()
{
	if(!m_bActionCam)
	{
		D3DXVECTOR3 vCamPos = D3DXVECTOR3(0.f, 0.f, 0.f);
		D3DXVECTOR3 vPlayerPos = m_tInfo.vPos;

		CMathMgr::MyRotationY(m_vCamDir, g_vLook, m_fCamAngle[ANGLE_Y]);
		D3DXVec3Normalize(&m_vCamDir, &m_vCamDir);

		vCamPos = -m_vCamDir;
		vCamPos *= m_fCamDistance;

		D3DXVECTOR3 vRight = D3DXVECTOR3(0.f, 0.f, 0.f);
		D3DXVec3Cross(&vRight, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_vCamDir);
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXMATRIX matRotAxis;
		D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fCamAngle[ANGLE_X]);

		D3DXVec3TransformNormal(&vCamPos, &vCamPos, &matRotAxis);

		vCamPos += vPlayerPos;

		if(m_dwState & STATE_MAPJUMP)
		{
			if(!m_bJumpSetting)
			{
				m_vCamMovingPos[1] = m_vCamDir * 15.f;
				m_vCamMovingPos[0] = D3DXVECTOR3(0.f, 0.f, 0.f);
				m_bJumpSetting = true;
			}
			MapJump(); // 맵점프
			vCamPos += m_vCamDis;
		}

		m_pCamMgr->SetCamPosition(vCamPos, vPlayerPos, D3DXVECTOR3(0.f, 1.f, 0.f));

		m_pFrustum->SetFrustum();
	}
	else
	{
		m_dwState &= (~STATE_MAPJUMP);
		m_pObjMgr->GetTile(m_iIndex)->SetObj(this);
	}
}

void CPlayer::KeyInput()
{
	int iCommendLimit = 8; // 누적반복 횟수

	if(m_bKeyTiming)
	{
		if(m_pKeyMgr->KeyPressed(KEY_UP, 0) == KEYDOWN
			|| m_pKeyMgr->KeyPressed(KEY_UP, 0) == KEYDRAG)
			m_Commend |= KEY_UP;
		if(m_pKeyMgr->KeyPressed(KEY_DOWN, 0) == KEYDOWN
			|| m_pKeyMgr->KeyPressed(KEY_DOWN, 0) == KEYDRAG)
			m_Commend |= KEY_DOWN;
		if(m_pKeyMgr->KeyPressed(KEY_LEFT, 0) == KEYDOWN
			|| m_pKeyMgr->KeyPressed(KEY_LEFT, 0) == KEYDRAG)
			m_Commend |= KEY_LEFT;
		if(m_pKeyMgr->KeyPressed(KEY_RIGHT, 0) == KEYDOWN
			|| m_pKeyMgr->KeyPressed(KEY_RIGHT, 0) == KEYDRAG)
			m_Commend |= KEY_RIGHT;
		if(m_pKeyMgr->KeyPressed(KEY_SPACE, 0) == KEYDOWN
			|| m_pKeyMgr->KeyPressed(KEY_SPACE, 0) == KEYDRAG)
			m_Commend |= KEY_SPACE;

		if(m_Commend && m_iCommenCnt > iCommendLimit) // 커맨드
		{
			if(m_Commend == (KEY_RIGHT | KEY_SPACE)
				&& BeatKeyCheck())
			{
				m_fRotAngle = m_fCamAngle[ANGLE_Y];
				m_fRotPlayer = m_fAngle[ANGLE_Y];
				m_dwState |= STATE_RROT;
				m_bKeyTiming = false;
			}
			else if(m_Commend == (KEY_LEFT | KEY_SPACE)
				&& BeatKeyCheck())
			{
				m_fRotAngle = m_fCamAngle[ANGLE_Y];
				m_fRotPlayer = m_fAngle[ANGLE_Y];
				m_dwState |= STATE_LROT;
				m_bKeyTiming = false;
			}
			else if(m_Commend == KEY_SPACE //점프
				&& BeatKeyCheck())
			{
				m_dwState |= STATE_MAPJUMP;
				m_bKeyTiming = false;
			}
			else if(m_Commend == (KEY_RIGHT | KEY_UP)//스펠 A
				&& BeatKeyCheck())
			{
				SpellCasting(1);
				m_bKeyTiming = false;
			}
			else if(m_Commend == (KEY_RIGHT | KEY_DOWN)//스펠 B
				&& BeatKeyCheck())
			{
				SpellCasting(2);
				m_bKeyTiming = false;
			}
			else if(m_Commend == (KEY_LEFT | KEY_UP)	//아이템 사용
				&& BeatKeyCheck())
			{
				UsingItem();
				m_bKeyTiming = false;
			}
			else if(m_Commend == (KEY_LEFT | KEY_RIGHT)	//팩 사용
				&& BeatKeyCheck())
			{
				UsingPack();
				m_bKeyTiming = false;
			}
			else if(m_Commend == (KEY_UP | KEY_DOWN)	//무기집 사용
				&& BeatKeyCheck())
			{
				SwitchAttack();
				m_bKeyTiming = false;
			}
			else if(m_Commend == (KEY_LEFT | KEY_DOWN)	//폭탄사용
				&& BeatKeyCheck())
			{
				UsingBomb();
				m_bKeyTiming = false;

				m_pDevice->SoundPlay(TEXT("Bomb_Ready"), 0);
			}
			else if(m_Commend == KEY_UP
				&& BeatKeyCheck())
			{
				m_dwState |= STATE_MOVE;
				m_fAngle[ANGLE_Y] = 0.f;
				m_bKeyTiming = false;
			}
			else if(m_Commend == KEY_DOWN
				&& BeatKeyCheck())
			{
				m_dwState |= STATE_MOVE;
				m_fAngle[ANGLE_Y] = D3DXToRadian(180.f);
				m_bKeyTiming = false;
			}
			else if(m_Commend == KEY_LEFT
				&& BeatKeyCheck())
			{
				m_dwState |= STATE_MOVE;
				m_fAngle[ANGLE_Y] = D3DXToRadian(-90.f);
				m_bKeyTiming = false;
			}
			else if(m_Commend == KEY_RIGHT
				&& BeatKeyCheck())
			{
				m_dwState |= STATE_MOVE;
				m_fAngle[ANGLE_Y] = D3DXToRadian(90.f);
				m_bKeyTiming = false;
			}
			else
			{
				m_pObjMgr->AddObj(OBJ_EFFECT, CObjFactory<CBeatMiss>::CreateObj());
				CComboMgr::GetInstance()->SetCombo(0);
				m_bKeyTiming = false;
			}
			if(m_Commend != 0)
				m_TempCommend = m_Commend;

			m_Commend = 0;
		}
	}

	if(m_iCommenCnt > iCommendLimit // 커맨드 누적 횟수
		&& (m_pKeyMgr->KeyPressed(KEY_UP, 0) == KEYEND || m_pKeyMgr->KeyPressed(KEY_UP, 0) == KEYUP)
		&& (m_pKeyMgr->KeyPressed(KEY_DOWN, 0) == KEYEND || m_pKeyMgr->KeyPressed(KEY_DOWN, 0) == KEYUP)
		&& (m_pKeyMgr->KeyPressed(KEY_LEFT, 0) == KEYEND || m_pKeyMgr->KeyPressed(KEY_LEFT, 0) == KEYUP)
		&& (m_pKeyMgr->KeyPressed(KEY_RIGHT, 0) == KEYEND || m_pKeyMgr->KeyPressed(KEY_RIGHT, 0) == KEYUP)
		&& (m_pKeyMgr->KeyPressed(KEY_SPACE, 0) == KEYEND || m_pKeyMgr->KeyPressed(KEY_SPACE, 0) == KEYUP))
	{
		m_Commend = 0;
 		m_iCommenCnt = 0;
		m_bKeyTiming = true;
	}

	if(GetAsyncKeyState('W') & 0x8000)
		m_fCamDistance -= 5.f * m_pTimeMgr->GetTime();
	if(GetAsyncKeyState('S') & 0x8000)
		m_fCamDistance += 5.f * m_pTimeMgr->GetTime();
	if(GetAsyncKeyState('A') & 0x8000)
		m_fCamAngle[ANGLE_X] += D3DXToRadian(120.f) * m_pTimeMgr->GetTime();
	if(GetAsyncKeyState('D') & 0x8000)
		m_fCamAngle[ANGLE_X] -= D3DXToRadian(120.f) * m_pTimeMgr->GetTime();
	if(GetAsyncKeyState('U') & 0x8000)
	{
		m_pObjMgr->SetActionCam(false);
		m_pObjMgr->SetStageEnd(true);
		m_bActionCam = false;
	}
	if(GetAsyncKeyState('M') & 0x8000)
	{
	}

	++m_iCommenCnt;
}

bool CPlayer::BeatKeyCheck()
{
	list<CObj*>* UIlist = CObjMgr::GetInstance()->GetObjList(OBJ_UI);

	float iMin1 = 99999;
	CObj* pObj1 = NULL;
	CObj* pObj2= NULL;
	for(list<CObj*>::iterator iter = UIlist->begin();
		iter != UIlist->end(); iter++ )
	{

		if((*iter)->GetObjkey() != TEXT("Bar") )
			continue;
		if ( ((CBar*)(*iter))->GetProgress() )
			continue;

		if (iMin1  >= abs((*iter)->GetInfo()->vPos.x))
		{
			iMin1 = abs((*iter)->GetInfo()->vPos.x);
			pObj2 = pObj1;
			pObj1 = (*iter);
		}
	}

	if( -9.0 <pObj1->GetInfo()->vPos.x && pObj1->GetInfo()->vPos.x <9.0 
		&&  -9.0 <pObj2->GetInfo()->vPos.x && pObj2->GetInfo()->vPos.x <9.0 
		&& ( ((CBar*)pObj1)->GetBeatDir() == BEAT_LEFT && ((CBar*)pObj2)->GetBeatDir() == BEAT_RIGHT
		|| ((CBar*)pObj2)->GetBeatDir() == BEAT_LEFT && ((CBar*)pObj1)->GetBeatDir() == BEAT_RIGHT) )
	{

		((CBar*)pObj1)->SetProgress(1);
		((CBar*)pObj2)->SetProgress(1);
		return true;
	}
	else
		return false;
}

const float CPlayer::GetCamAngleY() const
{
	return m_fCamAngle[ANGLE_Y];
}

CMonkey* CPlayer::GetIsMonkey()
{
	return m_pMonkey;
}

void CPlayer::SetCubeInfo(const CUBE _tCube)
{
	m_tCube.iDrawId = _tCube.iDrawId;
	m_tCube.dwType = _tCube.dwType;
	m_tCube.vPos = _tCube.vPos;
	m_tInfo.vPos = _tCube.vPos;

	SetCubeTexture(m_tCube.iDrawId);
} 

void CPlayer::SetIsMonkey(CMonkey* pMonkey)
{
	m_pMonkey = pMonkey;
}

void CPlayer::Release()
{
	::Safe_Destroy(m_pFrustum);
	for_each(m_vecChild.begin(), m_vecChild.end(), CDeleteObj());
	m_vecChild.clear();

	for(int i = 0; i < WP_END; ++i)
		m_vecWeapon[i].clear();

	for(int i = 0; i < STAT_END; ++i)
	{
		for_each(m_vecAni[i].begin(), m_vecAni[i].end(), CDeleteObj());
		m_vecAni[i].clear();
	}
}

void CPlayer::SetPlayerDirection()
{
	D3DXVec3TransformNormal(&m_tInfo.vDir, &g_vLook, &m_tInfo.matWorld);
	m_tInfo.vDir.y = 0.f;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
}

void CPlayer::PlayerState()
{
	float fBeatTime = m_pBeatMgr->GetBeatMax();
	float fRotAngle = 0.f;
	float fRadian = 0.f;

	if(m_dwState & STATE_RROT)
		fRadian = D3DXToRadian(90.f); // 회전할각도
	else if(m_dwState & STATE_LROT)
		fRadian = -D3DXToRadian(90.f); // 회전할각도

	if(m_dwState & STATE_RROT
		|| m_dwState & STATE_LROT) // 회전일 경우
	{
		m_iAttCombo = 0;
		fRotAngle = fRadian * GETTIME/fBeatTime * 5.f;

		m_fCamAngle[ANGLE_Y] += fRotAngle;
		m_fAngle[ANGLE_Y] -= fRotAngle;
		m_fTotalAngle += fRotAngle;

		if(m_fTotalAngle >= D3DXToRadian(90.f)
			|| m_fTotalAngle <= -D3DXToRadian(90.f))
		{
			m_fCamAngle[ANGLE_Y] = m_fRotAngle+fRadian;
			m_fAngle[ANGLE_Y] = m_fRotPlayer-fRadian;

			if((m_fCamAngle[ANGLE_Y] > D3DXToRadian(350.f)
				&& m_fCamAngle[ANGLE_Y] < D3DXToRadian(370.f))
				|| (m_fCamAngle[ANGLE_Y] < -D3DXToRadian(350.f)
				&& m_fCamAngle[ANGLE_Y] > -D3DXToRadian(370.f)))
				m_fCamAngle[ANGLE_Y] = 0.f;

			if((m_fAngle[ANGLE_Y] > D3DXToRadian(350.f)
				&& m_fAngle[ANGLE_Y] < D3DXToRadian(370.f))
				|| (m_fAngle[ANGLE_Y] < -D3DXToRadian(350.f)
				&& m_fAngle[ANGLE_Y] > -D3DXToRadian(370.f)))
				m_fAngle[ANGLE_Y] = 0.f;


			m_pCamMgr->SetShake();

			m_dwState &= (~STATE_RROT);
			m_dwState &= (~STATE_LROT);
		}
	}

	if(m_dwState & STATE_MOVING
		&& m_eTargetObjId == OBJ_END) // 강제로 움직이는 행동
	{
		float fDistance = m_fSpeed * (GETTIME/(0.3f*fBeatTime));
		m_fMoveDis += fDistance;

		//타일에 현재 좌표 표시 //////////////////////
		m_pObjMgr->GetTile(m_iIndex)->ResetObj();
		m_iIndex = m_pObjMgr->GetIndex(m_pTargetTile->GetInfo()->vPos);
		///////////////////////////////////////////////

		if(m_fMoveDis < 1.9f)
			m_tInfo.vPos += m_vMovingPos * fDistance;
		else
		{
			m_tInfo.vPos = m_pTargetTile->GetInfo()->vPos;
			RootingItem(m_pTargetTile);
			m_dwState &= (~STATE_MOVING);
		}
	}

	if(m_dwState & STATE_MOVE) // 방향키를 눌렀을때
	{
		if(m_dwState & STATE_IDLE
			&& !(m_dwState & STATE_MONGATT)
			&& !m_pMonkey) // 방향키 위치 탐색
			FindMovePoint();
		else if(m_pMonkey) // 몽키가 달라붙어 있을경우
			m_dwState |= STATE_MONGATT;

		if(m_eTargetObjId == OBJ_END
			&& !(m_dwState & STATE_ATT)
			&& !(m_dwState & STATE_MONGATT)) // 타일에 아무것도 없을때
		{
			//타일에 현재 좌표 표시 //////////////////////
			m_pObjMgr->GetTile(m_iIndex)->ResetObj();
			m_iIndex = m_pObjMgr->GetIndex(m_vTargetTile);
			///////////////////////////////////////////////
			m_eStatKey = STAT_MOVE;
			float fDistance = m_fSpeed * (GETTIME/(0.5f*fBeatTime));
			m_fMoveDis += fDistance;

			if(m_fMoveDis < 1.9f)
				m_tInfo.vPos += m_tInfo.vDir * fDistance;
			else
			{
				m_dwState &= (~STATE_MOVE);
				RootingItem(m_pTargetTile);
				m_tInfo.vPos = m_pTargetTile->GetInfo()->vPos;

				if(m_pMonkey) // 몽키가 달라붙어 있을경우
					m_dwState |= STATE_MONGATT;
			}
		}
		if(m_eTargetObjId == OBJ_CUBE) // 큐브가 있을때
		{
			m_dwState &= (~STATE_MOVE);

			if(m_pMonkey) // 몽키가 달라붙어 있을경우
				m_dwState |= STATE_MONGATT;

			else if(m_pTargetTile->GetObj()
				&& !(m_pTargetTile->GetObj()->GetCubeInfo().dwType & CUBE_NOTB)
				&& !(m_pTargetTile->GetObj()->GetCubeInfo().dwType & CUBE_BOOM))
			{
				m_pDevice->SoundPlay(TEXT("Player_Dig"),0);
				m_pTargetTile->GetObj()->SetDelete();
			}
			else
				m_pDevice->SoundPlay(TEXT("Player_DigFail"),0);
			m_pCamMgr->SetShake(0.3f);
		}
		if(m_eTargetObjId == OBJ_ETC)
		{
			m_dwState &= (~STATE_MOVE);

			if(m_pMonkey) // 몽키가 달라붙어 있을경우
				m_dwState |= STATE_MONGATT;
			else if(m_pTargetTile->GetObj()
				&& ((CEtc*)m_pTargetTile->GetObj())->GetIsItem()) // 상자가 있을때
				m_pTargetTile->GetObj()->SetDelete();
			m_pCamMgr->SetShake(0.3f);
		}
		if(m_dwState & STATE_MONGATT)  // 원숭이 때리기6
		{
			m_dwState &= (~STATE_MOVE);
			m_pMonkey->SetDamage(m_tUnitInfo.iAtt);
			m_pMonkey->SetHitSound();
			m_pMonkey->SetDamageDirection(m_tInfo.vPos);
			m_pCamMgr->SetShake(0.3f);
		}
	}

	if(m_dwState & STATE_JUMP)
		m_eStatKey = STAT_MOVE;

	if(!(m_dwState & STATE_MOVE) 
		&& !(m_dwState & STATE_ATT)
		&& !(m_dwState & STATE_HIT)
		&& !(m_dwState & STATE_RROT)
		&& !(m_dwState & STATE_LROT)
		&& !(m_dwState & STATE_SHOVEL)
		&& !(m_dwState & STATE_MOVING)
		&& !(m_dwState & STATE_JUMP)
		&& !(m_dwState & STATE_MAPJUMP))
	{
		m_fMoveDis = 0.f;
		m_fRotAngle = 0.f;
		m_fRotPlayer = 0.f;
		m_fTotalAngle = 0.f;
		m_fMJTime = 0.f;
		m_eStatKey = STAT_IDLE;
		m_dwState |= STATE_IDLE;
	}
	if(!m_pMonkey)
		m_dwState &= (~STATE_MONGATT);

	m_pObjMgr->GetTile(m_iIndex)->SetObj(this);
}

int CPlayer::FindMovePoint()
{
	m_vTargetTile = m_tInfo.vPos + m_tInfo.vDir * m_fSpeed; // 목표 타일 포스
	m_pTargetTile = m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_vTargetTile)); // 목표 타일의 포인터
	m_eTargetObjId = m_pTargetTile->GetTileObjId();
	m_dwState &= (~STATE_IDLE);

	if(m_eTargetObjId != OBJ_CUBE)
		AttackPoint(); // 무기의 따른 어택포인트 탐색

	return 0;
}

void CPlayer::MapJump() ////////// 점프 관련////////////
{
	float fBeatTime = 0.1f * m_pBeatMgr->GetBeatMax();
	m_fMJTime += GETTIME;

	if(m_fMJTime >= fBeatTime)
	{
		if(!m_bJumpSwap)
		{
			D3DXVECTOR3 vTemp = m_vCamMovingPos[0];
			m_vCamMovingPos[0] = m_vCamMovingPos[1];
			m_vCamMovingPos[1] = vTemp;

			m_fMJTime = 0.f;
			m_dwState |= STATE_JUMP;
			m_bJumpSwap = true;
			m_pObjMgr->SetJump(true);
		}
		else if(m_bJumpSwap)
		{
			m_fMJTime = 0.f;
			m_bJumpSetting = false;
			m_bJumpSwap = false;
			m_dwState &= (~STATE_MAPJUMP);
		}
	}

	if(m_fMJTime <= fBeatTime && m_bJumpSetting)
	{
		m_vCamDis.x = Linear(m_vCamMovingPos[0].x, m_vCamMovingPos[1].x, fBeatTime, m_fMJTime);
		m_vCamDis.y = Linear(m_vCamMovingPos[0].y, m_vCamMovingPos[1].y, fBeatTime, m_fMJTime);
		m_vCamDis.z = Linear(m_vCamMovingPos[0].z, m_vCamMovingPos[1].z, fBeatTime, m_fMJTime);
	}
}

bool CPlayer::AttackPoint()
{
	wstring strWeapon = m_pSlot->GetSlotItemKey(ITEM_ATTACK);
	WEAPONID eWeaponId = WP_END;
	D3DXVECTOR3 vWeapon;
	D3DXVECTOR3 vTile;
	CTile* pTile;

	if(strWeapon == TEXT("Dagger"))
		eWeaponId = WP_DAGGER;
	else if(strWeapon == TEXT("Broad_Sword"))
		eWeaponId = WP_BROAD;
	else if(strWeapon == TEXT("Spear"))
		eWeaponId = WP_SPEAR;
	else if(strWeapon == TEXT("Whip"))
		eWeaponId = WP_WHIP;
	else if(strWeapon == TEXT("Blunderbuss"))
		eWeaponId = WP_BLUNDERBUSS;

	int iTileNum = 0; // 때릴 타일 넘버
	for(size_t i = 0; i < m_vecWeapon[eWeaponId].size(); ++i)
	{
		CMathMgr::MyRotationY(vWeapon, m_vecWeapon[eWeaponId][i], m_fAngle[ANGLE_Y]+m_fCamAngle[ANGLE_Y]);

		vTile = m_tInfo.vPos + vWeapon; // 목표 타일 포스
		pTile = m_pObjMgr->GetTile(m_pObjMgr->GetIndex(vTile));

		if(pTile->GetTileObjId() == OBJ_MONSTER
			&& !((CMonster*)pTile->GetObj())->GetHide()) // 목표 타일의 포인터
		{
			if(eWeaponId == WP_WHIP) //채찍의 판정
			{
				if(i > 2)
				{
					CMathMgr::MyRotationY(vWeapon, m_vecWeapon[eWeaponId][i-2], m_fAngle[ANGLE_Y]+m_fCamAngle[ANGLE_Y]);

					if(m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos+vWeapon))->GetTileObjId() != OBJ_CUBE)
					{
						m_vecMonster.push_back(pTile->GetObj());
						iTileNum = i;
						break;
					}
				}
				else if(i < 3)
				{
					m_vecMonster.push_back(pTile->GetObj());
					iTileNum = i;
					break;
				}
			}
			else if(eWeaponId == WP_SPEAR && i == 0) // 스피어 첫째칸에 몬스터가 있을경우
			{
				m_vecMonster.push_back(pTile->GetObj());
				MonsterDamage(); // 몬스터 데미지 주기
				CreateAttackEffect(WP_DAGGER, iTileNum); // 이펙트 생성
				return true;
			}
			else if(eWeaponId == WP_BLUNDERBUSS)
			{
				m_vecMonster.push_back(pTile->GetObj());
				m_vTargetTile = m_tInfo.vPos + (-m_tInfo.vDir) * m_fSpeed; // 목표 타일 포스

				Moving(m_vTargetTile); // 뒤로 가기 판정 함수
				m_pDevice->SoundPlay(TEXT("Player_Blunderbuss"), 0);
			}
			else
				m_vecMonster.push_back(pTile->GetObj());
		}
	} 

	if(!m_vecMonster.size()) // 백터안에 몬스터가 없으면 false
	{
		m_iAttCombo = 0;
		return false;
	}
	else
	{
		if(m_iAttCombo < 4)
			++m_iAttCombo;

		TCHAR szAttSound[MIN_STR] = TEXT("");
		wsprintf(szAttSound, TEXT("Player_Att%d"), m_iAttCombo);
		CDevice::GetInstance()->SoundPlay(szAttSound, 0);
		CDevice::GetInstance()->SoundPlay(TEXT("Player_Hit"), 0);

		MonsterDamage(); // 몬스터 데미지 주기
		CreateAttackEffect(eWeaponId, iTileNum); // 이펙트 생성
	}

	return true;
}

void CPlayer::MonsterDamage()
{
	m_dwState &= (~STATE_MOVE);
	m_dwState |= STATE_ATT;

	for(vector<CObj*>::iterator iter = m_vecMonster.begin();
		iter != m_vecMonster.end(); ++iter)
	{
		((CMonster*)(*iter))->SetHitSound();
		((CMonster*)(*iter))->SetDamageDirection(m_tInfo.vPos);
		(*iter)->SetDamage(m_tUnitInfo.iAtt);
	}

	m_vecMonster.clear();
	m_pCamMgr->SetShake(0.3f);
}

void CPlayer::CreateAttackEffect(WEAPONID eWeaponId, int iNum) // WeaponEffect
{
	CPlayerAttack* pEffect = (CPlayerAttack*)CObjFactory<CPlayerAttack>::CreateObj();

	pEffect->SetAngle(m_fAngle[ANGLE_Y]-(90.f/180.f * D3DX_PI));
	pEffect->SetPos(D3DXVECTOR3(m_tInfo.vPos.x, 1.f, m_tInfo.vPos.z) + (m_tInfo.vDir*2));

	switch(eWeaponId)
	{
	case WP_DAGGER:
		pEffect->SetStateKey(TEXT("Dagger"));
		break;
	case WP_BROAD:
		pEffect->SetStateKey(TEXT("Broad"));
		break;
	case WP_SPEAR:
		pEffect->SetStateKey(TEXT("Spear"));
		pEffect->SetPos(D3DXVECTOR3(m_tInfo.vPos.x, 1.f, m_tInfo.vPos.z) + (m_tInfo.vDir*3));
		break;
	case WP_WHIP:
		switch(iNum)
		{
		case 0:
			pEffect->SetStateKey(TEXT("Whip_C"));
			break;
		case 1:
			pEffect->SetStateKey(TEXT("Whip_B"));
			break;
		case 2:
			pEffect->SetStateKey(TEXT("Whip_D"));
			break;
		case 3:
			pEffect->SetStateKey(TEXT("Whip_A"));
			break;
		case 4:
			pEffect->SetStateKey(TEXT("Whip_E"));
			break;
		}
		break;
	case WP_BLUNDERBUSS:
		pEffect->SetStateKey(TEXT("Blunderbuss"));
		pEffect->SetPos(D3DXVECTOR3(m_tInfo.vPos.x, 1.f, m_tInfo.vPos.z) + (m_tInfo.vDir*4));
		break;
	}

	m_pObjMgr->AddObj(OBJ_EFFECT, pEffect);
}
void CPlayer::SpellCasting(int Spell)
{
	if(!(m_pSlot->GetSlotType() & ITEM_SPELL))
		return;
	if (m_pSlot->GetSlotItemKey(ITEM_SPELL,Spell) == TEXT("SpellCool") ||
		m_pSlot->GetSlotItemKey(ITEM_SPELL,Spell) == TEXT(""))//쿨다운 체크
		return;

	wstring pSpellName = m_pSlot->GetSlotItemKey(ITEM_SPELL, Spell);
	m_pSlot->AddSlot(ITEM_SPELL, TEXT("SpellCool"), Spell);

	if(pSpellName == TEXT("Freeze")) // 발동한 스킬이 프리즈(트리즈 아님) 일경우
	{
		CFreeze* pEffect = (CFreeze*)CObjFactory<CFreeze>::CreateObj();
		pEffect->SetAngle(m_fAngle[ANGLE_Y]-(90.f/180.f * D3DX_PI));
		pEffect->SetPos(D3DXVECTOR3(m_tInfo.vPos.x, 1.f, m_tInfo.vPos.z) + (m_tInfo.vDir*4));
		pEffect->SetDamageInfo(m_tInfo.vPos, m_fAngle[ANGLE_Y]+m_fCamAngle[ANGLE_Y]);
		m_pObjMgr->AddObj(OBJ_EFFECT, pEffect);
		m_pDevice->SoundPlay(TEXT("Player_Freeze"), 0);
	}
	if(pSpellName == TEXT("Fireball")) // 발동한 스킬이 파이어 볼 일경우
	{
		CFireBall* pEffect = (CFireBall*)CObjFactory<CFireBall>::CreateObj(m_tInfo.vPos);
		m_pObjMgr->AddObj(OBJ_EFFECT, pEffect);
		m_pDevice->SoundPlay(TEXT("Player_FireBall"), 0);
		m_pDevice->SetSoundVolume(TEXT("Player_FireBall"), 1800);
		
	}
}

void CPlayer::RootingItem(CTile* pTile)
{
	CItem* pItem = pTile->GetItem();
	
	if(pItem)
	{
		DWORD dwItemType = pItem->GetType();
		wstring strObjKey = pItem->GetObjkey();


		if(pItem->GetPrice() != 0
			&& !(dwItemType & ITEM_GOLD))
		{
			if( m_tUnitInfo.iMoney -  pItem->GetPrice() >= 0)
			{
				m_tUnitInfo.iMoney -=  pItem->GetPrice();
				pItem->SetPrice(0);
				pItem->SetSelling(false);
				m_pDevice->SoundPlay(TEXT("Pickup_Purchase"), 0);
			}
			else
			{
				m_vTargetTile = m_tInfo.vPos + (-m_tInfo.vDir) * m_fSpeed; // 목표 타일 포스
				Moving(m_vTargetTile);
				return;
			}
		}
		else if(dwItemType & ITEM_GOLD) // 돈먹기
		{       
			m_tUnitInfo.iMoney += pItem->GetPrice();
			m_pDevice->SoundPlay(TEXT("Player_Gold_Pick"), 0);
			pItem->SetDelete();
			return;
		}


		if(m_pSlot->GetSlotType() & dwItemType
			&& !(dwItemType & ITEM_BOMB))
		{
			wstring strObjKey2;
			if((m_pSlot->GetSlotType() & ITEM_HOLSTER) && (m_pSlot->GetSlotItemKey(ITEM_HOLSTER) == TEXT("Holster") )&& (dwItemType & ITEM_ATTACK))
			{
				m_pSlot->AddSlot(ITEM_HOLSTER, strObjKey);
				pItem->SetDelete();
				return;
			}
			else if((m_pSlot->GetSlotType() & ITEM_PACK) && (m_pSlot->GetSlotItemKey(ITEM_PACK) == TEXT("Pack") )&& (dwItemType & ITEM_ITEM))
			{
				m_pSlot->AddSlot(ITEM_PACK, strObjKey);
				pItem->SetDelete();
				return;
			}
			else if(dwItemType & ITEM_SPELL)
			{
				strObjKey2 = m_pSlot->GetSlotItemKey(dwItemType, 1);
				pItem->SetDelete();
			}
			else
			{
				strObjKey2 = m_pSlot->GetSlotItemKey(dwItemType);
				pItem->SetObjKey(strObjKey2);
			}
		}
		else
			pItem->SetDelete();

		if( dwItemType & ITEM_ATTACK || dwItemType & ITEM_HEAD || dwItemType & ITEM_BODY
			|| dwItemType & ITEM_MISC || dwItemType & ITEM_FEET || dwItemType & ITEM_TORCH)
			ItemAbility(strObjKey);

		m_pSlot->AddSlot(dwItemType, strObjKey);
	}
}
void CPlayer::UsingItem(void)
{
	if(!(m_pSlot->GetSlotType() & ITEM_ITEM))
		return;

	wstring wstr = m_pSlot->GetSlotItemKey(ITEM_ITEM);

	if(ItemAbility(wstr))
		m_pSlot->PopSlot(ITEM_ITEM);

	m_pDevice->SoundPlay(TEXT("Player_Heal"), 0);
	m_pDevice->SoundPlay(TEXT("Player_Spell_Heal"), 0);
}

void CPlayer::UsingPack(void)
{

	wstring wstr = m_pSlot->GetSlotItemKey(ITEM_PACK);

	if(!(m_pSlot->GetSlotType() & ITEM_PACK) || wstr == TEXT("Pack"))
		return;

	if(ItemAbility(wstr))
		m_pSlot->AddSlot(ITEM_PACK, TEXT("Pack"),0,1);
}

void CPlayer::SwitchAttack(void)
{
	wstring wstr = m_pSlot->GetSlotItemKey(ITEM_HOLSTER);

	if(!(m_pSlot->GetSlotType() & ITEM_HOLSTER) || wstr == TEXT("Holster"))
		return;

	wstring wstr2 = m_pSlot->GetSlotItemKey(ITEM_ATTACK);

	m_pSlot->AddSlot(ITEM_HOLSTER,wstr2,0, 1);
	m_pSlot->AddSlot(ITEM_ATTACK,wstr,0, 1);
	m_pDevice->SoundPlay(TEXT("Player_Weapon_Pick"), 0);
}

void CPlayer::UsingBomb(void)
{
	if(!(m_pSlot->GetSlotType() & ITEM_BOMB) )
		return;

	CEffect* pKaBomb = ((CEffect*)CObjFactory<CKaBomb>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, 1.f , m_tInfo.vPos.z)));

	m_pObjMgr->AddObj(OBJ_EFFECT, pKaBomb);

	m_pSlot->PopSlot(ITEM_BOMB);
}

void CPlayer::Moving(const D3DXVECTOR3 vTarget)
{
	m_vTargetTile = vTarget; // 목표 타일 포스
	m_pTargetTile = m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_vTargetTile)); // 목표 타일의 포인터
	m_eTargetObjId = m_pTargetTile->GetTileObjId();
	if(m_eTargetObjId == OBJ_END)
	{
		m_vMovingPos = vTarget - m_tInfo.vPos;
		D3DXVec3Normalize(&m_vMovingPos, &m_vMovingPos);
		m_dwState |= STATE_MOVING;
	}
	else
	{
		m_vTargetTile = m_tInfo.vPos; // 목표 타일 포스
		m_pTargetTile = m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_vTargetTile)); // 목표 타일의 포인터
		m_eTargetObjId = m_pTargetTile->GetTileObjId();
	}
}

bool CPlayer::ItemAbility(wstring wstr)
{
	if(wstr == TEXT("Food_Apple"))
	{
		if( (m_tUnitInfo.iMaxHp*2) - m_tUnitInfo.iHp  >= 1)
		{
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Food_Cheese"))
	{
		if( (m_tUnitInfo.iMaxHp*2) - m_tUnitInfo.iHp  >= 1)
		{
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Food_Chicken"))
	{
		if( (m_tUnitInfo.iMaxHp*2) - m_tUnitInfo.iHp  >= 1)
		{
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Food_Meat"))
	{
		if( (m_tUnitInfo.iMaxHp*2) - m_tUnitInfo.iHp  >= 1)
		{
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Helmet"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Leather_Armor"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Boots_Explorers"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Ring_Luck"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Circlet"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Blood_Armor"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Chain_Armor"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Golden_Armor"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Grass_Armor"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Ki_Armor"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Plate_Armor"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Plate_Boots"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Red_Boots"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Crown"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Ring_Charisma"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Ring_Mighty"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Ring_Gold"))
	{
		if( m_tUnitInfo.iMaxHp < 5)
		{
			m_tUnitInfo.iMaxHp += 1;
			m_tUnitInfo.iHp += 2;
			return true;
		}
	}
	else if(wstr == TEXT("Torch_1"))
	{
		m_pObjMgr->SetTorchDistance(4);
		return true;
	}

	return false;
}

D3DXVECTOR3* CPlayer::GetCamDirection()
{
	return &m_vCamDir;
}

void CPlayer::SetCommand(const DWORD dwCommand)
{
	m_TempCommend = dwCommand;
}

const DWORD CPlayer::GetCommand() const
{
	return m_TempCommend;
}

void CPlayer::SetPlayerIndex(D3DXVECTOR3& vPos)
{
	m_iIndex = m_pObjMgr->GetIndex(vPos);
	m_pObjMgr->GetTile(m_iIndex)->SetObj(this);
}

const DWORD CPlayer::GetState() const
{
	return m_dwState;
}

void CPlayer::SetActionCamSwitch(bool bActionCam)
{
	m_bActionCam = bActionCam;
}

