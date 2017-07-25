
// vtkMFCDlgExDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "vtkMFCDlgEx.h"
#include "vtkMFCDlgExDlg.h"
#include "afxdialogex.h"

#include <vtkRendererCollection.h>
#include <vtkCamera.h>
#include <vtkLight.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkArrowSource.h>
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>
#include <vtkProperty.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkDecimatePro.h>
#include <vtkQuadricClustering.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkPlane.h>
#include <vtkClipPolyData.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkLandmarkTransform.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkFileOutputWindow.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageViewer2.h>
#include <vtkMarchingCubes.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolume.h>
#include <vtkExtractVOI.h>
#include <vtkImageData.h>
#include <vtkCallbackCommand.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCellPicker.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkAnnotatedCubeActor.h>		// �ڵ� �� �κп� �߰�

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CvtkMFCDlgExDlg ��ȭ ����



CvtkMFCDlgExDlg::CvtkMFCDlgExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CvtkMFCDlgExDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CvtkMFCDlgExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_BUTTON_EX_MESH, m_MenuButton[0] );
	DDX_Control( pDX, IDC_BUTTON_EX_VOLUME, m_MenuButton[1] );
	DDX_Control( pDX, IDC_BUTTON_EX_INTERFACE, m_MenuButton[2] );
}

BEGIN_MESSAGE_MAP(CvtkMFCDlgExDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONE, &CvtkMFCDlgExDlg::OnBnClickedButtonCone)
	ON_COMMAND_EX_RANGE( IDC_BUTTON_EX_MESH, IDC_BUTTON_EX_INTERFACE, &CvtkMFCDlgExDlg::OnBnClickedButtonExMesh )
	ON_COMMAND( ID_BUTTON_EX_VTKPOLYDATA, &CvtkMFCDlgExDlg::OnButtonExVtkpolydata )
	ON_COMMAND( ID_BUTTON_EX_VTKARROW, &CvtkMFCDlgExDlg::OnButtonExVtkarrow )
	ON_COMMAND( ID_BUTTON_EX_VTKSTLREADER, &CvtkMFCDlgExDlg::OnButtonExVtkstlreader )
	ON_COMMAND( ID_BUTTON_EX_VTKPROPERTY, &CvtkMFCDlgExDlg::OnButtonExVtkproperty )
	ON_COMMAND( ID_BUTTON_EX_VTKCLEANPOLYDATA, &CvtkMFCDlgExDlg::OnButtonExVtkcleanpolydata )
	ON_COMMAND( ID_BUTTON_EX_VTKPOLYDATANORMALS, &CvtkMFCDlgExDlg::OnButtonExVtkpolydatanormals )
	ON_COMMAND( ID_BUTTON_EX_DECIMATION, &CvtkMFCDlgExDlg::OnButtonExDecimation )
	ON_COMMAND( ID_BUTTON_EX_SMOOTHING, &CvtkMFCDlgExDlg::OnButtonExSmoothing )
	ON_COMMAND( ID_BUTTON_EX_CONNECTIVITYCHECK, &CvtkMFCDlgExDlg::OnButtonExConnectivitycheck )
	ON_COMMAND( ID_BUTTON_EX_CLIPPING, &CvtkMFCDlgExDlg::OnButtonExClipping )
	ON_COMMAND( ID_BUTTON_EX_TRANSFORM, &CvtkMFCDlgExDlg::OnButtonExTransform )
	ON_COMMAND( ID_BUTTON_EX_REGISTRATION_ICP, &CvtkMFCDlgExDlg::OnButtonExRegistrationICP )
	ON_COMMAND( ID_BUTTON_EX_VTKIMAGEDATA, &CvtkMFCDlgExDlg::OnButtonExVtkimagedata )
	ON_COMMAND( ID_BUTTON_EX_MARCHINGCUBES, &CvtkMFCDlgExDlg::OnButtonExMarchingcubes )
	ON_COMMAND( ID_BUTTON_EX_VOLUME_RENDERING, &CvtkMFCDlgExDlg::OnButtonExVolumeRendering )
	ON_COMMAND( ID_BUTTON_EX_VOLUME_CLIPPING, &CvtkMFCDlgExDlg::OnButtonExVolumeClipping )
	ON_COMMAND( ID_BUTTON_EX_PICKING, &CvtkMFCDlgExDlg::OnButtonExPicking )
	ON_COMMAND( ID_BUTTON_EX_WIDGET, &CvtkMFCDlgExDlg::OnButtonExWidget )
	ON_COMMAND( ID_BUTTON_EX_REGISTRATION_LANDMARK, &CvtkMFCDlgExDlg::OnButtonExRegistrationLandmark )
	ON_COMMAND( ID_BUTTON_EX_INTERACTOR, &CvtkMFCDlgExDlg::OnButtonExInteractor )
//	ON_BN_CLICKED( IDC_BUTTON_EX_VTKPOLYDATA, &CvtkMFCDlgExDlg::OnBnClickedButtonExVtkpolydata )
END_MESSAGE_MAP()


// CvtkMFCDlgExDlg �޽��� ó����

BOOL CvtkMFCDlgExDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	if (this->GetDlgItem(IDC_STATIC_FRAME))
	{
		this->InitVtkWindow(this->GetDlgItem(IDC_STATIC_FRAME)->GetSafeHwnd());	// Initialize VTK window
		this->ResizeVtkWindow();
	}

	// Menu �ε�
	m_Menu.LoadMenu( IDR_MENU );
	for( int i = 0; i < 3; i++ )
		m_MenuButton[i].m_hMenu = m_Menu.GetSubMenu( i )->GetSafeHmenu();

	// Warning Off
	vtkObject::GlobalWarningDisplayOff();
// 	vtkSmartPointer<vtkFileOutputWindow> fileOutputWindow =
// 		vtkSmartPointer<vtkFileOutputWindow>::New();
// 	fileOutputWindow->SetFileName( "output.txt" );
// 
// 	// Note that the SetInstance function is a static member of vtkOutputWindow.
// 	vtkOutputWindow::SetInstance( fileOutputWindow );

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CvtkMFCDlgExDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CvtkMFCDlgExDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CvtkMFCDlgExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CvtkMFCDlgExDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	this->ResizeVtkWindow();
}

void CvtkMFCDlgExDlg::InitVtkWindow(void* hWnd)
{
	// vtk Render Window ����
	if (m_vtkWindow == NULL)
	{
		// Interactor ����
		vtkSmartPointer<vtkRenderWindowInteractor> interactor =
			vtkSmartPointer<vtkRenderWindowInteractor>::New();
		// Trackball Camera ���ͷ��� ��Ÿ�� ����
		interactor->SetInteractorStyle(
			vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New());

		// Renderer ����
		vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
		renderer->SetBackground(0.0, 0.0, 0.0);		// ������ ���

		// RenderWindow ���� �� Dialog �ڵ�, Renderer ����
		m_vtkWindow = vtkSmartPointer<vtkRenderWindow>::New();
		m_vtkWindow->SetParentId(hWnd);
		m_vtkWindow->SetInteractor(interactor);
		m_vtkWindow->AddRenderer(renderer);
		m_vtkWindow->Render();
	}
}

void CvtkMFCDlgExDlg::ResizeVtkWindow()
{
	CRect rc;
	GetDlgItem(IDC_STATIC_FRAME)->GetClientRect(rc);
	m_vtkWindow->SetSize(rc.Width(), rc.Height());
}

void CvtkMFCDlgExDlg::OnBnClickedButtonCone()
{
	vtkSmartPointer<vtkRenderer> prevRenderer = m_vtkWindow->GetRenderers()->GetFirstRenderer();
	if( prevRenderer != NULL ) m_vtkWindow->RemoveRenderer( prevRenderer );

	// TODO: Add your control notification handler code here
	//////////////////////////////////////////////////////////////////////////
	// Create a cone source
	vtkSmartPointer<vtkConeSource> coneSource =
		vtkSmartPointer<vtkConeSource>::New();

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(coneSource->GetOutputPort());
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->SetBackground(.1, .2, .3); // Background color dark blue
	renderer->ResetCamera();

	vtkSmartPointer<vtkCamera> cam = 
		renderer->GetActiveCamera();		// renderer���� ī�޶� �޾ƿ���
	cam->SetClippingRange( 0.1, 10 );	// �׷��� depth ���� ����
	cam->SetFocalPoint( 0, 0, 0 );			// ī�޶� �ٶ󺸴� ����
	cam->SetViewUp( 0, 1, 0 );				// ī�޶��� upvector ����
	cam->SetPosition( 0, 0, 5 );			// ī�޶� ��ġ ����
	//cam->ParallelProjectionOn();			// on: orthogonal view / off: perspective view

	vtkSmartPointer<vtkLight> newLight = vtkSmartPointer<vtkLight>::New();
	newLight->SetColor( 1, 1, 0 );							// ����� ���� (���)
	newLight->SetFocalPoint( cam->GetFocalPoint() );	// ���ߴ� ����
	newLight->SetPosition( cam->GetPosition() );		// ���� ��ġ ����
	renderer->AddLight( newLight );

	//////////////////////////////////////////////////////////////////////////
	//rendering
	m_vtkWindow->AddRenderer(renderer);
	m_vtkWindow->Render();

}


BOOL CvtkMFCDlgExDlg::OnBnClickedButtonExMesh( UINT id )
{
	vtkSmartPointer<vtkRenderer> prevRenderer = m_vtkWindow->GetRenderers()->GetFirstRenderer();
	if( prevRenderer != NULL ) m_vtkWindow->RemoveRenderer( prevRenderer );

	// IDC_BUTTON_EX_MESH : menuID = 0
	// IDC_BUTTON_EX_Volume : menuID = 1
	// IDC_BUTTON_EX_Interface : menuID = 2
	int menuID = id - IDC_BUTTON_EX_MESH;

	if( m_MenuButton[menuID].m_nMenuResult == 0 ) {
		CRect rect;
		m_MenuButton[menuID].GetWindowRect( rect );
		m_Menu.GetSubMenu( menuID )->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.bottom, this );
	}
	else {
		this->PostMessage( WM_COMMAND, m_MenuButton[menuID].m_nMenuResult );
	}

	return TRUE;
}


void CvtkMFCDlgExDlg::OnButtonExVtkpolydata()
{
	vtkSmartPointer<vtkPoints> pPoints = 
		vtkSmartPointer<vtkPoints>::New();
	pPoints->InsertPoint(0,0.0,0.0,0.0);	// InsetPoint(ID,x,y,z);
	pPoints->InsertPoint(1,0.0,1.0,0.0);
	pPoints->InsertPoint(2,1.0,0.0,0.0);
	pPoints->InsertPoint(3,1.0,1.0,0.0);

	vtkSmartPointer<vtkCellArray> pPolys = 
		vtkSmartPointer<vtkCellArray>::New();
	pPolys->InsertNextCell(3);		// number of points
	pPolys->InsertCellPoint(0);		// Point's ID
	pPolys->InsertCellPoint(1);
	pPolys->InsertCellPoint(2);
	pPolys->InsertNextCell(3);
	pPolys->InsertCellPoint(1);
	pPolys->InsertCellPoint(3);
	pPolys->InsertCellPoint(2);

	vtkSmartPointer<vtkPolyData> pPolyData = 
		vtkSmartPointer<vtkPolyData>::New();
	pPolyData->SetPoints(pPoints);	// ��ġ ����
	pPolyData->SetPolys(pPolys);		// ���� ����

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData( pPolyData );
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );
	
	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExVtkarrow()
{
	vtkSmartPointer<vtkArrowSource> pArrow = 
		vtkSmartPointer<vtkArrowSource>::New();
	pArrow->SetShaftRadius( 0.03 );			// �Ķ���� ���� (Option)
	pArrow->SetShaftResolution( 100 );
	pArrow->SetTipRadius( 0.1 );
	pArrow->SetTipLength( 0.35 );
	pArrow->SetTipResolution( 100 );
	pArrow->Update();

	vtkSmartPointer<vtkPolyData> pPolyData = 
		pArrow->GetOutput();	// vtkPolyData �޾ƿ���

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData( pPolyData );
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );
	
	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExVtkstlreader()
{
	vtkSmartPointer<vtkSTLReader> pSTLReader = 
		vtkSmartPointer<vtkSTLReader>::New();
	pSTLReader->SetFileName("../data/example.stl");	// ���� ���� ����
	pSTLReader->Update();

	vtkSmartPointer<vtkPolyData> pPolyData = 
		pSTLReader->GetOutput();				// vtkPolyData �������� �޾ƿ���
	
	vtkSmartPointer<vtkSTLWriter> pSTLWriter = 
		vtkSmartPointer<vtkSTLWriter>::New();
	pSTLWriter->SetInputData(pPolyData);				// ������ vtkPolyData
	pSTLWriter->SetFileName("../data/example1.stl");	// ������ ���� ����
	pSTLWriter->Write();									// ���� �ϱ�

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData( pPolyData );
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );
	
	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( .1, .2, .3 ); // Background color dark blue
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExVtkproperty()
{
	vtkSmartPointer<vtkArrowSource> arrow = 
		vtkSmartPointer<vtkArrowSource>::New();				// ȭ��ǥ source

	vtkSmartPointer<vtkPolyDataMapper> mapper = 
		vtkSmartPointer<vtkPolyDataMapper>::New();			// PolyData mapper ����
	mapper->SetInputConnection(arrow->GetOutputPort());  // Mapper�� PolyData ����

	vtkSmartPointer<vtkActor> actor = 
		vtkSmartPointer<vtkActor>::New();						// Actor ����
	actor->SetMapper(mapper);									// Actor�� Mapper ����

	actor->GetProperty()->SetColor( 0, 1, 0 );	// ���� ����
	actor->GetProperty()->SetOpacity( 0.5 );		// ������ ���� 0.0:���� ~ 1.0:������
	actor->GetProperty()->SetPointSize( 1.0 );	// Vertex ������ ����
	actor->GetProperty()->SetLineWidth( 1.0 );	// Line �β� ����

	// VTK_POINTS, VTK_WIREFRAME, VTK_SURFACE
	actor->GetProperty()->SetRepresentation(VTK_SURFACE); // �׸��� ��� ����
	//actor->GetProperty()->SetTexture(pTexture);// vtkTexture (Texture Mapping)
	actor->GetProperty()->BackfaceCullingOn();	 // Culling On/Off
	actor->GetProperty()->LightingOn();	 // Lighting On/Off
	actor->GetProperty()->ShadingOn();	 // Shading On/Off
	
	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();
}

void CvtkMFCDlgExDlg::OnButtonExVtkcleanpolydata()
{
	vtkSmartPointer<vtkPoints> pPoints = 
		vtkSmartPointer<vtkPoints>::New();
	pPoints->InsertPoint( 0, 0.0, 0.0, 0.0 );	// InsetPoint(ID,x,y,z);
	pPoints->InsertPoint( 1, 0.0, 1.0, 0.0 );
	pPoints->InsertPoint( 2, 1.0, 0.0, 0.0 );
	pPoints->InsertPoint( 3, 1.0, 1.0, 0.0 );
	pPoints->InsertPoint( 4, 0.0, 1.0, 0.0 );	// 1�� ���� �ߺ�
	pPoints->InsertPoint( 5, 1.0, 0.0, 0.0 );	// 2�� ���� �ߺ�

	vtkSmartPointer<vtkCellArray> pPolys = 
		vtkSmartPointer<vtkCellArray>::New();
	pPolys->InsertNextCell( 3 );		// number of points
	pPolys->InsertCellPoint( 0 );		// Point's ID
	pPolys->InsertCellPoint( 1 );
	pPolys->InsertCellPoint( 2 );

	pPolys->InsertNextCell( 3 );
	pPolys->InsertCellPoint( 4 );
	pPolys->InsertCellPoint( 3 );
	pPolys->InsertCellPoint( 5 );

	vtkSmartPointer<vtkPolyData> pPolyData = 
		vtkSmartPointer<vtkPolyData>::New();
	pPolyData->SetPoints( pPoints );			// ��ġ ����
	pPolyData->SetPolys( pPolys );			// ���� ����

	int nPt = pPolyData->GetNumberOfPoints();		// nPt = 6
	int nPoly = pPolyData->GetNumberOfPolys();		// nPoly = 2

	vtkSmartPointer<vtkCleanPolyData> pClean = 
		vtkSmartPointer<vtkCleanPolyData>::New();
	pClean->SetInputData( pPolyData );
	pClean->Update();

	pPolyData->DeepCopy( pClean->GetOutput() );	// vtkPolyData �����ϱ�
	nPt = pPolyData->GetNumberOfPoints();			// nPt = 4
	nPoly = pPolyData->GetNumberOfPolys();			// nPoly = 2

	
	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData( pPolyData );
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );
	
	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExVtkpolydatanormals()
{
	// STL ���� �о����
	vtkSmartPointer<vtkSTLReader> stlReader =
		vtkSmartPointer<vtkSTLReader>::New();
	stlReader->SetFileName( "../data/example.stl" );
	stlReader->Update();

	//////////////////////////////////////////////////////////////////////////
	// Filter ó�� vtkPolyData ��
	// Normal vector ���
	vtkSmartPointer<vtkPolyDataNormals> normals =
		vtkSmartPointer<vtkPolyDataNormals>::New();
	normals->SetInputData( stlReader->GetOutput() );
	normals->ComputePointNormalsOn();			// Point normal ���
	normals->ComputeCellNormalsOn();				// Cell normal ���
	normals->Update();

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( normals->GetOutputPort() );
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );

	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExDecimation()
{
	// STL ���� �о����
	vtkSmartPointer<vtkSTLReader> stlReader =
		vtkSmartPointer<vtkSTLReader>::New();
	stlReader->SetFileName( "../data/example.stl" );
	stlReader->Update();

	//////////////////////////////////////////////////////////////////////////
	// Filter ó�� vtkPolyData(dental.stl) ��
	// 1) vtkDecimationPro
	vtkSmartPointer<vtkDecimatePro> decimatePro =
		vtkSmartPointer<vtkDecimatePro>::New();
	decimatePro->SetInputConnection( stlReader->GetOutputPort() );
	decimatePro->SetTargetReduction( 0.9 ); // ���� mesh 10% ����
	decimatePro->PreserveTopologyOn();
	decimatePro->Update();

	// 2) vtkQuadricClustering
	vtkSmartPointer<vtkQuadricClustering> qClustering =
		vtkSmartPointer<vtkQuadricClustering>::New();
	qClustering->SetInputConnection( stlReader->GetOutputPort() );
	qClustering->SetNumberOfDivisions( 10, 10, 10 );  // ���� ���� ���� (��������)
	qClustering->Update();

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	// �� ���� ��� �� �ϳ� �����Ͽ� ������
	mapper->SetInputConnection( decimatePro->GetOutputPort() );
	//mapper->SetInputConnection( qClustering->GetOutputPort() );

	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );

	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExSmoothing()
{
	// STL ���� �о����
	vtkSmartPointer<vtkSTLReader> stlReader =
		vtkSmartPointer<vtkSTLReader>::New();
	stlReader->SetFileName( "../data/example.stl" );
	stlReader->Update();

	//////////////////////////////////////////////////////////////////////////
	// Filter ó�� vtkPolyData(dental.stl) ��
	// Smoothing
	vtkSmartPointer<vtkWindowedSincPolyDataFilter> smoothFilter =
		vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New();
	smoothFilter->SetInputConnection( stlReader->GetOutputPort() );
	smoothFilter->SetNumberOfIterations( 100 );     //�ݺ����� Ƚ��
	smoothFilter->Update();

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( smoothFilter->GetOutputPort() );

	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );
	
	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExConnectivitycheck()
{
	// STL ���� �о����
	vtkSmartPointer<vtkSTLReader> stlReader =
		vtkSmartPointer<vtkSTLReader>::New();
	stlReader->SetFileName( "../data/example_connectivity.stl" );
	stlReader->Update();

	//////////////////////////////////////////////////////////////////////////
	// Filter ó�� vtkPolyData
	vtkSmartPointer<vtkPolyDataConnectivityFilter> conFilter = 
		vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	conFilter->SetInputConnection( stlReader->GetOutputPort() );

	// 1) ��� ���� ����
	conFilter->SetExtractionModeToAllRegions();

	// 2) ���� ū ���� ����
//	conFilter->SetExtractionModeToLargestRegion();

	// 3) seed�� ����� ���� ����
// 	conFilter->AddSeed(id);
// 	conFilter->SetExtractionModeToCellSeededRegions();     // 3-1) Ư�� cell seed
// 	conFilter->SetExtractionModeToPointSeededRegions();    // 3-2) Ư�� point seed

	// 4) Ư�� point�� ����� ���� ����� ���� ����
//	conFilter->SetClosestPoint(x,y,z);
//	conFilter->SetExtractionModeToClosestPointRegion(); 

	conFilter->Update();

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper = 
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( conFilter->GetOutputPort() );

	vtkSmartPointer<vtkActor> actor = 
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	
	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();

	// 1�� ��ٸ�
	Sleep( 1000 );
	
	// 2) ���� ū ���� ����
	conFilter->SetExtractionModeToLargestRegion();
	conFilter->Update();
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExClipping()
{
	// STL ���� �о����
	vtkSmartPointer<vtkSTLReader> stlReader = 
		vtkSmartPointer<vtkSTLReader>::New();
	stlReader->SetFileName("../data/example.stl");
	stlReader->Update();

	//////////////////////////////////////////////////////////////////////////
	// Filter ó�� vtkPolyData
	double center[3];
	stlReader->GetOutput()->GetCenter(center); // mesh data�� �߽�

	vtkSmartPointer<vtkPlane> plane = 
		vtkSmartPointer<vtkPlane>::New();	// clipping plane ����
	plane->SetOrigin(center);					// ���� ����
	plane->SetNormal(1.0, 0.0, 0.0);			// normal vector ����

	vtkSmartPointer<vtkClipPolyData> clipper = 
		vtkSmartPointer<vtkClipPolyData>::New();
	clipper->SetInputConnection(stlReader->GetOutputPort());	// clip ��� mesh ����
	clipper->SetClipFunction(plane);									// clip ��� ����
	clipper->Update();

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper = 
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( clipper->GetOutputPort() );
	vtkSmartPointer<vtkActor> actor = 
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	
	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExTransform()
{
	// STL ���� �о����
	vtkSmartPointer<vtkSTLReader> stlReader =
		vtkSmartPointer<vtkSTLReader>::New();
	stlReader->SetFileName( "../data/example.stl" );
	stlReader->Update();

	//////////////////////////////////////////////////////////////////////////
	// Filter ó�� vtkPolyData
	vtkSmartPointer<vtkTransform> pTransform =
		vtkSmartPointer<vtkTransform>::New();
	// 1) ��ȯ ���� ����
	pTransform->Translate( 10.0, 0.0, 0.0 );       // x������ 10.0 �̵�
	pTransform->RotateWXYZ( 30, 0.0, 1.0, 0.0 );   // y�� �߽����� 30deg ȸ��

	// 2) ��ȯ matrix ����
	//pTransform->SetMatrix(mat);              // vtkMatrix4x4�� ���� ����

	vtkSmartPointer<vtkTransformPolyDataFilter> pTransformFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	pTransformFilter->SetInputConnection( stlReader->GetOutputPort() );  // ��� mesh ����
	pTransformFilter->SetTransform( pTransform );  // ��ȯ transform ����
	pTransformFilter->Update();

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( pTransformFilter->GetOutputPort() );
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );

	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExRegistrationLandmark()
{
	// �ﰢ�� 1 ����
	vtkSmartPointer<vtkPoints> points1 =
		vtkSmartPointer<vtkPoints>::New();
	points1->InsertNextPoint( -1.5, 0, 0 );
	points1->InsertNextPoint( 1.5, 0, 0 );
	points1->InsertNextPoint( 0, 1, 0 );

	vtkSmartPointer<vtkCellArray> polys1 =
		vtkSmartPointer<vtkCellArray>::New();
	polys1->InsertNextCell( 3 );		// number of points
	polys1->InsertCellPoint( 0 );		// Point's ID
	polys1->InsertCellPoint( 1 );
	polys1->InsertCellPoint( 2 );

	vtkSmartPointer<vtkPolyData> polyData1 =
		vtkSmartPointer<vtkPolyData>::New();
	polyData1->SetPoints( points1 );
	polyData1->SetPolys( polys1 );

	// �ﰢ�� 2 ����
	vtkSmartPointer<vtkPoints> points2 =
		vtkSmartPointer<vtkPoints>::New();
	points2->InsertNextPoint( 4, 2, 0 );
	points2->InsertNextPoint( 2, 4, 0 );
	points2->InsertNextPoint( 2, 2, 0 );

	vtkSmartPointer<vtkCellArray> polys2 =
		vtkSmartPointer<vtkCellArray>::New();
	polys2->InsertNextCell( 3 );		// number of points
	polys2->InsertCellPoint( 0 );		// Point's ID
	polys2->InsertCellPoint( 1 );
	polys2->InsertCellPoint( 2 );

	vtkSmartPointer<vtkPolyData> polyData2 =
		vtkSmartPointer<vtkPolyData>::New();
	polyData2->SetPoints( points2 );
	polyData2->SetPolys( polys2 );

	// ������
	vtkSmartPointer<vtkPolyDataMapper> mapper1 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper1->SetInputData( polyData1 );
	mapper1->Update();
	vtkSmartPointer<vtkActor> actor1
		= vtkSmartPointer<vtkActor>::New();
	actor1->SetMapper( mapper1 );
	actor1->GetProperty()->SetRepresentationToWireframe();
	actor1->GetProperty()->SetColor( 1, 0, 0 );

	vtkSmartPointer<vtkPolyDataMapper> mapper2 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper2->SetInputData( polyData2 );
	mapper2->Update();
	vtkSmartPointer<vtkActor> actor2
		= vtkSmartPointer<vtkActor>::New();
	actor2->SetMapper( mapper2 );
	actor2->GetProperty()->SetRepresentationToWireframe();
	actor2->GetProperty()->SetColor( 0, 1, 0 );

	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor1 );
	renderer->AddActor( actor2 );
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();

	// 1�� ��ٸ�
	Sleep( 1000 );

	// Landmark ���� ����
	vtkSmartPointer<vtkLandmarkTransform> lmt =
		vtkSmartPointer<vtkLandmarkTransform>::New();
	lmt->SetSourceLandmarks( points1 );	// sorce������ ����(vtkPoints)
	lmt->SetTargetLandmarks( points2 );		// target������ ����(vtkPoints)
	lmt->SetModeToRigidBody();				// ��ü ��ȯ
	lmt->Update();

	actor1->SetUserTransform( lmt );
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExRegistrationICP()
{
	// STL ���� �о����
	vtkSmartPointer<vtkSTLReader> stlReader1 =
		vtkSmartPointer<vtkSTLReader>::New();
	stlReader1->SetFileName( "../data/example.stl" );
	stlReader1->Update();

	vtkSmartPointer<vtkSTLReader> stlReader2 =
		vtkSmartPointer<vtkSTLReader>::New();
	stlReader2->SetFileName( "../data/example_smooth_transform.stl" );
	stlReader2->Update();

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper1 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper1->SetInputConnection( stlReader1->GetOutputPort() );
	vtkSmartPointer<vtkActor> actor1 =
		vtkSmartPointer<vtkActor>::New();
	actor1->SetMapper( mapper1 );
	actor1->GetProperty()->SetColor( 1.0, 1.0, 0.5 );
	actor1->GetProperty()->SetOpacity( 0.5 );

	vtkSmartPointer<vtkPolyDataMapper> mapper2 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper2->SetInputConnection( stlReader2->GetOutputPort() );
	vtkSmartPointer<vtkActor> actor2 =
		vtkSmartPointer<vtkActor>::New();
	actor2->SetMapper( mapper2 );
	actor2->GetProperty()->SetOpacity( 0.5 );

	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor1 );
	renderer->AddActor( actor2 );
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();

	// 1�� ��ٸ�
	Sleep( 1000 );

	// ICP ����
	vtkSmartPointer<vtkIterativeClosestPointTransform> ICP =
		vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
	ICP->SetSource( stlReader1->GetOutput() );			// sorce������ ���� (vtkPolydata)
	ICP->SetTarget( stlReader2->GetOutput() );			// target������ ����(vtkPolydata)
	ICP->GetLandmarkTransform()->SetModeToRigidBody();// ��ü ��ȯ
	ICP->SetMaximumNumberOfIterations( 100 );		// �ִ� 100�� �ݺ����� (default:50��)
	ICP->SetMaximumNumberOfLandmarks( 50 );		// ��Ī �� ���� ���� (default:200��)
	ICP->Update();

	actor1->SetUserTransform( ICP );		// ��ȯ transform ����
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExVtkimagedata()
{
	vtkSmartPointer<vtkDICOMImageReader> dcmReader = 
		vtkSmartPointer<vtkDICOMImageReader>::New();
	// 1) file �� �� �ҷ����� (2���� vtkImageData)
	dcmReader->SetFileName("../data/CT/CT.00002.00020.dcm");
	// 2) folder�� �ִ� ���� ��� �ҷ����� (3���� vtkImageData)
//	dcmReader->SetDirectoryName("../data/CT");
	dcmReader->Update();

	// Visualize
	vtkSmartPointer<vtkImageViewer2> imageViewer = 
		vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection( dcmReader->GetOutputPort() );
	imageViewer->SetRenderWindow( m_vtkWindow );
	imageViewer->Render();
}


void CvtkMFCDlgExDlg::OnButtonExMarchingcubes()
{
	vtkSmartPointer<vtkDICOMImageReader> dcmReader =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	// folder�� �ִ� ���� ��� �ҷ����� (3���� vtkImageData)
	dcmReader->SetDirectoryName( "../data/CT" );
	dcmReader->Update();

	//////////////////////////////////////////////////////////////////////////
	// Marching cube filter	(vtkImageData �� vtkPolyData)
	vtkSmartPointer<vtkMarchingCubes> pMarchingCube =
		vtkSmartPointer<vtkMarchingCubes>::New();
	pMarchingCube->SetInputConnection( dcmReader->GetOutputPort() );
	pMarchingCube->SetValue( 0, 330 );			// ù��° iso-value ���� (id,value)
	pMarchingCube->ComputeScalarsOff();
	pMarchingCube->ComputeNormalsOn();	// Normal�� ���
	pMarchingCube->Update();

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( pMarchingCube->GetOutputPort() );
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );

	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExVolumeRendering()
{
	// CT ���� �ҷ�����
	vtkSmartPointer<vtkDICOMImageReader> dcmReader =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	dcmReader->SetDirectoryName( "../data/CT" );
	dcmReader->Update();

	// Rendering Opacity �� ����
	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =
		vtkSmartPointer<vtkPiecewiseFunction>::New();
	// 1) CT MUSCLE
	compositeOpacity->AddPoint( -3024, 0 );
	compositeOpacity->AddPoint( -155.41, 0 );
	compositeOpacity->AddPoint( 217.64, 0.68 );
	compositeOpacity->AddPoint( 419.74, 0.83 );
	compositeOpacity->AddPoint( 3071, 0.80 );
	// 2) CT BONE
// 	compositeOpacity->AddPoint( -3024, 0 );
// 	compositeOpacity->AddPoint( -16.45, 0 );
// 	compositeOpacity->AddPoint( 641.38, .72 );
// 	compositeOpacity->AddPoint( 3071, .71 );

	// Rendering Color �� ����
	vtkSmartPointer<vtkColorTransferFunction> color =
		vtkSmartPointer<vtkColorTransferFunction>::New();
	// 1) CT MUSCLE
	color->AddRGBPoint( -3024, 0, 0, 0 );
	color->AddRGBPoint( -155.41, .55, .25, .15 );
	color->AddRGBPoint( 217.64, .88, .60, .29 );
	color->AddRGBPoint( 419.74, 1, .94, .95 );
	color->AddRGBPoint( 3071, .83, .66, 1 );
	// 2) CT BONE
// 	color->AddRGBPoint( -3024, 0, 0, 0);
// 	color->AddRGBPoint( -16.45, .73, .25, .30);
// 	color->AddRGBPoint( 641.38, .91, .82, .55);
// 	color->AddRGBPoint( 3071, 1, 1, 1);

	// Smart Volume Mapper ���
	vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper =
		vtkSmartPointer<vtkSmartVolumeMapper>::New();
	volumeMapper->SetInputConnection( dcmReader->GetOutputPort() );
	volumeMapper->SetBlendModeToComposite();    // ���� ��� ���� (composite first)

	// Volume �׸��� �Ӽ� ����
	vtkSmartPointer<vtkVolumeProperty> volumeProperty =
		vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationType( VTK_LINEAR_INTERPOLATION );
	volumeProperty->SetColor( color );
	volumeProperty->SetScalarOpacity( compositeOpacity ); // composite first.

	// vtkVolume�� Volume rendering�� ���� Actor������ �Ѵ�
	vtkSmartPointer<vtkVolume> volume =
		vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper( volumeMapper );
	volume->SetProperty( volumeProperty );
	
	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	// vtkVolume�� AddActor�� �ƴ� AddViewProp �Լ��� �߰�
	renderer->AddViewProp( volume );		
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExVolumeClipping()
{
	// CT ���� �ҷ�����
	vtkSmartPointer<vtkDICOMImageReader> dcmReader =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	dcmReader->SetDirectoryName( "../data/CT" );
	dcmReader->Update();

	//////////////////////////////////////////////////////////////////////////
	// 1) ���ο� �������� �����س���
	int ext[6];
	dcmReader->GetOutput()->GetExtent( ext );  // volume�� extent �޾ƿ���

	// VOI ���� (x����� ������ �����ϱ�)
	vtkSmartPointer<vtkExtractVOI> extractVOI =
		vtkSmartPointer<vtkExtractVOI>::New();
	extractVOI->SetInputConnection( dcmReader->GetOutputPort() );
	extractVOI->SetVOI( ext[0], (ext[1] - ext[0]) / 2, ext[2], ext[3], ext[4], ext[5] );
	extractVOI->Update();

	vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper1 =
		vtkSmartPointer<vtkSmartVolumeMapper>::New();
	volumeMapper1->SetInputConnection( extractVOI->GetOutputPort() );
	volumeMapper1->SetBlendModeToComposite();

	//////////////////////////////////////////////////////////////////////////
	// 2) �������� clipping�ϱ�
	double center[3];
	dcmReader->GetOutput()->GetCenter( center ); // volume data�� �߽�
	vtkSmartPointer<vtkPlane> plane =
		vtkSmartPointer<vtkPlane>::New();	// clipping plane ����
	plane->SetOrigin( center );				// ���� ����
	plane->SetNormal( -1.0, 1.0, -1.0 );		// normal vector ����

	vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper2 =
		vtkSmartPointer<vtkSmartVolumeMapper>::New();
	volumeMapper2->SetInputConnection( dcmReader->GetOutputPort() );
	volumeMapper2->SetBlendModeToComposite();      // composite first
	volumeMapper2->AddClippingPlane( plane );

	// Rendering Opacity �� ����
	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =
		vtkSmartPointer<vtkPiecewiseFunction>::New();
	// 1) CT MUSCLE
	compositeOpacity->AddPoint( -3024, 0 );
	compositeOpacity->AddPoint( -155.41, 0 );
	compositeOpacity->AddPoint( 217.64, 0.68 );
	compositeOpacity->AddPoint( 419.74, 0.83 );
	compositeOpacity->AddPoint( 3071, 0.80 );

	// Rendering Color �� ����
	vtkSmartPointer<vtkColorTransferFunction> color =
		vtkSmartPointer<vtkColorTransferFunction>::New();
	// 1) CT MUSCLE
	color->AddRGBPoint( -3024, 0, 0, 0 );
	color->AddRGBPoint( -155.41, .55, .25, .15 );
	color->AddRGBPoint( 217.64, .88, .60, .29 );
	color->AddRGBPoint( 419.74, 1, .94, .95 );
	color->AddRGBPoint( 3071, .83, .66, 1 );

	// Volume �׸��� �Ӽ� ����
	vtkSmartPointer<vtkVolumeProperty> volumeProperty =
		vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationType( VTK_LINEAR_INTERPOLATION );
	volumeProperty->SetColor( color );
	volumeProperty->SetScalarOpacity( compositeOpacity ); // composite first.

	// vtkVolume�� Volume rendering�� ���� Actor������ �Ѵ�
	vtkSmartPointer<vtkVolume> volume =
		vtkSmartPointer<vtkVolume>::New();
	volume->SetProperty( volumeProperty );
	// �� ���� ��� �� �ϳ��� �����Ͽ� ������
//	volume->SetMapper( volumeMapper1 );
	volume->SetMapper( volumeMapper2 );

	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddViewProp( volume );
	renderer->SetBackground( .1, .2, .3 ); // Background color dark blue
	renderer->ResetCamera();

	//////////////////////////////////////////////////////////////////////////
	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();
}

//  1.	Call back �Լ� ����
void KeyPressCallbackFunc( vtkObject* caller, long unsigned int eventId, 
	void* clientData, void* callData )
{
	MessageBox( NULL, _T( "Key Press Event" ), _T( "Pop-up" ), MB_OK );
}

void CvtkMFCDlgExDlg::OnButtonExInteractor()
{
	// �� ���� �� ������
	vtkSmartPointer<vtkConeSource> coneSource =
		vtkSmartPointer<vtkConeSource>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( coneSource->GetOutputPort() );
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );

	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();

	//////////////////////////////////////////////////////////////////////////
	// 2.	vktCallbackCommand�� ������ �Լ� ����
	vtkSmartPointer<vtkCallbackCommand> keypressCallback =
		vtkSmartPointer<vtkCallbackCommand>::New();
	keypressCallback->SetCallback( KeyPressCallbackFunc );

	//////////////////////////////////////////////////////////////////////////
	// 3.	Interactor �� observer �߰�
	// AddObserve ( vtkCommand::�̺�Ʈ, vtkCallbackCommand-���� �� �Լ� )
	m_vtkWindow->GetInteractor()->AddObserver( vtkCommand::KeyPressEvent, keypressCallback );
}


//  1.	Call back �Լ� ����
void PickCallbackFunction( vtkObject* caller, long unsigned int eventId,
	void* clientData, void* callData )
{
	// Interactor ��������
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkRenderWindowInteractor::SafeDownCast( caller );
	if( interactor == NULL ) return;

	// ���콺 Ŭ�� ��ġ
	int pos[2];
	interactor->GetLastEventPosition( pos );

	// ���콺 Ŭ�� ��ġ���� Picking ����
	vtkSmartPointer<vtkCellPicker> picker =
		vtkSmartPointer<vtkCellPicker>::New();
	picker->SetTolerance( 0.005 );		// picking ���� ����
	picker->Pick( pos[0], pos[1], 0,
		interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer() );

	vtkIdType cellId = picker->GetCellId();	// -1�̸� picking ���� ����
	if( cellId != -1 ) MessageBox( NULL, _T( "Pick Event" ), _T( "Pop-up" ), MB_OK );
}

void CvtkMFCDlgExDlg::OnButtonExPicking()
{
	// �� ���� �� ������
	vtkSmartPointer<vtkConeSource> coneSource =
		vtkSmartPointer<vtkConeSource>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( coneSource->GetOutputPort() );
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );

	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();

	//////////////////////////////////////////////////////////////////////////
	//	2.	vktCallbackCommand�� ������ �Լ� ����
	vtkSmartPointer<vtkCallbackCommand> pickCallback =
		vtkSmartPointer<vtkCallbackCommand>::New();
	pickCallback->SetCallback( PickCallbackFunction );

	//////////////////////////////////////////////////////////////////////////
	// 3.	Interactor �� observer �߰�
	m_vtkWindow->GetInteractor()->
		AddObserver( vtkCommand::LeftButtonPressEvent, pickCallback );
}


void CvtkMFCDlgExDlg::OnButtonExWidget()
{
	// �� ���� �� ������
	vtkSmartPointer<vtkConeSource> coneSource =
		vtkSmartPointer<vtkConeSource>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( coneSource->GetOutputPort() );
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );
	
	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();
	
	// �Ʒ��� �� Widget ���� �κ��� �ּ�ó���ϰ�
	//   �� ���� �� �ּ��� �����Ͽ� Ȯ���� �� �� �ִ�.
	//////////////////////////////////////////////////////////////////////////
	// 1) Angle Widget ����
	m_angleWidget = vtkSmartPointer<vtkAngleWidget>::New();
	m_angleWidget->SetInteractor( m_vtkWindow->GetInteractor() );
	m_angleWidget->CreateDefaultRepresentation();
	m_angleWidget->On();
	
	//////////////////////////////////////////////////////////////////////////
	// 2) vtkImagePlaneWidget ���� (������ �ܸ�)
	vtkSmartPointer<vtkDICOMImageReader> dcmReader =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	dcmReader->SetDirectoryName( "../data/CT" );
	dcmReader->Update();

	m_imageWidget = vtkSmartPointer<vtkImagePlaneWidget>::New();
 	m_imageWidget->SetInteractor( m_vtkWindow->GetInteractor() );
	m_imageWidget->SetInputData( dcmReader->GetOutput() );
	m_imageWidget->RestrictPlaneToVolumeOn();
	m_imageWidget->SetPlaneOrientationToZAxes();
	m_imageWidget->SetSliceIndex( 20 );
	m_imageWidget->On();

	renderer->ResetCamera();
	m_vtkWindow->Render();
	
	//////////////////////////////////////////////////////////////////////////
	// 3) vtkCaptionWidget �ؽ�Ʈ �����Ͽ� �����ϱ�
	vtkSmartPointer<vtkCaptionActor2D> captionActor = 
		vtkSmartPointer<vtkCaptionActor2D>::New();
	captionActor->SetCaption( "vtk programming" );
	captionActor->GetTextActor()->GetTextProperty()->SetJustificationToCentered();
	captionActor->GetTextActor()->GetTextProperty()->SetVerticalJustificationToCentered();

	m_captionWidget = vtkSmartPointer<vtkCaptionWidget>::New();
	m_captionWidget->SetInteractor( m_vtkWindow->GetInteractor() );
	m_captionWidget->SetCaptionActor2D( captionActor );
	m_captionWidget->On();
	
	//////////////////////////////////////////////////////////////////////////
	// 4) vtkOrientationMarkerWidget �����Ͽ� �����ϱ�
	vtkSmartPointer<vtkAnnotatedCubeActor> cube =
		vtkSmartPointer<vtkAnnotatedCubeActor>::New();

	m_orientationWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	m_orientationWidget->SetOrientationMarker( cube );
	m_orientationWidget->SetInteractor( m_vtkWindow->GetInteractor() );
	m_orientationWidget->SetViewport( 0.0, 0.0, 0.2, 0.2 );
	m_orientationWidget->SetEnabled( TRUE );
	m_orientationWidget->On();
	
	//////////////////////////////////////////////////////////////////////////
	// 5) vtkTextWidget �����Ͽ� �����ϱ�
	vtkSmartPointer<vtkTextActor> textActor =
		vtkSmartPointer<vtkTextActor>::New();
	textActor->SetInput( "Text" );

	m_textWidget = vtkSmartPointer<vtkTextWidget>::New();
	m_textWidget->SetInteractor( m_vtkWindow->GetInteractor() );
	m_textWidget->SetTextActor( textActor );
	m_textWidget->On();
}

