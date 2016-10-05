#include <stdio.h>
#include "interrupt.h"

//���쳣�������ҳ�ʼ���жϿ�����
void system_init_exception(void)
{
	EXCEPTION_IRQ  = (unsigned int)irq_handle;
	EXCEPTION_FIQ = (unsigned int)irq_handle;
	
	// ��ʼ���жϿ������Ļ����Ĵ���
	intc_init();
}

// �����Ҫ������жϵ��жϴ������ĵ�ַ
void intc_clearvectaddr(void)
{
    VIC0ADDR = 0;
    VIC1ADDR = 0;
    VIC2ADDR = 0;
    VIC3ADDR = 0;
}

// ��ʼ���жϿ�����
void intc_init(void)
{
    //�ȹر������ж�
    intc_disable(NUM_ALL);

    // ѡ���ж�����ΪIRQ
    VIC0INTSELECT = 0x0;
    VIC1INTSELECT = 0x0;
    VIC2INTSELECT = 0x0;
    VIC3INTSELECT = 0x0;

    // ��VICxADDR
    intc_clearvectaddr();
}

//ע���������жϴ�������VICnVECTADDR�Ĵ�����Ӧ��ĳλ��
void intc_setvectaddr(unsigned int intnum, void (*handler)(void))
{
    //VIC0
    if(intnum<32)
    {
        *( (volatile unsigned int *)(VIC0VECTADDR + 4*(intnum-0)) ) = (unsigned)handler;
    }
    //VIC1
    else if(intnum<64)
    {
        *( (volatile unsigned int *)(VIC1VECTADDR + 4*(intnum-32)) ) = (unsigned)handler;
    }
    //VIC2
    else if(intnum<96)
    {
        *( (volatile unsigned int *)(VIC2VECTADDR + 4*(intnum-64)) ) = (unsigned)handler;
    }
    //VIC3
    else
    {
        *( (volatile unsigned int *)(VIC3VECTADDR + 4*(intnum-96)) ) = (unsigned)handler;
    }
}


// ʹ���ж�
// ͨ�����ε�intnum��ʹ��ĳ��������ж�Դ���жϺ���int.h�ж��壬�������жϺ�
//s5pv210ʹ���жϼĴ�������д1��Ч��ʹ�ܣ���д0��Ӱ�죬���Կ���ֱ�ӸļĴ�����ֵ��������Ҫ����ԭ��״̬
void intc_enable(unsigned int intnum)
{
	// ȷ��intnum���ĸ��Ĵ�������һλ
	// <32����0��31����Ȼ��VIC0
    if(intnum<32)
    {
        VIC0INTENABLE = (1 << intnum);
    }
    else if(intnum<64)
    {
        VIC1INTENABLE = (1 << (intnum-32));
    }
    else if(intnum<96)
    {
        VIC2INTENABLE = (1 << (intnum-64));
    }
    else if(intnum<NUM_ALL)
    {
        VIC3INTENABLE = (1 << (intnum-96));
    }
    // NUM_ALL : enable all interrupt
    else
    {
        VIC0INTENABLE = 0xFFFFFFFF;
        VIC1INTENABLE = 0xFFFFFFFF;
        VIC2INTENABLE = 0xFFFFFFFF;
        VIC3INTENABLE = 0xFFFFFFFF;
    }

}

// ��ֹ�ж�
// ͨ�����ε�intnum����ֹĳ��������ж�Դ���жϺ���int.h�ж��壬�������жϺ�
//s5pv210��ֹ�жϼĴ�������д1��Ч����ֹ����д0��Ӱ�죬���Կ���ֱ�ӸļĴ�����ֵ��������Ҫ����ԭ��״̬
void intc_disable(unsigned int intnum)
{
    if(intnum<32)
    {
        VIC0INTENCLEAR = (1 << intnum);
    }
    else if(intnum<64)
    {
        VIC0INTENCLEAR = (1 << (intnum-32));
    }
    else if(intnum<96)
    {
        VIC0INTENCLEAR = (1 << (intnum-64));
    }
    else if(intnum<NUM_ALL)
    {
        VIC0INTENCLEAR = (1 << (intnum-96));
    }
    // NUM_ALL : disable all interrupt
    else
    {
        VIC0INTENCLEAR = 0xFFFFFFFF;
        VIC1INTENCLEAR = 0xFFFFFFFF;
        VIC2INTENCLEAR = 0xFFFFFFFF;
        VIC3INTENCLEAR = 0xFFFFFFFF;
    }
}


// ͨ����ȡVICnIRQSTATUS�Ĵ������ж������ĸ���һλΪ1������֪�ĸ�VIC�����ж���
unsigned int intc_getvicirqstatus(unsigned int ucontroller)
{
    if(ucontroller == 0)
        return	VIC0IRQSTATUS;
    else if(ucontroller == 1)
        return 	VIC1IRQSTATUS;
    else if(ucontroller == 2)
        return 	VIC2IRQSTATUS;
    else if(ucontroller == 3)
        return 	VIC3IRQSTATUS;
    else
    {}
    return 0;
}


// �������жϴ��������˼����˵����ֻ�����жϴ��������Ǳ���/�ָ��ֳ�
void irq_handler(void)
{	
	// ��ȻӲ���Ѿ��Զ������ǰ�isr������VICnADDR�У�������Ϊ��4�����������Ǳ���
	// ��ѯ�����ĸ�VIC�ж��ˣ�Ҳ����˵isr�������ĸ�VICADDR�Ĵ�����
	unsigned int vicaddr[4] = {VIC0ADDR,VIC1ADDR,VIC2ADDR,VIC3ADDR};
    int i=0;
    void (*isr)(void) = NULL;

    for(i=0; i<4; i++)
    {
		// ����һ���ж�ʱ��4��VIC����3����ȫ0��1��������һλ����0
        if(intc_getvicirqstatus(i) != 0)
        {
            isr = (void (*)(void)) vicaddr[i];
            break;
        }
    }
    (*isr)();		// ͨ������ָ�������ú���
}
