//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TFormSysCfg : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *SG_Config;
	TLabel *Label1;
	TButton *btnSave;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall SG_ConfigDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
private:	// User declarations
public:		// User declarations
	__fastcall TFormSysCfg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSysCfg *FormSysCfg;
//---------------------------------------------------------------------------
#endif
