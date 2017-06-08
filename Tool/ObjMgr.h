#pragma once

#include "Include.h"

#include "Obj.h"

class CFrustum;

class CObjMgr
{
private:
	CObjMgr(void);
public:
	~CObjMgr(void);

public:
	DECLARE_SINGLETON(CObjMgr)

public:
	HRESULT Initialize(void);
	int		Progress(void);
	void	Render(void);

private:
	void	Release(void);

public:
	void AddObj(OBJID eObjId, CObj* pObj);
	const list<CObj*>* GetObjList(OBJID eObjid);

public:
	void CubeLastDelete(void);
	void CubeDataReset(void);

	void MonsterLastDelete(void);
	void MonsterDataReset(void);

	void ObjDelete(const OBJID& eObjId);

public:
	VTXTEX* GetUnitVtxTex();

private:
	bool m_bCubeRender;

	CFrustum* m_pFrustum;

	list<CObj*> m_ObjList[OBJ_END];
	
	vector<CObj*> m_vecCube;
	VTXTEX*		m_pVtxTex;
};