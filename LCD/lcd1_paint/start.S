//2016年09月23日17:50:03
#define WTCON     0xe2700000
#define SVC_STACK 0xd0037d80

.global _start
_start:
	/*开发板电源置锁*/
	ldr r0, = 0xe010e81c
	ldr r1, [r0]
	ldr r2, =( (0x3 << 8) | 0x1 )
	orr r1, r1, r2
	str r1, [r0]

	/*关看门狗*/
	ldr r0, =0x0	
	ldr r1, =WTCON
	str r0, [r1]
	
	/*初始化时钟*/
	bl clock_init 
	
    /*设置SVC栈（满减栈）*/
	ldr sp, =SVC_STACK

	/*初始化icache*/
	mrc p15, 0, r0, c1, c0, 0//读出cp15的值到r0中
	//bic r0, r0, #(1 << 12)//bit12为0 关icache（bic为位清零）
	orr r0, r0, #(1 << 12)//bit12为0 开icache（orr为按位或）
	mcr p15, 0, r0, c1, c0, 0//把r0的值写到cp15中

	/*初始化ddr*/
	bl sdram_asm_init

	bl main 
	b .
