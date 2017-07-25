#include "stdafx.h"
#include "DVManager.h"

#include "DICOMViewer.h"
#include "MainFrm.h"
#include "ChildView.h"

DVManager* DVManager::m_Instance = NULL;

DVManager::DVManager()
{
	vtkObject::GlobalWarningDisplayOff();
}


DVManager::~DVManager()
{
}

DVManager* DVManager::Mgr()
{
	if( m_Instance == NULL ) {
		m_Instance = new DVManager();
		atexit( Destroy );
	}
	return m_Instance;
}

vtkSmartPointer<vtkRenderWindow> DVManager::GetVtkWindow( int viewType )
{
	// viewType ���� �˻�
	if( viewType < 0 || viewType >= NUM_VIEW ) return NULL;

	// viewType �� vtkRenderWindow ��ȯ
	return m_vtkWindow[viewType];
}

void DVManager::InitVtkWindow( int viewType, void* hWnd )
{
	// viewType ���� �˻�
	if( viewType < 0 || viewType >= NUM_VIEW ) return;
	
	// vtk Render Window ����
	if( m_vtkWindow[viewType] == NULL ) {
		// Interactor ����
		vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

		// Renderer ����
		vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
		renderer->SetBackground( 0.0, 0.0, 0.0 );					// ������ ���

		// 3D View ����
		if( viewType == VIEW_3D ) {
			// Trackball Camera ���ͷ��� ��Ÿ�� ����
			interactor->SetInteractorStyle( vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New() );

			vtkSmartPointer<vtkCamera> camera = renderer->GetActiveCamera();
			camera->ParallelProjectionOn();				// Parallel Projection ���
			camera->SetPosition( 0.0, -1.0, 0.0 );		// ī�޶� ��ġ
			camera->SetViewUp( 0.0, 0.0, 1.0 );			// ī�޶� Up ����
		}
		// 2D View ����
		else {
			// Image ���ͷ��� ��Ÿ�� ����
			interactor->SetInteractorStyle( vtkSmartPointer<vtkInteractorStyleImage>::New() );

			vtkSmartPointer<vtkCamera> camera = renderer->GetActiveCamera();
			camera->ParallelProjectionOn();				// Parallel Projection ���
			camera->SetPosition( 0.0, 0.0, -1.0 );		// ī�޶� ��ġ
			camera->SetViewUp( 0.0, -1.0, 0.0 );			// ī�޶� Up ����
		}
		
		// RenderWindow ���� �� Dialog �ڵ�, Interactor, Renderer ����
		m_vtkWindow[viewType] = vtkSmartPointer<vtkRenderWindow>::New();
		m_vtkWindow[viewType]->SetParentId( hWnd );
		m_vtkWindow[viewType]->SetInteractor( interactor );
		m_vtkWindow[viewType]->AddRenderer( renderer );
		m_vtkWindow[viewType]->Render();
	}
}

void DVManager::ResizeVtkWindow( int viewType, int width, int height )
{
	// viewType ���� �˻� �� vtkRenderWindow �˻�
	if( viewType < 0 || viewType >= NUM_VIEW ) return;
	if( m_vtkWindow[viewType] == NULL ) return;

	// �ش� vtkRenderWindow ũ�� ����
	m_vtkWindow[viewType]->SetSize( width, height );
}

vtkSmartPointer<DicomLoader> DVManager::GetDicomLoader()
{
	// DicomLoader ��ü�� null �̸� ����
	if( m_DicomLoader == NULL ) {
		m_DicomLoader = vtkSmartPointer<DicomLoader>::New();
	}

	// DicomLoader ��ü ��ȯ
	return m_DicomLoader;
}

vtkSmartPointer<vtkRenderer> DVManager::GetRenderer( int viewType )
{
	// View Ÿ�� �˻�
	if( viewType < 0 || viewType >= NUM_VIEW ) return NULL;
	// vtkRenderWindow ���� ���� �˻�
	if( m_vtkWindow[viewType] == NULL ) return NULL;

	// �ش��ϴ� View Ÿ���� vtkRenderWindow���� ù ��° Renderer ��ȯ
	return m_vtkWindow[viewType]->GetRenderers()->GetFirstRenderer();
}

void DVManager::OnSelectDicomGroup( vtkSmartPointer<DicomGroup> group )
{
	// ������ �ʱ�ȭ
	ClearVolumeDisplay();

	// ���õ� DICOM �׷쿡�� Volume ������ �ε�
	GetDicomLoader()->LoadVolumeData( group );

	// ���� ǥ�� ������Ʈ
	UpdateAnnotation();

	// Volume ������ ������ ������Ʈ
	UpdateVolumeDisplay();

	// �⺻ View ������ ���
	CChildView* mainView = ((CMainFrame*)AfxGetMainWnd())->GetWndView();
	if( mainView == NULL ) return;

	// 2D View ��ũ�� �� ������Ʈ
	for( int viewType = VIEW_AXIAL; viewType <= VIEW_SAGITTAL; viewType++ ) {
		mainView->GetDlgVtkView( viewType )->UpdateScrollBar();
	}
}

void DVManager::ClearVolumeDisplay()
{
	// �ε�� Volume ������ �˻�
	vtkSmartPointer<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
	if( volumeData == NULL ) return;

	// 3D �信 ���� ������ ����
	GetRenderer( VIEW_3D )->RemoveViewProp( volumeData->GetVolumeRendering() );

	// �����̽� �信 �� �����̽� Actor ����
	for( int viewType = VIEW_AXIAL; viewType <= VIEW_SAGITTAL; viewType++ ) {
		GetRenderer( viewType )->RemoveActor( volumeData->GetSliceActor( viewType ) );
	}
}

void DVManager::UpdateVolumeDisplay()
{
	// �ε�� Volume ������ �˻�
	vtkSmartPointer<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
	if( volumeData == NULL ) return;

	// 3D �信 ���� ������ �߰�
	GetRenderer( VIEW_3D )->AddViewProp( volumeData->GetVolumeRendering() );
	GetRenderer( VIEW_3D )->ResetCamera();	// ī�޶� �缳��
	m_vtkWindow[VIEW_3D]->Render();			// ȭ�� ����
	
	// �����̽� �信 �� �����̽� Actor �߰�
	for( int viewType = VIEW_AXIAL; viewType <= VIEW_SAGITTAL; viewType++ ) {
		GetRenderer( viewType )->AddActor( volumeData->GetSliceActor( viewType ) );
		GetRenderer( viewType )->ResetCamera();	// ī�޶� �缳��
		m_vtkWindow[viewType]->Render();			// ȭ�� ����
	}
}

void DVManager::ScrollSliceIndex( int viewType, int pos )
{
	// ���� �ε�� Volume ������
	vtkSmartPointer<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
	if( volumeData == NULL ) return;

	// Volume �̹����� �ε��� ����
	volumeData->SetSliceIndex( viewType, pos );

	// ���� ǥ�� ������Ʈ
	UpdateAnnotation();

	// ȭ�� ������Ʈ
	m_vtkWindow[viewType]->Render();
}

void DVManager::ChangeVolumeRenderMode( int renderMode )
{
	// ���� �ε�� Volume ������ �˻�
	vtkSmartPointer<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
	if( volumeData == NULL ) return;
	
	// Volume �������� ��� ����
	volumeData->SetCurrentPresetMode( renderMode );

	// ȭ�� ������Ʈ
	m_vtkWindow[VIEW_3D]->Render();
}

void DVManager::UpdateAnnotation()
{
	// ���� ǥ�� ��ü ����
	for( int viewType = 0; viewType < NUM_VIEW; viewType++ ) {
		if( m_Annotation[viewType] == NULL ) {
			m_Annotation[viewType] = vtkSmartPointer<vtkCornerAnnotation>::New();
			m_Annotation[viewType]->SetMaximumFontSize( 20 );

			GetRenderer( viewType )->AddViewProp( m_Annotation[viewType] );
		}

		// 2D �����̽� ���� ǥ��
		UpdateSliceAnnotation( viewType );
	}
	
	// 3D ���� ǥ��
	m_Annotation[VIEW_3D]->SetText( 2, "3D" );
}

void DVManager::UpdateSliceAnnotation( int viewType )
{
	// 2D �����̽� View Ÿ�� �˻�
	if( viewType != VIEW_AXIAL &&
		viewType != VIEW_CORONAL &&
		viewType != VIEW_SAGITTAL ) return;

	// Volume ������ �˻�
	vtkSmartPointer<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
	if( volumeData == NULL ) return;
	
	// Volume �̹����� �ε��� ���� 
	int ext[6];
	volumeData->GetImageData()->GetExtent( ext );

	// ���� �� ���� : �����̽� �̸�/�ε���
	std::string leftTopText;
	switch( viewType ) {
	case VIEW_AXIAL:
		leftTopText = "Axial\n" + 
			std::to_string( volumeData->GetSliceIndex( viewType ) ) + 
			" / " + std::to_string( ext[5] );
		break;
	case VIEW_CORONAL:
		leftTopText = "Coronal\n" + 
			std::to_string( volumeData->GetSliceIndex( viewType ) ) + 
			" / " + std::to_string( ext[3] );
		break;
	case VIEW_SAGITTAL:
		leftTopText = "Sagittal\n" + 
			std::to_string( volumeData->GetSliceIndex( viewType ) ) + 
			" / " + std::to_string( ext[1] );
		break;
	}

	// �׷� ������ �˻�
	vtkSmartPointer<DicomGroup> group = GetDicomLoader()->GetCurrentGroup();
	if( group == NULL ) return;

	// ������ �� ���� : ȯ������
	std::string rightTopText = group->GetPatientName() + "\n" 
		+ group->GetPatientBirthDate() + "\n"
		+ group->GetPatientSex() + "\n"
		+ group->GetPatientAge() + "\n"
		+ group->GetPatientWeight() + "\n";

	// ���� �� �ڳ� (�ε��� 2) ���� ǥ��
	m_Annotation[viewType]->SetText( 2, leftTopText.c_str() );
	// ������ �� �ڳ� (�ε��� 3) ���� ǥ��
	m_Annotation[viewType]->SetText( 3, rightTopText.c_str() );
}
