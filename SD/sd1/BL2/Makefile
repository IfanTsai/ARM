BL2.bin: start.o main.o led.o
	arm-linux-ld -Ttext 0x23e00000 -o BL2.elf $^
	arm-linux-objcopy -O binary BL2.elf BL2.bin
	arm-linux-objdump -D BL2.elf > BL2_elf.dis
	
%.o : %.S
	arm-linux-gcc -o $@ $< -c -nostdlib

%.o : %.c
	arm-linux-gcc -o $@ $< -c -nostdlib 

clean:
	rm *.o *.elf *.bin *.dis mkx210 -f

	
	
