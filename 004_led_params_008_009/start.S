
.text
.global _start

_start:

	/* �����ڴ�: sp ջ */
	ldr sp, =4096  /* nand���� */
//	ldr sp, =0x40000000+4096  /* nor���� */

	mov r0, #4
	bl led_on

	ldr r0, =100000
	bl delay

	mov r0, #5
	bl led_on

halt:
	b halt
	
