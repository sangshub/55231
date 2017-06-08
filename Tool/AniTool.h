#pragma once

#include "AniDialog.h"
#include "Include.h"
#include "afxwin.h"


// CAniTool ��ȭ �����Դϴ�.

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
	CAniTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAniTool();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ANITOOL };

public:
	//�ִϸ��̼��� ���� �Լ���/////////////////////////////////////////////////////
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
	void SetCubeDataCopy(CUBEDATA& tOut, const CUBEDATA& tIn); // ������ ����
	vector<CCube*>*	GetAnimationCube(void);

public:
	void Release(void);

private:
	CAniDialog		m_AniDialog;

private:
	CAniView*		m_pAniView;
	CObjMgr*		m_pObjMgr;
	CTileMap*		m_pTileMap;
	CToolView*		m_pMainView; // ���� ��
	CBufferMgr*		m_pBufferMgr;
	CTextureMgr*	m_pTextureMgr;

private:
	bool			m_bParents;// �θ����Ȯ��
	bool			m_bSucces;// ������Ʈ �����Ϸ�

private:
	int					m_iMode; // ��, ��, ��
	int					m_iXYZ;
	bool				m_bKeyInput;
	bool				m_bEditMode;

	float				m_fValue;
	int					m_iObjCnt; // ������Ʈ �� ����
	CUBE				m_tCube; //ť�� �⺻����
	D3DXMATRIX			m_matParents; // �θ����
	VTXCUBE*			m_pCubeTex;
	VTXCUBE*			m_pConvertCubeTex;
	vector<ANI*>		m_vecAni; // �ִϸ��̼� ������
	vector<CCube*>		m_vecAnimation; // ������Ʈ��
	vector<CUBEDATA*>	m_vecCubeData; // ������Ʈ �ʱ� ������
	vector<CUBEDATA*>	m_vecActive; // ������Ʈ������ �ൿ ����

private: // ���̾�α� ����
	CUBEDATA	m_tCubeData;
	CListBox	m_ObjectList;
	CListBox	m_ActiveList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
	afx_msg void OnBnClickedObjectCreateSucces(); // ������Ʈ ���� �Ϸ�
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
