#ifndef __DRAW_GRAPHICS_H__
#define __DRAW_GRAPHICS_H__

#include <vcl.h>
#include <windows.h>
#include <algorithm>
#include <gdiplus.h>
#include "DataSouce.h"

class DataGrids
{
	//��������.
protected:
	const static int xMarkHeight = 8;
	const static int yMarkHeight = 8;
	const static int xAxisOffset = 4;
	const static int yAxisOffset = 4;
	const static int xMarkSpace = 15;
    const static int yMarkSpace = 15;

	static Gdiplus::GdiplusStartupInput m_stGdiplusStartupInput; // ��������.
	static ULONG_PTR  m_pulGdiplusToken;
	static bool m_bInitialized;

	inline int UserX(int x) {return (x < 0)? 0:(x > m_iWidth) ? m_iWidth:x;}
	inline int UserY(int y) {return (y < 0)? m_iHeight:(y > m_iHeight) ? 0:(m_iHeight-y);}

protected:
	DataItemSet *m_pstDataSet; // ��Ҫ���Ƶ�����.
	Gdiplus::Graphics* m_stGrp; // ����.
	int m_iHeight;
	int m_iWidth;
	double m_fDuty;				// Ĭ�ϵ�ռ�ձ�.
	double m_fHRatio;
	double m_fLRatio;
public:
	void DrawGrids(HDC _hdc);		// ������.
	void DrawCurve(void); 		// ��������.

public:
	inline void SetDuty(double _duty){ if (_duty <= 100 && _duty >= 1E-5) m_fDuty = _duty;}
	inline void SetHvScale(double Ratio){ if (Ratio <= 100 && Ratio >= 1E-5) m_fHRatio = Ratio;}
	inline void SetLvScale(double Ratio){ if (Ratio <= 100 && Ratio >= 1E-5) m_fLRatio = Ratio;}
	inline double GetDuty(void) const {return m_fDuty;}
	inline double GetHvRatio(void) const {return m_fHRatio;}
	inline double GetLvRatio(void) const {return m_fLRatio;}
	static void StartGdiProcess(void);
	static void EndGdiProcess(void);
	DataGrids(HDC _hdc,int width,int height,double _duty = 50,double hratio = 100 ,double lratio =100,DataItemSet * pD = NULL);
	~DataGrids(void);
	void SetDataSet(DataItemSet * pD);
};

#endif
