//---------------------------------------------------------------------------

#ifndef FormAuthH
#define FormAuthH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Mask.hpp>
//---------------------------------------------------------------------------
class TUnitFormAuth : public TForm
{
__published:	// IDE-managed Components
	TMaskEdit *MED_Password;
	TEdit *Edit1;
	TLabel *Label1;
	TLabel *Label2;
	TButton *btnLogin;
	TButton *Button1;
	void __fastcall btnLoginClick(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
    bool bAuthPassed;
public:		// User declarations
	inline bool IsAuthed(void){return bAuthPassed;}
	inline void ResetAuth(void){bAuthPassed = false;}
	__fastcall TUnitFormAuth(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TUnitFormAuth *UnitFormAuth;
//---------------------------------------------------------------------------
#endif
