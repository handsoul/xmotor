#include "DataSouce.h"
#include <vcl.h>
#include <windows.h>

#include <Registry.hpp>
#include <memory>
using namespace std;

const Params DefaultParams =
{
	{	 1.1,	1.3		},
	{	0.65,	0.85	},
	{     45,   55		},
	{
		{	0.9,	1.0	},
		{	 39,	41	},
		{	1990,	2010},
	},
    {   1.18,   0.86    }
};

static HANDLE hFILE = INVALID_HANDLE_VALUE;
static HANDLE hFMAP = INVALID_HANDLE_VALUE;
static void * pMappedAddr = NULL;
bool CreateApplicationFile(ConfigOjb ** ppojb)
{
	HANDLE hFile = CreateFile(	L"MotorTester.dll",
								GENERIC_READ|GENERIC_WRITE,
								0,
								NULL,
								OPEN_ALWAYS,
								0,
								NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{
		return false;
	}

	HANDLE hFileMapping = CreateFileMapping(hFile,
											NULL,
											PAGE_READWRITE,
											0,
											MAX_CONFIG_FILE_SIZE,
											NULL);
	if( INVALID_HANDLE_VALUE == hFileMapping)
	{
		CloseHandle(hFile);
		return false;
	}

	void *p = MapViewOfFile(hFileMapping,FILE_MAP_READ|FILE_MAP_WRITE,0,0,MAX_CONFIG_FILE_SIZE);
	if(p == NULL)
	{
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
		return false;
	}

	ConfigOjb * pobj = (ConfigOjb*)p;

	//检查数据.
	int i = 0;
	for(;i < (int)sizeof(pobj->MagicCode);i++)
	{
		if(pobj->MagicCode[i] != (unsigned char)i)
		{
			break;
		}
	}

	if(i != (int)sizeof(pobj->MagicCode))
	{
		//需要重新初始化.
		for(; i < (int)sizeof(pobj->MagicCode);i++)
		{
			pobj->MagicCode[i] = (unsigned char)i;
		}
		//重新配置参数.
		pobj->Params = DefaultParams;

		// 拷贝名.

        //Todo:修改为Base64编码.
		UnicodeString Name = "Admini";
        UnicodeString Password = "000000";
		memcpy(pobj->UserName,Name.c_str(),sizeof(wchar_t)*Name.Length());
		memcpy(pobj->Password,Password.c_str(),sizeof(wchar_t)*Password.Length());

        SyncToMappdFile();
	}

	hFILE = hFile;
	hFMAP = hFileMapping;
    pMappedAddr = p;

	return true;
}


inline void SyncToMappdFile(void)
{
	if(pMappedAddr != NULL)
	{
		FlushViewOfFile(pMappedAddr,MAX_CONFIG_FILE_SIZE);
    }
}


void CloseMappedFile(void)
{
	if(pMappedAddr != NULL)
	{
		UnmapViewOfFile(pMappedAddr);
		CloseHandle(hFMAP);
		CloseHandle(hFILE);
	}
}

/*
 *  检查文件是否存在.
 */
bool CheckFilePath(UnicodeString &s)
{
	// 获取当前路径.
	UnicodeString sPath = GetCurrentDir();
	sPath += "\\";
	sPath += Date().FormatString("yyyy-mm-dd");
	sPath += ".mdb";
	s = sPath;

	// 检查文件是否存在.
	return FileExists(sPath);
}

// 获得名称列表.
void TestProcedure::InitGetNameList(void)
{
	std::vector<DataItemSet*>::iterator it;

	m_sNameList = "(";
	for (it = m_vDataSet.begin(); it != m_vDataSet.end(); ++it)
	{
		(*it)->NameGroupToString(m_sNameList);
	}
	m_sNameList.Delete(m_sNameList.Length(), 1);
	m_sNameList += ")";

	for (it = m_vDataSet.begin(); it != m_vDataSet.end(); ++it)
	{
		(*it)->NameGroupToVector(vColumnStrList);
	}
}

void TestProcedure::UpdateValueList(void)
{
	std::vector<DataItemSet*>::iterator it;
	m_sValueList = "(";
	for (it = m_vDataSet.begin(); it != m_vDataSet.end(); ++it)
	{
		(*it)->ValueGroupToString(m_sValueList);
	}

	m_sValueList.Delete(m_sValueList.Length(), 1);
	m_sValueList += ")";
}

void TestProcedure::SqlBuilderCreateTable(const UnicodeString & sTableName)
{
	std::vector<UnicodeString>v;
	std::vector<UnicodeString*>::const_iterator is;

	m_sSqlCreateTable = "create table";
	m_sSqlCreateTable += " " + sTableName + " ";
	m_sSqlCreateTable += "(id int IDENTITY(1,1) primary key,记录时间 datetime default now(),";

	for (is = vColumnStrList.begin(); is < vColumnStrList.end(); ++is)
	{
		m_sSqlCreateTable += *(*is) + " double,";
	}

	// 删除最后一个","
	m_sSqlCreateTable.Delete(m_sSqlCreateTable.Length(), 1);
	m_sSqlCreateTable += ")";
}

TestProcedure::TestProcedure(void)
{
	m_vDataSet = std::vector<DataItemSet*>();
	m_sNameList = "";
	m_sValueList = "";
	vColumnStrList = std::vector<UnicodeString*>();
	m_sSqlTableColumnList = "";
	m_sSqlCreateTable = "";

	for (int i = 0; i < TestGroupNum; ++i)
	{
		DataItemSet *pds = new DataItemSet(i);
		m_vDataSet.push_back(pds);
	}
	InitGetNameList(); // 初始化数据列表.
	// UpdateValueList(); // 初始化数据值表.
}

DataItemSet& TestProcedure:: operator[](int nIndex) const
{
	if (nIndex < 0 || nIndex >= static_cast<int>(m_vDataSet.size()))
	{
		std::out_of_range e("indexer out of range");
		throw e;
	}

	return *(const_cast<DataItemSet*>(m_vDataSet[nIndex]));
}

int TestProcedure::Length(void) const
{
	return (int)(m_vDataSet.size());
}

TestProcedure::~TestProcedure(void)
{
	std::vector<DataItemSet*>::iterator dit;
	std::vector<UnicodeString*>::iterator sit;

	for (dit = m_vDataSet.begin(); dit != m_vDataSet.end(); ++dit)
	{
		delete *dit;
	}

	for (sit = vColumnStrList.begin(); sit != vColumnStrList.end(); ++sit)
	{
		delete *sit;
	}

	m_vDataSet.clear();
	vColumnStrList.clear();
}

void TestProcedure::SqlCreateTable(const UnicodeString & sTableName,
	UnicodeString &sql)
{
	if (sTableName != m_sName)
	{
		SqlBuilderCreateTable(sTableName); // 创建列表的名称.
	}

	sql = UnicodeString(m_sSqlCreateTable);
}

void TestProcedure::SqlInsertTableValue(const UnicodeString &sTableName,
	UnicodeString &sql)
{

	UpdateValueList();

	sql = UnicodeString("insert into " + sTableName + " " + m_sNameList +
		" values " + m_sValueList);
}


bool TestProcedure::DataUpdated(void)
{
	std::vector<DataItemSet*>::iterator dit;

	for (dit = m_vDataSet.begin(); dit != m_vDataSet.end(); ++dit)
	{
		if((*dit)->GetUpdatedFlag() == false)
		{
			return false;
		}
	}

	return true;
}

void TestProcedure::ResetUpdated(void)
{

	std::vector<DataItemSet*>::iterator dit;

	for (dit = m_vDataSet.begin(); dit != m_vDataSet.end(); ++dit)
	{
		(*dit)->ResetUpdatedFlag();
	}
}
