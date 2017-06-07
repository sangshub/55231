#include "StdAfx.h"
#include "Obj.h"
#include "Tile.h"
#include "Device.h"
#include "TimeMgr.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "CamMgr.h"
#include "ObjMgr.h"
#include "BeatMgr.h"
#include "ParticleMgr.h"
#include "Pipeline.h"
#include "KeyMgr.h"
#include "Cube.h"
#include "Frozen.h"
#include "ObjFactory.h"
#include "ComboMgr.h"
#include "Rand.h"
#include "Monster.h"
#include "SceneMgr.h"

CObj::CObj(void)
: m_bIsDelete(false)
, m_bFreeze(false)
, m_bMegaJump(false)
, m_bBoxMove(false)
, m_bStageEnd(false)
, m_bStageStart(false)
, m_dwVtxCnt(0)
, m_dwIndexCnt(0)
, m_iDarkness(0)
, m_fFreezeTime(0.f)
, m_fLightCnt(1.f)
, m_fMegaY(1.f)
, m_fMegaTime(0.f)
, m_fMegaPow(0.6f)
, m_fGravity(-250.f)
, m_fFullTime(0.f)
, m_fSpeed(0.f)
, m_iSceneCnt(0) //애니매이션에 사용되는 씬 개수
, m_fSceneTime(0.f) //애니매이션에 사용되는 씬의 타임 개수
, m_pDevice(CDevice::GetInstance())
, m_pTimeMgr(CTimeMgr::GetInstance())
, m_pBufferMgr(CBufferMgr::GetInstance())
, m_pTextureMgr(CTextureMgr::GetInstance())
, m_pCamMgr(CCamMgr::GetInstance())
, m_pObjMgr(CObjMgr::GetInstance())
, m_pBeatMgr(CBeatMgr::GetInstance())
, m_pKeyMgr(CKeyMgr::GetInstance())
, m_pPaticleMgr(CParticleMgr::GetInstance())
, m_pComboMgr(CComboMgr::GetInstance())
, m_pSceneMgr(CSceneMgr::GetInstance())
, m_pVtxCol(NULL)
, m_pConvertVtxCol(NULL)
, m_pVtxTex(NULL)
, m_pConvertVtxTex(NULL)
, m_pCubeTex(NULL)
, m_pConvertCubeTex(NULL)
, m_pIndex(NULL)
, m_pmatParents(NULL)
, m_pFrozen(NULL)
, m_pvCamDir(NULL)
, m_pTile(NULL)
, m_wstrStateKey(TEXT(""))
, m_wstrObjKey(TEXT(""))
, m_eStatKey(STAT_IDLE)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tUnitInfo, sizeof(UNIT));
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	ZeroMemory(m_fAngle, sizeof(float) * ANGLE_END);
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CObj::~CObj(void)
{
	Release();
}

void CObj::Release()
{
	::Safe_Delete_Array(m_pVtxCol);
	::Safe_Delete_Array(m_pConvertVtxCol);
	::Safe_Delete_Array(m_pVtxTex);
	::Safe_Delete_Array(m_pConvertVtxTex);
	::Safe_Delete_Array(m_pCubeTex);
	::Safe_Delete_Array(m_pConvertCubeTex);
	::Safe_Delete_Array(m_pIndex);

	if(m_pFrozen)
	{
		m_pFrozen->SetDelete();
		m_pFrozen = NULL;
	}
}

const float CObj::GetAngleY() const
{
	return m_fAngle[ANGLE_Y];
}

const INFO* CObj::GetInfo() const
{
	return &m_tInfo;
}

void* CObj::GetVtxPointer() const
{
	if(m_pVtxTex)
		return m_pVtxTex;
	else if(m_pConvertVtxTex)
		return m_pConvertVtxTex;
	else if(m_pCubeTex)
		return m_pCubeTex;
	else if(m_pConvertCubeTex)
		return m_pConvertCubeTex;
	else 
		return NULL;
}

const DWORD CObj::GetVtxCnt() const
{
	return m_dwVtxCnt;
}

 INDEX* CObj::GetIndex() const
{
	return m_pIndex;
}

const DWORD CObj::GetIndexCnt() const
{
	return m_dwIndexCnt;
}

const OBJID CObj::GetObjId() const
{
	return m_eObjId;
}

void CObj::SetPos(const D3DXVECTOR3& vPos)
{
	m_tInfo.vPos = vPos;

	if(m_eObjId == OBJ_TERRAIN || m_eObjId == OBJ_CUBE || m_eObjId == OBJ_EFFECT || m_eObjId == OBJ_UI || m_eObjId == OBJ_PLAYER || m_eObjId == OBJ_TRAP)
		return;

	if(m_eObjId == OBJ_ITEM)
		m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->SetItem((CItem*)this);
	else
		m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->SetObj(this);

}

void CObj::SetObjKey(const wstring& wstrObjKey)
{
	m_wstrObjKey = wstrObjKey;
}

void CObj::SetStateKey(const wstring& wstrStateKey)
{
	m_wstrStateKey = wstrStateKey;
}
void CObj::SetDirection()
{
	D3DXVec3TransformNormal(&m_tInfo.vDir, &g_vLook, &m_tInfo.matWorld);
	m_tInfo.vDir.y = 0.f;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
}

void CObj::FrameMove(const bool bLoop)
{
	m_tFrame.fFrame += m_tFrame.fCnt * GETTIME / m_pBeatMgr->GetBeatMax();

	if(m_tFrame.fFrame > m_tFrame.fMax)
	{
		if(bLoop)
			m_tFrame.fFrame = 0.f;
		else
			m_tFrame.fFrame = m_tFrame.fMax - 0.1f;
	}
}

void CObj::SetFrame(const float fSpeed)
{
	m_tFrame.fMax = (float)m_pTextureMgr->GetImgCount(m_wstrObjKey, m_wstrStateKey, TEXT(""));
	m_tFrame.fCnt = m_tFrame.fMax * fSpeed;
}

void CObj::BeginRenderState(const BYTE byAlpha)
{
	m_pDevice->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0x00000010);
	m_pDevice->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pDevice->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); 
	m_pDevice->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
	m_pDevice->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pDevice->GetDevice()->SetRenderState( D3DRS_TEXTUREFACTOR, 
		D3DCOLOR_ARGB(byAlpha, 255, 255, 255));
}

void CObj::SetDark(const BYTE iDarkness)
{
	m_pDevice->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
	m_pDevice->GetDevice()->SetRenderState(D3DRS_BLENDFACTOR, D3DCOLOR_ARGB(255, iDarkness, iDarkness, iDarkness));
}

void CObj::EndRenderState()
{
	m_pDevice->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pDevice->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pDevice->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE); 
	m_pDevice->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
}

void CObj::SetBillBoard()
{
	D3DXMATRIX matView;

	m_pCamMgr->GetMatrix(D3DTS_VIEW, matView);

	D3DXMATRIX matBill;
	D3DXMatrixIdentity(&matBill);

	matBill = matView;
	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));

	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_tInfo.matWorld = matBill * m_tInfo.matWorld;
}
const wstring CObj::GetObjkey() const
{
	return m_wstrObjKey;
}

const wstring CObj::GetStatekey() const
{
	return m_wstrStateKey;
}

void CObj::SetBeatMax(const float Max)
{
	m_tFrame.fMax = Max;
}
//애니메이션 관련///////////////////////////////////////////////////////////
void CObj::AniPlay(const STATEKEY& eStatKey)
{
	if(m_eAniStateKey != eStatKey)
	{
		m_iSceneCnt = 0;
		m_fSceneTime = 0.f;
		m_fAniFullTime = 0.f;
		m_eAniStateKey = eStatKey;
	}
	if(m_vecAni[m_eStatKey].size() > 1)
	{
		if(AnimationPlayData(m_vecAni[m_eStatKey][m_iSceneCnt], m_vecAni[m_eStatKey][m_iSceneCnt+1]))
		{
			++m_iSceneCnt;
			if(m_iSceneCnt+1 >= m_vecAni[m_eStatKey].size())
			{
				m_iSceneCnt = 0;
				m_fAniFullTime = 0;

				//상태 해제////////////////////////////////
				m_dwState &= (~STATE_MOVE);
				m_dwState &= (~STATE_ATT);
				m_dwState &= (~STATE_HIT);
				m_dwState &= (~STATE_SHOVEL);
				m_dwState &= (~STATE_JUMP);
			}
		}
	}
}

void CObj::BossAniPlay(const STATEKEY& eStatKey)
{
	if(m_eAniStateKey != eStatKey)
	{
		m_iSceneCnt = 0;
		m_fSceneTime = 0.f;
		m_fAniFullTime = 0.f;
		m_eAniStateKey = eStatKey;
	}
	if(m_vecAni[m_eStatKey].size() > 1)
	{
		if(AnimationPlayData(m_vecAni[m_eStatKey][m_iSceneCnt], m_vecAni[m_eStatKey][m_iSceneCnt+1]))
		{
			++m_iSceneCnt;
			if(m_iSceneCnt+1 >= m_vecAni[m_eStatKey].size())
			{
				m_iSceneCnt = 0;
				m_fAniFullTime = 0;

				//상태 해제////////////////////////////////
				if(m_eStatKey == STAT_DDR)
					return;

				m_eStatKey = STAT_IDLE;
			}
		}
	}
}

const bool CObj::AnimationPlayData(ANI* pAni1, ANI* pAni2)
{
	D3DXMATRIX matTrans, matScale, matRot;
	D3DXVECTOR3 vResult; // 이동값 백터
	D3DXVECTOR3 vReScale; // 스케일값 백터
	D3DXQUATERNION Quat;
	float fTime = pAni1->fTime * m_pBeatMgr->GetBeatMax();

	/// 위치값의 선형보간
	vResult.x = Linear(pAni1->vPos.x, pAni2->vPos.x, fTime, m_fSceneTime);
	vResult.y = Linear(pAni1->vPos.y, pAni2->vPos.y, fTime, m_fSceneTime);
	vResult.z = Linear(pAni1->vPos.z, pAni2->vPos.z, fTime, m_fSceneTime);
	D3DXMatrixTranslation( &matTrans, vResult.x, vResult.y, vResult.z );	/// 이동행렬을 구한다.

	/// 스케일값의 선형보간
	vReScale.x = Linear(pAni1->vScale.x, pAni2->vScale.x, fTime, m_fSceneTime);
	vReScale.y = Linear(pAni1->vScale.y, pAni2->vScale.y,fTime, m_fSceneTime);
	vReScale.z = Linear(pAni1->vScale.z, pAni2->vScale.z, fTime, m_fSceneTime);
	D3DXMatrixScaling( &matScale, vReScale.x, vReScale.y, vReScale.z );	/// 크기행렬을 구한다.

	/// 회전값의 구면선형보간
	D3DXQuaternionSlerp( &Quat, &pAni1->qRot, &pAni2->qRot, m_fSceneTime/fTime);
	D3DXMatrixRotationQuaternion( &matRot, &Quat );	/// 사원수를 회전행렬값으로 변환

	m_matLocal = matScale*matRot*matTrans;
	m_matParents = matRot*matTrans;

	m_fSceneTime += GETTIME;
	m_fAniFullTime += GETTIME;

	if( m_fSceneTime >= fTime)
	{
		m_fSceneTime = m_fSceneTime - fTime;
		return true;
	}

	return false;
}

const float CObj::Linear(const float v0, const float v1, const float fTime ,const float t)
{
	return v0 * (1.f - (t / fTime)) + v1 * (t / fTime);
}
// 맵 전체 점프////////////////////////////////////////////////////////////
void CObj::MegaJump() 
{
	if(!m_bMegaJump)
		return;

	float fGravitySpeed = 0.f;
	m_fMegaTime += GETTIME;

	fGravitySpeed = m_fMegaPow*m_fMegaTime + (m_fGravity*m_fMegaTime*m_fMegaTime)*0.5f;
	m_tInfo.vPos.y = m_fMegaY+fGravitySpeed;

	if(m_fMegaTime >= m_fFullTime*0.5f && m_tInfo.vPos.y <= 0.1f)
	{
		m_fMegaTime = 0.f;
		m_tInfo.vPos.y = m_fMegaY;
		m_bMegaJump = false;
	}

	if(m_eObjId == OBJ_CUBE 
		&& m_tCube.dwType & CUBE_THRO)
		BoxMove(m_fMegaTime);
}

void CObj::BoxMove(const float fTime)
{
	OBJID eObjId = OBJ_END;
	if(m_pTile->GetObj())
		eObjId = m_pTile->GetObj()->GetObjId();

	if(!m_bBoxMove
		&& eObjId != OBJ_CUBE
		&& eObjId != OBJ_PLAYER)
	{
		m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_vOriPos))->SetObj(NULL);
		m_bBoxMove = true;
	}

	if(m_bBoxMove)
	{
		D3DXVECTOR3 vMove = -((*m_pvCamDir)*2.f);
		if(m_bMegaJump)
		{
			m_tInfo.vPos.x = m_vOriPos.x + (vMove.x)*(fTime/m_fFullTime);
			m_tInfo.vPos.z = m_vOriPos.z + (vMove.z)*(fTime/m_fFullTime);
		}
		else
		{
			m_tInfo.vPos = m_pTile->GetInfo()->vPos;
			m_tInfo.vPos.y = 1.f;

			if(eObjId == OBJ_MONSTER)
			{
				m_pTile->GetObj()->SetDamage(20);
				((CMonster*)m_pTile->GetObj())->SetDamageDirection(m_vOriPos);
				m_pDevice->SoundPlay(TEXT("Kwang"), 0);
			}

			m_pTile->SetObj(this);
			m_bBoxMove = false;
		}
	}
}
////////////////////////////////////////////////////////////////////////////
//우수수 떨어지기(StageEnd)//////////////////////////////////////////////////
void CObj::StageEndScene(void)
{
	if(m_bStageEnd)
	{
		float fGravitySpeed = 0.f;
		m_fMegaTime += GETTIME;

		fGravitySpeed = (m_fGravity*m_fMegaTime*m_fMegaTime)*0.5f;
		m_tInfo.vPos.y = m_fMegaY+fGravitySpeed;

		if(m_tInfo.vPos.y < -20.f)
			m_bIsDelete = true;
	}
}
/////////////////////////////////////////////////////////////////////////////
void CObj::ConvertAniData(ANI& tAni, const CUBEDATA& tCubeData)
{
	tAni.vPos = D3DXVECTOR3( tCubeData.fPosX, tCubeData.fPosY, tCubeData.fPosZ );	/// 위치 키(0,0,0)
	tAni.vScale = D3DXVECTOR3( tCubeData.fScaleX, tCubeData.fScaleY, tCubeData.fScaleZ );	/// 위치 키(0,0,0)

	D3DXQuaternionRotationYawPitchRoll( &tAni.qRot
		, D3DXToRadian(tCubeData.fAngle[ANGLE_Y])
		, D3DXToRadian(tCubeData.fAngle[ANGLE_X])
		, D3DXToRadian(tCubeData.fAngle[ANGLE_Z]));	/// Y, X, Z 순서

	tAni.fTime = tCubeData.fTime;
}

void CObj::SetAniData(const CUBEDATA& tCubeData, const STATEKEY& eStatKey)
{
	ANI* pAni = new ANI;

	ConvertAniData(*pAni, tCubeData);

	m_vecAni[eStatKey].push_back(pAni);
}

void CObj::ReleaseAnimation(void)
{
	for(int  i = 0; i < STAT_END; ++i)
	{
		for_each(m_vecAni[i].begin(), m_vecAni[i].end(), CDeleteObj());
		m_vecAni[i].clear();
	}
}

void CObj::LoadObject(const wstring& FilePath)
{
	HANDLE hFile = NULL;
	DWORD dwByte = 0;

	hFile = CreateFile(FilePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL, NULL);

	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pCubeTex);

	int iCount = 0;
	CUBEDATA* pCubeData = new CUBEDATA;
	while(true)
	{
		ReadFile(hFile, pCubeData, sizeof(CUBEDATA), &dwByte, NULL);

		if(dwByte == 0)
		{
			Safe_Delete(pCubeData);
			break;
		}

		if(iCount == 0)
		{
			SetLocalMatrix(pCubeData);
			SetCubeTexture(pCubeData->iImageNum);
		}
		else
		{
			CCube* pCube = new CCube;
			pCube->Initialize();
			pCube->SetParentsMatrix(&m_tInfo.matWorld);
			pCube->SetLocalMatrix(pCubeData);
			pCube->SetCubeTexture(pCubeData->iImageNum);
			m_vecChild.push_back(pCube);
		}

		++iCount;
	}
	m_iChildCnt = m_vecChild.size();
	CloseHandle(hFile);
}
void CObj::SetParentsMatrix(D3DXMATRIX* _pmatParents){m_pmatParents = _pmatParents;}
void CObj::SetLocalMatrix(const CUBEDATA* pCubeData)
{
	CPipeline::MakeWorldMatrix(m_matLocal
		, D3DXVECTOR3(pCubeData->fScaleX, pCubeData->fScaleY, pCubeData->fScaleZ)
		, pCubeData->fAngle
		, D3DXVECTOR3(pCubeData->fPosX, pCubeData->fPosY, pCubeData->fPosZ));

	CPipeline::MakeWorldMatrix(m_matParents
		, D3DXVECTOR3(1.f, 1.f, 1.f)
		, pCubeData->fAngle
		, D3DXVECTOR3(pCubeData->fPosX, pCubeData->fPosY, pCubeData->fPosZ));
}

void CObj::LoadAnimation(const wstring& FilePath, STATEKEY eStatKey)
{
	HANDLE hFile = NULL;
	DWORD dwByte = 0;

	hFile = CreateFile(FilePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL, NULL);

	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pCubeTex);

	CUBEDATA* pCubeData = new CUBEDATA[m_iChildCnt+1];
	while(true)
	{
		for(int i = 0; i < m_iChildCnt+1; ++i)
			ReadFile(hFile, &pCubeData[i], sizeof(CUBEDATA), &dwByte, NULL);

		if(dwByte == 0)
		{
			::Safe_Delete_Array(pCubeData);
			break;
		}
		
		for(int i = 0; i < m_iChildCnt+1; ++i)
		{
			if(i == 0)
				SetAniData(pCubeData[i], eStatKey);
			else
				m_vecChild[i-1]->SetAniData(pCubeData[i], eStatKey);
		}
	}
	CloseHandle(hFile);
}

void CObj::SetAniStateKey(const STATEKEY& eStatKey)
{
	m_eStatKey = eStatKey;
}

void CObj::SetCubeTexture(const int iDrawId)
{
	m_pCubeTexture = m_pTextureMgr->GetCubeTex(iDrawId);
	m_tCube.iDrawId = iDrawId;
}

void CObj::GetPlayerToDistance() // 화면 시야 관리
{
	if(m_eObjId != OBJ_TERRAIN && m_eObjId != OBJ_MONSTER)
		m_iDarkness = m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->GetDarkness();
}

void CObj::SetDelete()
{
	m_bIsDelete = true;	
}

const UNIT* CObj::GetUnitInfo() const
{
	return &m_tUnitInfo;
}

void CObj::SetDamage(const int iAtt)
{
	if(m_tUnitInfo.iDef < iAtt)
		m_tUnitInfo.iHp += (m_tUnitInfo.iDef - iAtt);
	else
		m_tUnitInfo.iHp -= 1;

	if(m_tUnitInfo.iHp <= 0)
		m_tUnitInfo.iHp = 0;

	if(m_eObjId == OBJ_PLAYER)
		m_pDevice->SoundPlay(TEXT("Player_Hit"), 0);
}

void CObj::SetUnitInfo(const UNIT& tUnit)
{
	m_tUnitInfo.iAtt = tUnit.iAtt;
	m_tUnitInfo.iDef = tUnit.iDef;
	m_tUnitInfo.iHp = tUnit.iHp;
	m_tUnitInfo.iMaxHp = tUnit.iMaxHp;
}

void CObj::SetAngle(const float fAngle)
{
	m_fAngle[ANGLE_Y] = fAngle;
}

const CUBE& CObj::GetCubeInfo() const
{
	return m_tCube;
}

void CObj::SetFrozen(const bool bFreeze)
{
	m_bFreeze = bFreeze;
	m_pFrozen = (CFrozen*)CObjFactory<CFrozen>::CreateObj();
	m_pFrozen->SetFrozenUnit(this);
	m_pObjMgr->AddObj(OBJ_EFFECT, m_pFrozen);
}

void CObj::SetFrozenTimer(void)
{
	if(m_bFreeze)
	{
		m_fFreezeTime += GETTIME;

		if(m_fFreezeTime > 3.f*m_pBeatMgr->GetBeatMax())
		{
			m_pFrozen->SetDelete();
			m_pFrozen = NULL;
			m_bFreeze = false;
			m_fFreezeTime = 0.f;
		}
	}
}

void CObj::SetStructFrame(const FRAME& tFrame)
{
	m_tFrame = tFrame;
}

const BYTE CObj::GetDarkness() const // 내 손 안의 흑염룡이 날뛴다! 크크큭 다크니스!
{
	return m_iDarkness;
}

void CObj::SetMegaJump(const bool bJump)
{
	m_bMegaJump = bJump;

	if(m_bMegaJump)
		m_fMegaPow = CRand::Random(20, 40) * 1.f;

	if(m_eObjId == OBJ_CUBE && m_tCube.dwType & CUBE_THRO && !m_bBoxMove)
	{
		m_fMegaPow = 40.f;
		m_fFullTime = (-m_fMegaPow)/(m_fGravity) *2.f;
		m_vOriPos = m_tInfo.vPos;
		m_pTile = m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_vOriPos-((*m_pvCamDir)*2.f)));
	}
}

void CObj::SetDarkness(const BYTE byDarkness)
{
	m_iDarkness = byDarkness;
}

void CObj::SetStageEnd(const bool bStageEnd)
{
	if(!m_bStageEnd)
		m_fMegaTime = 0.f;
	m_bStageEnd = bStageEnd;	
}

void CObj::SetStageStart(const bool bStageStart)
{
	m_bStageStart = bStageStart;
}
void CObj::UsingOBB(void)
{
	m_tOBB.vCenterPos = m_tInfo.vPos;
	m_tOBB.vAxisDir[0] = m_tInfo.vDir;
	m_tOBB.vAxisDir[1] = D3DXVECTOR3(0,1,0);
	D3DXVec3Cross(&m_tOBB.vAxisDir[2], &m_tOBB.vAxisDir[0], &m_tOBB.vAxisDir[1]);
	m_tOBB.fAxisLen[0] = D3DXVec3Length(&m_tOBB.vAxisDir[0]);
	m_tOBB.fAxisLen[1] = D3DXVec3Length(&m_tOBB.vAxisDir[1]);
	m_tOBB.fAxisLen[2] = D3DXVec3Length(&m_tOBB.vAxisDir[2]);
}
