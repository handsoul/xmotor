//---------------------------------------------------------------------------


#include <System.hpp>
#include <windows.h>
#pragma hdrstop

#include "BeepThread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall BeepThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall BeepThread::BeepThread(bool CreateSuspended)
	: TThread(false),BeepCnt(0),BeepFreq(4000),BeepDuration(200),BeepInterval(200)
{
	m_lock = new TCriticalSection;
	m_bSuspended = false;
	ThreadSuspend();
}

void __fastcall BeepThread::BeepService(void)
{
	while(BeepCnt-- > 0)
	{
		//PlaySound(L"C:\Windows\Media\Heritage\Windows Error.wav",NULL,SND_SYNC);
        Beep(BeepFreq,BeepDuration);
		::Sleep(BeepInterval);
    }
}

//---------------------------------------------------------------------------
void __fastcall BeepThread::Execute()
{
	//---- Place thread code here ----
    while(this->Terminated == false)
	{
		BeepService();
        m_lock->Acquire();
	}
}
//---------------------------------------------------------------------------
