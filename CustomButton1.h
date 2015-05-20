//---------------------------------------------------------------------------

#ifndef CustomButton1H
#define CustomButton1H
//---------------------------------------------------------------------------
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE MyRoundButton : public TButton
{
typedef TButton inherited;
private:
	void __fastcall UpdateRegion();
protected:
	virtual void __fastcall CreateWnd();
public:
	__fastcall MyRoundButton(TComponent* Owner);
	virtual void __fastcall SetBounds(int ALeft, int ATop, int AWidth,int AHeight);
__published:
};
//---------------------------------------------------------------------------
#endif
