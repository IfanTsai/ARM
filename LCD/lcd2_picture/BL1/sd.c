#include "sd.h"
typedef unsigned int bool;

// ͨ���ţ�0������2
// ��ʼ�����ţ�45
// ��ȡ����������2048
// ��ȡ������ڴ��ַ��0x23E00000
// with_init��0
typedef bool(*pCopySDMMCtoMem)(int, unsigned int, unsigned short, unsigned int*, bool);
typedef void(*jump)();

// ��SD����45������ʼ������2048���������ݵ�DDR��0x23E00000��Ȼ����ת��23E00000ȥִ��
void copy_bl2_to_ddr()
{
	// ��ȡSD��������DDR��
	pCopySDMMCtoMem p1 = (pCopySDMMCtoMem)(*((unsigned int *)0xD0037F98));
	p1(2, SD_START_BLOCK, SD_BLOCK_CNT, (unsigned int *)DDR_START_ADDR, 0);	

	//����BL1��BL2�Ƿֿ������γ�����޷�ʹ��ldr��ת��
	//����ʹ�ú������õķ�������ת
	jump p2 = (jump)DDR_START_ADDR;
	p2();
}






















