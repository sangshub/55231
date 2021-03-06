#pragma once

#include "atlbase.h"

class CTextureRenderer;
class CMediaObj
{
private:
	CComPtr<IGraphBuilder>  m_pGB;          // GraphBuilder
	CComPtr<IMediaControl>  m_pMC;          // Media Control
	CComPtr<IMediaPosition> m_pMP;          // Media Position
	CComPtr<IMediaEvent>    m_pME;          // Media Event
	CComPtr<IBaseFilter>    m_pRenderer;    // our custom renderer
	CTextureRenderer*		pCTR;			// TextureRenderer 

private:
	BOOL m_bPause;

public:
	HRESULT Initialize(TCHAR* wFileName);
	void Release(void);

public:
	void SetTexMovie(DWORD dwStage);
	void SetTime(double time) {m_pMP->put_CurrentPosition(time);}

public:
	double GetStopTime(void);
	double GetCurrentPosition(void) const;

public:
	BOOL Play(void);

public:
	CMediaObj(void);
	~CMediaObj(void);
};
