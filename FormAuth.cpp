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
	UnicodeString sPasswd(pCfg->Password);

	if(MED_Password->Text == sPasswd)
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

void TUnitFormAuth::SetChangePassMode(bool ifSet)
{
	LabelNew->Visible = ifSet;
	LabelConfirmNew->Visible = ifSet;
	MED_NewPassword->Visible = ifSet;
	MED_ConfirmNewPassword->Visible = ifSet;
	btnChangePass->Visible = !ifSet;
	btnCancle->Visible = ifSet;
	btnSetNewPassword->Visible = ifSet;
	btnLogin->Visible = !ifSet;
	this->Height = (ifSet)? 276 : 206;
}

//---------------------------------------------------------------------------
void __fastcall TUnitFormAuth::FormClose(TObject *Sender, TCloseAction &Action)
{
	this->MED_Password->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TUnitFormAuth::FormActivate(TObject *Sender)
{
	this->SetChangePassMode(false);
}
//---------------------------------------------------------------------------


void __fastcall TUnitFormAuth::btnChangePassClick(TObject *Sender)
{
	this->SetChangePassMode(true);
}
//---------------------------------------------------------------------------

void __fastcall TUnitFormAuth::btnSetNewPasswordClick(TObject *Sender)
{
	UnicodeString sPasswd(pCfg->Password);

	if(this->MED_Password->Text != sPasswd)
	{
		//
		ShowMessage("ԭ�����������");
        return;
	}

    if(this->MED_Password->Text.Length() < 6)
	{
		//
		ShowMessage("�����벻������6λ");
		return;
	}

    if(this->MED_NewPassword->Text != this->MED_ConfirmNewPassword->Text)
	{
		//
		ShowMessage("�������벻ƥ��");
		return;
	}

	memcpy(pCfg->Password,MED_NewPassword->Text.c_str(),(MED_NewPassword->Text.Length() + 1)*sizeof(wchar_t));
	SyncToMappdFile();
	ShowMessage("�޸ĳɹ�~");

	this->MED_Password->Text = "";

    this->SetChangePassMode(false);
}
//---------------------------------------------------------------------------

void __fastcall TUnitFormAuth::btnCancleClick(TObject *Sender)
{
    this->SetChangePassMode(false);
}
//---------------------------------------------------------------------------

