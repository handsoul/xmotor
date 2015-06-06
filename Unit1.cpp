// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "FormAuth.h"
#include "SerialForm.h"
#include "DataSouce.h"
#include "ADOX_TLB.h"
#include "Unit4.h"
#include "ShowStatus.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ADOX_OCX"
#pragma link "frxCtrls"
#pragma resource "*.dfm"
TForm1 *Form1;

double StartPortion = 0;

bool bPassCfg[6][2] =
{
	{true,true},
	{true,true},
	{true,true},
	{true,true},
	{true,true},
	{true,true},
};


double TForm1::CalcMode(double period, double N)
{
	double t = period * N;//PaintBox1->Width * CommTimer->Interval / 1000.0;
    double st = PaintBox1->Width * CommTimer->Interval / 1000.0;
	double step = st / t;

	//每个波形的宽度.
	return step;
}

// 分别对应1/40/2000
double SpeedMode[3][2] =
{
	{0.8 ,4},
	{1.0/32  ,8},
	{1.0/1600,20},
};

int SpeedIndex = 0;

// ---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner),m_bResponse(false),m_eWorkStep(STEP_IDLE),m_bStarted(false)
{
	DataGrids::StartGdiProcess();
	//m_stTestProcedure  = TestProcedure();
	m_sTableName = "设备检测数据";

	pBeepThread = new BeepThread(false);
	m_bTestPassed[0] = false;
	m_bTestPassed[1] = false;
	m_bTestPassed[2] = false;

	m_bNoSigFlag = false;
}

void __fastcall TForm1::LoadResources(void)
{
	//连接按钮用.
	m_aBmp[0] = new TBitmap;
	m_aBmp[0]->LoadFromResourceName((unsigned int)HInstance,"bmpWrong");

	m_aBmp[1] = new TBitmap;
	m_aBmp[1]->LoadFromResourceName((unsigned int)HInstance,"bmpOK");

	m_aBmp[2] = new TBitmap;
	m_aBmp[2]->LoadFromResourceName((unsigned int)HInstance,"bmpPressW");

	m_aBmp[3] = new TBitmap;
	m_aBmp[3]->LoadFromResourceName((unsigned int)HInstance,"bmpPress");

	//链接状态显示.
   	m_aBmpSt[0] = new TPngImage;
	m_aBmpSt[0]->LoadFromResourceName((unsigned int)HInstance,"bmpDataWrong");
	m_aBmpSt[1] = new TPngImage;
	m_aBmpSt[1]->LoadFromResourceName((unsigned int)HInstance,"bmpDataOK");

	m_aFanIcons[0] = new TBitmap;
	m_aFanIcons[0]->LoadFromResourceName((unsigned int)HInstance,"fan1");
	m_aFanIcons[1] = new TBitmap;
	m_aFanIcons[1]->LoadFromResourceName((unsigned int)HInstance,"fan2");
	m_aFanIcons[2] = new TBitmap;
	m_aFanIcons[2]->LoadFromResourceName((unsigned int)HInstance,"fan3");

	// 测试状态表示用.
	UnicodeString sRNameBig[6] =
	{
		"png90NotTest",
		"png90TestNg",
		"png90TestPass",
		"png90Progress1",
		"png90Progress2",
		"png90Progress3",
	};

  	UnicodeString sRNameSmall[6] =
	{
		"png36NotTest",
		"png36TestNg",
		"png36TestPass",
		"png36Progress1",
		"png36Progress2",
		"png36Progress3",
	};

	for(int i = 0;i < 6;i++)
	{
        m_aPngStBig[i] = new TPngImage;
		m_aPngStBig[i]->LoadFromResourceName((unsigned int)HInstance,sRNameBig[i]);
	}

	for(int i = 0;i < 6;i++)
	{
        m_aPngStSmall[i] = new TPngImage;
		m_aPngStSmall[i]->LoadFromResourceName((unsigned int)HInstance,sRNameSmall[i]);
	}
}

void __fastcall TForm1::SearchComponents(void)
{
	for(int i = 0 ;i < 3;i++)
	{
		UnicodeString s = "btnTestMode" + IntToStr(i+1);
		aBtnTestMode[i] = dynamic_cast<TBitBtn*>(this->FindComponent(s));
		aBtnTestMode[i]->OnClick = TestBtnClick;
	}

	for(int i = 0 ;i < 3;i++)
	{
		UnicodeString s = "ImageMode" + IntToStr(i+1);
		aImageMode[i] =  dynamic_cast<TImage*>(this->FindComponent(s));
	}
}

void __fastcall TForm1::InitComponents(void)
{
	for(int i = 0; i < 3;i++)
	{
		aImageMode[i]->Picture->Assign(m_aPngStSmall[0]);
	}
}


void __fastcall TForm1::TestBtnClick(TObject *Sender)
{
	static unsigned char buf[128];

	int idx =-1;
	for(int i =0  ;i < 3;i++)
	{
		if (aBtnTestMode[i] == dynamic_cast<TBitBtn*>(Sender))
		{
			idx = i;
			break;
		}
	}

	if (idx < 0)
	{
		return ;
	}

	if(idx == 0)
	{
		ResetStatus();
	}

	StartTestMode(idx+1);

	unsigned short vh = pCfg->Params.VoltLimit[1] * 2.65 * 4096 / 5;
	unsigned short vl = pCfg->Params.VoltLimit[0] * 2.65 * 4096 / 5;

	SndData(buf,idx+1,vh,vl);
	m_bStarted = true;
	m_bUpdateFlag = false;//不更新.
	m_startPos = PaintBox1->Width;//
	StartPortion = 1;
	m_bNoSigFlag = false;

    ShowSysInfo("");
	//PostMessage(Form4->Handle,WM_USER + 0x101,0,0);
	//this->CheckBox1->Checked = true;
}


void __fastcall TForm1::StartTestMode(int mode)
{
	iCurrStep = mode;
	m_bResponse = false;
	//设置响应标记.
}


bool __fastcall TForm1::CheckIfPass(int index)
{
	Params * p = &(pCfg->Params);

	// 0 -- 转速.

	bPassCfg[0][0] = (m_stTestProcedure[index][0].GetMin() >= p->RSpeedRange[index][0])
					&&(m_stTestProcedure[index][0].GetMin() <= p->RSpeedRange[index][1]);
	bPassCfg[0][1] =(m_stTestProcedure[index][0].GetMax() >= p->RSpeedRange[index][0])
					&&(m_stTestProcedure[index][0].GetMax() <= p->RSpeedRange[index][1]);

	// 1 -- 占空比
	bPassCfg[1][0] = (m_stTestProcedure[index][1].GetMin() >= p->DutyRange[0])
					 &&(m_stTestProcedure[index][1].GetMin() <= p->DutyRange[1]);

	bPassCfg[1][1] =(m_stTestProcedure[index][1].GetMax() >= p->DutyRange[0])
					&&(m_stTestProcedure[index][1].GetMax() <= p->DutyRange[1]);

	// 2 -- 低电平电压值
	bPassCfg[2][0] = (m_stTestProcedure[index][2].GetMin() >= p->LowVoltRange[0])
					&&(m_stTestProcedure[index][2].GetMin() <= p->LowVoltRange[1]);
	bPassCfg[2][1] =  (m_stTestProcedure[index][2].GetMax() >= p->LowVoltRange[0])
					&& (m_stTestProcedure[index][2].GetMax() <= p->LowVoltRange[1]);

	// 3 -- 高电平电压值
	bPassCfg[3][0] = (m_stTestProcedure[index][3].GetMin() >= p->HighVoltRange[0])
					&&(m_stTestProcedure[index][3].GetMin() <= p->HighVoltRange[1]);
	bPassCfg[3][1] = (m_stTestProcedure[index][3].GetMax() >= p->HighVoltRange[0])
					&&(m_stTestProcedure[index][3].GetMax() <= p->HighVoltRange[1]);

	// 4 -- 上升沿时间.
	bPassCfg[4][0] = (m_stTestProcedure[index][4].GetMin() <= 1.5);
	bPassCfg[4][1] = (m_stTestProcedure[index][4].GetMax() <= 1.5);

	// 5 -- 下降沿时间.
	bPassCfg[5][0] = (m_stTestProcedure[index][5].GetMax() <= 1.5);
	bPassCfg[5][1] = (m_stTestProcedure[index][5].GetMax() <= 1.5);

	for(int i = 0 ;i < sizeof(bPassCfg)/sizeof(bPassCfg[0]);i++)
	{
		if((bPassCfg[i][0] == false)|| (bPassCfg[i][0] == false))
		{
			return false;
		}
	}

	return true;
}

#define NO_TEST_IDX 0
#define TEST_NG_IDX 1
#define TEST_PASS_IDX 2

void __fastcall TForm1::ResetStatus(void)
{
	m_bTestPassed[0] = false;
	m_bTestPassed[1] = false;
	m_bTestPassed[2] = false;

	ImageTestModeOverAll->Picture->Assign(m_aPngStBig[NO_TEST_IDX]);
	aImageMode[0]->Picture->Assign(m_aPngStSmall[NO_TEST_IDX]);
	aImageMode[1]->Picture->Assign(m_aPngStSmall[NO_TEST_IDX]);
	aImageMode[2]->Picture->Assign(m_aPngStSmall[NO_TEST_IDX]);
}


void __fastcall TForm1::HandleResult(void)
{
	static const aiTestTimeout[3] = {8000,6000,6000};//每种环节的超时时间.
	static int internalTimer = 0;

	switch(m_eWorkStep)
	{
		case STEP_IDLE:
			if(m_bStarted == true)
			{
				m_eWorkStep = STEP_WAITING;
				internalTimer = 0;
			}
		break;
		case STEP_WAITING:
			if(m_bResponse == true) //已经处理完成.
			{
				m_eWorkStep = STEP_HANDLING;
			}
			else if(m_bNoSigFlag == true)
			{
				m_eWorkStep = STEP_TIMEOUT;
			}
			else if(++internalTimer >= (aiTestTimeout[iCurrStep-1]/CommTimer->Interval))
			{
				m_eWorkStep = STEP_TIMEOUT;
			}

			ShowTestBlink(); //闪烁进度标示.
		break;
		case STEP_HANDLING:
		{	//检验结果.
			int _idx = 0;
			m_bStarted = false;//处理完成.
			//显示结果.
			m_bTestPassed[iCurrStep-1] =CheckIfPass(iCurrStep-1);

			//触发界面刷新.
			SG_WaveData->Invalidate();
			_idx = m_bTestPassed[iCurrStep-1] ? TEST_PASS_IDX:TEST_NG_IDX;
			aImageMode[iCurrStep-1]->Picture->Assign(m_aPngStSmall[_idx]);

			if(m_bTestPassed[iCurrStep-1] == false)
			{
				pBeepThread->StartBeep(5,200,100);
				ImageTestModeOverAll->Picture->Assign(m_aPngStBig[TEST_NG_IDX]);
			}
			else
			{
				ImageTestModeOverAll->Picture->Assign(m_aPngStBig[NO_TEST_IDX]);
			}

			//焦点更新.
			if((m_bTestPassed[iCurrStep-1]) && (iCurrStep < 3))
			{
				aBtnTestMode[iCurrStep]->SetFocus();
			}
			else
			{
				aBtnTestMode[0]->SetFocus();
			}

			if(m_bTestPassed[0] && m_bTestPassed[1] && m_bTestPassed[2] )
			{
				ImageTestModeOverAll->Picture->Assign(m_aPngStBig[TEST_PASS_IDX]);
				btnStoreToDataBase->SetFocus();
			}

			m_eWorkStep = STEP_IDLE;
		}
		break;
		case STEP_TIMEOUT:
			m_bStarted = false;//处理完成.
			//显示结果是未定,界面数据不更新.
			aImageMode[iCurrStep-1]->Picture->Assign(m_aPngStSmall[TEST_NG_IDX]);
			ImageTestModeOverAll->Picture->Assign(m_aPngStBig[TEST_NG_IDX]);
			ShowSysInfo("响应超时，请检查设备连线",InfoType_Warn);
			m_eWorkStep = STEP_IDLE;
			aBtnTestMode[0]->SetFocus();

			pBeepThread->StartBeep(5,200,100);

		break;
		default:
			m_eWorkStep = STEP_IDLE;
		break;
	}
}

void __fastcall TForm1::ShowTestResult(int iTestMode)
{
	if(iTestMode > 3 || iTestMode < 1)
	{
		return;
	} //检测过程中.

	if(m_bResponse == false)
	{
		//超出时间未响应.认为检测失败.
		ShowMessage("检测设备未响应，请检查连接线！");
		return;
	}

	aImageMode[iCurrStep-1]->Picture->Assign(m_aPngStSmall[2]);
}



#define BLINK_PERIOD 400

void __fastcall TForm1::SetGroupBox(TGroupBox * grp , bool bVal)
{
	for(int i =0  ;i < grp->ControlCount;i++)
	{
		grp->Controls[i]->Enabled = bVal;
	}
}

void __fastcall TForm1::ShowTestBlink(bool TestEnd)
{
	static int s_imageIdx = 3;
	static int s_iLocalCnt =0;

	if (iCurrStep == 0)
	{
		return;
	}

	if ( (++s_iLocalCnt)*CommTimer->Interval <  BLINK_PERIOD)
	{
		return;
	}

	s_iLocalCnt = 0;

	if (TestEnd == false)
	{
		aImageMode[iCurrStep-1]->Picture->Assign(m_aPngStSmall[s_imageIdx]);
		ImageTestModeOverAll->Picture->Assign(m_aPngStBig[s_imageIdx]);

		if (++s_imageIdx > 5)
		{
			s_imageIdx = 3;
		}
	}
}

void __fastcall TForm1::ExecTestMode(void)
{
}


static TColor bColor;
void __fastcall TForm1::ShowSysInfo(UnicodeString str, eInfoType eType)
{
	static const TColor SysColors[] = {clLime,clYellow,clRed};
	stbSysBar->Panels->Items[2]->Text = str;

	if(str.IsEmpty())
	{
        bColor = clBtnFace;
	}
	else
	{
        bColor = SysColors[eType];
	}

}


void __fastcall TForm1::ShowConnectInfo(bool DataBaseReady ,bool DeviceReady)
{
	stbSysBar->Panels->Items[0]->Text = (UnicodeString)"数据库:" + (UnicodeString)( DataBaseReady ? "OK":"XX");
	stbSysBar->Panels->Items[1]->Text = (UnicodeString)"设备:" + (UnicodeString)( DataBaseReady ? "OK":"XX");
}

void __fastcall TForm1::Loging(UnicodeString str, eInfoType eType, bool toConsole)// 系统日志.用于调试.
{
}

void __fastcall TForm1::ShowData(DataItemSet & ds)
{
	//表格中显示数据.
	for(int i = 0;i < 6;i++)
	{
		SG_WaveData->Cells[1][i+1] = FloatToStrF(ds[i].GetMin(),ffFixed,10,2);
		SG_WaveData->Cells[2][i+1] = FloatToStrF(ds[i].GetAvg(),ffFixed,10,2);
		SG_WaveData->Cells[3][i+1] = FloatToStrF(ds[i].GetMax(),ffFixed,10,2);
	}

	SG_WaveData->Cells[1][7] = FloatToStrF(ds[0].GetMin()/60,ffFixed,10,2);
	SG_WaveData->Cells[2][7] = FloatToStrF(ds[0].GetAvg()/60,ffFixed,10,2);
	SG_WaveData->Cells[3][7] = FloatToStrF(ds[0].GetMax()/60,ffFixed,10,2);

	SG_WaveData->Cells[3][3] = "--";
	SG_WaveData->Cells[1][4] = "--";

	m_stDg->SetDuty(ds[1].GetMax());
	m_stDg->SetHvScale(100*ds[3].GetMax()/1.60);
	m_stDg->SetLvScale(100*ds[3].GetMin()/1.60);
	PaintBox1->Invalidate();
}


int __fastcall TForm1::GetIndex(void)
{
	if(ADOConnection1->Connected == false)
	{
		return -1;
	}

	ADOQuery1->SQL->Clear();
	ADOQuery1->SQL->Add("select max(id) as maxIndex from " + m_sTableName);
	ADOQuery1->Open();

	if(ADOQuery1->RecordCount == 0)
	{
		return 0; // 表中尚无任何记录.
	}

	int id = ADOQuery1->FieldByName("maxIndex")->AsInteger;

    ADOQuery1->SQL->Clear();

	//ShowMessage("id = " + IntToStr(id));

    return id;
}


bool __fastcall TForm1::prepareDataBase(void)
{
	//创建数据库并检查表格.
	UnicodeString sql;
	// 创建数据库.
	UnicodeString sPath;

	if(ADOConnection1->Connected == true)
	{
		return true;
	}

	if (CheckFilePath(sPath) == false)
	{
		WideString s = "Provider=Microsoft.Jet.OLEDB.4.0;" "Data Source='" +
			(WideString)sPath + "';Jet OLEDB:Engine Type=5";
		TADOXCatalog *pCatalog = new TADOXCatalog(NULL);
		//TADOXTable *pt = new TADOXTable(NULL);
		pCatalog->Create(s.c_bstr());
	}

	sPath = "Provider=Microsoft Jet 4.0 OLE DB Provider;Data Source=" + sPath;
	sPath += ";Persist Security Info=False";
	ADOConnection1->ConnectionString = sPath;
	ADOConnection1->LoginPrompt = false;

	try
	{
		ADOConnection1->Open();
	}
	catch (...)
	{
	}

	if (ADOConnection1->Connected)
	{
		ShowSysInfo("数据库连接成功");
	}
	else
	{
		ShowSysInfo("数据库连接失败");
	}
	ADOQuery1->Connection = ADOConnection1;

	TStrings *pList = new TStringList();
	bool bHasTable = false;

	ADOConnection1->GetTableNames(pList, false);

	for (int i = 0; i < pList->Count; i++)
	{
		if (pList->Strings[i] == m_sTableName)
		{
			bHasTable = true;
			break;
		}
	}

	if (bHasTable == false)
	{
		sql = "";
		m_stTestProcedure.SqlCreateTable(m_sTableName, sql);

		ADOQuery1->SQL->Clear();
		ADOQuery1->SQL->Add(sql);
		ADOQuery1->ExecSQL();
		ADOQuery1->SQL->Clear();
	}

	// 创建表格完毕.
	return true;
}

bool __fastcall TForm1::storeDataBase(UnicodeString serialNo)
{
	if(ADOConnection1->Connected == false)
	{
		return false;
	}



	UnicodeString sql = "";

	m_stTestProcedure.SqlInsertTableValue(m_sTableName, sql,serialNo);
	ADOQuery1->SQL->Add(sql);
	ADOQuery1->ExecSQL();
	ADOQuery1->SQL->Clear();
	return true;
}


#define _MAKEWORD(H,L) (unsigned short)((((unsigned char)H) << 8) | ((unsigned char)L))
#define _MAKELONG(a,b,c,d) (unsigned int)(((a)<<24)|((b)<<16)|((c)<<8)|((d)<<0))


const double fusScale  = 1.0f/16;
const double DutyScale = 1024/100.0;
const double voltScale = 5000/(4096*2.65);

bool __fastcall TForm1::ParseDataSliced(unsigned char * buf,unsigned int len)
{
	unsigned int length = 0;

	while(len > 0)
	{
		int _len = buf[2];

		ParseData(buf,_len + 3 ,&length);

		if(len > length)
		{
			len -= (3 + _len);
			buf += (3 + _len);
		}
		else
		{
			break;
		}

		if(len < 4)
		{
			break;
		}
	}
}

bool __fastcall TForm1::ParseData(unsigned char * buf,unsigned int len , unsigned int * usedLength)
{

	const double cycles[3] = {4,40,400};

	unsigned char checksum;
	double ds[12];

	*usedLength = buf[2];

	//对数据进行校验.
	if(len < (1 + 1 + 1 + 1))
	{
		return false; //数据长度较短.
	}

	if( buf[0] != 0xAA || buf[1] != 0x55)
	{
		mm_ComRec->Lines->Add("帧头错误");
		return false; // 帧头错误.
	}

	if( (int)(buf[2]) > (int)(len - 3)) // 长度不够.
	{
		mm_ComRec->Lines->Add("长度错误");
		return false;
	}


	for(int i = 3 ;i < (int)(len -1) ;i++)
	{
		checksum += buf[i];
	}

	if(checksum != buf[len-1])
	{
	//	return false; // 校验错误.
	}

	// 分解数据.
	if(len < 1 + 1 + 1) // 数据长度不够.
	{
		mm_ComRec->Lines->Add("长度不足");
		return false;
	}

	// 跳过命令字节.
	for(int i = 0 ;i< 12;i+=2)
	{
		ds[i] = _MAKEWORD(buf[1 +(3+i)],buf[1 + (3+i+1)]);
	}

	unsigned char * pd = buf + 3;

	if((*pd >= 1) && (*pd <=3))
	{
		mm_ComRec->Lines->Add("启动模式" + IntToStr((*pd))+"成功");
	}
	else if((*pd >= 0x11) && (*pd <= 0x13)) //返回数据.
	{
		unsigned char Mode = *pd - 0x11;

		if( *(pd + 1) == 0xFF) //检测数据超时.
		{
			m_bResponse = false;
			mm_ComRec->Lines->Add("未检测到有效信号，请检查连接线/端盖");
			m_bNoSigFlag = true;
			return false;
		}


		if(len < 51)
		{
			mm_ComRec->Lines->Add("数据长度不够.预期=51,实际长度 = " + IntToStr((int)len));
			return false;
		}

		pd ++;

		double pmax,pmin,pa,dmax,dmin,da,lmin,lmax,la,hmin,hmax,ha,rmax,rmin,ra,Fmax,Fmin,Fa;

		hmax = _MAKEWORD(pd[0],pd[1]);
		pd += 2;

		ha   = _MAKEWORD(pd[0],pd[1]);
		pd += 2;

		hmin = 1200;//固定值.

		lmax = 800;

		lmin = _MAKEWORD(pd[0],pd[1]);
		pd += 2;

		la 	 = _MAKEWORD(pd[0],pd[1]);
		pd += 2;

		pmax = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		pmin = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		pa   = _MAKELONG(pd[0],pd[1],pd[2],pd[3]) / cycles[Mode];
		pd += 4;


		rmax = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		rmin = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		ra   = _MAKELONG(pd[0],pd[1],pd[2],pd[3]) / cycles[Mode];
		pd += 4;

		Fmax = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		Fmin = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		Fa   = _MAKELONG(pd[0],pd[1],pd[2],pd[3]) / cycles[Mode];
		pd += 4;

		dmax = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		dmin = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		da   = _MAKELONG(pd[0],pd[1],pd[2],pd[3]) / cycles[Mode];
		//pd += 4;


		std::vector<double> vd ;
		vd.push_back(60000000.0/(pmin*fusScale*48));
		vd.push_back(60000000.0/(pa*fusScale*48));
		vd.push_back(60000000.0/(pmax*fusScale*48));
		vd.push_back(dmax/DutyScale);
		vd.push_back(da/DutyScale);
		vd.push_back(dmin/DutyScale);
		vd.push_back(lmax/1000.0);
		vd.push_back(la*voltScale/1000.0);
		vd.push_back(lmin*voltScale/1000.0);
		vd.push_back(hmax*voltScale/1000.0);
		vd.push_back(ha*voltScale/1000.0);
		vd.push_back(hmin/1000.0);
		vd.push_back(rmax*fusScale);
		vd.push_back(ra*fusScale);
		vd.push_back(rmin*fusScale);
		vd.push_back(Fmax*fusScale);
		vd.push_back(Fa*fusScale);
		vd.push_back(Fmin*fusScale);

		// 查询数据内容.
		this->UpdateDateItemByVector(vd,(int)Mode);
		m_bResponse = true;
		m_bUpdateFlag = false;

		SYSTEMTIME SystemTime;
 		GetLocalTime(&SystemTime);
		mm_ComRec->Lines->Add(IntToStr(SystemTime.wMinute) + ":" + IntToStr(SystemTime.wSecond)  + " : " + IntToStr(SystemTime.wMilliseconds));

        return true;
	}
	else if (*pd == 0xFF) //测量失败
	{
		//测量失败.
		//::Beep(4000,400);
		m_bResponse = true;
		mm_ComRec->Lines->Add("未检测到有效信号，请检查连接线/端盖");
		m_bNoSigFlag = true;
        m_bUpdateFlag = false;
		return false;
	}
	else if ((*pd >= 0x21) && (*pd <= 0x23))
	{
		unsigned char Mode = *pd - 0x11;

		m_startPos = this->PaintBox1->Width;
		StartPortion = 1;
		m_bUpdateFlag = true;
		mm_ComRec->Lines->Add("下位机检测到正常信号");

		if(len < 51)
		{
			mm_ComRec->Lines->Add("数据长度不够.预期=51,实际长度 = " + IntToStr((int)len));
			m_bNoSigFlag = true;
			m_bUpdateFlag = false;
			return false;
		}

		pd ++;

		double pmax,pmin,pa,dmax,dmin,da,lmin,lmax,la,hmin,hmax,ha,rmax,rmin,ra,Fmax,Fmin,Fa;

		hmax = _MAKEWORD(pd[0],pd[1]);
		pd += 2;

		ha   = _MAKEWORD(pd[0],pd[1]);
		pd += 2;

		hmin = 1200;//固定值.

		lmax = 800;

		lmin = _MAKEWORD(pd[0],pd[1]);
		pd += 2;

		la 	 = _MAKEWORD(pd[0],pd[1]);
		pd += 2;

		pmax = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		pmin = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		pa   = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;

		pa = 60000000.0/(pa*fusScale*48);

		// 转速.

		if(pa < 10)
		{
			SpeedIndex = 0;
		}
		else if(pa < 200)
		{
			SpeedIndex = 1;
		}
		else
		{
			SpeedIndex = 2;
		}


		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);
		mm_ComRec->Lines->Add(IntToStr(SystemTime.wMinute) + ":" + IntToStr(SystemTime.wSecond)  + " : " + IntToStr(SystemTime.wMilliseconds));	}
	else
	{
		mm_ComRec->Lines->Add("未识别的命令码:0x" + IntToHex(*pd,2));
		m_bUpdateFlag = false;
	}

	m_bResponse = false;

	return true;
}

void __fastcall TForm1::OnRecvMessage(TMessage &msg)
{
	static unsigned char buf[4096];

	if(msg.Msg != MSG_RECV_COMMDATA)
	{
		return;
	}
	// 读取.

	if(comx.IsPortOpen() ==false)
	{
        return;
	}

	int len = comx.ReadPort(buf);

	if(len == 0)
	{
        return;
	}

	UnicodeString s = "";
    s.sprintf(L"收到%6d字节:",len);

	for(int i = 0;i < len;  i++)
	{
        s += IntToHex(buf[i],2).UpperCase() + " ";
	}

    mm_ComRec->Lines->Add(s);

	//处理数据.
    ParseDataSliced(buf,len);
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::btnStoreToDataBaseClick(TObject *Sender)
{
	UnicodeString sTime,sNo;
    static int s_iDataTestCounter = GetIndex(); // 查询当前最大索引.

/*    OKRightDlg->SetData(this->m_stTestProcedure);
	OKRightDlg->ShowModal();

	if(OKRightDlg->GetResult() == false)
	{
		ShowMessage("数据未存入数据库");
		return;
	}
*/
	sTime = ::Date().FormatString("yyyymmdd");
	sNo = "";
	sNo.sprintf(L"%04d",++s_iDataTestCounter);

	sTime += sNo;

	labelSerialNO->Caption = sTime;



	storeDataBase(sTime);
	//Memo1->Lines->Add(sql);

	btnTestMode1->SetFocus();
	ResetStatus();
}
// ---------------------------------------------------------------------------
void __fastcall TForm1::PaintBox1Paint(TObject *Sender)
{
	//PaintBox图像绘制.
	/*TCanvas *pc = PaintBox1->Canvas;
	Gdiplus::Graphics g(pc->Handle);

	g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	Gdiplus::Pen p(Gdiplus::Color(255, 0, 0, 0));

	g.DrawRectangle(&p,0,0,PaintBox1->Width-1,PaintBox1->Height-1);

	g.DrawLine(&p,0,0,PaintBox1->Width,PaintBox1->Height);
	g.DrawLine(&p,PaintBox1->Width,0,0,PaintBox1->Height);
	*/
	static phase = 0;
	static pos = 0;

	if(PaintBox1->Visible == false)
	{
		return;
	}

	if (m_stDg == NULL)
	{
		m_stDg = new DataGrids(PaintBox1->Canvas->Handle,PaintBox1->Width,PaintBox1->Height);
		PaintBox1->ControlStyle = PaintBox1->ControlStyle << csOpaque;
	}

	double step = CalcMode(SpeedMode[SpeedIndex][0],SpeedMode[SpeedIndex][1]);

	if(m_bUpdateFlag)
	{
		if( m_startPos > 0)
		{
			m_startPos -= step;
			StartPortion = m_startPos * 1.0 / PaintBox1->Width;
		}
		else
		{
			double __step = 0;
			if(SpeedIndex == 0)
			{
				__step = step;
			}
			else if(SpeedIndex == 1)
			{
				__step = (PaintBox1->Width/SpeedMode[SpeedIndex][1]) * 100 / 360.0;
			}
			else
			{
				__step = (PaintBox1->Width/SpeedMode[SpeedIndex][1]) * 170 / 360.0;
			}

			phase += __step;
		}
	}

	m_stDg->DrawWave(PaintBox1->Canvas->Handle,50,1.0/SpeedMode[SpeedIndex][0],1,m_startPos,0.02/step,phase);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OnEraseBkgnd(TWMEraseBkgnd & msg)
{
	msg.Result = true;
}

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	//Gdiplus::GdiplusShutdown(gdiplusToken);
	DataGrids::EndGdiProcess();
	//Form3->Close();
	if(ADOConnection1->Connected)
	{
		ADOConnection1->Close();
	}

	CloseMappedFile();
	if(pBeepThread != NULL)
	{
		pBeepThread->ThreadResume();
		pBeepThread->Terminate();
		pBeepThread->WaitFor();
		delete pBeepThread;
	}

	if( comx.IsPortOpen() == true)
	{
		comx.ClosePort();
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::stbSysBarDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel,
		  const TRect &Rect)
{
	if (Panel->Index == 2)
	{
		StatusBar->Canvas->Brush->Color = bColor;
		StatusBar->Canvas->FillRect(Rect);
	}

	::DrawText(StatusBar->Canvas->Handle,((WideString)(Panel->Text)).c_bstr(),Panel->Text.Length(),&Rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::SG_WaveDataDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
		  TGridDrawState State)
{
	//
	TStringGrid *s = dynamic_cast<TStringGrid*>(Sender);
	UnicodeString Str = s->Cells[ACol][ARow];
	TRect rect = s->CellRect(ACol,ARow);
	if (ACol == 0 || ARow == 0 || ACol == 4)
	{
		s->Canvas->Brush->Color = cl3DLight;
	}
	else
	{
		s->Canvas->Brush->Color = clWhite;
	}

	s->Canvas->FillRect(Rect);

	if (ACol == 4 && ARow >0)
	{
		s->Canvas->Font->Color = clBlue;
	}
	else
	{
		int _idx = (ACol == 1)?0:(ACol==3)?1:-1;

		if(_idx != -1 && ARow != 0)
		{
			s->Canvas->Font->Color = bPassCfg[ARow-1][_idx] ? clWindowText:clRed;
		}
		else s->Canvas->Font->Color = clWindowText;
	}

	::DrawText(s->Canvas->Handle,((WideString)(Str)).c_bstr(),Str.Length(),&Rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}
//---------------------------------------------------------------------------



void __fastcall TForm1::TrackBar1Change(TObject *Sender)
{
	//
	TTrackBar * tbar = dynamic_cast<TTrackBar*>(Sender);
    m_stDg->SetDuty(tbar->Position);
    PaintBox1->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TrackBar2Change(TObject *Sender)
{
	// 设置高度比值.
	TTrackBar * tbar = dynamic_cast<TTrackBar*>(Sender);
	m_stDg->SetHvScale(tbar->Max - tbar->Position);
	m_stDg->SetLvScale(tbar->Max - tbar->Position);
    PaintBox1->Invalidate();
}
//---------------------------------------------------------------------------
#define randomf(x) ((double)random(x * 100)/100.0)
#define MAXVAL(a,b) (((a) > (b)) ? (a) :(b))
#define MINVAL(a,b) (((a) < (b)) ? (a) :(b))

void __fastcall TForm1::Button1Click(TObject *Sender)
{	//
#if 0
	double v1,v2 ;
	DataItemSet *pds = new DataItemSet(0);
	//频率.

	v1 = 10 + randomf(5);
	v2 = 10 + randomf(5);
	pds->SetFreqVal(MAXVAL(v1,v2),MINVAL(v1,v2));

	v1 = 50 + random(10);
	v2 = 50 + random(10);
	pds->SetDutyVal(MAXVAL(v1,v2),MINVAL(v1,v2));

	v1 = 0.6 + randomf(0.24);
	v2 = 0.6 + randomf(0.24);
	pds->SetLVVal(MAXVAL(v1,v2),MINVAL(v1,v2));

	v1 = 1.2 + randomf(0.4);
	v2 = 1.2 + randomf(0.4);
	pds->SetHVVal(MAXVAL(v1,v2),MINVAL(v1,v2));

	v1 = randomf(2.5);
	v2 = randomf(2.5);
	pds->SetRTVal(MAXVAL(v1,v2),MINVAL(v1,v2));

	v1 = randomf(2.5);
	v2 = randomf(2.5);
	pds->SetFTVal(MAXVAL(v1,v2),MINVAL(v1,v2));

	ShowData(*pds);

	delete pds;
#endif

	static int deltpase = 0;



}
//---------------------------------------------------------------------------



void __fastcall TForm1::CommTimerTimer(TObject *Sender)
{
	static int iSeq = 0;
	static int iTimer_200ms = 0;
	static int iTimer_100ms = 0;

	if (CheckBox1->Checked)
	{
		//Button1Click(Button1);
	}

	if(comx.IsPortOpen())
	{
		if( ++iTimer_200ms >= 200 / CommTimer->Interval)
		{
			iTimer_200ms = 0;
			iSeq = (++iSeq) % 3;
			ImageFan->Picture->Assign(m_aFanIcons[iSeq]);
		}

		HandleResult();
	}

	PaintBox1->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SearchPorts(void)
{
    std::vector<UnicodeString> v;
	cbSerialPorts->Items->Clear();
    EmurateSerialPorts(v);

	for(int i = 0 ;i < (int)v.size();i++)
	{
        cbSerialPorts->Items->Add(v[i]);
	}

	cbSerialPorts->Style = csDropDownList;
    cbSerialPorts->ItemIndex = -1;
}

void __fastcall TForm1::FormPaint(TObject *Sender)
{
	// 按钮绘制.

	TPoint px = bitbtn_Store->ClientToParent(TPoint(1,1));
//	HRGN hRgnR = CreateEllipticRgn(px.X,px.Y,bitbtn_Store->Width-3,bitbtn_Store->Height-3);
//	SetWindowRgn(bitbtn_Store->Handle,hRgnR,TRUE);
	Gdiplus::Graphics *pg = new Gdiplus::Graphics(this->Canvas->Handle);

	pg->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	Gdiplus::Pen p(Gdiplus::Color(90,90,90),1);
	Gdiplus::GraphicsPath gp;

    gp.AddEllipse(px.X,px.Y,bitbtn_Store->Width-4,bitbtn_Store->Height-4);
	//pg->DrawPath(&p);
    pg->DrawPath(&p,&gp);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::bitbtn_StoreClick(TObject *Sender)
{
	/*static int idx = 0;
	idx = (++idx)%2;
	iconDataBase->Picture->Assign(m_aBmpSt[idx]);
	iconDevice->Picture->Assign(m_aBmpSt[idx]);
	*/
	if(comx.IsPortOpen() == false)
    {
		if(cbSerialPorts->Items->Count == 0 || cbSerialPorts->ItemIndex == -1)
		{
			return;
		}

		if(comx.OpenPort(cbSerialPorts->Text))
		{
			m_iBmpIdx = 1;
			comx.SetNotifyWindow(this->Handle);
			cbSerialPorts->Enabled = false;
			SetGroupBox(grpTest,true);
            btnTestMode1->SetFocus();
		}
		else
		{
            ShowMessage("打开端口失败!");
		}
	}
	else
	{
		comx.ClosePort();
		m_iBmpIdx = 0;
		cbSerialPorts->Enabled = true;
        SetGroupBox(grpTest,false);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::bitbtn_StoreMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	bitbtn_Store->Glyph = m_aBmp[m_iBmpIdx + 2];
}
//---------------------------------------------------------------------------

void __fastcall TForm1::bitbtn_StoreMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    bitbtn_Store->Glyph = m_aBmp[m_iBmpIdx];
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpdateDateItemByVector(std::vector<double> &v,int idx)
{
	m_stTestProcedure[idx].UpdateAllData(v);
	ShowData(m_stTestProcedure[idx]);
}

void __fastcall TForm1::UpdateDataItem(double fmax,double fmin,double dmax,double dmin,double lmax,double lmin,double hmax,double hmin,double rmax,double rmin,double Fmax,double Fmin)
{
	//显示在表格中.
	DataItemSet *pds = new DataItemSet(iCurrStep,
										fmax,fmin,
										dmax,dmin,
										lmax,lmin,
										hmax,hmin,
										rmax,rmin,
										Fmax,Fmin);
	ShowData(*pds);



    delete pds;
}



bool TForm1::SndData(unsigned char *pbuf, unsigned char mode , unsigned short vh, unsigned short vl)
{
	int idx =  0;
	unsigned char checksum = 0;

	pbuf[idx++] = 0xaa;
	pbuf[idx++] = 0x55;
	pbuf[idx++] = 6;
	pbuf[idx++] = mode;
	pbuf[idx++] = (vh >> 8);
	pbuf[idx++] = (vh >> 0);
	pbuf[idx++] = (vl >> 8);
	pbuf[idx++] = (vl >> 0);

	for(int i = 3;i< idx;i++)
	{
		checksum += pbuf[i];
	}

	pbuf[idx++] = checksum;


	UnicodeString s = "";

	for(int i =0;i < idx ;i++)
	{
		s  += IntToHex(pbuf[i],2) + " ";
	}

    mm_ComRec->Lines->Add("发送:" + s);

	if(comx.IsPortOpen() == false)
	{
        return false;
	}

	return comx.WritePort(pbuf,idx);
}

#include <System.SysUtils.hpp>

void __fastcall TForm1::FormCreate(TObject *Sender)
{
	TDate endDate, currentDate;

	currentDate = Now();
	endDate = TDate(2015,7,1);

	if(currentDate >= endDate)
	{
		Application->Terminate();
	}
	SetGroupBox(grpTest,false);
	if(CreateApplicationFile(&pCfg) == false)
	{
        ShowMessage("无法初始化配置文件");
	}

	SearchPorts();

	m_startPos = this->PaintBox1->Width;
	StartPortion = 1;

    Label4->Font->Color = clBlue;

	// stbSysBar->Font->Height = stbSysBar->Height - 6;
	// SG_WaveData->DrawingStyle =  gdsClassic;

	mm_ComRec->Lines->Clear();
	mm_ComRec->Visible = false;

	// 设置表格内容.
	UnicodeString sTable[] =
	{
		"参数",
		"转速",
		"占空比",
		"低电平电压",
		"高电平电压",
		"上升沿时间",
		"下降沿时间",
		"频率"
	};

	for(int i = 0;i < (int)ITEMS_OF(sTable);i++)
	{
		SG_WaveData->Cells[0][i] = sTable[i];
	}

	SG_WaveData->Cells[1][0] = "最小值";
	SG_WaveData->Cells[2][0] = "平均值";
	SG_WaveData->Cells[3][0] = "最大值";
	SG_WaveData->Cells[4][0] = "单位";

	SG_WaveData->Cells[4][1] = "rpm";
	SG_WaveData->Cells[4][2] = "%";
	SG_WaveData->Cells[4][3] = "V";
	SG_WaveData->Cells[4][4] = "V";
	SG_WaveData->Cells[4][5] = "μs";
	SG_WaveData->Cells[4][6] = "μs";
	SG_WaveData->Cells[4][7] = "Hz";
	//TrackBar1->Max = 100;
	//TrackBar2->Max = 100;
	// 防止闪烁.
	this->DoubleBuffered = true;

	HRGN hRgnR = CreateEllipticRgn(1,1,bitbtn_Store->Width-1,bitbtn_Store->Height-1);
	SetWindowRgn(bitbtn_Store->Handle,hRgnR,TRUE);

	LoadResources();
	SearchComponents();
	InitComponents();

	bitbtn_Store->Glyph = m_aBmp[0];
	m_iBmpIdx = 0;

	iconDataBase->Picture->Assign(m_aBmpSt[0]);
	iconDevice->Picture->Assign(m_aBmpSt[0]);
	prepareDataBase();

	ED_HighVoltage->Text = FloatToStrF(voltageH/1000.0,ffGeneral,10,3);
	ED_LowVoltage->Text  = FloatToStrF(voltageL/1000.0,ffGeneral,10,3);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	//按键.
	if(Key == VK_F11  && Shift.Contains(ssCtrl))
	{
		mm_ComRec->Visible = !mm_ComRec->Visible;
		PaintBox1->Visible = !PaintBox1->Visible;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnSqlTestClick(TObject *Sender)
{
	//
	int idx = this->GetIndex();

	ShowMessage("MaxIndex = " + IntToStr(idx));
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormResize(TObject *Sender)
{
	if(m_stDg != NULL)
	{
		m_stDg->SetSize(PaintBox1->Width,PaintBox1->Height);
	}

	SG_WaveData->DefaultRowHeight = (SG_WaveData->Height -3) / SG_WaveData->RowCount - 1;

	m_startPos = PaintBox1->Width * StartPortion;

	//SG_WaveData->Height = (SG_WaveData->DefaultRowHeight + 1) * SG_WaveData->RowCount + 3;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::btnCheckCfgClick(TObject *Sender)
{
	//UnitFormAuth->Parent = this;

	UnitFormAuth->SetAuthInfo(pCfg);

	if(UnitFormAuth->IsAuthed() == false)
	{
		UnitFormAuth->ShowModal();
	}

	if(UnitFormAuth->IsAuthed())
	{
		UnitFormAuth->ResetAuth();
		//UnitFormAuth->SetReload();
		FormSysCfg->SetData(&(pCfg->Params));
        FormSysCfg->ShowModal();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button4Click(TObject *Sender)
{
	//查看配置.
	//FormSysCfg->Enabled = false;
	FormSysCfg->SetPreviewMode(true);
	FormSysCfg->SetData(&(pCfg->Params));
	FormSysCfg->ShowModal();
	FormSysCfg->SetPreviewMode(false);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
	pBeepThread->StartBeep(5,200,100);
}
//---------------------------------------------------------------------------



