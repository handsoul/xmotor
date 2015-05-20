// ---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <Vcl.OleServer.hpp>
#include "ADOX_OCX.h"
#include <Vcl.ExtCtrls.hpp>
#include <windows.h>
#include <VCLTee.TeeGDIPlus.hpp>
#include <VCLTee.TeeData.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <VCLTee.TeCanvas.hpp>
#include <VCLTee.TeeEdiGrad.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Graphics.hpp>
#include <algorithm>
#include <gdiplus.h>
#include "DrawGraphics.h"

typedef enum TagInfoType
{
	InfoType_Info,
	InfoType_Warn,
	InfoType_Error,
}eInfoType;

// ---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published: // IDE-managed Components
	TADOConnection *ADOConnection1;
	TADOStoredProc *ADOStoredProc1;
	TADOQuery *ADOQuery1;
	TButton *btnSqlTest;
	TButton *Button1;
	TStringGrid *SG_WaveData;
	TStatusBar *stbSysBar;
	TTimer *CommTimer;
	TCheckBox *CheckBox1;
	TBitBtn *bitbtn_Store;
	TImage *iconDataBase;
	TImage *iconDevice;
	TLabel *Label2;
	TLabel *Label1;
	TLabel *labelTestCounter;
	TImage *ImageFan;
	TGroupBox *GroupBox1;
	TBitBtn *btnTestMode1;
	TImage *ImageMode1;
	TBitBtn *btnTestMode3;
	TBitBtn *btnTestMode2;
	TImage *ImageMode2;
	TImage *ImageMode3;
	TLabel *Label4;
	TLabel *labelSerialNO;
	TButton *Button2;
	TImage *ImageTestModeOverAll;
	TPaintBox *PaintBox1;
	TEdit *ED_HighVoltage;
	TEdit *ED_LowVoltage;
	TLabel *Label3;
	TLabel *Label5;
	void __fastcall btnSqlTestClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall PaintBox1Paint(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall stbSysBarDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall SG_WaveDataDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
		  TGridDrawState State);
	void __fastcall TrackBar1Change(TObject *Sender);
	void __fastcall TrackBar2Change(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall CommTimerTimer(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall bitbtn_StoreClick(TObject *Sender);
	void __fastcall bitbtn_StoreMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall bitbtn_StoreMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall FormCreate(TObject *Sender);


private: // User declarations

	static inline GetHighLimit(short h, short l)
	{
		return (short)(h-l)*0.05f + l;
	}


	static inline GetLowLimit(short h, short l)
	{
        return (short)(h-l)*0.95f + l;
	}

	static const unsigned short defaultVoltH = (1180 * 2.65f * 4096  / 5000);
	static const unsigned short defaultVoltL = ( 860 * 2.65f * 4096  / 5000);



	TBitmap *m_aBmp[4];
	TPngImage *m_aBmpSt[2];
	int m_iBmpIdx;
	int m_aiFlags[3];

	TBitBtn* aBtnTestMode[3]; // 3个测试模式.
	TImage * aImageMode[3];   // 用于显示状态.
	TPngImage * m_aPngStSmall[6];  //
	TPngImage * m_aPngStBig[6];    //
	TBitmap   * m_aFanIcons[3];	   // 风扇图标.

	DataGrids *m_stDg;

	void __fastcall ShowSysInfo(UnicodeString str, eInfoType eType=InfoType_Info);
	void __fastcall ShowConnectInfo(bool DataBaseReady = false,bool DeviceReady=false);
	void __fastcall Loging(UnicodeString str, eInfoType eType, bool toConsole = false); // 系统日志.用于调试.
	void __fastcall ShowData(DataItemSet & ds);
	void __fastcall SearchComponents(void);
	void __fastcall LoadResources(void);
	void __fastcall InitComponents(void);
	void __fastcall TestBtnClick(TObject *Sender);
private:
	// 测试过程相关.
	int iCurrStep ; //当前测试步骤.
	int iTestEnd;
    UnicodeString m_sTableName;
	TestProcedure m_stTestProcedure;// = TestProcedure();

	void __fastcall StartTestMode(int i);
	void __fastcall ShowTestBlink(bool TestEnd = false);
	void __fastcall ShowTestResult(int iTestMode);
	void __fastcall ExecTestMode(void);

	bool SndData(unsigned char *pbuf, unsigned char mode , unsigned short vh = defaultVoltH, unsigned short vl = defaultVoltL);

	bool __fastcall prepareDataBase(void);

	bool __fastcall storeDataBase(void);

public : // User declarations
	__fastcall TForm1(TComponent* Owner);

	void __fastcall TForm1::UpdateDateItemByVector(std::vector<double> &v);

	__fastcall void UpdateDataItem( double ,double ,double ,double ,double ,double ,
								double ,double ,double ,double ,double ,double );
};

// ---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
// ---------------------------------------------------------------------------
#endif
