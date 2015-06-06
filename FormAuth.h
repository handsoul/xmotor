//---------------------------------------------------------------------------

#ifndef FormAuthH
#define FormAuthH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Mask.hpp>
#include "DataSouce.h"
//---------------------------------------------------------------------------
class TUnitFormAuth : public TForm
{
__published:	// IDE-managed Components
	TMaskEdit *MED_Password;
	TEdit *Edit1;
	TLabel *Label1;
	TLabel *Label2;
	TButton *btnLogin;
	TButton *btnChangePass;
	TMaskEdit *MED_NewPassword;
	TMaskEdit *MED_ConfirmNewPassword;
	TLabel *LabelConfirmNew;
	TLabel *LabelNew;
	TButton *btnSetNewPassword;
	TButton *btnCancle;
	void __fastcall btnLoginClick(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall btnChangePassClick(TObject *Sender);
	void __fastcall btnSetNewPasswordClick(TObject *Sender);
	void __fastcall btnCancleClick(TObject *Sender);
private:	// User declarations
	bool bAuthPassed;
	void SetChangePassMode(bool ifSet = false);
	ConfigOjb * pCfg;

public:		// User declarations
	inline void SetAuthInfo(ConfigOjb * _pCfg){pCfg = _pCfg;}
	inline bool IsAuthed(void){return bAuthPassed;}
	inline void ResetAuth(void){bAuthPassed = false;}
	__fastcall TUnitFormAuth(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TUnitFormAuth *UnitFormAuth;
//---------------------------------------------------------------------------
#endif
