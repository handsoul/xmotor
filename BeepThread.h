//---------------------------------------------------------------------------

#ifndef BeepThreadH
#define BeepThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <vcl.h>
//---------------------------------------------------------------------------
class BeepThread : public TThread
{
private:
protected:
	TCriticalSection *m_lock;
	void __fastcall Execute();

	int BeepCnt; //Beep´ÎÊý.
	int BeepDuration;//ms
	int BeepInterval;//Beep¼ä¸ô.ms
	int BeepFreq;

	volatile bool m_bSuspended;
	void __fastcall BeepThread::BeepService(void);
public:

	void StartBeep(int repeatCnt = 1, int beepDuration = 200 , int beepInterval = 200 , int freq = 4000)
	{
		BeepCnt = repeatCnt;
		BeepDuration = beepDuration;
		BeepInterval = beepInterval;
		BeepFreq = freq;
		ThreadResume();
	}

	void ThreadSuspend(void)
	{
		if(m_bSuspended == false)
		{
			m_lock->Acquire();
			m_bSuspended = true;
		}
	}

	void ThreadResume(void)
	{
		if(m_bSuspended == true)
		{
			m_lock->Release();
			m_bSuspended = false;
		}
	}

	__fastcall BeepThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
