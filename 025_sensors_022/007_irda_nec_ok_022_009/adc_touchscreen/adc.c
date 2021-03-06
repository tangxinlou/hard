#include "../s3c2440_soc.h"

void adc_init(int channel)
{
	/* [15] : ECFLG,  1 = End of A/D conversion
	 * [14] : PRSCEN, 1 = A/D converter prescaler enable
	 * [13:6]: PRSCVL, adc clk = PCLK / (PRSCVL + 1)
	 * [5:3] : SEL_MUX, 000 = AIN 0
	 * [2]   : STDBM
	 * [0]   : 1 = A/D conversion starts and this bit is cleared after the startup.
	 */
	ADCCON = (1<<14) | (49<<6) | (channel<<3);

	ADCDLY = 0xff;	
}

int adc_read(int channel)
{
	adc_init(channel);
	
	/* ����ADC */
	ADCCON |= (1<<0);

	while (!(ADCCON & (1<<15)));  /* �ȴ�ADC���� */

	return ADCDAT0 & 0x3ff;
}

