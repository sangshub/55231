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
	virtual void StopBgm()PURE; // Bgm���߱�
	virtual void NextStage()PURE; // ���� ��������

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
	const bool MapInput(MAP* pMap); // ���� �ʵ� ����
	void MapConnect(); // ���ʿ� ���������� ����
	void MapCreate(); // �����ϰ� �����ϴ� ��ü����
	void MapSort(const D3DXVECTOR3& vPos); // �� ������ Ʋ���� ��� ������

protected:
	static bool m_bStageInit;
	bool m_bBossStage;

	int m_iTileX;
	int m_iTileZ;
	int m_iDrawId; // �������� �⺻ Ÿ��
	int m_iVolume;

	CObjMgr*		m_pObjMgr;
	CBufferMgr*		m_pBufferMgr;
	CBeatMgr*		m_pBeatMgr;
	CObj*		m_pMiniMap;

	D3DXVECTOR3 m_vecShop; // ���� ���� ��ġ
	D3DXVECTOR3 m_vSort; // ���������� Ʋ������ ��ġ
	D3DXVECTOR3 m_vStart;

	MAP* m_StartMap;
	MAP* m_EndMap;
	vector<TILE*> m_vecTileInfo;
	vector<MAP*> m_vecRandomMap;
	vector<MAP*> m_vecChooseMap;
};