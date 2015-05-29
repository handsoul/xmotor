//----------------------------------------------------------------------------
#ifndef ShowStatusH
#define ShowStatusH
//----------------------------------------------------------------------------
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Graphics.hpp>
#include <System.Classes.hpp>
#include <System.SysUtils.hpp>
#include <Winapi.Windows.hpp>
#include <System.hpp>
#include <Vcl.Grids.hpp>
#include "DataSouce.h"
//----------------------------------------------------------------------------
class TOKRightDlg : public TForm
{
__published:
	TButton *OKBtn;
	TButton *CancelBtn;
	TStringGrid *SG_Data;
	void __fastcall CancelBtnClick(TObject *Sender);
	void __fastcall OKBtnClick(TObject *Sender);
private:
    bool EnsureState;
public:
	bool GetResult(void){return EnsureState;}
    void __fastcall SetData(TestProcedure &t);
	virtual __fastcall TOKRightDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TOKRightDlg *OKRightDlg;
//----------------------------------------------------------------------------
#endif    
