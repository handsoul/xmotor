//---------------------------------------------------------------------------

#ifndef Unit4H
#define Unit4H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
#include "SerialPort.h"
#include "DataSouce.h"
#include "cnComm.h"

class TForm4 : public TForm
{
__published:	// IDE-managed Components
	TComboBox *cbSerialPorts;
	TComboBox *cbBaudRate;
	TComboBox *cbParity;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TComboBox *cbStopBits;
	TButton *btnOpenSerialPort;
	TMemo *mm_ComRec;
	TButton *btnSendData;
	TMemo *mm_SendBox;
	TTimer *Timer1;
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall btnOpenSerialPortClick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall mm_SendBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall btnSendDataClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
	CnComm * m_pcnCom;
	SerialPortBase * m_pCom;
	bool ParseData(unsigned char * _buf,unsigned int len);

public:		// User declarations
	__fastcall TForm4(TComponent* Owner);
	void __fastcall OnRecvMessage(TMessage & msg);
	void __fastcall OnSndMessage(TMessage & msg);

    bool __fastcall SendMsg(unsigned char * pbuf, int len);

	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(MSG_RECV_COMMDATA + 1, TMessage , OnSndMessage)
		MESSAGE_HANDLER(MSG_RECV_COMMDATA,TMessage,OnRecvMessage)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TForm4 *Form4;
//---------------------------------------------------------------------------
#endif
