#pragma once

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRendererCollection.h>
#include <vtkCornerAnnotation.h>

#include "DicomLoader.h"

class DVManager
{
	//////////////////////////////////////////////////////////////////////////
	/// Singleton ������ �̿��� ���� ���� �Ŵ��� ����
	//////////////////////////////////////////////////////////////////////////
private:
	DVManager();
	virtual ~DVManager();

	static DVManager* m_Instance;
	static void Destroy() { delete m_Instance; m_Instance = NULL; }

public:
	/// ���� �Ŵ��� ��ü
	static DVManager* Mgr();
	
public:
	/// View Type
	static const int NUM_VIEW = 4;
	enum ViewType { VIEW_AXIAL, VIEW_CORONAL, VIEW_SAGITTAL, VIEW_3D };

protected:
	/// Vtk Render Windows
	vtkSmartPointer<vtkRenderWindow> m_vtkWindow[NUM_VIEW];

	/// DICOM Loader
	vtkSmartPointer<DicomLoader> m_DicomLoader;

	/// ���� ǥ��
	vtkSmartPointer<vtkCornerAnnotation> m_Annotation[NUM_VIEW];

public:
	/// Vtk Render Windows
	vtkSmartPointer<vtkRenderWindow> GetVtkWindow( int viewType );

	/// Vtk Window �ʱ�ȭ (OnInitDialog)
	void InitVtkWindow( int viewType, void* hWnd );

	/// Vtk Window ũ�� ���� (OnSize)
	void ResizeVtkWindow( int viewType, int width, int height );

	/// DICOM Loader ��ü
	vtkSmartPointer<DicomLoader> GetDicomLoader();

	/// View Ÿ�Կ� ���� VTK ������
	vtkSmartPointer<vtkRenderer> GetRenderer( int viewType );

	/// DICOM Group ���� ��, ȭ�� ������Ʈ �� �ʱ�ȭ
	void OnSelectDicomGroup( vtkSmartPointer<DicomGroup> group );

	/// Volume �׸��� �ʱ�ȭ
	void ClearVolumeDisplay();

	/// Volume �׸��� ������Ʈ
	void UpdateVolumeDisplay();

	/// ��ũ�ѹٸ� ���� �����̽� �ε��� ����
	void ScrollSliceIndex( int viewType, int pos );

	/// Volume ������ ��� ����
	void ChangeVolumeRenderMode( int renderMode );

	/// ���� ǥ�� ������Ʈ
	void UpdateAnnotation();

	/// DICOM �����̽� ���� ǥ��
	void UpdateSliceAnnotation( int viewType );
};

