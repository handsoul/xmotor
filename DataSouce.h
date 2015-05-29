// ---------------------------------------------------------------------------

#ifndef DataSouceH
#define DataSouceH

#include <vcl.h>
#include <vector>

#define ITEMS_OF(array) (sizeof(array)/sizeof(array[0]))




#pragma pack (push,1)

typedef struct tagParams
{
	double HighVoltRange[2]; 	// 高电平范围.
	double LowVoltRange[2];  	// 低电平范围.
	double DutyRange[2];     	// 占空比范围.
	double RSpeedRange[3][2];   // 转速范围.3个模式下的转速范围.
	double VoltLimit[2];		// 高电平门限.低电平门限.
}Params;

#define MAX_CONFIG_FILE_SIZE 	(100*1024)

//定义一个配置数据.
typedef struct tagConfigFileOjb
{
	unsigned char 	_gap[1024];		// 空白区.
	unsigned char 	MagicCode[128]; // 数据.
    wchar_t         UserName[128];
	wchar_t         Password[128];  // 密码存放区.
	union
	{
 		Params Params;
		double _data[128];
	};

}ConfigOjb;

#pragma pack(pop)

bool CreateApplicationFile(ConfigOjb ** ppojb);
inline void SyncToMappdFile(void);
void CloseMappedFile(void);

class BaseDataItem
{
protected:
	const UnicodeString m_sName;
	const int m_iIdx;
	double m_fMax;
	double m_fMin;
	double m_fAvg;

public:
	const static char SpaceChar = ',';

	inline double GetMax(void) const
	{
		return m_fMax;
	}

	inline double GetMin(void) const
	{
		return m_fMin;
	}

	inline double GetAvg(void) const
	{
		return m_fAvg;
	}

	inline UnicodeString GetName(void) const
	{
		return UnicodeString(m_sName);
	}

	inline void SetMax(double val)
	{
		m_fMax = val;
	}

	inline void SetMin(double val)
	{
		m_fMin = val;
	}

	inline void SetAvg(double val)
	{
		m_fAvg = val;
	}

	inline void SetVal(double max, double min)
	{
		m_fMax = max;
		m_fMin = min;
	};

	inline void SetVal(double max,double avg, double min)
	{
		m_fMax = max;
		m_fMin = min;
		m_fAvg = avg;
	};


	BaseDataItem(const UnicodeString _sName, const int Idx, double _max,
		double _min) : m_sName(_sName), m_fMax(_max), m_fMin(_min), m_iIdx(Idx)
	{
	}

	BaseDataItem(const UnicodeString _sName, const int Idx, double _val)
		: m_sName(_sName), m_fMax(_val), m_fMin(_val), m_iIdx(Idx)
	{
	}

	// 数据转换为字符串.
	inline UnicodeString ValueToString(void)
	{
		return FloatToStrF(m_fMin, ffFixed, 10, 3) + "," + FloatToStrF(m_fMin,
			ffFixed, 10, 3);
	}

	// 获得名称字符串.
	inline UnicodeString NameString(void)
	{
		return m_sName + IntToStr(m_iIdx) + "Max" + SpaceChar + m_sName +
			IntToStr(m_iIdx) + "Min";
	}
};

// 频率
class Frequence : public BaseDataItem
{
public:
	Frequence(const int _idx, double _max, double _min)
		: BaseDataItem("转速", _idx, _max, _min)
	{
	}

	Frequence(const int _idx, double val) : BaseDataItem("转速", _idx, val, val)
	{
	}

};

// 占空比.
class Duty : public BaseDataItem
{
public:
	Duty(const int _idx, double _max, double _min)
		: BaseDataItem("占空比", _idx, _max, _min)
	{
	}

	Duty(const int _idx, double val) : BaseDataItem("占空比", _idx, val, val)
	{
	}
};

// 低电平电压.
class LowLevelVoltage : public BaseDataItem
{
public:
	LowLevelVoltage(const int _idx, double _max, double _min)
		: BaseDataItem("低电平电压", _idx, _max, _min)
	{
	}

	LowLevelVoltage(const int _idx, double val)
		: BaseDataItem("低电平电压", _idx, val, val)
	{
	}
};

// 高电平电压.
class HighLevelVoltage : public BaseDataItem
{
public:
	HighLevelVoltage(const int _idx, double _max, double _min)
		: BaseDataItem("高电平电压", _idx, _max, _min)
	{
	}

	HighLevelVoltage(const int _idx, double val)
		: BaseDataItem("高电平电压", _idx, val, val)
	{
	}
};

// 上升沿时间.
class RisingEdgeTime : public BaseDataItem
{
public:
	RisingEdgeTime(const int _idx, double _max, double _min)
		: BaseDataItem("上升沿时间", _idx, _max, _min)
	{
	}

	RisingEdgeTime(const int _idx, double val)
		: BaseDataItem("上升沿时间", _idx, val, val)
	{
	}

};

// 下降沿时间.
class FallingEdgeTime : public BaseDataItem
{
public:
	FallingEdgeTime(const int _idx, double _max, double _min)
		: BaseDataItem("下降沿时间", _idx, _max, _min)
	{
	}

	FallingEdgeTime(const int _idx, double val)
		: BaseDataItem("下降沿时间", _idx, val, val)
	{
	}
};

class DataItemSet
{
	// 有固定的顺序.
protected:
	std::vector<BaseDataItem*>m_vItems;
	int m_iIndex;
	bool m_bUpdated;

public:

	inline bool GetUpdatedFlag(void) const {return m_bUpdated;}
    inline void ResetUpdatedFlag(void) {m_bUpdated = false;}

	DataItemSet(int _idx, double fMax, double fMin, double dMax, double dMin,
		double LvMax, double LvMin, double HvMax, double HvMin, double RtMax,
		double RtMin, double FtMax, double FtMin) : m_iIndex(_idx)
	{
		// 禁止8030警告.
#pragma warn - 8030
		m_vItems.push_back(static_cast<BaseDataItem*>
			(new Frequence(m_iIndex, 0)));
		m_vItems.push_back(static_cast<BaseDataItem*>(new Duty(m_iIndex, 0)));
		m_vItems.push_back(static_cast<BaseDataItem*>
			(new LowLevelVoltage(m_iIndex, 0)));
		m_vItems.push_back(static_cast<BaseDataItem*>
			(new HighLevelVoltage(m_iIndex, 0)));
		m_vItems.push_back(static_cast<BaseDataItem*>
			(new RisingEdgeTime(m_iIndex, 0)));
		m_vItems.push_back(static_cast<BaseDataItem*>
			(new FallingEdgeTime(m_iIndex, 0)));
#pragma warn . 8030
		SetAllValue(fMax, fMin, dMax, dMin, LvMax, LvMin, HvMax, HvMin, RtMax,
			RtMin, FtMax, FtMin);
	}


	bool UpdateAllData(std::vector<double> &v)
	{
		if(v.size() < m_vItems.size()*3)
		{
			return false;
		}

		for(int i = 0; i < (int)m_vItems.size();i++)
		{
			m_vItems[i]->SetVal(v[i*3],v[i*3+1],v[i*3+2]);
		}

		m_bUpdated = true;

		return true;
	}

	DataItemSet(int idx) : m_iIndex(idx)
	{
		// 禁止8030警告.
#pragma warn - 8030
		m_vItems.push_back(static_cast<BaseDataItem*>
			(new Frequence(m_iIndex, 0)));
		m_vItems.push_back(static_cast<BaseDataItem*>(new Duty(m_iIndex, 0)));
		m_vItems.push_back(static_cast<BaseDataItem*>
			(new LowLevelVoltage(m_iIndex, 0)));
		m_vItems.push_back(static_cast<BaseDataItem*>
			(new HighLevelVoltage(m_iIndex, 0)));
		m_vItems.push_back(static_cast<BaseDataItem*>
			(new RisingEdgeTime(m_iIndex, 0)));
		m_vItems.push_back(static_cast<BaseDataItem*>
			(new FallingEdgeTime(m_iIndex, 0)));
#pragma warn . 8030
	}

	~DataItemSet()
	{
		std::vector<BaseDataItem*>::const_iterator it;
		for (it = m_vItems.begin(); it != m_vItems.end(); ++it)
		{
		   delete(*it);
		}
	}

	void SetIndex(int _idx)
	{
		m_iIndex = _idx;
	}

	int GetIndex(void)
	{
		return m_iIndex;
	}

	void SetFreqVal(double max = 1, double min = 0,double avg=0)
	{
		m_vItems[0]->SetVal(max, avg ,min);
	}

	void SetDutyVal(double max = 1, double min = 0 , double avg=0)
	{
		m_vItems[1]->SetVal(max, avg ,min);
	}

	void SetLVVal(double max = 0.8, double min = 0.8, double avg=0)
	{
		m_vItems[2]->SetVal(max, avg ,min);
	}

	void SetHVVal(double max = 1.6, double min = 1.6, double avg=0)
	{
		m_vItems[3]->SetVal(max, avg ,min);
	}

	void SetRTVal(double max = 1, double min = 0,double avg=0)
	{
		m_vItems[4]->SetVal(max, avg ,min);
	}

	void SetFTVal(double max = 0, double min = 0,double avg=0)
	{
		m_vItems[5]->SetVal(max, avg ,min);
	}

	void SetAllValue(double fMax, double fMin, double dMax, double dMin,
		double LvMax, double LvMin, double HvMax, double HvMin, double RtMax,
		double RtMin, double FtMax, double FtMin)
	{
		SetFreqVal(fMax, fMin);
		SetDutyVal(dMax, dMin);
		SetLVVal(LvMax, LvMin);
		SetHVVal(HvMax, HvMin);
		SetRTVal(RtMax, RtMin);
		SetFTVal(FtMax, FtMin);
	}

	BaseDataItem & operator[](int nIndex) const
	{
		if (nIndex < 0 || nIndex >= static_cast<int>(m_vItems.size()))
		{
			std::out_of_range e("indexer out of range");
			throw e;
		}

		return *(const_cast<BaseDataItem*>(m_vItems[nIndex]));
	}

	// #define MIN_FIRST
	void NameGroupToString(UnicodeString & s, bool DeleteLastSpaceChar = false)
	{
		std::vector<BaseDataItem*>::const_iterator it;

		if (DeleteLastSpaceChar == true)
		{
			s = "";
		}

		for (it = m_vItems.begin(); it != m_vItems.end(); ++it)
		{
#ifndef MIN_FIRST
			s += (*it)->GetName() + "模式" + IntToStr(m_iIndex+1) + "最大" + ",";
			s += (*it)->GetName() + "模式" + IntToStr(m_iIndex+1) + "最小" + ",";
#else
			s += (*it)->GetName() + "模式" + IntToStr(m_iIndex+1) + "最小" + ",";
			s += (*it)->GetName() + "模式" + IntToStr(m_iIndex+1) + "最大" + ",";
#endif
		}

		if (DeleteLastSpaceChar)
		{
			s.Delete(s.Length(), 1); // 删除最后一个","
		}
	}

	// 返回名称列表.
	void NameGroupToVector(std::vector<UnicodeString*> & v,
		bool AppendMode = true)
	{
		if (AppendMode == false)
		{
			v.clear();
		}

		std::vector<BaseDataItem*>::const_iterator it;
		UnicodeString *pStr;
		for (it = m_vItems.begin(); it != m_vItems.end(); ++it)
		{
#ifndef MIN_FIRST
			pStr = new UnicodeString((*it)->GetName() + "模式" + IntToStr(m_iIndex+1)
				+ "最大");
			v.push_back(pStr);
			pStr = new UnicodeString((*it)->GetName()  + "模式"+ IntToStr(m_iIndex+1)
				+ "最小");
			v.push_back(pStr);
#else
			pStr = new UnicodeString((*it)->GetName()  + "模式"+ IntToStr(m_iIndex+1)
				+ "最小");
			v.push_back(pStr);
			pStr = new UnicodeString((*it)->GetName()  + "模式"+ IntToStr(m_iIndex+1)
				+ "最大");
			v.push_back(pStr);
#endif
		}
	}

	void ValueGroupToString(UnicodeString &s, bool DeleteLastSpaceChar = false)
	{
		std::vector<BaseDataItem*>::const_iterator it;
		for (it = m_vItems.begin(); it != m_vItems.end(); ++it)
		{
#ifndef MIN_FIRST
			s += FloatToStrF((*it)->GetMax(), ffGeneral, 10, 3) + ",";
			s += FloatToStrF((*it)->GetMin(), ffGeneral, 10, 3) + ",";
#else
			s += FloatToStrF((*it)->GetMin(), ffGeneral, 10, 3) + ",";
			s += FloatToStrF((*it)->GetMax(), ffGeneral, 10, 3) + ",";
#endif
		}
		/* if (DeleteLastSpaceChar)
		 {
		 s.Delete(s.Length(),1); // 删除最后一个","
		 } */
	}

};
#if 1

// 测试结果数据存储类.
class TestProcedure
{
	// 共3组数据.
protected:
	const static int TestGroupNum = 3; // 共3组测试.

	std::vector<DataItemSet*>m_vDataSet;

	UnicodeString m_sNameList;
	UnicodeString m_sValueList;

	std::vector<UnicodeString*>vColumnStrList;

	UnicodeString m_sSqlTableColumnList;
	UnicodeString m_sSqlCreateTable;

	UnicodeString m_sName;

	void InitGetNameList(void);
	void UpdateValueList(void);
	void SqlBuilderCreateTable(const UnicodeString & sTableName);

public:
	TestProcedure(void);
	~TestProcedure(void);
	DataItemSet & operator[](int nIndex) const ;
	int Length(void) const ;
	void SqlCreateTable(const UnicodeString & sTableName, UnicodeString &sql);
	void SqlInsertTableValue(const UnicodeString &sTableName,
		UnicodeString &sql);

	bool DataUpdated(void);
	void ResetUpdated(void);
};
#endif

bool CheckFilePath(UnicodeString &s);

#endif
