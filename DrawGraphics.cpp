#include "DrawGraphics.h"
#include <queue>

Gdiplus::GdiplusStartupInput DataGrids::m_stGdiplusStartupInput; // ��������.
ULONG_PTR  DataGrids::m_pulGdiplusToken = static_cast<ULONG_PTR>(0);
bool DataGrids::m_bInitialized = false;

void DataGrids::StartGdiProcess(void)
{
	if (m_bInitialized == false)
	{
		try
		{
			GdiplusStartup(&m_pulGdiplusToken, &m_stGdiplusStartupInput, NULL);
		}
		catch(...)
		{
			m_bInitialized = false;
			return;
		}
		m_bInitialized = true;
	}
}

void DataGrids::EndGdiProcess(void)
{
	if (m_bInitialized ==true)
	{
		Gdiplus::GdiplusShutdown(m_pulGdiplusToken);
		m_bInitialized = false;
	}
}

DataGrids::DataGrids(HDC _hdc,int width , int height , double _duty,double hratio,double lratio,DataItemSet * pD)
    :m_iHeight(height),m_iWidth(width),m_fDuty(_duty),m_fHRatio(hratio),m_fLRatio(lratio)
{
}



DataGrids::~DataGrids(void)
{
    delete m_stGrp;
}

void DataGrids::SetDataSet(DataItemSet * pD)
{

}

// ������.
void DataGrids::DrawGrids(HDC _hdc)
{
	// ����x/y�ļ�ͷ.
	WideString xStr = "��ѹ(V)";
	WideString yStr = "ʱ��(S)";

	m_stGrp = new Gdiplus::Graphics(_hdc);
	m_stGrp->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	/*FontFamily fontFamily(L"����_GB2312");
	Font font(&fontFamily, 30, FontStyleRegular, UnitPixel);
	*/
	Gdiplus::SolidBrush solidBrush(Gdiplus::Color(255, 0, 0, 255));
	Gdiplus::Pen p(Gdiplus::Color(255, 0, 0, 0),1);
	Gdiplus::AdjustableArrowCap cap(8,4,true);
	Gdiplus::Font font(L"����",12);
	Gdiplus::SolidBrush s( Gdiplus::Color(255, 0, 0, 0));

#define X(_x) UserX((_x)+xAxisOffset)
#define Y(_y) UserY((_y)+yAxisOffset)
	// �̶���.Y��.
	p.SetColor(Gdiplus::Color(255, 0, 0, 255));
	for(int i = 0; i * yMarkSpace < m_iHeight-yMarkSpace-8;i++)
	{
		m_stGrp->DrawLine(&p,X(0),Y(0+i*yMarkSpace),X(yMarkHeight+(((i%5)==0)?yMarkHeight:0)),Y(0+i*yMarkSpace));
	}

	for(int i = 0; i * yMarkSpace < m_iWidth-xMarkSpace-8;i++)
	{
		m_stGrp->DrawLine(&p,X(i*xMarkSpace),Y(0),X(i*xMarkSpace),Y(xMarkHeight+((i%5)==0?xMarkHeight:0)));
	}
	p.SetColor(Gdiplus::Color(255, 0, 0, 0));
	// Draw X,Y axises.
	p.SetCustomEndCap(&cap);
	m_stGrp->DrawLine(&p,X(0),Y(0),X(0),Y(m_iHeight)); // Draw Y axis
	m_stGrp->DrawLine(&p,X(0),Y(0),X(m_iWidth),Y(0));  // Draw X axis

	m_stGrp->DrawString(xStr.c_bstr(),xStr.Length(),&font,Gdiplus::PointF(30,10),&s);
	m_stGrp->DrawString(yStr.c_bstr(),yStr.Length(),&font,Gdiplus::PointF(m_iWidth-80,m_iHeight-50),&s);

#undef X(_x)
#undef Y(_y)

	DrawCurve();

	delete m_stGrp;
}
// ���߻���.
void DataGrids::DrawCurve(void)
{

#define SHOW_LOWLEVEL 0

#define X(_x) UserX((_x)+xMarginLeft)
#define Y(_y) UserY((_y)+yMarginDown)

	WideString strList[] =
	{
		L"Vl(�͵�ƽ��ѹ)",
		L"Vh(�ߵ�ƽ��ѹ)",
		L"Tr(������)",
		L"Th(�ߵ�ƽ)",
		L"Tf(�½���)",
		L"Tl(�͵�ƽ)",
	};

	Gdiplus::SolidBrush solidBrush(Gdiplus::Color(255, 0, 0, 255));
	Gdiplus::Pen pX(Gdiplus::Color(255, 0, 0, 0),1);
	Gdiplus::AdjustableArrowCap cap(8,6,true);
	Gdiplus::Font font(L"Times New Roman",8);
	Gdiplus::SolidBrush s( Gdiplus::Color(255, 0, 0, 0));
	Gdiplus::PointF *pComm = new Gdiplus::PointF[4];

	// ������.�½��ص�б��.
	Gdiplus::Pen p(Gdiplus::Color(255, 0, 0, 255),2);
	//�Ƚ�������.
	Gdiplus::PointF *points = new Gdiplus::PointF[10];

	// �͵�ƽλ��Ϊ���ø߶ȵ� %20.
	// �ߵ�ƽλ��λ���ø߶ȵ� %80.
	// ռ�ձ�λ%50.
	// ������/�½���ʹ�õ�%5��λ��.
	// �͵�ƽ�ȿ�ʼ.
	// ��ʼ����.(xMarkHeight + 20 , yMarksHeigh + 20).
	// ��������.(width - 20, heigh - 20).

	double fLvPos = 0.1 + 0.1 * m_fLRatio / 100, fHvPos = 0.4 + 0.4 * m_fHRatio/100, fHvLen = (m_fDuty*0.8/100.0), fLvLen = ((100-m_fDuty)*0.8/100), fRiseLen = 0.05,fFallLen= 0.05;
	double xMarginLeft = xMarkHeight + 30;
	double yMarginDown = yMarkHeight + 20;
	double xMarginRight  = 20;
	double yMarginTop  = 20;
	double RealWidth = m_iWidth - xMarginLeft - xMarginRight;
	double RealHeight = m_iHeight - yMarginDown - yMarginTop;
	int idx = 0;
	points[idx++] = Gdiplus::PointF((int)X(0),(int)Y(fLvPos * RealHeight));
	points[idx++] = Gdiplus::PointF((int)X(fHvLen * RealWidth),(int)Y(fLvPos * RealHeight));
	points[idx++] = Gdiplus::PointF((int)X((fHvLen + fRiseLen)* RealWidth),(int)Y(fHvPos * RealHeight));
	points[idx++] = Gdiplus::PointF((int)X((fHvLen + fRiseLen + fLvLen)* RealWidth),(int)Y(fHvPos * RealHeight));
	points[idx++] = Gdiplus::PointF((int)X((fHvLen + fRiseLen + fLvLen + fFallLen)* RealWidth),(int)Y(fLvPos * RealHeight));
	points[idx++] = Gdiplus::PointF((int)X(RealWidth),(int)Y(fLvPos * RealHeight));

	m_stGrp->DrawLines(&p,points,idx);

	//����λ����.
	pX.SetDashStyle(Gdiplus::DashStyleDash);
	pX.SetAlignment(Gdiplus::PenAlignmentCenter);
	pX.SetDashOffset(20.0);

	// P0 ���. P1 �����ص����. P2�����ؽ�����. P3 �½������. P4 �½��صĽ�����. P5 ������.
#if SHOW_LOWLEVEL
	pComm[0].X = (int)points[0].X;
	pComm[0].Y = (int)points[0].Y;
	pComm[1].X = pComm[0].X;
	pComm[1].Y = UserY(m_iHeight - yMarginTop);
	m_stGrp->DrawLines(&pX,pComm,2);
#endif

	pComm[0].X = (int)points[1].X;
	pComm[0].Y = (int)points[1].Y;
	pComm[1].X = pComm[0].X;
	pComm[1].Y = UserY(m_iHeight - yMarginTop);
	m_stGrp->DrawLines(&pX,pComm,2);

	pComm[0].X = (int)points[2].X;
	pComm[0].Y = (int)points[2].Y;
	pComm[1].X = pComm[0].X;
	pComm[1].Y = UserY(m_iHeight - yMarginTop - 35);
	m_stGrp->DrawLines(&pX,pComm,2);

	pComm[0].X = (int)points[3].X;
	pComm[0].Y = (int)points[3].Y;
	pComm[1].X = pComm[0].X;
	pComm[1].Y = UserY(m_iHeight - yMarginTop - 35);
	m_stGrp->DrawLines(&pX,pComm,2);

	pComm[0].X = (int)points[4].X;
	pComm[0].Y = (int)points[4].Y;
	pComm[1].X = pComm[0].X;
	pComm[1].Y = UserY(m_iHeight - yMarginTop);
	m_stGrp->DrawLines(&pX,pComm,2);

	// ׼�����ʵļ�ͷ.
	cap.SetFillState(false);
	pX.SetCustomEndCap(&cap);
	pX.SetCustomStartCap(&cap);


	// Vl -- .P0~P1�е�λ�ÿ�ʼ.��������--˫��ͷ.
	pComm[0].X = (int)(points[0].X + points[1].X) / 2;
	pComm[0].Y = (int)points[0].Y;
	pComm[1].X = pComm[0].X;
	pComm[1].Y = UserY(xAxisOffset);

	m_stGrp->DrawLines(&pX,pComm,2);
	m_stGrp->DrawString(strList[0].c_bstr(),strList[0].Length(),&font,Gdiplus::PointF(pComm[0].X + 10 ,(pComm[0].Y + pComm[1].Y)/2),&s);

	// Vh -- .P2~P3�е�λ�ÿ�ʼ.��������--˫��ͷ.
	// �ߵ�ƽ��.
	pComm[0].X = (int)(points[2].X + points[3].X) / 2;
	pComm[0].Y = (int)points[2].Y;
	pComm[1].X = pComm[0].X;
	pComm[1].Y = UserY(xAxisOffset);
	m_stGrp->DrawLines(&pX,pComm,2);
	m_stGrp->DrawString(strList[1].c_bstr(),strList[1].Length(),&font,Gdiplus::PointF(pComm[0].X + 10 ,(pComm[0].Y + pComm[1].Y)/2),&s);

	//
#if SHOW_LOWLEVEL
	pComm[0].X =  points[0].X;
	pComm[0].Y =  UserY(m_iHeight - yMarginTop - 30);
	pComm[1].X =  points[1].X;
	pComm[1].Y =  UserY(m_iHeight - yMarginTop - 30);
	m_stGrp->DrawLines(&pX,pComm,2);
	m_stGrp->DrawString(strList[5].c_bstr(),strList[5].Length(),&font,Gdiplus::PointF((pComm[0].X + pComm[1].X)/2 -30 , pComm[0].Y + 10 ),&s);
#endif

	pComm[0].X =  points[1].X;
	pComm[0].Y =  UserY(m_iHeight - yMarginTop - 40);
	pComm[1].X =  points[2].X;
	pComm[1].Y =  UserY(m_iHeight - yMarginTop - 40);
	m_stGrp->DrawLines(&pX,pComm,2);
	m_stGrp->DrawString(strList[2].c_bstr(),strList[2].Length(),&font,Gdiplus::PointF(pComm[0].X + 5 , pComm[0].Y - 30 ),&s);

	pComm[2].X =  points[3].X;
	pComm[2].Y =  UserY(m_iHeight - yMarginTop - 40);
	pComm[3].X =  points[4].X;
	pComm[3].Y =  UserY(m_iHeight - yMarginTop - 40);
	m_stGrp->DrawLines(&pX,pComm+2,2);
	m_stGrp->DrawString(strList[4].c_bstr(),strList[4].Length(),&font,Gdiplus::PointF(pComm[2].X - 30 , pComm[2].Y - 20 ),&s);

	pComm[1].Y =  pComm[1].Y + 10;
	pComm[2].Y =  pComm[1].Y;
	m_stGrp->DrawLines(&pX,pComm+1,2);
	m_stGrp->DrawString(strList[3].c_bstr(),strList[3].Length(),&font,Gdiplus::PointF((pComm[1].X + pComm[2].X)/2 - 30, pComm[2].Y + 15 ),&s);

	delete points;
	delete pComm;
#undef X(_x)
#undef Y(_y)
}

int GetVal(double duty, double freq,double t)//��λ�á��ʾ.
{
	double period;

	freq = (freq <1E-2)? 1E-2:(freq > 1E4) ? 1E4:freq;

	period = 1/freq;

	duty = (duty > 100) ? 100: (duty < 0) ? 0:duty;

	t =  t - ((int)( t / period )) * period;

	double tr = period * duty / 100;

	if(t < tr)
		return 1;
	else
		return 0;

}

#define GetPosition(x) ((x == 0)? (((m_fLRatio + 100)/100) * 0.1 * m_iHeight):((((m_fHRatio/100) * 0.1 + 0.8)* m_iHeight)))

// ���߻���.
void DataGrids::DrawWave(HDC _hdc , double duty , double freq , double step,double startPos , double dt ,double phase)
{
	Gdiplus::PointF ps,pe;
	int state = GetVal(duty,freq,0);

	Gdiplus::Pen pen(Gdiplus::Color(0,0,255),1);
	Gdiplus::Pen penr(Gdiplus::Color(0,0,0),1);

	Gdiplus::Graphics g(_hdc);

	//g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	step = (step < 1) ? 1 : step ;
	startPos = (startPos > m_iWidth) ? m_iWidth:startPos;

	ps = Gdiplus::PointF(startPos,GetPosition(state));
	for(int i =0 ;i < (m_iWidth - startPos) / step ; i++)
	{
		state = GetVal(duty,freq,(i + phase)*dt);
		pe = Gdiplus::PointF(i*step + startPos ,GetPosition(state));
		g.DrawLine(&pen,ps,pe);
		ps = pe;
	}

	g.DrawRectangle(&penr,0,0,m_iWidth-1,m_iHeight-1);
}
