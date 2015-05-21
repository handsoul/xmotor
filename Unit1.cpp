// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "SerialForm.h"
#include "DataSouce.h"
#include "ADOX_TLB.h"
#include "Unit4.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ADOX_OCX"
#pragma link "frxCtrls"
#pragma resource "*.dfm"
TForm1 *Form1;

// ---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner)
{
	DataGrids::StartGdiProcess();
	//m_stTestProcedure  = TestProcedure();
	m_sTableName = "设备检测数据";
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

	StartTestMode(idx+1);


	SndData(buf,idx+1);
	PostMessage(Form4->Handle,WM_USER + 0x101,0,0);
	//this->CheckBox1->Checked = true;
}


void __fastcall TForm1::StartTestMode(int mode)
{

	if (iCurrStep != 0)
	{
		return;
	}
	else if(mode > 3 || mode < 1)
	{
		return;
	}

    iCurrStep = mode;
}


void __fastcall TForm1::ShowTestResult(int iTestMode)
{
	if(iTestMode > 3 || iTestMode < 1)
	{
		return;
	}

    aImageMode[iCurrStep-1]->Picture->Assign(m_aPngStSmall[2]);
}



#define BLINK_PERIOD 400

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
	// 执行过程.

	static int aiTestTime[3] = {5000,3000,3000};
	static int iTcnt =0 ;

	if (iCurrStep == 0)
	{
		iTestEnd = 1;
	}
	else
	{
		if(++iTcnt <= (int)(aiTestTime[iCurrStep-1]/CommTimer->Interval))
		{
			iTestEnd = 0;
		}
		else
		{
			ShowTestResult(iCurrStep);
			iTestEnd = 1;
			iCurrStep = 0;
			iTcnt = 0;
			this->CheckBox1->Checked = false;
		}
	}
}


void __fastcall TForm1::ShowSysInfo(UnicodeString str, eInfoType eType)
{
	stbSysBar->Panels->Items[2]->Text = str;
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
		SG_WaveData->Cells[1][i+1] = FloatToStrF(ds[i].GetMin(),ffGeneral,10,2);
		SG_WaveData->Cells[2][i+1] = FloatToStrF(ds[i].GetAvg(),ffGeneral,10,2);
		SG_WaveData->Cells[3][i+1] = FloatToStrF(ds[i].GetMax(),ffGeneral,10,2);
	}

	m_stDg->SetDuty(ds[1].GetMax());
	m_stDg->SetHvScale(100*ds[3].GetMax()/1.60);
	m_stDg->SetLvScale(100*ds[3].GetMin()/1.60);
	PaintBox1->Invalidate();
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
		TADOXTable *pt = new TADOXTable(NULL);
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
		ShowSysInfo("连接成功");
	}
	else
	{
		ShowSysInfo("连接失败");
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

bool __fastcall TForm1::storeDataBase(void)
{
	if(ADOConnection1->Connected == false)
	{
		return false;
	}

	UnicodeString sql = "";
	m_stTestProcedure.SqlInsertTableValue(m_sTableName, sql);
	ADOQuery1->SQL->Add(sql);
	ADOQuery1->ExecSQL();
	ADOQuery1->SQL->Clear();
	return true;
}


#define _MAKEWORD(H,L) (unsigned short)((((unsigned char)H) << 8) | ((unsigned char)L))
#define _MAKELONG(a,b,c,d) (unsigned int)(((a)<<24)|((b)<<16)|((c)<<8)|((d)<<0))


const double fusScale  = 1.0f/16;
const double DutyScale = 100.0f/1024;
const double voltScale = 5000/(4096*2.65);

bool __fastcall TForm1::ParseData(unsigned char * buf,unsigned int len)
{

    const double cycles[3] = {4,96,1000};

	unsigned char checksum;
	double ds[12];

	//对数据进行校验.
	if(len < (2 + 1 + 1 + 2))
	{
		return false; //数据长度较短.
	}

	if( buf[0] != 0xAA || buf[1] != 0x55)
	{
		mm_ComRec->Lines->Add("帧头错误");
		return false; // 帧头错误.
	}

	if( buf[2] > len - 3) // 长度不够.
	{
		mm_ComRec->Lines->Add("长度错误");
		return false;
	}


	for(int i = 3 ;i < len -1 ;i++)
	{
		checksum += buf[i];
	}

	if(checksum != buf[len-1])
	{
	//	return false; // 校验错误.
	}

	// 分解数据.
	if(len < 3 + 1 + 1) // 数据长度不够.
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
		pd += 4;


		std::vector<double> vd ;
		vd.push_back(60000000.0/pmax*fusScale);
		vd.push_back(60000000.0/pa*fusScale);
		vd.push_back(60000000.0/pmin*fusScale);
		vd.push_back(dmax/DutyScale);
		vd.push_back(da/DutyScale);
		vd.push_back(dmin/DutyScale);
		vd.push_back(lmax);
		vd.push_back(la*voltScale);
		vd.push_back(lmin*voltScale);
		vd.push_back(hmax*voltScale);
		vd.push_back(ha*voltScale);
		vd.push_back(hmin);
		vd.push_back(rmax*fusScale);
		vd.push_back(ra*fusScale);
		vd.push_back(rmin*fusScale);
		vd.push_back(Fmax*fusScale);
		vd.push_back(Fa*fusScale);
		vd.push_back(Fmin*fusScale);


		// 查询数据内容.
		this->UpdateDateItemByVector(vd);
	}
	else
	{
		mm_ComRec->Lines->Add("未识别的命令码:0x" + IntToHex(*pd,2));
	}

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
    ParseData(buf,len);
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::btnSqlTestClick(TObject *Sender)
{
#if 0
	static TestProcedure t = TestProcedure();

	UnicodeString sql;
	// 创建数据库.
	UnicodeString sPath;

	if (CheckFilePath(sPath) == false)
	{
		WideString s = "Provider=Microsoft.Jet.OLEDB.4.0;" "Data Source='" +
			(WideString)sPath + "';Jet OLEDB:Engine Type=5";
		TADOXCatalog *pCatalog = new TADOXCatalog(NULL);
		TADOXTable *pt = new TADOXTable(NULL);
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
		ShowSysInfo("连接成功");
	}
	else
	{
		ShowSysInfo("连接失败");
	}
	ADOQuery1->Connection = ADOConnection1;

	UnicodeString sTableName = "FirstTable";
	TStrings *pList = new TStringList();
	bool bHasTable = false;

	ADOConnection1->GetTableNames(pList, false);

	for (int i = 0; i < pList->Count; i++)
	{
		if (pList->Strings[i] == sTableName)
		{
			bHasTable = true;
			break;
		}
	}

	if (bHasTable == false)
	{
		sql = "";
		t.SqlCreateTable(sTableName, sql);

		ADOQuery1->SQL->Clear();
		ADOQuery1->SQL->Add(sql);
		ADOQuery1->ExecSQL();
		ADOQuery1->SQL->Clear();
	}

	// 插入记录.
#if 1
	// for (int i = 0; i < 10; i++) {
	sql = "";
	t.SqlInsertTableValue(sTableName, sql);
	ADOQuery1->SQL->Add(sql);

	//Memo1->Lines->Add("执行SQL:" + sql);
	// }

	ADOQuery1->ExecSQL();
	ADOQuery1->SQL->Clear();
#endif

	ADOConnection1->Close();
#endif
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
	static int s_iDataTestCounter = 0;
	static TestProcedure t = TestProcedure();

	t[0].SetDutyVal(45, 44);
	t[1].SetFreqVal(20, 19);
	t[2].SetLVVal(1930, 1920);
	UnicodeString sql;

	t.SqlInsertTableValue("t1", sql);

	UnicodeString sTime,sNo;
	sTime = ::Date().FormatString("yyyymmdd");
	sNo = "";
	sNo.sprintf(L"%04d",++s_iDataTestCounter);

	sTime += sNo;

	labelSerialNO->Caption = sTime;

	//Memo1->Lines->Add(sql);
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

	if(PaintBox1->Visible == false)
	{
	    return;
	}

	if (m_stDg == NULL)
	{
		m_stDg = new DataGrids(PaintBox1->Canvas->Handle,PaintBox1->Width,PaintBox1->Height);
	}

	m_stDg->DrawGrids(PaintBox1->Canvas->Handle);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	//Gdiplus::GdiplusShutdown(gdiplusToken);
	DataGrids::EndGdiProcess();
	//Form3->Close();
	if(ADOConnection1->Connected)
	{
		ADOConnection1->Close();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::stbSysBarDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel,
          const TRect &Rect)
{
	if (Panel->Index == 2)
	{
		StatusBar->Canvas->Brush->Color = clLime;
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
	if (ACol == 0 || ARow == 0 || ACol == 4) {
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
		s->Canvas->Font->Color = clWindowText;
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
}
//---------------------------------------------------------------------------



void __fastcall TForm1::CommTimerTimer(TObject *Sender)
{
	static int iSeq = 0;

	if (CheckBox1->Checked)
	{
		Button1Click(Button1);
	}

	ShowTestBlink(iTestEnd > 0);
	ExecTestMode();


	iSeq = (++iSeq) % 3;

    ImageFan->Picture->Assign(m_aFanIcons[iSeq]);
}
//---------------------------------------------------------------------------





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

	if(comx.OpenPort("COM1",115200))
	{
		m_iBmpIdx = 1;
        comx.SetNotifyWindow(this->Handle);
	}
	else
	{
		comx.ClosePort();
        m_iBmpIdx = 0;
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


void __fastcall TForm1::UpdateDateItemByVector(std::vector<double> &v)
{
	m_stTestProcedure[iCurrStep-1].UpdateAllData(v);
	ShowData(m_stTestProcedure[iCurrStep-1]);

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

    return Form4->SendMsg(pbuf,idx);
}

void __fastcall TForm1::FormCreate(TObject *Sender)
{
	// 启动时显示启动窗口.
    //
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
	};

	for(int i = 0;i < ITEMS_OF(sTable);i++)
	{
		SG_WaveData->Cells[0][i] = sTable[i];
	}

	SG_WaveData->Cells[1][0] = "最小值";
	SG_WaveData->Cells[2][0] = "平均值";
	SG_WaveData->Cells[3][0] = "最大值";
	SG_WaveData->Cells[4][0] = "单位";

	SG_WaveData->Cells[4][1] = "rpm";
	SG_WaveData->Cells[4][2] = "%";
	SG_WaveData->Cells[4][3] = "mV";
	SG_WaveData->Cells[4][4] = "mV";
	SG_WaveData->Cells[4][5] = "μs";
	SG_WaveData->Cells[4][6] = "μs";

	//TrackBar1->Max = 100;
	//TrackBar2->Max = 100;
	// 防止闪烁.
	this->DoubleBuffered = true;

	HRGN hRgnR = CreateEllipticRgn(3,3,bitbtn_Store->Width-3,bitbtn_Store->Height-3);
	SetWindowRgn(bitbtn_Store->Handle,hRgnR,TRUE);

	LoadResources();
	SearchComponents();
    InitComponents();

	bitbtn_Store->Glyph = m_aBmp[0];
	m_iBmpIdx = 0;

	iconDataBase->Picture->Assign(m_aBmpSt[0]);
	iconDevice->Picture->Assign(m_aBmpSt[0]);

	prepareDataBase();
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

