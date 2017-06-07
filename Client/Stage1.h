#pragma once

#include "Stage.h"

class CStage1 : public CStage
{
public:
	CStage1();
	~CStage1();

public:
	virtual HRESULT Initialize();
	virtual void Progress();
	virtual void Render();

public:
	virtual void StopBgm(); // Bgm멈추기
	virtual void NextStage(); // 다음 스테이지
};