#pragma once

#define	NO_COPY(CLASSNAME)							\
		private:									\
		CLASSNAME(const CLASSNAME&);				\
		CLASSNAME& operator = (const CLASSNAME&);	\

#define DECLARE_SINGLETON(CLASSNAME)				\
		NO_COPY(CLASSNAME)							\
		private:									\
			static CLASSNAME* pInstance;			\
		public:										\
			static CLASSNAME* GetInstance(void);	\
			static void DestroyInstance(void);		\

#define	IMPLEMENT_SINGLETON(CLASSNAME)				\
		CLASSNAME* CLASSNAME::pInstance = NULL;		\
		CLASSNAME* CLASSNAME::GetInstance(void){	\
			if(pInstance == NULL)					\
				pInstance = new CLASSNAME;			\
			return pInstance;						\
		}											\
		void CLASSNAME::DestroyInstance(void){		\
			if(pInstance != NULL) {					\
				delete pInstance;					\
				pInstance = NULL;					\
			}										\
		}	

#define ERR_MSG(message)	MessageBox(g_hWnd, message, TEXT("System Message"), MB_OK)

#define GETTIME		CTimeMgr::GetInstance()->GetTime()

//#pragma chMSG(�� �κ� ������ ��) //������ ����ϸ� �������� �� �޽����� ���� �� ����
#define chSTR2(x) #x
#define chSTR(x) chSTR2(x)
#define chMSG(desc) message(__FILE__ "(" chSTR(__LINE__) "): �ڡڡ�" #desc "�ڡڡ�")

//MFC���� �� ��� ������ �ǽɵǴ� cpp ��ܿ� �ش� �ڵ带 ������� ��

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif