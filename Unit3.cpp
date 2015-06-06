//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
#include "DataSouce.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSysCfg *FormSysCfg;
//---------------------------------------------------------------------------
__fastcall TFormSysCfg::TFormSysCfg(TComponent* Owner)
	: TForm(Owner),m_param(NULL)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSysCfg::FormResize(TObject *Sender)
{
	SG_Config->DefaultColWidth = (SG_Config->Width-3) / SG_Config->ColCount -1;
	SG_Config->DefaultRowHeight = (SG_Config->Height-3)/ SG_Config->RowCount -1;
}
//---------------------------------------------------------------------------
void __fastcall TFormSysCfg::FormCreate(TObject *Sender)
{
	SG_Config->Cells[0][0] = "有效范围";
	SG_Config->Cells[1][0] = "最小值";
	SG_Config->Cells[2][0] = "最大值";
	SG_Config->Cells[3][0] = "单位";

	SG_Config->Cells[0][1] = "高电平";
	SG_Config->Cells[0][2] = "低电平";
	SG_Config->Cells[0][3] = "占空比";
	SG_Config->Cells[0][4] = "转速1rpm";
	SG_Config->Cells[0][5] = "转速40rpm";
	SG_Config->Cells[0][6] = "转速2000rpm";

	SG_Config->Cells[3][1] = "V";
	SG_Config->Cells[3][2] = "V";
	SG_Config->Cells[3][3] = "%";
	SG_Config->Cells[3][4] = "rpm";
	SG_Config->Cells[3][5] = "rpm";
	SG_Config->Cells[3][6] = "rpm";
}

void __fastcall TFormSysCfg::LoadCfgData(const Params * p)
{
	if(p == NULL)
	{
		return;
	}

	SG_Config->Cells[1][1] = p->HighVoltRange[0];
	SG_Config->Cells[2][1] = p->HighVoltRange[1];

	SG_Config->Cells[1][2] = p->LowVoltRange[0];
	SG_Config->Cells[2][2] = p->LowVoltRange[1];

	SG_Config->Cells[1][3] = p->DutyRange[0];
	SG_Config->Cells[2][3] = p->DutyRange[1];

	SG_Config->Cells[1][4] = p->RSpeedRange[0][0];
	SG_Config->Cells[2][4] = p->RSpeedRange[0][1];

	SG_Config->Cells[1][5] = p->RSpeedRange[1][0];
	SG_Config->Cells[2][5] = p->RSpeedRange[1][1];

	SG_Config->Cells[1][6] = p->RSpeedRange[2][0];
	SG_Config->Cells[2][6] = p->RSpeedRange[2][1];

	ED_HeightVoltage->Text = FloatToStrF(p->VoltLimit[1],ffFixed,10,3);
	ED_LowVoltage->Text = FloatToStrF(p->VoltLimit[0],ffFixed,10,3);

    ParamBuf.Params = *p;
}

void __fastcall TFormSysCfg::SetPreviewMode(bool Mode)
{
	this->SG_Config->Enabled = !Mode;
	this->ED_HeightVoltage->ReadOnly = Mode;
	this->ED_LowVoltage->ReadOnly = Mode;
	this->btnLoadDefault->Enabled = !Mode;
	this->btnSave->Enabled = !Mode;
	this->Caption = Mode?"系统配置预览":"系统配置管理";
	this->Label1->Caption = Mode?"当前配置预览":"当前配置";

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
	}
	else
	{
        s->Canvas->Brush->Color = cl3DLight;
	}

	s->Canvas->FillRect(Rect);

	if (ACol == 3 && ARow >0)
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

void __fastcall TFormSysCfg::FormActivate(TObject *Sender)
{
    LoadCfgData(m_param);
}
//---------------------------------------------------------------------------

void __fastcall TFormSysCfg::SG_ConfigSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect)
{
    TStringGrid * psg = dynamic_cast<TStringGrid*>(Sender);

	if(ACol == 0 || ACol == 3 || ARow == 0)
	{
        psg->Options >> goEditing;
	}
	else
	{
        psg->Options << goEditing;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormSysCfg::btnLoadDefaultClick(TObject *Sender)
{
	this->LoadCfgData(&DefaultParams);
    ParamBuf.Params = DefaultParams;
}
//---------------------------------------------------------------------------


void __fastcall TFormSysCfg::SG_ConfigSetEditText(TObject *Sender, int ACol, int ARow,
          const UnicodeString Value)
{
	//尝试检查是否是正常的浮点数.
	if(ACol == 0 || ACol ==  3 || ARow == 0)
	{
        return;
	}

	TStringGrid * psg = dynamic_cast<TStringGrid*>(Sender);

	if(psg == NULL) return;

	double xval = 0;
	try{
		xval = StrToFloat(psg->Cells[ACol][ARow]);
	}
	catch(...)
	{
		//ShowMessage(psg->Cells[0][ARow] + ":输入数据格式不正确");
		psg->Cells[ACol][ARow] = psg->Cells[ACol][ARow].Delete(psg->Cells[ACol][ARow].Length(),1);
		return;
	}

	//数据录入.
	ParamBuf.ValBuf[ARow-1][ACol-1] = xval;
}
//---------------------------------------------------------------------------


void __fastcall TFormSysCfg::ED_HeightVoltageChange(TObject *Sender)
{
	double xval = 0;
    TEdit* ped = dynamic_cast<TEdit*>(Sender);

	try{
		xval = StrToFloat(ped->Text);
	}
	catch(...)
	{
		//ShowMessage("高电平门限输入数据格式不正确");
		ped->Text = ped->Text.Delete(ped->Text.Length(),1);
		return;
	}

	ParamBuf.Params.VoltLimit[1] = xval;
}
//---------------------------------------------------------------------------

void __fastcall TFormSysCfg::ED_LowVoltageChange(TObject *Sender)
{
	double xval = 0;
	TEdit* ped = dynamic_cast<TEdit*>(Sender);

	try
	{
		xval = StrToFloat(ped->Text);
	}
	catch(...)
	{
		//ShowMessage("低电平门限输入数据格式不正确");
		ped->Text = ped->Text.Delete(ped->Text.Length(),1);
		return;
	}

	ParamBuf.Params.VoltLimit[0] = xval;
}
//---------------------------------------------------------------------------



#include "DataSouce.h"

void __fastcall TFormSysCfg::btnSaveClick(TObject *Sender)
{
	::Params para = ParamBuf.Params;
	*m_param = ParamBuf.Params;//para;//DefaultParams;//ParamBuf.Params;
	::SyncToMappdFile();
	ShowMessage("参数已保存");
}
//---------------------------------------------------------------------------

