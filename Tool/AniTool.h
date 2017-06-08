#pragma once

#include "AniDialog.h"
#include "Include.h"
#include "afxwin.h"


// CAniTool 대화 상자입니다.

class CAniView;
class CToolView;
class CObjMgr;
class CTileMap;
class CBufferMgr;
class CTextureMgr;
class CCube;
class CAniTool : public CDialog
{
	DECLARE_DYNAMIC(CAniTool)

public:
	CAniTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAniTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ANITOOL };

public:
	//애니메이션을 위한 함수들/////////////////////////////////////////////////////
	void CreateAniData(ANI& tAni, CUBEDATA& tCubeData);
	void ConvertAniData(void);
	////////////////////////////////////////////////////////////////////////////////
	void InsertCube(CCube* pCube, CUBEDATA* pCubeData);
	void ResetCubeData(void);
	void TransformCube(CCube* pCube);
	void CreateWorldMatrix(D3DXMATRIX& matWorld);
	void CreateParentsMatrix(D3DXMATRIX& matWorld);
	bool KeyInput(void);

public:
	void SetAniView(CAniView* _pAniView);
	void SetTileMap(CTileMap* _pTileMap);
	void SetCubeDataCopy(CUBEDATA& tOut, const CUBEDATA& tIn); // 데이터 복사
	vector<CCube*>*	GetAnimationCube(void);

public:
	void Release(void);

private:
	CAniDialog		m_AniDialog;

private:
	CAniView*		m_pAniView;
	CObjMgr*		m_pObjMgr;
	CTileMap*		m_pTileMap;
	CToolView*		m_pMainView; // 메인 뷰
	CBufferMgr*		m_pBufferMgr;
	CTextureMgr*	m_pTextureMgr;

private:
	bool			m_bParents;// 부모생성확인
	bool			m_bSucces;// 오브젝트 생성완료

private:
	int					m_iMode; // 스, 자, 이
	int					m_iXYZ;
	bool				m_bKeyInput;
	bool				m_bEditMode;

	float				m_fValue;
	int					m_iObjCnt; // 오브젝트 총 개수
	CUBE				m_tCube; //큐브 기본정보
	D3DXMATRIX			m_matParents; // 부모행렬
	VTXCUBE*			m_pCubeTex;
	VTXCUBE*			m_pConvertCubeTex;
	vector<ANI*>		m_vecAni; // 애니메이션 데이터
	vector<CCube*>		m_vecAnimation; // 오브젝트들
	vector<CUBEDATA*>	m_vecCubeData; // 오브젝트 초기 데이터
	vector<CUBEDATA*>	m_vecActive; // 오브젝트마다의 행동 백터

private: // 다이얼로그 변수
	CUBEDATA	m_tCubeData;
	CListBox	m_ObjectList;
	CListBox	m_ActiveList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedParentsCreate();
	afx_msg void OnBnClickedChildCreate();
	afx_msg void OnLbnSelchangeSelectObject();
	afx_msg void OnBnClickedCubeDataChange();
	afx_msg void OnBnClickedCubeDelete();
	afx_msg void OnBnClickedAddActive();
	afx_msg void OnLbnSelchangeActiveList();
	afx_msg void OnBnClickedObjectCreateSucces(); // 오브젝트 생성 완료
	afx_msg void OnBnClickedAnimationPlay();
	afx_msg void OnBnClickedAniStop();
	afx_msg void OnBnClickedActiveDelete();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedObjectSave();
	afx_msg void OnBnClickedObjectLoad();
	afx_msg void OnBnClickedAllDelete();
	afx_msg void OnBnClickedAniTimeChange();
	afx_msg void OnBnClickedAnimationLoad();
	afx_msg void OnBnClickedImageNumChange();
};
