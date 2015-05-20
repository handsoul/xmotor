//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitSerialPort.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformSerial *formSerial;
//---------------------------------------------------------------------------
__fastcall TformSerial::TformSerial(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
