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

//#pragma chMSG(이 부분 수정할 것) //식으로 사용하면 컴파일할 때 메시지를 남길 수 있음
#define chSTR2(x) #x
#define chSTR(x) chSTR2(x)
#define chMSG(desc) message(__FILE__ "(" chSTR(__LINE__) "): ★★★" #desc "★★★")

//MFC에서 릭 잡고 싶으면 의심되는 cpp 상단에 해당 코드를 집어넣을 것

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif