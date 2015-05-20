//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "CustomButton1.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(MyRoundButton *)
{
	new MyRoundButton(NULL);
}
//---------------------------------------------------------------------------
__fastcall MyRoundButton::MyRoundButton(TComponent* Owner)
	: TButton(Owner)
{

}

void __fastcall MyRoundButton::CreateWnd()
{
	inherited::CreateWnd();
	UpdateRegion();
}

void __fastcall MyRoundButton::SetBounds(int ALeft, int ATop, intAWidth, int AHeight)
{
	inherited::SetBounds(ALeft, ATop, AWidth, AHeight);
	UpdateRegion();
}

void __fastcall MyRoundButton:UpdateRegion()
{
	if( HandleAllocated() )
	{
		// clear the previous region, if any
		SetWindowRgn(Handle, NULL, FALSE);

		HRGN hRgn = CreateEllipticRgn(0, 0, Width, Height);
		if( hRgn == NULL )
		{
			RaiseLastWin32Error();
		}
		if( !SetWindowRgn(Handle, hRgn, TRUE) )
		{
			DeleteObject(hRgn);
			RaiseLastWin32Error();
		}
	}
}

//---------------------------------------------------------------------------
namespace Custombutton1
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(MyRoundButton)};
		RegisterComponents(L"Samples", classes, 0);
	}
}
//---------------------------------------------------------------------------
