// ---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

// ---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published: // IDE-managed Components

	TLabel *Label1;
	TLabel *label_StartInfo;
	TTimer *Timer1;

	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);

private: // User declarations
		public : // User declarations
	__fastcall TForm2(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
// ---------------------------------------------------------------------------
#endif
