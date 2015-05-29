//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ShowStatus.h"

//---------------------------------------------------------------------
#pragma resource "*.dfm"
TOKRightDlg *OKRightDlg;
//--------------------------------------------------------------------- 
__fastcall TOKRightDlg::TOKRightDlg(TComponent* AOwner)
	: TForm(AOwner),EnsureState(false)
{
}
//---------------------------------------------------------------------
void __fastcall TOKRightDlg::CancelBtnClick(TObject *Sender)
{
	this->EnsureState = false;
	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TOKRightDlg::SetData(TestProcedure &t)
{
    // 初始化显示数据名
	for(int i = 0 ;i < t.Length();i++)
	{
		// 共三个模式.
        SG_Data->Cells[0][i*(t.Length()+1) + 1] = "模式" + IntToStr(i+1);
	}
}

void __fastcall TOKRightDlg::OKBtnClick(TObject *Sender)
{
	this->EnsureState = true;
	this->Close();
}
//---------------------------------------------------------------------------

