#ifndef __SERIAL_COMM_H__
#define __SERIAL_COMM_H__

#include "SerialPort.h"
#include <queue>


#define MAX_DATA_SIZE 40
#define HEAD_LEN	  2

typedef struct tagSerialFrame
{
	unsigned char m_ucHead[HEAD_LEN]; 	//头部.
	unsigned char m_ucLength;  			//数据部分长度.
	unsigned char m_ucCmd;     			//命令.
    unsigned char m_ucData[MAX_DATA_SIZE];
	unsigned char m_ucChecksum;			//校验和.

}SERIAL_FRAME_ST;

// 从接收数据中得到解析的帧.
bool GetFrames(SERIAL_FRAME_ST *frames, unsigned char * buf, unsigned len);

#endif
