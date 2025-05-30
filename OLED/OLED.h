#ifndef _OLED_H_
#define _OLED_H_
#include <stdint.h>
#include <stdbool.h>
#include "bsp_sys.h"
//单片机头文件
#include "stm32f1xx.h"

//#include "stdlib.h"






#define  u8 unsigned char                                                
#define  u32 unsigned int
#define OLED_CMD  0 //写命令
#define OLED_DATA 1 //写数据
#define OLED_MODE 0

/*   //下面这些是固件库四线IIC引脚的初始化
//引脚定义：PA6 :OLED_SCL;PA7：OLED_SDIN


#define OLED_CS_Clr()  OLED_CS=0
#define OLED_CS_Set()  OLED_CS=1
#define OLED_RST_Clr() OLED_RST=0
#define OLED_RST_Set() OLED_RST=1
#define OLED_DC_Clr() OLED_DC=0
#define OLED_DC_Set() OLED_DC=1  //还不知道这上面六句是干嘛的 

////#define OLED_SCLK_Clr() OLED_SCL=0
////#define OLED_SCLK_Set() OLED_SCL=1
//#define OLED_SCLK_Clr() GPIO_WriteBit(GPIOA,GPIO_Pin_6, Bit_RESET);
//#define OLED_SCLK_Set() GPIO_WriteBit(GPIOA,GPIO_Pin_6, Bit_SET);

////#define OLED_SDIN_Clr() OLED_SDIN=0
////#define OLED_SDIN_Set() OLED_SDIN=1
//#define OLED_SDIN_Clr() GPIO_WriteBit(GPIOA,GPIO_Pin_7, Bit_RESET);
//#define OLED_SDIN_Set() GPIO_WriteBit(GPIOA,GPIO_Pin_7, Bit_SET);*/



//引脚定义：PB7 :OLED_SCLK;PB6：OLED_SDIN


#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);	
#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET);

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);




//OLED模式设置
//0:4线串行模式
//1:并行8080模式

#define SIZE 16
#define XLevelL     0x02
#define XLevelH     0x10
#define Max_Column  128
#define Max_Row     64
#define Brightness  0xFF
#define X_WIDTH     128
#define Y_WIDTH     64
//-----------------OLED端口定义----------------


//OLED控制用函数
void OLED_GPIO_Init(void);
void OLED_WR_Byte(unsigned dat,unsigned cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_ShowCHinese1(u8 x,u8 y,u8 no);

void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void fill_picture(unsigned char fill_Data);
void Picture(void);
void IIC_Start(void);
void IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void IIC_Wait_Ack(void);



#endif /* 9_6_OLED_OLED_H_ */
