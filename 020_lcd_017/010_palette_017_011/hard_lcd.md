/*******************************************************
$ Author       : 唐新楼 
$ Last modified: 2020-10-29 21:47
$ Email        : **************
$ blog         : https://blog.csdn.net/daweigongting
$ Filename     : hard_lcd.md
$ Description  : 
*******************************************************/

# lcd test
```c

/d/work/hard/020_lcd_017/008_font_017_009/main.c:12:
int main(void)
{
	led_init();
	//interrupt_init();  /* 初始化中断控制器 */
	key_eint_init();   /* 初始化按键, 设为中断源 */
	//timer_init();
	
	puts("\n\rg_A = ");
	printHex(g_A);
	puts("\n\r");

	//nor_flash_test();
	lcd_test();
	
	return 0;
}

```


## lcd init
- 每个屏都有自动的参数，这些参数包括如下
- 屏幕的每个像素使用多少位数据表述
- 以多大的频率发射电子枪
- 确定有效数据的六个时序参数
- 以及各个信号是高电平有效还是低电平有效，诸如此类的各个信号的极性控制参数

- 不同的控制器有不同的设置函数，所以每个控制器需要特定的控制器的设置函数
- 所以在初始化之前就把所有屏幕的参数加载了，同时也把所有的控制器对应的函数也加载了，通过对应控制器名字选择对应的设置函数
- 题外话：linux中通过设备树选择加载不同屏的参数，同时通过设备树，选择不同的控制，选择不同的设置函数
```c

/d/work/hard/020_lcd_017/008_font_017_009/lcd/lcd.c:55:
int lcd_init(void)
{
	/* 注册LCD */
	lcd_4_3_add();

	/* 注册LCD控制器 */
	lcd_contoller_add();
	
	/* 选择某款LCD */
	select_lcd("lcd_4.3");

	/* 选择某款LCD控制器 */
	select_lcd_controller("s3c2440");

	/* 使用LCD的参数, 初始化LCD控制器 */
	lcd_controller_init(g_p_lcd_selected);
}

```

### lcd pinal add

```c

/d/work/hard/020_lcd_017/008_font_017_009/lcd/lcd_4.3.c:36:
void lcd_4_3_add(void)
{
	register_lcd(&lcd_4_3_params);
}

```


```c

/d/work/hard/020_lcd_017/008_font_017_009/lcd/lcd.c:9:
int register_lcd(p_lcd_params plcd)
{
	int i;
	for (i = 0; i < LCD_NUM; i++)
	{
		if (!p_array_lcd[i])
		{
			p_array_lcd[i] = plcd;
			return i;
		}
	}
	return -1;		
}

```


```c

/d/work/hard/020_lcd_017/008_font_017_009/lcd/lcd_4.3.c:6:
lcd_params lcd_4_3_params = {
	.name = "lcd_4.3",
	.pins_pol = {
		.de    = NORMAL,	/* normal: 高电平时可以传输数据 */
		.pwren = NORMAL,    /* normal: 高电平有效 */
		.vclk  = NORMAL,	/* normal: 在下降沿获取数据 */
		.rgb   = NORMAL,	/* normal: 高电平表示1 */
		.hsync = INVERT,    /* normal: 高脉冲 */
		.vsync = INVERT, 	/* normal: 高脉冲 */
	},
	.time_seq = {
		/* 垂直方向 */
		.tvp=	10, /* vysnc脉冲宽度 */
		.tvb=	2,  /* 上边黑框, Vertical Back porch */
		.tvf=	2,  /* 下边黑框, Vertical Front porch */

		/* 水平方向 */
		.thp=	41, /* hsync脉冲宽度 */
		.thb=	2,  /* 左边黑框, Horizontal Back porch */
		.thf=	2,  /* 右边黑框, Horizontal Front porch */

		.vclk=	9,  /* MHz */
	},
	.xres = 480,
	.yres = 272,
	.bpp  = 32,  /* 16, no 24bpp */
	.fb_base = LCD_FB_BASE,
};

```


```c

/d/work/hard/020_lcd_017/008_font_017_009/lcd/lcd.h:38:
typedef struct lcd_params {
	char *name;
	
	/* 引脚极性 */
	pins_polarity pins_pol;
	
	/* 时序 */
	time_sequence time_seq;
	
	/* 分辨率, bpp */
	int xres;
	int yres;
	int bpp;
	
	/* framebuffer的地址 */
	unsigned int fb_base;
}lcd_params, *p_lcd_params; 

```


```c

/d/work/hard/020_lcd_017/008_font_017_009/lcd/lcd.h:14:
typedef struct pins_polarity {
	int de;    /* normal: 高电平时可以传输数据 */
	int pwren; /* normal: 高电平有效 */
	int vclk;  /* normal: 在下降沿获取数据 */
	int rgb;   /* normal: 高电平表示1 */
	int hsync; /* normal: 高脉冲 */
	int vsync; /* normal: 高脉冲 */
}pins_polarity, *p_pins_polarity;

typedef struct time_sequence {
	/* 垂直方向 */
	int tvp; /* vysnc脉冲宽度 */
	int tvb; /* 上边黑框, Vertical Back porch */
	int tvf; /* 下边黑框, Vertical Front porch */

	/* 水平方向 */
	int thp; /* hsync脉冲宽度 */
	int thb; /* 左边黑框, Horizontal Back porch */
	int thf; /* 右边黑框, Horizontal Front porch */

	int vclk;
}time_sequence, *p_time_sequence; 

```

### lcd controler add


```c

/d/work/hard/020_lcd_017/008_font_017_009/lcd/lcd_controller.c:70:
void lcd_contoller_add(void)
{
	s3c2440_lcd_contoller_add();
}

```


```c

/d/work/hard/020_lcd_017/008_font_017_009/lcd/s3c2440_lcd_controller.c:145:
void s3c2440_lcd_contoller_add(void)
{
	register_lcd_controller(&s3c2440_lcd_controller);
}

```


```c

/d/work/hard/020_lcd_017/008_font_017_009/lcd/lcd_controller.c:9:
int register_lcd_controller(p_lcd_controller plcdcon)
{
	int i;
	for (i = 0; i < LCD_CONTROLLER_NUM; i++)
	{
		if (!p_array_lcd_controller[i])
		{
			p_array_lcd_controller[i] = plcdcon;
			return i;
		}
	}
	return -1;		
}

```


```c

/d/work/hard/020_lcd_017/008_font_017_009/lcd/s3c2440_lcd_controller.c:137:
struct lcd_controller s3c2440_lcd_controller = {
	.name    = "s3c2440",
	.init    = s3c2440_lcd_controller_init,
	.enable  = s3c2440_lcd_controller_enalbe,
	.disable = s3c2440_lcd_controller_disable,
};

```

### 匹配 lcd 


```c

/d/work/hard/020_lcd_017/010_palette_017_011/lcd/lcd.c:23:
int select_lcd(char *name)
{
	int i;
	for (i = 0; i < LCD_NUM; i++)
	{
		if (p_array_lcd[i] && !strcmp(p_array_lcd[i]->name, name))
		{
			g_p_lcd_selected = p_array_lcd[i];
			return i;
		}
	}
	return -1;		
}

```

```c

/d/work/hard/020_lcd_017/008_font_017_009/lcd/lcd_controller.c:23:
int select_lcd_controller(char *name)
{
	int i;
	for (i = 0; i < LCD_CONTROLLER_NUM; i++)
	{
		if (p_array_lcd_controller[i] && !strcmp(p_array_lcd_controller[i]->name, name))
		{
			g_p_lcd_controller_selected = p_array_lcd_controller[i];
			return i;
		}
	}
	return -1;		
}

```

### 初始化lcd 控制器


```c

/d/work/hard/020_lcd_017/008_font_017_009/lcd/lcd_controller.c:42:
int lcd_controller_init(p_lcd_params plcdparams)
{
	/* 调用所选择的LCD控制器的初始化函数 */
	if (g_p_lcd_controller_selected)
	{
		g_p_lcd_controller_selected->init(plcdparams);
		return 0;
	}
	return -1;
}

```

```c

/d/work/hard/020_lcd_017/008_font_017_009/lcd/s3c2440_lcd_controller.c:24:
void s3c2440_lcd_controller_init(p_lcd_params plcdparams)
{
	int pixelplace;
	unsigned int addr;

	jz2440_lcd_pin_init();
	
	/* [17:8]: clkval, vclk = HCLK / [(CLKVAL+1) x 2]
	 *                   9   = 100M /[(CLKVAL+1) x 2], clkval = 4.5 = 5
	 *                 CLKVAL = 100/vclk/2-1
	 * [6:5]: 0b11, tft lcd
	 * [4:1]: bpp mode
	 * [0]  : LCD video output and the logic enable/disable
	 */
	//int clkval = (double)HCLK/plcdparams->time_seq.vclk/2-1+0.5;
	int clkval = 5;
	int bppmode = plcdparams->bpp == 8  ? 0xb :\
				  plcdparams->bpp == 16 ? 0xc :\
				  0xd;  /* 0xd: 24,32bpp */
	LCDCON1 = (clkval<<8) | (3<<5) | (bppmode<<1) ;

	/* [31:24] : VBPD    = tvb - 1
	 * [23:14] : LINEVAL = line - 1
	 * [13:6]  : VFPD    = tvf - 1
	 * [5:0]   : VSPW    = tvp - 1
	 */
	LCDCON2 = 	((plcdparams->time_seq.tvb - 1)<<24) | \
	            ((plcdparams->yres - 1)<<14)         | \
				((plcdparams->time_seq.tvf - 1)<<6)  | \
				((plcdparams->time_seq.tvp - 1)<<0);

	/* [25:19] : HBPD	 = thb - 1
	 * [18:8]  : HOZVAL  = 列 - 1
	 * [7:0]   : HFPD	 = thf - 1
	 */
	LCDCON3 =	((plcdparams->time_seq.thb - 1)<<19) | \
				((plcdparams->xres - 1)<<8)		      | \
				((plcdparams->time_seq.thf - 1)<<0);

	/* 
	 * [7:0]   : HSPW	 = thp - 1
	 */
	LCDCON4 =	((plcdparams->time_seq.thp - 1)<<0);

    /* 用来设置引脚极性, 设置16bpp, 设置内存中象素存放的格式
     * [12] : BPP24BL
	 * [11] : FRM565, 1-565
	 * [10] : INVVCLK, 0 = The video data is fetched at VCLK falling edge
	 * [9]  : HSYNC是否反转
	 * [8]  : VSYNC是否反转
	 * [7]  : INVVD, rgb是否反转
	 * [6]  : INVVDEN
	 * [5]  : INVPWREN
	 * [4]  : INVLEND
	 * [3]  : PWREN, LCD_PWREN output signal enable/disable
	 * [2]  : ENLEND
	 * [1]  : BSWP
	 * [0]  : HWSWP
	 */

	pixelplace = plcdparams->bpp == 32 ? (0) : \
	             plcdparams->bpp == 16 ? (1) : \
	             (1<<1);  /* 8bpp */
	
	LCDCON5 = (plcdparams->pins_pol.vclk<<10) |\
	          (plcdparams->pins_pol.rgb<<7)   |\
	          (plcdparams->pins_pol.hsync<<9) |\
	          (plcdparams->pins_pol.vsync<<8) |\
 			  (plcdparams->pins_pol.de<<6)    |\
			  (plcdparams->pins_pol.pwren<<5) |\
			  (1<<11) | pixelplace;

	/* framebuffer地址 */
	/*
	 * [29:21] : LCDBANK, A[30:22] of fb
	 * [20:0]  : LCDBASEU, A[21:1] of fb
	 */
	addr = plcdparams->fb_base & ~(1<<31);
	LCDSADDR1 = (addr >> 1);

	/* 
	 * [20:0] : LCDBASEL, A[21:1] of end addr
	 */
	addr = plcdparams->fb_base + plcdparams->xres*plcdparams->yres*plcdparams->bpp/8;
	addr >>=1;
	addr &= 0x1fffff;
	LCDSADDR2 = addr;//	
}

```


```c

/d/work/hard/020_lcd_017/008_font_017_009/lcd/s3c2440_lcd_controller.c:8:
void jz2440_lcd_pin_init(void)
{
	/* 初始化引脚 : 背光引脚 */
	GPBCON &= ~0x3;
	GPBCON |= 0x01;

	/* LCD专用引脚 */
	GPCCON = 0xaaaaaaaa;
	GPDCON = 0xaaaaaaaa;

	/* PWREN */
	GPGCON |= (3<<8);
}

```

## lcd_enable


```c
/d/work/hard/020_lcd_017/010_palette_017_011/lcd/lcd.c:45:
void lcd_enable(void)
{
	lcd_controller_enable();
}
 

```

```c

/d/work/hard/020_lcd_017/010_palette_017_011/lcd/lcd_controller.c:54:
void lcd_controller_enable(void)
{
	if (g_p_lcd_controller_selected)
	{
		g_p_lcd_controller_selected->enable();
	}
}

```


```c

/d/work/hard/020_lcd_017/008_font_017_009/lcd/s3c2440_lcd_controller.c:113:
void s3c2440_lcd_controller_enalbe(void)
{
	/* 背光引脚 : GPB0 */
	GPBDAT |= (1<<0);
	
	/* pwren    : 给LCD提供AVDD  */
	LCDCON5 |= (1<<3);
	
	/* LCDCON1'BIT 0 : 设置LCD控制器是否输出信号 */
	LCDCON1 |= (1<<0);
}

```




