//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSysCfg *FormSysCfg;
//---------------------------------------------------------------------------
__fastcall TFormSysCfg::TFormSysCfg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSysCfg::FormResize(TObject *Sender)
{
	SG_Config->DefaultColWidth = (SG_Config->Width-7) / SG_Config->ColCount ;
	SG_Config->DefaultRowHeight = (SG_Config->Height-12)/ SG_Config->RowCount;
}
//---------------------------------------------------------------------------
void __fastcall TFormSysCfg::FormCreate(TObject *Sender)
{
	SG_Config->Cells[0][0] = "门限";
	SG_Config->Cells[1][0] = "最大值";
	SG_Config->Cells[2][0] = "最小值";
	SG_Config->Cells[3][0] = "单位";

    SG_Config->Cells[1][0] = "";

}
//---------------------------------------------------------------------------
void __fastcall TFormSysCfg::SG_ConfigDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{

	TStringGrid *s = dynamic_cast<TStringGrid*>(Sender);
	UnicodeString Str = s->Cells[ACol][ARow];
	TRect rect = s->CellRect(ACol,ARow);
	if (ACol != 0 && ARow != 0)
	{
		s->Canvas->Brush->Color = clWhite;
		s->Canvas->FillRect(Rect);
	}

	if (ACol == 4 && ARow >0)
	{
		s->Canvas->Font->Color = clBlue;
	}
	else
	{
		s->Canvas->Font->Color = clWindowText;
	}

	::DrawText(s->Canvas->Handle,((WideString)(Str)).c_bstr(),Str.Length(),&Rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}
//---------------------------------------------------------------------------
