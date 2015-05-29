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
	//��֤
	if(MED_Password->Text == "000000")
	{
		//��֤ͨ��.
		bAuthPassed = true;
        this->Close();
	}
	else
	{
		bAuthPassed = false;
		//Application->MessageBox(L"�������",L"������֤",MB_OK);
        ShowMessage("�������!");
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
