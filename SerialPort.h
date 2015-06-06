#ifndef __SERIAL_PORT_H__
#define __SERIAL_PORT_H__

#include <Windows.h>
#include <vcl.h>
#include <queue>
#include <vector>

#define MSG_RECV_COMMDATA	(WM_USER + 618)

// ǰ������.
class SerialPortBase;

class SerialPortThread : public TThread
{
protected:
	SerialPortBase *m_pCom;
	TCriticalSection *m_lock;
	void __fastcall Execute(void);


public:

	volatile bool m_bSuspended;
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
		//ev
	}

	SerialPortThread(SerialPortBase *pCom)
		:TThread(false),m_pCom(pCom),m_bSuspended(false)
	{
		//FreeOnTerminate =true;
		m_lock = new TCriticalSection;
		//pev	   = new TEvent(NULL,

		m_bSuspended = false;
	}

};

//ʹ�ú�̨�߳��շ�����.
class SerialPortBase
{

public:
	static const int  BaudRateList[5];//={2400,4800,9600,38400,115200};//�������б�.
	static const int DefaultStartBits = 1;
	static const int DefaultStopBits = ONESTOPBIT;
	static const int DefaultParity	 = 0; // ��żУ��λ.
	static const int DefaultBaudRate = 9600;
	static const int DefaultSize;

protected:
	HANDLE m_hSerialPort; // ���ھ��.
	COMMTIMEOUTS m_stTimeout; //��ʱ����
    SerialPortThread *pthread;

	std::queue< std::vector<unsigned char> * > m_vRecvBuf;// ���ջ�����.
	std::queue< std::vector<unsigned char> * > m_vSndBuf; // ���ͻ�����.

	bool m_bPortOpen;
	OVERLAPPED ovl;
	OVERLAPPED ovr;
	OVERLAPPED ovw;

	int m_iStartBits;// ��ʼλ.
	int m_iStopBits; // ֹͣλ.
	int m_iBardRate; // ������.
	int m_iParityMode;//��żУ��λģʽ.
	HWND pHwin;

	DCB m_stSettings;

public :
	SerialPortBase::SerialPortBase(void);
	bool 	IsPortOpen(void) {return m_bPortOpen;}
	bool 	ReadSerialPortInternal(void);
	bool	SetBaudRate(int _baudRate = 9600);
	bool 	OpenPort(UnicodeString strCommName="COM5",int baud=9600,int stopBits=0,int parity=0);
	void 	ClosePort(void);
	int		ReadPort(unsigned char * buf);
	bool 	WritePort(unsigned char * buf,int len);
	void 	ClearBuf(void);
	void 	SetNotifyWindow(HWND  h)
	{
		pHwin = h;
	}

	void NotifyWindow(void)
	{
		if(pHwin != NULL && ::IsWindow(pHwin))//���ھ����Ч
		{
			PostMessage(pHwin, MSG_RECV_COMMDATA, 0, 0);
		}
	}

	~SerialPortBase(void);
};


void EmurateSerialPorts(std::vector<UnicodeString> &v);

#endif
