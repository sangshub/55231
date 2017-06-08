#pragma once

template <typename T>
void Safe_Delete(T& Temp)
{
	delete Temp;
	Temp = NULL;
}

template <typename T>
void Safe_Delete_Array(T& Temp)
{
	if(Temp == NULL)
		return;
	delete[] Temp;
	Temp = NULL;
}

template <typename T>
void Safe_Release(T& Temp)
{
	Temp->Release();
	Temp = NULL;
}

template <typename T>
void Safe_Destroy(T& Temp)
{
	Temp->DestroyInstance();
	Temp = NULL;
}

struct CDeleteObj
{
	template <typename T>
	void operator() (T& Temp)
	{
		delete Temp;
		Temp = NULL;
	}
};

struct CDeleteMapObj
{
	template <typename T>
	void operator() (T& Temp)
	{
		delete Temp.second;
		Temp.second = NULL;
	}
};

struct ReleaseMapObj
{
	template <typename T>
	void operator() (T& Temp)
	{
		Temp.second->Release();
		Temp.second = NULL;
	}
};