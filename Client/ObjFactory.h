#pragma once

template <typename T>
class CObjFactory
{
public:
	static CObj* CreateObj(void)
	{
		CObj* pObj = new T;

		if(FAILED(pObj->Initialize()))
		{
			ERR_MSG(TEXT("Object Initialize Fail"));

			return NULL;
		}

		return pObj;
	}

	static CObj* CreateObj(const D3DXVECTOR3& vPos)
	{
		CObj* pObj = new T;

		if(FAILED(pObj->Initialize()))
		{
			ERR_MSG(TEXT("Object Initialize Fail"));

			return NULL;
		}
		pObj->SetPos(vPos);

		return pObj;
	}
};