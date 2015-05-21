//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit4.h"
#include "Unit1.h"
#include <vector>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
	: TForm(Owner)
{
	this->m_pCom == NULL;
	m_pcnCom == NULL;
}
//---------------------------------------------------------------------------
void __fastcall TForm4::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	//this->CanClose = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm4::FormClose(TObject *Sender, TCloseAction &Action)
{
	if(m_pCom != NULL)
		delete m_pCom;
}
//---------------------------------------------------------------------------

void __fastcall TForm4::OnSndMessage(TMessage & msg)
{
	if(msg.Msg == MSG_RECV_COMMDATA + 1)
	{
    	mm_ComRec->Lines->Add("收到数据发送请求");
	}
}

void __fastcall TForm4::btnOpenSerialPortClick(TObject *Sender)
{

	if( m_pCom->IsPortOpen() == false)
	{
		if  (this->m_pCom->OpenPort(cbSerialPorts->Text,115200) == false)
		{
			ShowMessage("打开失败");
		}
		else
		{
			btnOpenSerialPort->Caption = "关闭";
			this->m_pCom->SetNotifyWindow(this->Handle);
		}
	}
	else
	{
		this->m_pCom->ClosePort();
		btnOpenSerialPort->Caption = "打开";
	}
}

void __fastcall TForm4::OnRecvMessage(TMessage & msg)
{
	static unsigned char sbuf[1024];
	int len = 0;
	UnicodeString s = "";

	if(msg.Msg == MSG_RECV_COMMDATA)
	{
		if (this->m_pCom == NULL || this->m_pCom->IsPortOpen() == false)
		{
			return;
		}

		if((len = this->m_pCom->ReadPort(sbuf)) > 0)
		{
			s = "接收数据";

			for(int i =0  ;i < len;i++)
			{
				s += IntToHex(sbuf[i],2) + " ";
			}
			ParseData(sbuf,len);
			this->mm_ComRec->Lines->Add(s);
		}
	}
}


bool __fastcall TForm4::SendMsg(unsigned char * pbuf, int len)
{
	if(this->m_pCom == NULL || this->m_pCom->IsPortOpen() == false)
	{
		return false;
	}

	UnicodeString s = "发送数据:";


	if (this->m_pCom->WritePort(pbuf,len))
	{
		for(int i =0  ;i < len ;i ++)
		{
			s += IntToHex(pbuf[i],2) + " ";
		}
		mm_ComRec->Lines->Add(s);
		return true;
	}
	else
	{
		s = "发送失败";
		mm_ComRec->Lines->Add(s);
		return false;
	}

	return false;
}


#define _MAKEWORD(H,L) (unsigned short)((((unsigned char)H) << 8) | ((unsigned char)L))
#define _MAKELONG(a,b,c,d) (unsigned int)(((a)<<24)|((b)<<16)|((c)<<8)|((d)<<0))


const double fusScale  = 1.0f/16;
const double DutyScale = 1024;
const double voltScale = 5000/(4096*2.65);

bool TForm4::ParseData(unsigned char * buf,unsigned int len)
{

	const double cycles[3] = {4,96,1000};

	unsigned char checksum;
	double ds[12];

	//对数据进行校验.
	if(len < (2 + 1 + 1 + 2))
	{
		return false; //数据长度较短.
	}

	if( buf[0] != 0xAA || buf[1] != 0x55)
	{
		mm_ComRec->Lines->Add("帧头错误");
		return false; // 帧头错误.
	}

	if( buf[2] > len - 3) // 长度不够.
	{
		mm_ComRec->Lines->Add("长度错误");
		return false;
	}


	for(int i = 3 ;i < len -1 ;i++)
	{
		checksum += buf[i];
	}

	if(checksum != buf[len-1])
	{

	//	return false; // 校验错误.
	}

	// 分解数据.
	if(len < 3 + 1 + 1) // 数据长度不够.
	{
		mm_ComRec->Lines->Add("长度不足");
		return false;
	}

    // 跳过命令字节.
	for(int i = 0 ;i< 12;i+=2)
	{
        ds[i] = _MAKEWORD(buf[1 +(3+i)],buf[1 + (3+i+1)]);
	}

	unsigned char * pd = buf + 3;

	if((*pd >= 1) && (*pd <=3))
	{
		mm_ComRec->Lines->Add("启动模式" + IntToStr((*pd))+"成功");
	}
	else if((*pd >= 0x11) && (*pd <= 0x13)) //返回数据.
	{

		unsigned char Mode = *pd - 0x11;

		if(len < 51)
		{
			mm_ComRec->Lines->Add("数据长度不够.预期=51,实际长度 = " + IntToStr((int)len));
			return false;
		}

		pd ++;

		double pmax,pmin,pa,dmax,dmin,da,lmin,lmax,la,hmin,hmax,ha,rmax,rmin,ra,Fmax,Fmin,Fa;

		hmax = _MAKEWORD(pd[0],pd[1]);
		pd += 2;

		ha   = _MAKEWORD(pd[0],pd[1]);
		pd += 2;

		hmin = 1200;//固定值.

		lmax = 800;

		lmin = _MAKEWORD(pd[0],pd[1]);
		pd += 2;

		la 	 = _MAKEWORD(pd[0],pd[1]);
		pd += 2;

		pmax = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		pmin = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		pa   = _MAKELONG(pd[0],pd[1],pd[2],pd[3]) / cycles[Mode];
		pd += 4;


		rmax = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		rmin = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		ra   = _MAKELONG(pd[0],pd[1],pd[2],pd[3]) / cycles[Mode];
		pd += 4;

		Fmax = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		Fmin = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		Fa   = _MAKELONG(pd[0],pd[1],pd[2],pd[3]) / cycles[Mode];
		pd += 4;

		dmax = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		dmin = _MAKELONG(pd[0],pd[1],pd[2],pd[3]);
		pd += 4;
		da   = _MAKELONG(pd[0],pd[1],pd[2],pd[3]) / cycles[Mode];
		pd += 4;


		std::vector<double> vd ;
		vd.push_back(60000000.0/pmax*fusScale);
		vd.push_back(60000000.0/pa*fusScale);
		vd.push_back(60000000.0/pmin*fusScale);
		vd.push_back(dmax/DutyScale);
		vd.push_back(da/DutyScale);
		vd.push_back(dmin/DutyScale);
		vd.push_back(lmax);
		vd.push_back(la*voltScale);
		vd.push_back(lmin*voltScale);
		vd.push_back(hmax*voltScale);
		vd.push_back(ha*voltScale);
		vd.push_back(hmin);
		vd.push_back(rmax*fusScale);
		vd.push_back(ra*fusScale);
		vd.push_back(rmin*fusScale);
		vd.push_back(Fmax*fusScale);
		vd.push_back(Fa*fusScale);
		vd.push_back(Fmin*fusScale);


		// 查询数据内容.
		Form1->UpdateDateItemByVector(vd);
	}
	else
	{
		mm_ComRec->Lines->Add("未识别的命令码:0x" + IntToHex(*pd,2));
	}

	return true;
}

//----------------------------------------------  -----------------------------
void __fastcall TForm4::Timer1Timer(TObject *Sender)
{
	// 刷新数据.
#if 0
	static unsigned char sbuf[1024];
	int len = 0;
	UnicodeString s = "";
	if (this->m_pCom == NULL || this->m_pCom->IsPortOpen() == false)
	{
		return;
	}

	while((len = this->m_pCom->ReadPort(sbuf)) > 0)
	{
		s = "接收数据";

		for(int i =0  ;i < len;i++)
		{
			s += IntToHex(sbuf[i],2) + " ";
		}


		ParseData(sbuf,len);

		this->mm_ComRec->Lines->Add(s);
	}
#endif
}
//---------------------------------------------------------------------------

void __fastcall TForm4::mm_SendBoxKeyPress(TObject *Sender, System::WideChar &Key)

{
	//只能输入16进制数据.

	if(!((Key >= '0' && Key <= '9')
		||(Key >= 'a' && Key <= 'f')
		||(Key >= 'A' && Key <= 'F')
		||(Key == ' ')
		||(Key == '\n')
		||(Key == '\t')
		||(Key == VK_DELETE)
		||(Key == VK_LEFT)
		||(Key == VK_RIGHT)
        ||(Key == '\r')))
	{
        Key = '\0';
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm4::btnSendDataClick(TObject *Sender)
{
	unsigned char buf[1024];
	//
	if(this->m_pCom == NULL || this->m_pCom->IsPortOpen() == false)
	{
        return;
	}

	// 解析数据并准备发送.
	UnicodeString s = this->mm_SendBox->Text;
	s = StringReplace(s,"\t","",TReplaceFlags()<<rfReplaceAll);
	s = StringReplace(s,"\r","",TReplaceFlags()<<rfReplaceAll);
	s = StringReplace(s,"\n","",TReplaceFlags()<<rfReplaceAll);
	s = StringReplace(s," ","",TReplaceFlags()<<rfReplaceAll);

	if(s.Length()%2 > 0)
	{
        s += "0"; // 补全.
	}

	for(int i = 0 ;i < (int)s.Length() && i < (int)sizeof(buf);i+=2)
	{
		unsigned int x = StrToIntDef("$"+s.SubString0(i,2),0x100);//StrToIntDef(s.SubString(i+1,2));

		if(x == 0x100)
		{
			ShowMessage("Contains Invalid Char");
            return;
		}
        buf[i/2] = x;
	}

    this->m_pCom->WritePort(buf,s.Length()/2);
}
//---------------------------------------------------------------------------

void __fastcall TForm4::FormCreate(TObject *Sender)
{
	// 添加各种选项.
	 //= new SerialPortBase();

	 std::vector<UnicodeString> vcom;

	 this->m_pCom = new SerialPortBase();

	 mm_ComRec->Lines->Clear();
	 mm_SendBox->Lines->Clear();

	 this->Timer1->Interval = 30;
	 this->Timer1->Enabled = true;

	 cbSerialPorts->Items->Clear();

	 // 枚举串口

     EmurateSerialPorts(vcom);

	 for(int i = 0 ;i < vcom.size();i++)
	 {
		cbSerialPorts->Items->Add(vcom[i]);
	 }
	 cbSerialPorts->ItemIndex = -1;


	 for(int i = 0;i < sizeof(SerialPortBase::BaudRateList)/sizeof(SerialPortBase::BaudRateList[0]);i++)
	 {
		cbBaudRate->Items->Add(IntToStr(SerialPortBase::BaudRateList[i]));
	 }
	 cbBaudRate->ItemIndex = 2;

	 cbParity->Items->Add("无");
	 cbParity->Items->Add("奇校验");
	 cbParity->Items->Add("偶校验");

	 cbParity->ItemIndex = 2;

	 cbStopBits->Items->Add("1");
	 cbStopBits->Items->Add("1.5");
	 cbStopBits->Items->Add("2");

	 cbStopBits->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button1Click(TObject *Sender)
{
//枚举串口.

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
		i++;// 串口计数
   }
   dwName = sizeof(Name);
   dwSizeofPortName = sizeof(szPortName);
  } while((Status == ERROR_SUCCESS)||(Status == ERROR_MORE_DATA));

  RegCloseKey(hKey);
 }
}
//---------------------------------------------------------------------------

