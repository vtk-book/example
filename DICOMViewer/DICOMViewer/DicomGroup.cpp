#include "stdafx.h"
#include "DicomGroup.h"

#include <gdcmReader.h>
#include <gdcmFile.h>
#include <gdcmDataSet.h>
#include <gdcmStringFilter.h>
#include <gdcmTag.h>

DicomGroup::DicomGroup()
{
}


DicomGroup::~DicomGroup()
{
}

void DicomGroup::AddImageFile( const char *filePath )
{
	// ��ȿ�� ���� ������� �˻�
	if( filePath == NULL ) return;

	// ���� ��Ͽ� �߰�
	m_FileList.push_back( filePath );
}

void DicomGroup::LoadDicomInfo()
{
	if( m_FileList.size() == 0 ) return;
	
	// GDCM���� DICOM ���� �б�
	gdcm::Reader reader;
	reader.SetFileName( m_FileList[0].c_str() );
	if( !reader.Read() ) return;
	
	// DICOM ���Ͽ��� Tag ������ �о�� �غ�
	gdcm::File &file = reader.GetFile();
	gdcm::DataSet &ds = file.GetDataSet();
	gdcm::StringFilter sf;
	sf.SetFile( file );

	// DICOM �±� ���� �б�
	gdcm::Tag tagPatientName( 0x0010, 0x0010 );
	gdcm::Tag tagPatientBirthDate( 0x0010, 0x0030 );
	gdcm::Tag tagPatientSex( 0x0010, 0x0040 );
	gdcm::Tag tagPatientAge( 0x0010, 0x1010 );
	gdcm::Tag tagPatientWeight( 0x0010, 0x1030 );
	gdcm::Tag tagStudyDescription( 0x0008, 0x1030 );
	gdcm::Tag tagSeriesDescription( 0x0008, 0x103e );

	if( ds.FindDataElement( tagPatientName ) ) 
		PatientName = sf.ToString( tagPatientName );
	if( ds.FindDataElement( tagPatientBirthDate ) ) 
		PatientBirthDate = sf.ToString( tagPatientBirthDate );
	if( ds.FindDataElement( tagPatientSex ) ) 
		PatientSex = sf.ToString( tagPatientSex );
	if( ds.FindDataElement( tagPatientAge ) ) 
		PatientAge = sf.ToString( tagPatientAge );
	if( ds.FindDataElement( tagPatientWeight ) ) 
		PatientWeight = sf.ToString( tagPatientWeight );
	if( ds.FindDataElement( tagStudyDescription ) ) 
		StudyDescription = sf.ToString( tagStudyDescription );
	if( ds.FindDataElement( tagSeriesDescription ) ) 
		SeriesDescription = sf.ToString( tagSeriesDescription );
}
