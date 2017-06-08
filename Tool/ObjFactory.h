#pragma once

class CObj;

template <typename T>
class CObjFactory
{
public:
	static CObj* CreateObj()
	{
		CObj* pObj = new T;

		pObj->Initialize();

		return pObj;
	}

	static CObj* CreateObj(const D3DXVECTOR3& vPos)
	{
		CObj* pObj = new T;

		pObj->Initialize();
		pObj->SetPos(vPos);

		return pObj;
	}

	static CObj* CreateObj(const BYTE byDrawId)
	{
		CObj* pObj = new T;

		pObj->Initialize();
		((CCube*)pObj)->SetCubeTexture(byDrawId);

		return pObj;
	}
};