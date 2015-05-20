#ifndef __SERIAL_COMM_H__
#define __SERIAL_COMM_H__

#include "SerialPort.h"
#include <queue>


#define MAX_DATA_SIZE 40
#define HEAD_LEN	  2

typedef struct tagSerialFrame
{
	unsigned char m_ucHead[HEAD_LEN]; 	//ͷ��.
	unsigned char m_ucLength;  			//���ݲ��ֳ���.
	unsigned char m_ucCmd;     			//����.
    unsigned char m_ucData[MAX_DATA_SIZE];
	unsigned char m_ucChecksum;			//У���.

}SERIAL_FRAME_ST;

// �ӽ��������еõ�������֡.
bool GetFrames(SERIAL_FRAME_ST *frames, unsigned char * buf, unsigned len);

#endif
