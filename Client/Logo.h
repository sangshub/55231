#pragma once

#include "SceneObj.h"

#include "Include.h"

class CLogo : public CSceneObj
{
public:
	CLogo();
	~CLogo();

public:
	virtual HRESULT Initialize();
	virtual void Progress();
	virtual void Render();

private:
	virtual void Release();

public:
	virtual void StopBgm(); // Bgm���߱�
	virtual void NextStage(); // Next Stage�Ѿ��
	void ReStage();

public:
	void FrameMove(void);

private:
	FRAME m_tFrame;
};