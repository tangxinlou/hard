/*******************************************************
$ Author       : 唐新楼 
$ Last modified: 2020-11-07 19:59
$ Email        : **************
$ blog         : https://blog.csdn.net/daweigongting
$ Filename     : uart_hard.md
$ Description  : 
*******************************************************/

# 硬件原理
- 简称：通用的异步接收器和发送器
- 用途打印信息(打印调试)，外接模块，gps模块和gps模块
- 优点结构简单可靠，只需要三条线就可以了
- 使用串口需要设置波特率，数据格式，比如数据位多少，停止位多少，校验位多少，流量控制

## 发送数据流程
- 设置波特率，波特率可以决定发送数据一位的时间
- TXD从高电平拉低到低电平，从低电平开始计时，维持一个位的时间
- TXD从低电平的第二个位的时刻开始发送第一位数据，直到发送到数据位最后一位
- 发送了最后一位后，会发送一个校验位，奇偶校验就是确认之前发送1的个数
- 发送停止位,可以约定停止位的个数

## 串口控制器
- 数据是逐位发送，需要一个移位寄存器
- 数据从引脚发送到移位器，从移位器-buffer-内存
- 怎么发现已经发送了数据或者接收了数据，是通过中断
- 每一位发送的时间，等一个1/波特率,发送一个字节的数据需要10位，1个开始位，8个数据位，1个停止位
- 当使用fofo功能时,buffer 是64个字节，当不使用buffer是1个字节

# 程序编写

## 初始化
- 设置两个引脚为串口
- 因为电平需要3.3v，所以引脚需要上拉
- 设置波特率,串口使用pclk
- 发送的模式使用中断或者查询的方式，通过中断和查询知道数据发送完成或者接到数据
- 设置数据的格式，8位数据1位停止位0位校验位1位停止位


## 功能
- 发送数据
UTRSTAT0第二位为1的时候的时候表示发送buffer和移位器都已经发送完了，可以往数据寄存器写值了,
当写到寄存器后，这个值就会跑到发送buffer中，进而跑到移位器中，进而发送出去

- 接收数据
UTRSTAT0第0位为1的时候的时候表示接收buffer和移位器中有数据，可以读数据寄存器写值了,
当数据通过移位器到buffer中到数据寄存器中，这时可以读寄存器到内存 


# 引生

韦东山写的读写串口函数，是通过在读的时候判断状态寄存器,进而判断buffer是否有值，然后主动读写数据寄存器
这个相当于一个printf 函数和scan函数，但是面试的时候他们问的串口函数，是基于那种能够不断自动打印数据的串口函数
这里就有一个问题，当我们把发送模式配置成中断及查询模式后,通过查询状态寄存器就可以知道是否发送数据完成，或者有数据
需要接收，但是我们想实现那种有数据来了自动写到内存中，这个时候就需要中断来判断了,比如在中断来再去判断状态寄存器，然后读取数据寄存器的数值
但是这里需要一个概念：
在中断模式，当buffer中的数据被发送了，buffer里面没有数据了，通过触发了发送完成中断，然后cpu就知道要把utxd中的值发送到uart中，
这时utxd是空的,当这时我们一旦写了值在里面，那么cpu立马就会把着发送到buffer里
同样在buffer满了就会触发一个中断，这时cpu自动把buffer中的值发送到urxd中
所以中断模式和dma模式就是通过中断和dma请求的方式，决定cpu也就是数据寄存器的值到uartbuffer中的传输。



















