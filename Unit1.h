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
#include "SerialPort.h"
#include "cnComm.h"
#include "BeepThread.h"

typedef enum TagInfoType
{
	InfoType_Info,
	InfoType_Warn,
	InfoType_Error,
}eInfoType;

typedef enum tagWorkingSteps
{
	STEP_IDLE = 0,
	STEP_WAITING,
	STEP_HANDLING,
    STEP_TIMEOUT,
}eWorkStep;

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
	TGroupBox *grpTest;
	TBitBtn *btnTestMode1;
	TImage *ImageMode1;
	TBitBtn *btnTestMode3;
	TBitBtn *btnTestMode2;
	TImage *ImageMode2;
	TImage *ImageMode3;
	TLabel *Label4;
	TLabel *labelSerialNO;
	TButton *btnStoreToDataBase;
	TImage *ImageTestModeOverAll;
	TEdit *ED_HighVoltage;
	TEdit *ED_LowVoltage;
	TLabel *Label3;
	TLabel *Label5;
	TMemo *mm_ComRec;
	TImage *Image1;
	TComboBox *cbSerialPorts;
	TPaintBox *PaintBox1;
	TLabel *Label6;
	TLabel *Label7;
	TButton *btnCheckCfg;
	TButton *Button4;
	void __fastcall btnStoreToDataBaseClick(TObject *Sender);
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
	void __fastcall bitbtn_StoreClick(TObject *Sender);
	void __fastcall bitbtn_StoreMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall bitbtn_StoreMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall btnSqlTestClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnCheckCfgClick(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);


private: // User declarations

	static inline GetHighLimit(short h, short l)
	{
		return (short)(h-l)*0.05f + l;
	}


	static inline GetLowLimit(short h, short l)
	{
		return (short)(h-l)*0.95f + l;
	}

	double CalcMode(double period, double N);

	static const unsigned short voltageH = 1180;
	static const unsigned short voltageL = 860;

	static const unsigned short defaultVoltH = (voltageH * 2.65f * 4096  / 5000);
	static const unsigned short defaultVoltL = (voltageL * 2.65f * 4096  / 5000);

	TBitmap *m_aBmp[4];
	TPngImage *m_aBmpSt[2];
	int m_iBmpIdx;
	int m_aiFlags[3];

	eWorkStep m_eWorkStep;

    ConfigOjb * pCfg;

	TBitBtn* aBtnTestMode[3]; // 3个测试模式.
	TImage * aImageMode[3];   // 用于显示状态.
	TPngImage * m_aPngStSmall[6];  //
	TPngImage * m_aPngStBig[6];    //
	TBitmap   * m_aFanIcons[3];	   // 风扇图标.

	DataGrids *m_stDg;
    BeepThread * pBeepThread;

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
	bool m_bUpdateFlag;
	bool m_bStarted;
	bool m_bTestPassed[3];
	bool m_bNoSigFlag;

	UnicodeString m_sTableName;
	TestProcedure m_stTestProcedure;// = TestProcedure();

	SerialPortBase comx;
	bool m_bResponse;

	double m_startPos;


	void __fastcall SetGroupBox(TGroupBox * grp , bool bVal);
	void __fastcall StartTestMode(int i);
	void __fastcall ShowTestBlink(bool TestEnd = false);
	void __fastcall ShowTestResult(int iTestMode);
	void __fastcall ExecTestMode(void);
	bool __fastcall CheckIfPass(int idx);

	int __fastcall GetIndex(void);

	bool SndData(unsigned char *pbuf, unsigned char mode , unsigned short vh = defaultVoltH, unsigned short vl = defaultVoltL);

	bool __fastcall prepareDataBase(void);
	bool __fastcall storeDataBase(UnicodeString serialNo = "");

	bool __fastcall TryOpenPort(void);// 尝试打开串口并通信....
	bool __fastcall ClosePort(void);  // 断开串口...

	void __fastcall HandleResult(void);

	void __fastcall OnRecvMessage(TMessage &msg);
	void __fastcall OnEraseBkgnd(TWMEraseBkgnd& msg);

	void __fastcall ResetStatus(void);
	bool __fastcall ParseDataSliced(unsigned char * buf,unsigned int len);
	bool __fastcall ParseData(unsigned char * buf,unsigned int len, unsigned int * usedLen);


	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(MSG_RECV_COMMDATA,TMessage,OnRecvMessage)
		//VCL_MESSAGE_HANDLER(WM_ERASEBKGND,TWMEraseBkgnd,OnEraseBkgnd)
	END_MESSAGE_MAP(TForm)


public : // User declarations
	__fastcall TForm1(TComponent* Owner);
	void __fastcall SearchPorts(void);

	void __fastcall TForm1::UpdateDateItemByVector(std::vector<double> &v,int idx);

	__fastcall void UpdateDataItem( double ,double ,double ,double ,double ,double ,
								double ,double ,double ,double ,double ,double );
};

// ---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
// ---------------------------------------------------------------------------
#endif
