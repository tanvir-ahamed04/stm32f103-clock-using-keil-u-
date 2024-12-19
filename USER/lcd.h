#ifndef __LCD_H
#define __LCD_H		
#include "stdlib.h"
#include "stm32f10x.h"

#define Horizontal_or_Vertical  1	//定义是否使用横屏 		0,不使用.1,使用.

#if Horizontal_or_Vertical==1
#define MAX_CHAR_POSX 312
#define MAX_CHAR_POSY 232 

#define L2R_U2D  (0<<5)|(1<<4)|(1<<3);  //从左到右,从上到下
#define L2R_D2U  (0<<5)|(0<<4)|(1<<3); //从左到右,从下到上
#define R2L_U2D  (0<<5)|(0<<4)|(0<<3);//从右到左,从上到下
#define R2L_D2U  (1<<5)|(0<<4)|(1<<3); //从右到左,从下到上

#define U2D_L2R  (0<<5)|(1<<4)|(0<<3); //从上到下,从左到右
#define U2D_R2L  (1<<5)|(1<<4)|(0<<3);//从上到下,从右到左
#define D2U_L2R  (0<<5)|(0<<4)|(0<<3); //从下到上,从左到右
#define D2U_R2L  (1<<5)|(0<<4)|(0<<3); //从下到上,从右到左

#else     
#define MAX_CHAR_POSX 232
#define MAX_CHAR_POSY 312

#define L2R_U2D  (1<<5)|(1<<4)|(0<<3);  //从左到右,从上到下
#define L2R_D2U  (0<<5)|(1<<4)|(0<<3); //从左到右,从下到上
#define R2L_U2D  (1<<5)|(0<<4)|(0<<3);//从右到左,从上到下
#define R2L_D2U  (0<<5)|(0<<4)|(0<<3); //从右到左,从下到上

#define U2D_L2R  (0<<5)|(0<<4)|(0<<3); //从上到下,从左到右
#define U2D_R2L  (1<<5)|(0<<4)|(1<<3);//从上到下,从右到左
#define D2U_L2R  (0<<5)|(1<<4)|(1<<3); //从下到上,从左到右
#define D2U_R2L  (0<<5)|(0<<4)|(1<<3); //从下到上,从右到左

#endif   
	   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色

// SRT 是string的缩写
#define STR_WIDTH		8		/* 字符宽度 */
#define STR_HEIGHT		16		/* 字符高度 */

////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义---------------- 
#define	LCD_LED GPIO_SetBits(GPIOC,GPIO_Pin_10) //LCD背光    		 PC10 
   
#define	LCD_CS_SET  GPIO_SetBits(GPIOC,GPIO_Pin_9);    //片选端口  	     PC9
#define	LCD_RS_SET	GPIO_SetBits(GPIOC,GPIO_Pin_8);    //数据/命令          PC8	   
#define	LCD_WR_SET	GPIO_SetBits(GPIOC,GPIO_Pin_7);   //写数据			 PC7
#define	LCD_RD_SET	GPIO_SetBits(GPIOC,GPIO_Pin_6);    //读数据			 PC6
								    
#define	LCD_CS_CLR  GPIO_ResetBits(GPIOC,GPIO_Pin_9);     //片选端口  	     PC9
#define	LCD_RS_CLR	GPIO_ResetBits(GPIOC,GPIO_Pin_8);    //数据/命令          PC8	   
#define	LCD_WR_CLR	GPIO_ResetBits(GPIOC,GPIO_Pin_7);    //写数据			 PC7
#define	LCD_RD_CLR	GPIO_ResetBits(GPIOC,GPIO_Pin_6);    //读数据			 PC6

//PB0~15,作为数据线
#define DATAOUT(x) GPIOB->ODR=x; //数据输出
#define DATAIN     GPIOB->IDR;   //数据输入
//////////////////////////////////////////////////////////////////////



//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

extern u16 BACK_COLOR, POINT_COLOR ;  

void LCD_GPIO_Configuration(void);	    															  
void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(u16 Color);	 
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_Scan(void);
void LCD_DrawPoint(u16 x,u16 y,u16 Color);

void LCD_GPIOB_Configuration(u8 sel);
u16 LCD_RD_DATA(void);//读取LCD数据	
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
void LCD_WriteRAM_Prepare(void);
void LCD_WR_DATA(u16 data);
#endif  
	 
	 



