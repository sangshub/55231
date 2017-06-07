#pragma once

#include "SceneObj.h"
#include "Include.h"

class CObjMgr;
class CBufferMgr;
class CBeatMgr;
class CObj;

class CStage : public CSceneObj
{
public:
	CStage();
	virtual ~CStage();

public:
	static bool GetStageInit();
	const int GetTileX() const;
	const int GetTileZ() const;
	const bool GetBossStage() const;

public:
	virtual HRESULT Initialize()PURE;
	virtual void Progress()PURE;
	virtual void Render()PURE;

private:
	void Release();

public:
	virtual void StopBgm()PURE; // Bgm멈추기
	virtual void NextStage()PURE; // 다음 스테이지

protected:
	void FileLoad(const wstring& wstrFilePath);
	void CubeLoad(const wstring& wstrCubePath, MAP* pMap = NULL);
	void ItemLoad(const wstring& wstrItemPath, MAP* pMap = NULL);
	void MonsterLoad(const wstring& wstrItemPath, MAP* pMap = NULL);
	void ObjLoad(const wstring& wstrItemPath, MAP* pMap = NULL);
	void AllDataLoad(const wstring& wstrPath, MAP* pMap = NULL);

	void ShopVolume();
	void ShopItem();
	void StageBaseObjInit();

	void MapLoad(const wstring& wstrFilePath, const int iOption = 2);
	const bool MapInput(MAP* pMap); // 랜덤 맵들 생성
	void MapConnect(); // 최초에 랜덤연결을 설정
	void MapCreate(); // 생성하고 연결하는 전체과정
	void MapSort(const D3DXVECTOR3& vPos); // 맵 원점이 틀어질 경우 재조정

protected:
	static bool m_bStageInit;
	bool m_bBossStage;

	int m_iTileX;
	int m_iTileZ;
	int m_iDrawId; // 스테이지 기본 타일
	int m_iVolume;

	CObjMgr*		m_pObjMgr;
	CBufferMgr*		m_pBufferMgr;
	CBeatMgr*		m_pBeatMgr;
	CObj*		m_pMiniMap;

	D3DXVECTOR3 m_vecShop; // 상점 주인 위치
	D3DXVECTOR3 m_vSort; // 랜덤맵으로 틀어지는 위치
	D3DXVECTOR3 m_vStart;

	MAP* m_StartMap;
	MAP* m_EndMap;
	vector<TILE*> m_vecTileInfo;
	vector<MAP*> m_vecRandomMap;
	vector<MAP*> m_vecChooseMap;
};