// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
// ---------------------------------------------------------------------------
USEFORM("ShowStatus.cpp", OKRightDlg);
USEFORM("Unit1.cpp", Form1);
USEFORM("Unit4.cpp", Form4);
USEFORM("Unit3.cpp", FormSysCfg);
USEFORM("Unit2.cpp", Form2);
USEFORM("FormAuth.cpp", UnitFormAuth);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		//Application->CreateForm(__classid(TForm4), &Form4);
		Application->CreateForm(__classid(TForm1), &Form1);
		Application->CreateForm(__classid(TOKRightDlg), &OKRightDlg);
		Application->CreateForm(__classid(TFormSysCfg), &FormSysCfg);
		Application->CreateForm(__classid(TUnitFormAuth), &UnitFormAuth);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
// ---------------------------------------------------------------------------
