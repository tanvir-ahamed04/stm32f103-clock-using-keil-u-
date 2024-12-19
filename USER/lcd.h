#ifndef __LCD_H
#define __LCD_H		
#include "stdlib.h"
#include "stm32f10x.h"

#define Horizontal_or_Vertical  1	//�����Ƿ�ʹ�ú��� 		0,��ʹ��.1,ʹ��.

#if Horizontal_or_Vertical==1
#define MAX_CHAR_POSX 312
#define MAX_CHAR_POSY 232 

#define L2R_U2D  (0<<5)|(1<<4)|(1<<3);  //������,���ϵ���
#define L2R_D2U  (0<<5)|(0<<4)|(1<<3); //������,���µ���
#define R2L_U2D  (0<<5)|(0<<4)|(0<<3);//���ҵ���,���ϵ���
#define R2L_D2U  (1<<5)|(0<<4)|(1<<3); //���ҵ���,���µ���

#define U2D_L2R  (0<<5)|(1<<4)|(0<<3); //���ϵ���,������
#define U2D_R2L  (1<<5)|(1<<4)|(0<<3);//���ϵ���,���ҵ���
#define D2U_L2R  (0<<5)|(0<<4)|(0<<3); //���µ���,������
#define D2U_R2L  (1<<5)|(0<<4)|(0<<3); //���µ���,���ҵ���

#else     
#define MAX_CHAR_POSX 232
#define MAX_CHAR_POSY 312

#define L2R_U2D  (1<<5)|(1<<4)|(0<<3);  //������,���ϵ���
#define L2R_D2U  (0<<5)|(1<<4)|(0<<3); //������,���µ���
#define R2L_U2D  (1<<5)|(0<<4)|(0<<3);//���ҵ���,���ϵ���
#define R2L_D2U  (0<<5)|(0<<4)|(0<<3); //���ҵ���,���µ���

#define U2D_L2R  (0<<5)|(0<<4)|(0<<3); //���ϵ���,������
#define U2D_R2L  (1<<5)|(0<<4)|(1<<3);//���ϵ���,���ҵ���
#define D2U_L2R  (0<<5)|(1<<4)|(1<<3); //���µ���,������
#define D2U_R2L  (0<<5)|(0<<4)|(1<<3); //���µ���,���ҵ���

#endif   
	   
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

// SRT ��string����д
#define STR_WIDTH		8		/* �ַ���� */
#define STR_HEIGHT		16		/* �ַ��߶� */

////////////////////////////////////////////////////////////////////
//-----------------LCD�˿ڶ���---------------- 
#define	LCD_LED GPIO_SetBits(GPIOC,GPIO_Pin_10) //LCD����    		 PC10 
   
#define	LCD_CS_SET  GPIO_SetBits(GPIOC,GPIO_Pin_9);    //Ƭѡ�˿�  	     PC9
#define	LCD_RS_SET	GPIO_SetBits(GPIOC,GPIO_Pin_8);    //����/����          PC8	   
#define	LCD_WR_SET	GPIO_SetBits(GPIOC,GPIO_Pin_7);   //д����			 PC7
#define	LCD_RD_SET	GPIO_SetBits(GPIOC,GPIO_Pin_6);    //������			 PC6
								    
#define	LCD_CS_CLR  GPIO_ResetBits(GPIOC,GPIO_Pin_9);     //Ƭѡ�˿�  	     PC9
#define	LCD_RS_CLR	GPIO_ResetBits(GPIOC,GPIO_Pin_8);    //����/����          PC8	   
#define	LCD_WR_CLR	GPIO_ResetBits(GPIOC,GPIO_Pin_7);    //д����			 PC7
#define	LCD_RD_CLR	GPIO_ResetBits(GPIOC,GPIO_Pin_6);    //������			 PC6

//PB0~15,��Ϊ������
#define DATAOUT(x) GPIOB->ODR=x; //�������
#define DATAIN     GPIOB->IDR;   //��������
//////////////////////////////////////////////////////////////////////



//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

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
u16 LCD_RD_DATA(void);//��ȡLCD����	
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
void LCD_WriteRAM_Prepare(void);
void LCD_WR_DATA(u16 data);
#endif  
	 
	 



