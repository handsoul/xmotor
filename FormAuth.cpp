//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormAuth.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TUnitFormAuth *UnitFormAuth;
//---------------------------------------------------------------------------
__fastcall TUnitFormAuth::TUnitFormAuth(TComponent* Owner)
	: TForm(Owner),bAuthPassed(false)
{
}
//---------------------------------------------------------------------------
void __fastcall TUnitFormAuth::btnLoginClick(TObject *Sender)
{
	//验证
	if(MED_Password->Text == "000000")
	{
		//验证通过.
		bAuthPassed = true;
        this->Close();
	}
	else
	{
		bAuthPassed = false;
		//Application->MessageBox(L"密码错误！",L"密码验证",MB_OK);
        ShowMessage("密码错误!");
	}
}
//---------------------------------------------------------------------------
void __fastcall TUnitFormAuth::FormKeyPress(TObject *Sender, System::WideChar &Key)
{
	if(Key == VK_ESCAPE)
	{
        this->Close();
	}
	else if(Key == VK_RETURN)
	{
        btnLoginClick(btnLogin);
	}
}
//---------------------------------------------------------------------------
void __fastcall TUnitFormAuth::FormClose(TObject *Sender, TCloseAction &Action)
{
    this->MED_Password->Text = "";
}
//---------------------------------------------------------------------------
