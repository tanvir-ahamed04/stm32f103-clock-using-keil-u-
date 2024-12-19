#ifndef __TEXT_H__
#define __TEXT_H__	 
 

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode,u16 Color);
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 Color);
//add 12/26/2023
void LCD_ShowTimeString(u16 x,u16 y,const u8 *p,u16 Color); 
void LCD_ShowBigChar(u16 x,u16 y,u8 num,u16 Color);		
#endif
