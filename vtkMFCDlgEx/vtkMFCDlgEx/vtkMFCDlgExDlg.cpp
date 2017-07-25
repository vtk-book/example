
// vtkMFCDlgExDlg.cpp : 구현 파일
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
#include <vtkAnnotatedCubeActor.h>		// 코드 앞 부분에 추가

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CvtkMFCDlgExDlg 대화 상자



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


// CvtkMFCDlgExDlg 메시지 처리기

BOOL CvtkMFCDlgExDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	if (this->GetDlgItem(IDC_STATIC_FRAME))
	{
		this->InitVtkWindow(this->GetDlgItem(IDC_STATIC_FRAME)->GetSafeHwnd());	// Initialize VTK window
		this->ResizeVtkWindow();
	}

	// Menu 로드
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

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CvtkMFCDlgExDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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
	// vtk Render Window 생성
	if (m_vtkWindow == NULL)
	{
		// Interactor 생성
		vtkSmartPointer<vtkRenderWindowInteractor> interactor =
			vtkSmartPointer<vtkRenderWindowInteractor>::New();
		// Trackball Camera 인터랙션 스타일 적용
		interactor->SetInteractorStyle(
			vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New());

		// Renderer 생성
		vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
		renderer->SetBackground(0.0, 0.0, 0.0);		// 검은색 배경

		// RenderWindow 생성 후 Dialog 핸들, Renderer 설정
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
		renderer->GetActiveCamera();		// renderer에서 카메라 받아오기
	cam->SetClippingRange( 0.1, 10 );	// 그려질 depth 영역 설정
	cam->SetFocalPoint( 0, 0, 0 );			// 카메라가 바라보는 지점
	cam->SetViewUp( 0, 1, 0 );				// 카메라의 upvector 설정
	cam->SetPosition( 0, 0, 5 );			// 카메라 위치 설정
	//cam->ParallelProjectionOn();			// on: orthogonal view / off: perspective view

	vtkSmartPointer<vtkLight> newLight = vtkSmartPointer<vtkLight>::New();
	newLight->SetColor( 1, 1, 0 );							// 조명색 설정 (노랑)
	newLight->SetFocalPoint( cam->GetFocalPoint() );	// 비추는 지점
	newLight->SetPosition( cam->GetPosition() );		// 조명 위치 설정
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
	pPolyData->SetPoints(pPoints);	// 위치 정보
	pPolyData->SetPolys(pPolys);		// 형태 정보

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
	pArrow->SetShaftRadius( 0.03 );			// 파라미터 설정 (Option)
	pArrow->SetShaftResolution( 100 );
	pArrow->SetTipRadius( 0.1 );
	pArrow->SetTipLength( 0.35 );
	pArrow->SetTipResolution( 100 );
	pArrow->Update();

	vtkSmartPointer<vtkPolyData> pPolyData = 
		pArrow->GetOutput();	// vtkPolyData 받아오기

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
	pSTLReader->SetFileName("../data/example.stl");	// 읽을 파일 지정
	pSTLReader->Update();

	vtkSmartPointer<vtkPolyData> pPolyData = 
		pSTLReader->GetOutput();				// vtkPolyData 형식으로 받아오기
	
	vtkSmartPointer<vtkSTLWriter> pSTLWriter = 
		vtkSmartPointer<vtkSTLWriter>::New();
	pSTLWriter->SetInputData(pPolyData);				// 저장할 vtkPolyData
	pSTLWriter->SetFileName("../data/example1.stl");	// 저장할 파일 지정
	pSTLWriter->Write();									// 저장 하기

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
		vtkSmartPointer<vtkArrowSource>::New();				// 화살표 source

	vtkSmartPointer<vtkPolyDataMapper> mapper = 
		vtkSmartPointer<vtkPolyDataMapper>::New();			// PolyData mapper 생성
	mapper->SetInputConnection(arrow->GetOutputPort());  // Mapper에 PolyData 연결

	vtkSmartPointer<vtkActor> actor = 
		vtkSmartPointer<vtkActor>::New();						// Actor 생성
	actor->SetMapper(mapper);									// Actor에 Mapper 연결

	actor->GetProperty()->SetColor( 0, 1, 0 );	// 색상 설정
	actor->GetProperty()->SetOpacity( 0.5 );		// 불투명도 설정 0.0:투명 ~ 1.0:불투명
	actor->GetProperty()->SetPointSize( 1.0 );	// Vertex 사이즈 설정
	actor->GetProperty()->SetLineWidth( 1.0 );	// Line 두께 설정

	// VTK_POINTS, VTK_WIREFRAME, VTK_SURFACE
	actor->GetProperty()->SetRepresentation(VTK_SURFACE); // 그리기 방법 설정
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
	pPoints->InsertPoint( 4, 0.0, 1.0, 0.0 );	// 1번 점과 중복
	pPoints->InsertPoint( 5, 1.0, 0.0, 0.0 );	// 2번 점과 중복

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
	pPolyData->SetPoints( pPoints );			// 위치 정보
	pPolyData->SetPolys( pPolys );			// 형태 정보

	int nPt = pPolyData->GetNumberOfPoints();		// nPt = 6
	int nPoly = pPolyData->GetNumberOfPolys();		// nPoly = 2

	vtkSmartPointer<vtkCleanPolyData> pClean = 
		vtkSmartPointer<vtkCleanPolyData>::New();
	pClean->SetInputData( pPolyData );
	pClean->Update();

	pPolyData->DeepCopy( pClean->GetOutput() );	// vtkPolyData 복사하기
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
	// STL 파일 읽어오기
	vtkSmartPointer<vtkSTLReader> stlReader =
		vtkSmartPointer<vtkSTLReader>::New();
	stlReader->SetFileName( "../data/example.stl" );
	stlReader->Update();

	//////////////////////////////////////////////////////////////////////////
	// Filter 처리 vtkPolyData →
	// Normal vector 계산
	vtkSmartPointer<vtkPolyDataNormals> normals =
		vtkSmartPointer<vtkPolyDataNormals>::New();
	normals->SetInputData( stlReader->GetOutput() );
	normals->ComputePointNormalsOn();			// Point normal 계산
	normals->ComputeCellNormalsOn();				// Cell normal 계산
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
	// STL 파일 읽어오기
	vtkSmartPointer<vtkSTLReader> stlReader =
		vtkSmartPointer<vtkSTLReader>::New();
	stlReader->SetFileName( "../data/example.stl" );
	stlReader->Update();

	//////////////////////////////////////////////////////////////////////////
	// Filter 처리 vtkPolyData(dental.stl) →
	// 1) vtkDecimationPro
	vtkSmartPointer<vtkDecimatePro> decimatePro =
		vtkSmartPointer<vtkDecimatePro>::New();
	decimatePro->SetInputConnection( stlReader->GetOutputPort() );
	decimatePro->SetTargetReduction( 0.9 ); // 전제 mesh 10% 감소
	decimatePro->PreserveTopologyOn();
	decimatePro->Update();

	// 2) vtkQuadricClustering
	vtkSmartPointer<vtkQuadricClustering> qClustering =
		vtkSmartPointer<vtkQuadricClustering>::New();
	qClustering->SetInputConnection( stlReader->GetOutputPort() );
	qClustering->SetNumberOfDivisions( 10, 10, 10 );  // 분할 개수 설정 (생략가능)
	qClustering->Update();

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	// 두 가지 결과 중 하나 선택하여 렌더링
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
	// STL 파일 읽어오기
	vtkSmartPointer<vtkSTLReader> stlReader =
		vtkSmartPointer<vtkSTLReader>::New();
	stlReader->SetFileName( "../data/example.stl" );
	stlReader->Update();

	//////////////////////////////////////////////////////////////////////////
	// Filter 처리 vtkPolyData(dental.stl) →
	// Smoothing
	vtkSmartPointer<vtkWindowedSincPolyDataFilter> smoothFilter =
		vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New();
	smoothFilter->SetInputConnection( stlReader->GetOutputPort() );
	smoothFilter->SetNumberOfIterations( 100 );     //반복연산 횟수
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
	// STL 파일 읽어오기
	vtkSmartPointer<vtkSTLReader> stlReader =
		vtkSmartPointer<vtkSTLReader>::New();
	stlReader->SetFileName( "../data/example_connectivity.stl" );
	stlReader->Update();

	//////////////////////////////////////////////////////////////////////////
	// Filter 처리 vtkPolyData
	vtkSmartPointer<vtkPolyDataConnectivityFilter> conFilter = 
		vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	conFilter->SetInputConnection( stlReader->GetOutputPort() );

	// 1) 모든 영역 추출
	conFilter->SetExtractionModeToAllRegions();

	// 2) 가장 큰 영역 추출
//	conFilter->SetExtractionModeToLargestRegion();

	// 3) seed로 연결된 영역 추출
// 	conFilter->AddSeed(id);
// 	conFilter->SetExtractionModeToCellSeededRegions();     // 3-1) 특정 cell seed
// 	conFilter->SetExtractionModeToPointSeededRegions();    // 3-2) 특정 point seed

	// 4) 특정 point와 가까운 점과 연결된 영역 추출
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

	// 1초 기다림
	Sleep( 1000 );
	
	// 2) 가장 큰 영역 추출
	conFilter->SetExtractionModeToLargestRegion();
	conFilter->Update();
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExClipping()
{
	// STL 파일 읽어오기
	vtkSmartPointer<vtkSTLReader> stlReader = 
		vtkSmartPointer<vtkSTLReader>::New();
	stlReader->SetFileName("../data/example.stl");
	stlReader->Update();

	//////////////////////////////////////////////////////////////////////////
	// Filter 처리 vtkPolyData
	double center[3];
	stlReader->GetOutput()->GetCenter(center); // mesh data의 중심

	vtkSmartPointer<vtkPlane> plane = 
		vtkSmartPointer<vtkPlane>::New();	// clipping plane 생성
	plane->SetOrigin(center);					// 원점 설정
	plane->SetNormal(1.0, 0.0, 0.0);			// normal vector 설정

	vtkSmartPointer<vtkClipPolyData> clipper = 
		vtkSmartPointer<vtkClipPolyData>::New();
	clipper->SetInputConnection(stlReader->GetOutputPort());	// clip 대상 mesh 설정
	clipper->SetClipFunction(plane);									// clip 평면 설정
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
	// STL 파일 읽어오기
	vtkSmartPointer<vtkSTLReader> stlReader =
		vtkSmartPointer<vtkSTLReader>::New();
	stlReader->SetFileName( "../data/example.stl" );
	stlReader->Update();

	//////////////////////////////////////////////////////////////////////////
	// Filter 처리 vtkPolyData
	vtkSmartPointer<vtkTransform> pTransform =
		vtkSmartPointer<vtkTransform>::New();
	// 1) 변환 직접 설정
	pTransform->Translate( 10.0, 0.0, 0.0 );       // x축으로 10.0 이동
	pTransform->RotateWXYZ( 30, 0.0, 1.0, 0.0 );   // y축 중심으로 30deg 회전

	// 2) 변환 matrix 적용
	//pTransform->SetMatrix(mat);              // vtkMatrix4x4로 설정 가능

	vtkSmartPointer<vtkTransformPolyDataFilter> pTransformFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	pTransformFilter->SetInputConnection( stlReader->GetOutputPort() );  // 대상 mesh 설정
	pTransformFilter->SetTransform( pTransform );  // 변환 transform 설정
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
	// 삼각형 1 생성
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

	// 삼각형 2 생성
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

	// 렌더링
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

	// 1초 기다림
	Sleep( 1000 );

	// Landmark 정합 수행
	vtkSmartPointer<vtkLandmarkTransform> lmt =
		vtkSmartPointer<vtkLandmarkTransform>::New();
	lmt->SetSourceLandmarks( points1 );	// sorce데이터 설정(vtkPoints)
	lmt->SetTargetLandmarks( points2 );		// target데이터 설정(vtkPoints)
	lmt->SetModeToRigidBody();				// 강체 변환
	lmt->Update();

	actor1->SetUserTransform( lmt );
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExRegistrationICP()
{
	// STL 파일 읽어오기
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

	// 1초 기다림
	Sleep( 1000 );

	// ICP 정합
	vtkSmartPointer<vtkIterativeClosestPointTransform> ICP =
		vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
	ICP->SetSource( stlReader1->GetOutput() );			// sorce데이터 설정 (vtkPolydata)
	ICP->SetTarget( stlReader2->GetOutput() );			// target데이터 설정(vtkPolydata)
	ICP->GetLandmarkTransform()->SetModeToRigidBody();// 강체 변환
	ICP->SetMaximumNumberOfIterations( 100 );		// 최대 100번 반복루프 (default:50번)
	ICP->SetMaximumNumberOfLandmarks( 50 );		// 매칭 점 개수 설정 (default:200개)
	ICP->Update();

	actor1->SetUserTransform( ICP );		// 변환 transform 설정
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExVtkimagedata()
{
	vtkSmartPointer<vtkDICOMImageReader> dcmReader = 
		vtkSmartPointer<vtkDICOMImageReader>::New();
	// 1) file 한 개 불러오기 (2차원 vtkImageData)
	dcmReader->SetFileName("../data/CT/CT.00002.00020.dcm");
	// 2) folder에 있는 파일 모두 불러오기 (3차원 vtkImageData)
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
	// folder에 있는 파일 모두 불러오기 (3차원 vtkImageData)
	dcmReader->SetDirectoryName( "../data/CT" );
	dcmReader->Update();

	//////////////////////////////////////////////////////////////////////////
	// Marching cube filter	(vtkImageData → vtkPolyData)
	vtkSmartPointer<vtkMarchingCubes> pMarchingCube =
		vtkSmartPointer<vtkMarchingCubes>::New();
	pMarchingCube->SetInputConnection( dcmReader->GetOutputPort() );
	pMarchingCube->SetValue( 0, 330 );			// 첫번째 iso-value 설정 (id,value)
	pMarchingCube->ComputeScalarsOff();
	pMarchingCube->ComputeNormalsOn();	// Normal을 계산
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
	// CT 영상 불러오기
	vtkSmartPointer<vtkDICOMImageReader> dcmReader =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	dcmReader->SetDirectoryName( "../data/CT" );
	dcmReader->Update();

	// Rendering Opacity 값 설정
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

	// Rendering Color 값 설정
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

	// Smart Volume Mapper 사용
	vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper =
		vtkSmartPointer<vtkSmartVolumeMapper>::New();
	volumeMapper->SetInputConnection( dcmReader->GetOutputPort() );
	volumeMapper->SetBlendModeToComposite();    // 블렌딩 모드 설정 (composite first)

	// Volume 그리기 속성 설정
	vtkSmartPointer<vtkVolumeProperty> volumeProperty =
		vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationType( VTK_LINEAR_INTERPOLATION );
	volumeProperty->SetColor( color );
	volumeProperty->SetScalarOpacity( compositeOpacity ); // composite first.

	// vtkVolume은 Volume rendering을 위한 Actor역할을 한다
	vtkSmartPointer<vtkVolume> volume =
		vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper( volumeMapper );
	volume->SetProperty( volumeProperty );
	
	//////////////////////////////////////////////////////////////////////////
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	// vtkVolume은 AddActor가 아닌 AddViewProp 함수로 추가
	renderer->AddViewProp( volume );		
	renderer->SetBackground( .1, .2, .3 );
	renderer->ResetCamera();

	//rendering
	m_vtkWindow->AddRenderer( renderer );
	m_vtkWindow->Render();
}


void CvtkMFCDlgExDlg::OnButtonExVolumeClipping()
{
	// CT 영상 불러오기
	vtkSmartPointer<vtkDICOMImageReader> dcmReader =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	dcmReader->SetDirectoryName( "../data/CT" );
	dcmReader->Update();

	//////////////////////////////////////////////////////////////////////////
	// 1) 새로운 볼륨으로 추출해내기
	int ext[6];
	dcmReader->GetOutput()->GetExtent( ext );  // volume의 extent 받아오기

	// VOI 설정 (x축방향 반으로 절단하기)
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
	// 2) 렌더링만 clipping하기
	double center[3];
	dcmReader->GetOutput()->GetCenter( center ); // volume data의 중심
	vtkSmartPointer<vtkPlane> plane =
		vtkSmartPointer<vtkPlane>::New();	// clipping plane 생성
	plane->SetOrigin( center );				// 원점 설정
	plane->SetNormal( -1.0, 1.0, -1.0 );		// normal vector 설정

	vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper2 =
		vtkSmartPointer<vtkSmartVolumeMapper>::New();
	volumeMapper2->SetInputConnection( dcmReader->GetOutputPort() );
	volumeMapper2->SetBlendModeToComposite();      // composite first
	volumeMapper2->AddClippingPlane( plane );

	// Rendering Opacity 값 설정
	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =
		vtkSmartPointer<vtkPiecewiseFunction>::New();
	// 1) CT MUSCLE
	compositeOpacity->AddPoint( -3024, 0 );
	compositeOpacity->AddPoint( -155.41, 0 );
	compositeOpacity->AddPoint( 217.64, 0.68 );
	compositeOpacity->AddPoint( 419.74, 0.83 );
	compositeOpacity->AddPoint( 3071, 0.80 );

	// Rendering Color 값 설정
	vtkSmartPointer<vtkColorTransferFunction> color =
		vtkSmartPointer<vtkColorTransferFunction>::New();
	// 1) CT MUSCLE
	color->AddRGBPoint( -3024, 0, 0, 0 );
	color->AddRGBPoint( -155.41, .55, .25, .15 );
	color->AddRGBPoint( 217.64, .88, .60, .29 );
	color->AddRGBPoint( 419.74, 1, .94, .95 );
	color->AddRGBPoint( 3071, .83, .66, 1 );

	// Volume 그리기 속성 설정
	vtkSmartPointer<vtkVolumeProperty> volumeProperty =
		vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationType( VTK_LINEAR_INTERPOLATION );
	volumeProperty->SetColor( color );
	volumeProperty->SetScalarOpacity( compositeOpacity ); // composite first.

	// vtkVolume은 Volume rendering을 위한 Actor역할을 한다
	vtkSmartPointer<vtkVolume> volume =
		vtkSmartPointer<vtkVolume>::New();
	volume->SetProperty( volumeProperty );
	// 두 가지 방법 중 하나를 선택하여 렌더링
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

//  1.	Call back 함수 생성
void KeyPressCallbackFunc( vtkObject* caller, long unsigned int eventId, 
	void* clientData, void* callData )
{
	MessageBox( NULL, _T( "Key Press Event" ), _T( "Pop-up" ), MB_OK );
}

void CvtkMFCDlgExDlg::OnButtonExInteractor()
{
	// 콘 생성 및 렌더링
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
	// 2.	vktCallbackCommand에 생성된 함수 연결
	vtkSmartPointer<vtkCallbackCommand> keypressCallback =
		vtkSmartPointer<vtkCallbackCommand>::New();
	keypressCallback->SetCallback( KeyPressCallbackFunc );

	//////////////////////////////////////////////////////////////////////////
	// 3.	Interactor 에 observer 추가
	// AddObserve ( vtkCommand::이벤트, vtkCallbackCommand-실행 될 함수 )
	m_vtkWindow->GetInteractor()->AddObserver( vtkCommand::KeyPressEvent, keypressCallback );
}


//  1.	Call back 함수 생성
void PickCallbackFunction( vtkObject* caller, long unsigned int eventId,
	void* clientData, void* callData )
{
	// Interactor 가져오기
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkRenderWindowInteractor::SafeDownCast( caller );
	if( interactor == NULL ) return;

	// 마우스 클릭 위치
	int pos[2];
	interactor->GetLastEventPosition( pos );

	// 마우스 클릭 위치에서 Picking 수행
	vtkSmartPointer<vtkCellPicker> picker =
		vtkSmartPointer<vtkCellPicker>::New();
	picker->SetTolerance( 0.005 );		// picking 감도 설정
	picker->Pick( pos[0], pos[1], 0,
		interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer() );

	vtkIdType cellId = picker->GetCellId();	// -1이면 picking 되지 않음
	if( cellId != -1 ) MessageBox( NULL, _T( "Pick Event" ), _T( "Pop-up" ), MB_OK );
}

void CvtkMFCDlgExDlg::OnButtonExPicking()
{
	// 콘 생성 및 렌더링
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
	//	2.	vktCallbackCommand에 생성된 함수 연결
	vtkSmartPointer<vtkCallbackCommand> pickCallback =
		vtkSmartPointer<vtkCallbackCommand>::New();
	pickCallback->SetCallback( PickCallbackFunction );

	//////////////////////////////////////////////////////////////////////////
	// 3.	Interactor 에 observer 추가
	m_vtkWindow->GetInteractor()->
		AddObserver( vtkCommand::LeftButtonPressEvent, pickCallback );
}


void CvtkMFCDlgExDlg::OnButtonExWidget()
{
	// 콘 생성 및 렌더링
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
	
	// 아래의 각 Widget 설정 부분을 주석처리하고
	//   한 가지 씩 주석을 해제하여 확인해 볼 수 있다.
	//////////////////////////////////////////////////////////////////////////
	// 1) Angle Widget 설정
	m_angleWidget = vtkSmartPointer<vtkAngleWidget>::New();
	m_angleWidget->SetInteractor( m_vtkWindow->GetInteractor() );
	m_angleWidget->CreateDefaultRepresentation();
	m_angleWidget->On();
	
	//////////////////////////////////////////////////////////////////////////
	// 2) vtkImagePlaneWidget 설정 (볼륨의 단면)
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
	// 3) vtkCaptionWidget 텍스트 설정하여 생성하기
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
	// 4) vtkOrientationMarkerWidget 설정하여 생성하기
	vtkSmartPointer<vtkAnnotatedCubeActor> cube =
		vtkSmartPointer<vtkAnnotatedCubeActor>::New();

	m_orientationWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	m_orientationWidget->SetOrientationMarker( cube );
	m_orientationWidget->SetInteractor( m_vtkWindow->GetInteractor() );
	m_orientationWidget->SetViewport( 0.0, 0.0, 0.2, 0.2 );
	m_orientationWidget->SetEnabled( TRUE );
	m_orientationWidget->On();
	
	//////////////////////////////////////////////////////////////////////////
	// 5) vtkTextWidget 설정하여 생성하기
	vtkSmartPointer<vtkTextActor> textActor =
		vtkSmartPointer<vtkTextActor>::New();
	textActor->SetInput( "Text" );

	m_textWidget = vtkSmartPointer<vtkTextWidget>::New();
	m_textWidget->SetInteractor( m_vtkWindow->GetInteractor() );
	m_textWidget->SetTextActor( textActor );
	m_textWidget->On();
}

