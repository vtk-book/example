#pragma once


// CDlgVtkView ��ȭ �����Դϴ�.

class CDlgVtkView : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVtkView)

public:
	CDlgVtkView(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgVtkView();
	
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VTK_VIEW };

protected:
	/// �� Dialog�� View Type
	int m_ViewType;

	/// Scroll Bar ��ü
	CScrollBar m_ScrollBar;

public:
	/// �� Dialog�� View Type ��� / ����
	int GetViewType() const { return m_ViewType; }
	void SetViewType( int val ) { m_ViewType = val; }
	
	/// Volume �����Ͱ� �ε�Ǹ� �� Slice�� �ε��� ������ ���� ��ũ�ѹ� ������Ʈ
	void UpdateScrollBar();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg void OnSize( UINT nType, int cx, int cy );
};
