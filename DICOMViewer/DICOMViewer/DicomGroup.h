#pragma once

#include <vtkObject.h>

#include <vector>
#include <string>

class DicomGroup :
	public vtkObject
{
public:
	/// vtkSmartPointer를 통한 생성 및 해제
	vtkTypeMacro( DicomGroup, vtkObject );
	static DicomGroup *New() { return new DicomGroup; };

protected:
	/// New() 함수 이외의 생성 방지
	DicomGroup();
	virtual ~DicomGroup();
	
protected:
	/// 환자 정보
	std::string PatientID;
	std::string PatientName;
	std::string PatientBirthDate;
	std::string PatientSex;
	std::string PatientAge;
	std::string PatientWeight;

	// Study 정보
	std::string StudyID;
	std::string StudyDescription;

	// Series 정보
	std::string SeriesNum;
	std::string SeriesDescription;

	/// DICOM 파일 목록
	std::vector<std::string> m_FileList;

public:
	vtkGetMacro( PatientID, std::string );
	vtkSetMacro( PatientID, std::string );
	
	vtkGetMacro( StudyID, std::string );
	vtkSetMacro( StudyID, std::string );
	
	vtkGetMacro( SeriesNum, std::string );
	vtkSetMacro( SeriesNum, std::string );

	vtkGetMacro( PatientName, std::string );
	vtkGetMacro( PatientBirthDate, std::string );
	vtkGetMacro( PatientSex, std::string );
	vtkGetMacro( PatientAge, std::string );
	vtkGetMacro( PatientWeight, std::string );
	vtkGetMacro( StudyDescription, std::string );
	vtkGetMacro( SeriesDescription, std::string );

	/// PatientID, StudyID, SeriesNum을 비교하여 동일한 그룹인지 확인
	bool IsSameGroup( std::string patientID, std::string studyID, std::string seriesNum ) {
		if( PatientID.compare( patientID ) != 0 ) return false;
		if( StudyID.compare( studyID ) != 0 ) return false;
		if( SeriesNum.compare( seriesNum ) != 0 ) return false;

		return true;
	}

	/// DICOM 파일 목록
	std::vector<std::string> GetFileList() const { return m_FileList; }

	/// 이 그룹에 DICOM 파일 추가
	void AddImageFile( const char *filePath );

	/// 추가 DICOM 태그 정보 읽기
	void LoadDicomInfo();
};

