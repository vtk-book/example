#pragma once

#include <vtkObject.h>

#include <vector>
#include <string>

class DicomGroup :
	public vtkObject
{
public:
	/// vtkSmartPointer�� ���� ���� �� ����
	vtkTypeMacro( DicomGroup, vtkObject );
	static DicomGroup *New() { return new DicomGroup; };

protected:
	/// New() �Լ� �̿��� ���� ����
	DicomGroup();
	virtual ~DicomGroup();
	
protected:
	/// ȯ�� ����
	std::string PatientID;
	std::string PatientName;
	std::string PatientBirthDate;
	std::string PatientSex;
	std::string PatientAge;
	std::string PatientWeight;

	// Study ����
	std::string StudyID;
	std::string StudyDescription;

	// Series ����
	std::string SeriesNum;
	std::string SeriesDescription;

	/// DICOM ���� ���
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

	/// PatientID, StudyID, SeriesNum�� ���Ͽ� ������ �׷����� Ȯ��
	bool IsSameGroup( std::string patientID, std::string studyID, std::string seriesNum ) {
		if( PatientID.compare( patientID ) != 0 ) return false;
		if( StudyID.compare( studyID ) != 0 ) return false;
		if( SeriesNum.compare( seriesNum ) != 0 ) return false;

		return true;
	}

	/// DICOM ���� ���
	std::vector<std::string> GetFileList() const { return m_FileList; }

	/// �� �׷쿡 DICOM ���� �߰�
	void AddImageFile( const char *filePath );

	/// �߰� DICOM �±� ���� �б�
	void LoadDicomInfo();
};

