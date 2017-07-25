#pragma once

class CDicomGroupView :
	public CDockablePane
{
public:
	CDicomGroupView();
	virtual ~CDicomGroupView();

	/// DICOM �׷� Ʈ�� ��Ʈ���� ID
	enum { ID_DICOM_GROUP_TREE = 1 };

protected:
	/// DICOM �׷� Ʈ�� ��Ʈ��
	CTreeCtrl m_DicomGroupTree;

	/// DICOM �׷� Ʈ������ �̿��� ������ �̹��� ���
	CImageList m_Images;
	
public:
	/// DICOM Tree ������Ʈ
	void UpdateDicomTree();

	/// DICOM Tree���� �̸����� Ʈ�� �׸� ã��, ������ ���� ����
	HTREEITEM GetDicomGroupItem( CString itemText, HTREEITEM parentItem );

	/// DICOM Tree ��� ��ġ��
	void ExpandAllDicomGroupTree();

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnNMDblclk( NMHDR *pNMHDR, LRESULT *pResult );
};

