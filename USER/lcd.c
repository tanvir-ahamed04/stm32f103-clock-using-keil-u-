/******************** 尚学科技 **************************
 * 实验平台：开拓者STM32开发板
 * 库版本  ：ST3.5.0
 * 作者    ：尚学科技团队 
 * 淘宝    ：http://shop102218275.taobao.com/
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 *版权所有，盗版必究。
**********************************************************************************/

#include "lcd.h"
#include "stdlib.h"

#include "usart1.h"
 
 //画笔颜色,背景颜色
u16 POINT_COLOR,BACK_COLOR=WHITE;  

//写16位数据函数
void LCD_WR_DATA(u16 data)
{
LCD_RS_SET;
LCD_CS_CLR;
DATAOUT(data);
LCD_WR_CLR;
LCD_WR_SET;
LCD_CS_SET;
} 

/*****************************************************************************
** 函数名称: LCD_RD_DATA
** 功能描述: 写寄存器函数
** 返回值:读到的值
*****************************************************************************/  
void LCD_WR_REG(u8 data)
{ 
	LCD_RS_CLR;//写地址  
 	LCD_CS_CLR; 
	DATAOUT(data); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;   
}


 
/*****************************************************************************
** 函数名称: LCD_RD_DATA
** 功能描述: 读LCD数据
** 返回值:读到的值
*****************************************************************************/  

u16 LCD_RD_DATA(void)
{										   
	u16 t;

  LCD_GPIOB_Configuration(0);
	LCD_RS_SET;
	LCD_CS_CLR;
	//读取数据(读寄存器时,并不需要读2次)
	LCD_RD_CLR;		   
	LCD_RD_SET;
	t=DATAIN;  
	LCD_CS_SET; 
  LCD_GPIOB_Configuration(1);
  return t;  
}	
/*****************************************************************************
** 函数名称: LCD_WriteReg
** 功能描述: 写寄存器
** LCD_Reg:寄存器编号
** LCD_RegValue:要写入的值
*****************************************************************************/  

void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   


//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
LCD_WR_REG(0x22);
}	 

/*****************************************************************************
** 函数名称:LCD_DisplayOn
** 功能描述: 开启LCD显示
** 功能描述: 关闭LCD显示
*****************************************************************************/  

void LCD_Display(u8 off_on)
{
if(off_on==1)	
LCD_WriteReg(0x07,0x0173); 			//开启显示
else 
LCD_WriteReg(0x07, 0x0);//关闭显示 	
}
   



//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
if(Horizontal_or_Vertical)
{
	//横屏显示
		LCD_WriteReg(0x20,Ypos);
		LCD_WriteReg(0x21,319-Xpos);
}
		//竖屏显示					   
else
{
	  LCD_WriteReg(0x20, Xpos);
		LCD_WriteReg(0x21, Ypos);	
}	

}



/*****************************************************************************
** 函数名称: LCD_Scan_Dir
** 功能描述: 设置LCD的自动扫描方向
*****************************************************************************/  
void LCD_Scan(void)
{
	u16 regval=0;
  regval|=L2R_U2D; //从左到右,从上到下
	regval|=1<<12;  
	LCD_WriteReg(0X03,regval);
	
} 
/*****************************************************************************
** 函数名称: LCD_DrawPoint
** 功能描述: 画点
** x:0~239
** y:0~319
** POINT_COLOR:此点的颜色
*****************************************************************************/


void LCD_DrawPoint(u16 x,u16 y,u16 Color)
{
	LCD_SetCursor(x,y);//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WR_DATA(Color); 
} 


/*****************************************************************************
** 函数名称: LCD_GPIO_Configuration
** 功能描述: LCD_IO口配置
*****************************************************************************/
void LCD_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;	   //GPIO_Pin_10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC	

	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB

	GPIO_SetBits(GPIOB,GPIO_Pin_All);
 }


/*****************************************************************************
** 函数名称: LCD_GPIOB_Configuration
** 功能描述: lcd数据传输用到的IO配置
*****************************************************************************/

void LCD_GPIOB_Configuration(u8 sel)
{
  GPIO_InitTypeDef GPIO_InitStructure;
if(sel==1)
{
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB

	GPIO_SetBits(GPIOB,GPIO_Pin_All);
}
else 
{
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB
}
}

 

/*****************************************************************************
** 函数名称: LCD_Init
** 功能描述: LCD初始化
*****************************************************************************/
void LCD_Init(void)
{  
	LCD_GPIO_Configuration();

	LCD_WriteReg(0x0000,0x0001);//可以去掉

 
	  LCD_WriteReg(0x00E5,0x78F0); 
		LCD_WriteReg(0x0001,0x0100); 
		LCD_WriteReg(0x0002,0x0700); 
		LCD_WriteReg(0x0003,0x1030); 
		LCD_WriteReg(0x0004,0x0000); 
		LCD_WriteReg(0x0008,0x0202);  
		LCD_WriteReg(0x0009,0x0000);
		LCD_WriteReg(0x000A,0x0000); 
		LCD_WriteReg(0x000C,0x0000); 
		LCD_WriteReg(0x000D,0x0000);
		LCD_WriteReg(0x000F,0x0000);
		//power on sequence VGHVGL
		LCD_WriteReg(0x0010,0x0000);   
		LCD_WriteReg(0x0011,0x0007);  
		LCD_WriteReg(0x0012,0x0000);  
		LCD_WriteReg(0x0013,0x0000); 
		LCD_WriteReg(0x0007,0x0000); 
		//vgh 
		LCD_WriteReg(0x0010,0x1690);   
		LCD_WriteReg(0x0011,0x0227);
		//delayms(100);
		//vregiout 
		LCD_WriteReg(0x0012,0x009D); //0x001b
		//delayms(100); 
		//vom amplitude
		LCD_WriteReg(0x0013,0x1900);
		//delayms(100); 
		//vom H
		LCD_WriteReg(0x0029,0x0025); 
		LCD_WriteReg(0x002B,0x000D); 
		//gamma
		LCD_WriteReg(0x0030,0x0007);
		LCD_WriteReg(0x0031,0x0303);
		LCD_WriteReg(0x0032,0x0003);// 0006
		LCD_WriteReg(0x0035,0x0206);
		LCD_WriteReg(0x0036,0x0008);
		LCD_WriteReg(0x0037,0x0406); 
		LCD_WriteReg(0x0038,0x0304);//0200
		LCD_WriteReg(0x0039,0x0007); 
		LCD_WriteReg(0x003C,0x0602);// 0504
		LCD_WriteReg(0x003D,0x0008); 
		//ram
		LCD_WriteReg(0x0050,0x0000); 
		LCD_WriteReg(0x0051,0x00EF);
		LCD_WriteReg(0x0052,0x0000); 
		LCD_WriteReg(0x0053,0x013F);  
		LCD_WriteReg(0x0060,0xA700); 
		LCD_WriteReg(0x0061,0x0001); 
		LCD_WriteReg(0x006A,0x0000); 
		//
		LCD_WriteReg(0x0080,0x0000); 
		LCD_WriteReg(0x0081,0x0000); 
		LCD_WriteReg(0x0082,0x0000); 
		LCD_WriteReg(0x0083,0x0000); 
		LCD_WriteReg(0x0084,0x0000); 
		LCD_WriteReg(0x0085,0x0000); 
		//
		LCD_WriteReg(0x0090,0x0010); 
		LCD_WriteReg(0x0092,0x0600); 
		
		LCD_WriteReg(0x0007,0x0133);
		LCD_WriteReg(0x00,0x0022);//	

	LCD_Scan();		 
	LCD_LED;//点亮背光	 
}  		  

/*****************************************************************************
** 函数名称: LCD_Clear
** 功能描述: 清屏幕函数
** Color:要清屏的填充色
*****************************************************************************/

void LCD_Clear(u16 Color)
{
	u32 index=0;      
	LCD_SetCursor(0x00,0x0000);//设置光标位置 
	LCD_WriteRAM_Prepare();     //开始写入GRAM	 	  
	for(index=0;index<76800;index++)
	{
		LCD_WR_DATA(Color);    
	}
} 






