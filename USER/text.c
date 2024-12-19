/******************** 尚学科技 **************************
 * 实验平台：开拓者STM32开发板
 * 库版本  ：ST3.5.0
 * 作者    ：尚学科技团队 
 * 淘宝    ：http://shop102218275.taobao.com/
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 *版权所有，盗版必究。
**********************************************************************************/
#include "lcd.h"
#include "text.h"													   
#include "font.h" 
/*****************************************************************************
** 函数名称: LCD_ShowChar
** 功能描述: 在指定位置显示一个字符
**在指定位置显示一个字符
**x:0~234
**y:0~308
**num:要显示的字符:" "--->"~"
**size:字体大小 12/16
**mode:叠加方式(1)还是非叠加方式(0)
*****************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode,u16 Color)
{  

    u8 temp;
    u8 j,t;
	  u8 count;
	u16 x0=x;   
    if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;	    
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	if(mode==0) //正常显示
	{
		for(j=0;j<size;j++)
		{
			//字体大小选择
			if(size==12)
			{
				temp=asc2_1206[num][j];
				count=6;
			}//调用1206字体
			else 
			{
				temp=asc2_1608[num][j];	
				count=8;
			}				//调用1608字体
			for(t=0;t<count;t++)
		    {                 
		    if(temp&0x01)
				{
				LCD_DrawPoint(x,y,Color);	
				}
				else 
				{
				LCD_DrawPoint(x,y,BACK_COLOR);	
				}
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}   	   	 	  
}   


/*****************************************************************************
** 函数名称: LCD_ShowString
** 功能描述: 显示字符串函数
** x,y:起点坐标  
** *p:字符串起始地址
** 用16字体
*****************************************************************************/
void LCD_ShowString(u16 x,u16 y,const u8 *pstr,u16 Color)
{         
    while(*pstr!='\0')
    {       
        if(x>MAX_CHAR_POSX)
        {
         x=0;
				 y+=STR_HEIGHT;
        }
        if(y>MAX_CHAR_POSY)
        {
         y=x=0;
        LCD_Clear(WHITE);
        }
        LCD_ShowChar(x,y,*pstr,16,0,Color);
        x+=STR_WIDTH;
        pstr++;
    }  
}

/*****************************************************************************
** 函数名称: LCD_ShowTimeChar
** 功能描述: 在指定位置显示一个字符
**在指定位置显示一个字符
**x:0~234
**y:0~308
**num:要显示的字符:" "--->"~"
**size:字体大小 12/16
**mode:叠加方式(1)还是非叠加方式(0)
*****************************************************************************/
void LCD_ShowTimeChar(u16 x,u16 y,u8 num,u8 size,u8 mode,u16 Color)
{  

    u8 temp;
    u8 j,t;
	  u8 count;
	u16 x0=x;   
    if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;	    
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	
	if (num==0)
			num=16;
	
	if(mode==0) //正常显示
	{
		for(j=0;j<size;j++)
		{
			//字体大小选择
			if(size==12)
			{
				temp=asc2_1206[num][j];
				count=6;
			}//调用1206字体
			else 
			{
				temp=asc2_1608[num][j];	
				count=8;
			}				//调用1608字体
			for(t=0;t<count;t++)
		    {                 
		    if(temp&0x01)
				{
				LCD_DrawPoint(x,y,Color);	
				}
				else 
				{
				LCD_DrawPoint(x,y,BACK_COLOR);	
				}
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}   	   	 	  
}  

void LCD_ShowTimeString(u16 x,u16 y,const u8 *pstr,u16 Color)//add 12/26/2023
{         
    while(*pstr!='\0')
    {       
        if(x>MAX_CHAR_POSX)
        {
         x=0;
				 y+=STR_HEIGHT;
        }
        if(y>MAX_CHAR_POSY)
        {
         y=x=0;
        LCD_Clear(WHITE);
        }
        LCD_ShowTimeChar(x,y,*pstr,16,0,Color);
        x+=STR_WIDTH;
        pstr++;
    }  
}

void LCD_ShowBigChar(u16 x,u16 y,u8 num,u16 Color)	
{
	u8 temp,count,t,j,i=0;
	u16 x0=x;
	  count=8;
	
		for(j=0;j<64;j=j+2)
		{
					
      temp=asc2_3216[num][j];
			for(t=0;t<count;t++)
		  {                 
		    if(temp&0x01)
				{
				LCD_DrawPoint(x,y,Color);	
				}
				else 
				{
				LCD_DrawPoint(x,y,BACK_COLOR);	
				}
				temp>>=1; 
				x++;
		  }
			
			temp=asc2_3216[num][j+1];	
			for(t=0;t<count;t++)
		  {                 
		    if(temp&0x01)
				{
				LCD_DrawPoint(x,y,Color);	
				}
				else 
				{
				LCD_DrawPoint(x,y,BACK_COLOR);	
				}
				temp>>=1; 
				x++;
		  }
			
			x=x0;
			y++;
		}
		
}





