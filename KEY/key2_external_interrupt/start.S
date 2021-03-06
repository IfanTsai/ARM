#define WTCON     0xe2700000
#define SVC_STACK 0xd0037d80
#define IRQ_STACK 0xd0037f80

.global _start
.global irq_handle

_start:
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

	bl main//短跳转
	b .

irq_handle:
   /*保存处理器现场*/
   ldr sp, =IRQ_STACK//设置异常模式下的栈
   sub lr, lr, #4//因为ARM取指令存在着流水线
   stmfd sp!, {r0-r12, lr}//保存lr的原因是防止在异常处理函数中调用了函数导致覆盖了lr的值，保险起见就一起保存了
   
   /*跳转*/
   bl irq_handler

   /*恢复处理器现场*/
   ldmfd sp!, {r0-r12, pc}^//将返回地址给pc指针，并且恢复cpsr中的值

