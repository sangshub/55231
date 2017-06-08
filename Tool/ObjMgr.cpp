#include "StdAfx.h"
#include "ObjMgr.h"
#include "BufferMgr.h"
#include "Frustum.h"

IMPLEMENT_SINGLETON(CObjMgr)

struct RenderObj
{
	void operator()(CObj* pObj)
	{
		pObj->Render();
	}
};

CObjMgr::CObjMgr(void)
: m_bCubeRender(true)
, m_pFrustum(CFrustum::GetInstance())
{
}

CObjMgr::~CObjMgr(void)
{
	Release();

}
HRESULT CObjMgr::Initialize(void)
{
	m_pVtxTex = new VTXTEX[4];
	CBufferMgr::GetInstance()->GetVtxInfo(TEXT("UnitTex"), m_pVtxTex);


	return S_OK;
}
int CObjMgr::Progress(void)
{
	int iResult = 0;

	for( int i = 0; i < OBJ_END; i++)
	{
		if(i == OBJ_CUBE)
		{
			for(list<CObj*>::iterator iter = m_ObjList[i].begin();
				iter != m_ObjList[i].end(); )
			{
				if(m_pFrustum->FrustumCulling((*iter)->GetInfo()->vPos))
				{
					iResult = (*iter)->Progress();

					if(iResult == 1)
					{
						::Safe_Delete(*iter);
						iter = m_ObjList[i].erase(iter);
					}
					else
					{
						m_vecCube.push_back(*iter);
						++iter;
					}
				}
				else
					++iter;
			}
		}
		else
		{
			for(list<CObj*>::iterator iter = m_ObjList[i].begin();
				iter != m_ObjList[i].end(); )
			{
				iResult = (*iter)->Progress();

				if(iResult ==1 )
				{
					::Safe_Delete(*iter);
					iter = m_ObjList[i].erase(iter);
				}
				else
					iter++;
			}
		}
	}

	if(GetAsyncKeyState('K') & 0x0001)
	{
		if(m_bCubeRender)
			m_bCubeRender = false;
		else
			m_bCubeRender = true;
	}

	return iResult;
}
void CObjMgr::Render(void)
{
	for(int i = 0 ;  i < OBJ_END; i++)
	{
		if(i == OBJ_CUBE)
		{
			if(m_bCubeRender)
				for_each(m_vecCube.begin(), m_vecCube.end(), ::RenderObj());

			vector<CObj*>().swap(m_vecCube);
		}
		else
		{
			for_each(m_ObjList[i].begin(), m_ObjList[i].end(), ::RenderObj());
		}
	}
}
void CObjMgr::Release(void)
{
	for(int i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), CDeleteObj());
		m_ObjList[i].clear();
	}

	vector<CObj*>().swap(m_vecCube);

	::Safe_Delete_Array(m_pVtxTex);
}
void CObjMgr::AddObj(OBJID eObjId, CObj* pObj)
{
	m_ObjList[eObjId].push_back(pObj);

}
const list<CObj*>* CObjMgr::GetObjList(OBJID eObjid)
{
	return &m_ObjList[eObjid];
}
VTXTEX* CObjMgr::GetUnitVtxTex()
{
	return m_pVtxTex;
}

void CObjMgr::CubeLastDelete(void)
{
	::Safe_Delete(m_ObjList[OBJ_CUBE].back());
	m_ObjList[OBJ_CUBE].pop_back();
}

void CObjMgr::CubeDataReset(void)
{
	for_each(m_ObjList[OBJ_CUBE].begin(), m_ObjList[OBJ_CUBE].end(), CDeleteObj());
	m_ObjList[OBJ_CUBE].clear();
}

void CObjMgr::MonsterLastDelete(void)
{
	::Safe_Delete(m_ObjList[OBJ_MONSTER].back());
	m_ObjList[OBJ_MONSTER].pop_back();

}

void CObjMgr::MonsterDataReset(void)
{
	for_each(m_ObjList[OBJ_MONSTER].begin(), m_ObjList[OBJ_MONSTER].end(), CDeleteObj());
	m_ObjList[OBJ_MONSTER].clear();

}

void CObjMgr::ObjDelete(const OBJID& eObjId)
{
	for_each(m_ObjList[eObjId].begin(), m_ObjList[eObjId].end(), CDeleteObj());
	m_ObjList[eObjId].clear();
}