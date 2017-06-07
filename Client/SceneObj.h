#pragma once

class CSceneMgr;
class CDevice;
class CTextureMgr;
class CCamMgr;

class CSceneObj
{
public:
	virtual HRESULT Initialize()PURE;
	virtual void Progress()PURE;
	virtual void Render()PURE;

private:
	virtual void Release()PURE;

public:
	virtual void StopBgm()PURE; // Bgm���߱�
	virtual void NextStage()PURE; // ���� ��������

protected:
	CSceneMgr* m_pSceneMgr;
	CCamMgr* m_pCamMgr;
	CDevice* m_pDevice;
	CTextureMgr* m_pTextureMgr;

public:
	CSceneObj();
	virtual ~CSceneObj();
};