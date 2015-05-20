#include <SerialPort.h>

const int SerialPortBase::BaudRateList[5]={2400,4800,9600,38400,115200};//波特率列表.


SerialPortBase::SerialPortBase(void):
	m_iStartBits(DefaultStartBits),m_iStopBits(DefaultStopBits),m_iBardRate(DefaultBaudRate),m_iParityMode(DefaultParity),m_bPortOpen(false)
{
	pthread = new SerialPortThread(this);
	memset(&ovl,0,sizeof(ovl));
	memset(&ovr,0,sizeof(ovr));
	pHwin = NULL;
}

SerialPortBase::~SerialPortBase(void)
{
	ClosePort();
	if(pthread != NULL)
	{
		pthread->Terminate();
		pthread->WaitFor();
		delete pthread;
		pthread = NULL;
	}
}

bool SerialPortBase::SetBaudRate(int _baudRate)
{
	return false;
}

bool SerialPortBase::OpenPort(UnicodeString strCommName,int baud,int stopBits,int parity)
{
	// 打开串口
	HANDLE hdl = CreateFile(strCommName.c_str(),
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,//FILE_ATTRIBUTE_NORMAL, //正常模式.非异步.
							NULL
							);

	if (hdl == INVALID_HANDLE_VALUE)
	{
		//打开异常.
		return false;
	}

	this->m_hSerialPort = hdl;

	// 读取串口设置.
	if (GetCommState(hdl,&m_stSettings) == false)
	{
		CloseHandle(hdl);
		return false;
	}

	// 修改设置.
	m_stSettings.BaudRate = baud;//this->m_iBardRate;
	m_stSettings.fParity  = parity;//this->m_iParityMode;
	m_stSettings.StopBits = stopBits;//this->m_iStopBits;
	m_stSettings.ByteSize = 8;	// 数据长度.

	if (SetCommState(hdl,&m_stSettings) == false)
	{
		CloseHandle(hdl);
		return false;
	}

	//设置串口缓冲区
	if (SetupComm(hdl, 4096, 4096) == false)
	{
		CloseHandle(hdl);
        return false;
	}

	// 超时设置.
	if (GetCommTimeouts(hdl,&m_stTimeout) == false)
	{
		CloseHandle(hdl);
		return false;
	}

	// 事件设置.
	if(SetCommMask(hdl,EV_RXCHAR | EV_ERR | EV_CTS | EV_DSR | EV_BREAK | EV_TXEMPTY | EV_RING | EV_RLSD ) == false)//监控接收事件.
	{
		CloseHandle(hdl);
		return false;
	}

	m_stTimeout.ReadTotalTimeoutMultiplier = 2;
	m_stTimeout.ReadIntervalTimeout = 100;
   	m_stTimeout.ReadTotalTimeoutConstant	= 500;
	m_stTimeout.WriteTotalTimeoutMultiplier = 1;
	m_stTimeout.WriteTotalTimeoutConstant	= 1000;

	if (SetCommTimeouts(hdl,&m_stTimeout) == false )
	{
		CloseHandle(hdl);
		return false;
	}

	// 完成设置.
	// 清空缓冲区.
	PurgeComm(hdl,PURGE_TXABORT | PURGE_RXABORT |PURGE_TXCLEAR |PURGE_RXCLEAR );

	if (ovl.hEvent != NULL)
	{
		ResetEvent(ovl.hEvent);
	}
	else
	{
		ovl.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	}

	if (ovl.hEvent != NULL)
	{
        ResetEvent(ovr.hEvent);
	}
	else
	{
		ovr.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	}

	if (ovw.hEvent != NULL)
	{
		ResetEvent(ovw.hEvent);
	}
	else
	{
		ovw.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	}

	this->m_bPortOpen = true;
	//pthread->m_bTerminated = false;
	//this->pthread->Started

	return true;
}
void SerialPortBase::ClosePort(void)
{
	if(this->m_bPortOpen == true)
	{
		/*
		pthread->m_bTerminated = true;
		pthread->Terminate();
		pthread->WaitFor();
		*/
		this->m_bPortOpen = false;

		::Sleep(200);

		CloseHandle(this->m_hSerialPort);

	}
}
// 仅内部使用.实际通过线程读取.
bool SerialPortBase::ReadSerialPortInternal(void)
{
	static unsigned char _buf[1024]; // 临时缓冲区.

	DWORD dwError;
	DWORD iCount;
	DWORD dwFlags;

	if(this->m_bPortOpen == false)
	{
		return false;
	}

	/*if(ClearCommError(m_hSerialPort, &dwError, NULL) && dwError > 0)
		PurgeComm(m_hSerialPort, PURGE_RXABORT);

	if(WaitCommEvent(m_hSerialPort,&dwFlags,&ovw) == false)
	{
		if((dwError = ::GetLastError()) == ERROR_IO_PENDING)
			::GetOverlappedResult(m_hSerialPort, &ovw, &iCount, TRUE);
		else
			return false;
	}

	if( (dwError & EV_RXCHAR) == 0)
	{
		return false;//非接收事件
	} */

    WaitForSingleObject(

	if (ReadFile(this->m_hSerialPort,_buf,sizeof(_buf),&iCount,&ovr) == true)
	{
		if(iCount > 0)
		{
			std::vector<unsigned char>  * pv = new std::vector<unsigned char>;
			for(int i =  0;i < (int)iCount;i++)
			{
				pv->push_back(_buf[i]);
			}
			this->m_vRecvBuf.push(pv);
			return true;
		}
	}
	else
	{

		DWORD dwErr = GetLastError();
		if(dwErr == ERROR_IO_PENDING)
		{
			if(GetOverlappedResult(this->m_hSerialPort,&ovr,&iCount,TRUE) == TRUE)
			{
				if(iCount > 0)
				{
					std::vector<unsigned char>  * pv = new std::vector<unsigned char>;
					for(int i =  0;i < (int)iCount;i++)
					{
						pv->push_back(_buf[i]);
					}
					this->m_vRecvBuf.push(pv);
					return true;
				}
			}
			else
			{
				::Sleep(10);
			}
		}
	}

	return false;
}

bool SerialPortBase::WritePort(unsigned char * buf, int len)
{
	unsigned long iRealCnt = 0;
	DWORD iCount;
    DWORD dwError;

	if(this->m_bPortOpen == false)
	{
        return false;
	}

	if(ClearCommError(m_hSerialPort, &dwError, NULL) && dwError > 0)
		PurgeComm(m_hSerialPort, PURGE_TXABORT);

	WriteFile(this->m_hSerialPort,buf,len,&iCount,&ovl);

	if(iCount < len)
	{
		// 等待发送事件完成.
        GetOverlappedResult(m_hSerialPort,&ovl,&iRealCnt,true);
	}

	return true;
}

void SerialPortBase::ClearBuf(void)
{
	if(this->m_hSerialPort == false)
	{
		return ;
	}

	PurgeComm(this->m_hSerialPort,PURGE_TXABORT | PURGE_RXABORT |PURGE_TXCLEAR |PURGE_RXCLEAR );

	while(this->m_vRecvBuf.size() > 0)
	{
		delete this->m_vRecvBuf.front();
		this->m_vRecvBuf.pop();
	}

	while(this->m_vSndBuf.size() > 0)
	{
		delete this->m_vSndBuf.front();
		this->m_vRecvBuf.pop();
	}
}

int SerialPortBase::ReadPort(unsigned char * buf)
{
	if(this->m_vRecvBuf.size() == 0)
	{
		return 0;
	}

	int size = this->m_vRecvBuf.front()->size();
	for(int i = 0 ;i < size;i++)
	{
		buf[i] = (*this->m_vRecvBuf.front())[i];
	}

	delete this->m_vRecvBuf.front();
	this->m_vRecvBuf.pop();
	return size;
}

static int resume = 0;

void __fastcall SerialPortThread::Execute(void)
{
	while(this->Terminated == false)
	{
		if(this->m_pCom->ReadSerialPortInternal())
		{
			this->m_pCom->NotifyWindow();
		}
	}
}


// 枚举串口.
void EmurateSerialPorts(std::vector<UnicodeString> &v)
{
	int i = 0;
	wchar_t Name[50];
	char szPortName[50];
	LONG Status;
	DWORD dwIndex = 0;
	DWORD dwName;
	DWORD dwSizeofPortName;
	DWORD Type;
	HKEY hKey;
	UnicodeString strSerialList[256];  // 临时定义 256 个字符串组，因为系统最多也就 256 个
	const wchar_t * data_Set= L"HARDWARE\\DEVICEMAP\\SERIALCOMM\\";
	dwName = sizeof(Name);
	dwSizeofPortName = sizeof(szPortName);
	//long ret0 = (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKey));
	long ret0 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKey); //打开一个制定的注册表键,成功返回ERROR_SUCCESS即“0”值
	if(ret0 == ERROR_SUCCESS)
	{
		do
		{
			Status = RegEnumValue(hKey, dwIndex++, Name, &dwName, NULL, &Type, szPortName, &dwSizeofPortName);//读取键值
			if((Status == ERROR_SUCCESS)||(Status == ERROR_MORE_DATA))
			{
				strSerialList[i] = UnicodeString((wchar_t*)szPortName);       // 串口字符串保存
				//mm_ComRec->Lines->Add(strSerialList[i]);
				v.push_back(strSerialList[i]);
				i++;// 串口计数
			}

			dwName = sizeof(Name);
			dwSizeofPortName = sizeof(szPortName);
		} while((Status == ERROR_SUCCESS)||(Status == ERROR_MORE_DATA));

		RegCloseKey(hKey);
	}
 }
