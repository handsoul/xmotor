//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H


//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include "DataSouce.h"

//---------------------------------------------------------------------------

class TFormSysCfg : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *SG_Config;
	TLabel *Label1;
	TButton *btnSave;
	TEdit *ED_HeightVoltage;
	TEdit *ED_LowVoltage;
	TLabel *Label2;
	TLabel *Label3;
	TButton *btnLoadDefault;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall SG_ConfigDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
		  TGridDrawState State);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall SG_ConfigSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall btnLoadDefaultClick(TObject *Sender);
	void __fastcall SG_ConfigSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
	void __fastcall ED_HeightVoltageChange(TObject *Sender);
	void __fastcall ED_LowVoltageChange(TObject *Sender);
	void __fastcall btnSaveClick(TObject *Sender);

private:	// User declarations

	Params * m_param;
    union{
		Params Params;
        double ValBuf[sizeof(::Params)/(sizeof(double)*2)][2];
    }ParamBuf;
public:		// User declarations
	void __fastcall SetData(Params * _param) {m_param = _param;}
	void __fastcall LoadCfgData(const Params * p);
	__fastcall TFormSysCfg(TComponent* Owner);
	void __fastcall SetPreviewMode(bool Mode = false);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormSysCfg *FormSysCfg;
//---------------------------------------------------------------------------
#endif
