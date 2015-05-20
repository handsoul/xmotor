// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;

// ---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner) : TForm(Owner)
{
	// ���ر�����.
	this->BorderStyle = bsNone;
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::Timer1Timer(TObject *Sender)
{
	// ������Ϣ�б�.
	const static String s_aStrInfo[] =
	{(String)"��������", (String)"������ݿ�", (String)"����豸", };

	static int s_iIndx = 0;

	if (s_iIndx >= 3)
	{
		Timer1->Enabled = false;
		this->Close();
		return;
	}

	label_StartInfo->Caption = s_aStrInfo[s_iIndx++];
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::FormPaint(TObject *Sender)
{
	// �������Χ���ƿ���.
	//this->Canvas->Pen->Color = clBlue;
	//this->Canvas->Pen->Width = 1;
	//this->Canvas->Rectangle(1, 1, this->Width, this->Height);

	HRGN hRegion = ::CreateRoundRectRgn (0, 0, ClientWidth, ClientHeight,30,30);
	Canvas->Brush->Style = bsSolid;
	Canvas->Brush->Color = RGB(90, 90, 90);
	//SetWindowRgn(this->Handle,hRegion,false);
	::FrameRgn(this->Canvas->Handle, hRegion, Canvas->Brush->Handle, 1, 1);
	::DeleteObject(hRegion); // Don't leak a GDI object

}
// ---------------------------------------------------------------------------


void __fastcall TForm2::FormActivate(TObject *Sender)
{
    HRGN hRegion = ::CreateRoundRectRgn (0, 0, ClientWidth, ClientHeight,30,30);
	SetWindowRgn(this->Handle,hRegion,false);
}
//---------------------------------------------------------------------------

