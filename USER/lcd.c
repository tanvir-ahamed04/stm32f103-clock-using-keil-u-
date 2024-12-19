/******************** ��ѧ�Ƽ� **************************
 * ʵ��ƽ̨��������STM32������
 * ��汾  ��ST3.5.0
 * ����    ����ѧ�Ƽ��Ŷ� 
 * �Ա�    ��http://shop102218275.taobao.com/
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 *��Ȩ���У�����ؾ���
**********************************************************************************/

#include "lcd.h"
#include "stdlib.h"

#include "usart1.h"
 
 //������ɫ,������ɫ
u16 POINT_COLOR,BACK_COLOR=WHITE;  

//д16λ���ݺ���
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
** ��������: LCD_RD_DATA
** ��������: д�Ĵ�������
** ����ֵ:������ֵ
*****************************************************************************/  
void LCD_WR_REG(u8 data)
{ 
	LCD_RS_CLR;//д��ַ  
 	LCD_CS_CLR; 
	DATAOUT(data); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;   
}


 
/*****************************************************************************
** ��������: LCD_RD_DATA
** ��������: ��LCD����
** ����ֵ:������ֵ
*****************************************************************************/  

u16 LCD_RD_DATA(void)
{										   
	u16 t;

  LCD_GPIOB_Configuration(0);
	LCD_RS_SET;
	LCD_CS_CLR;
	//��ȡ����(���Ĵ���ʱ,������Ҫ��2��)
	LCD_RD_CLR;		   
	LCD_RD_SET;
	t=DATAIN;  
	LCD_CS_SET; 
  LCD_GPIOB_Configuration(1);
  return t;  
}	
/*****************************************************************************
** ��������: LCD_WriteReg
** ��������: д�Ĵ���
** LCD_Reg:�Ĵ������
** LCD_RegValue:Ҫд���ֵ
*****************************************************************************/  

void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   


//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
LCD_WR_REG(0x22);
}	 

/*****************************************************************************
** ��������:LCD_DisplayOn
** ��������: ����LCD��ʾ
** ��������: �ر�LCD��ʾ
*****************************************************************************/  

void LCD_Display(u8 off_on)
{
if(off_on==1)	
LCD_WriteReg(0x07,0x0173); 			//������ʾ
else 
LCD_WriteReg(0x07, 0x0);//�ر���ʾ 	
}
   



//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
if(Horizontal_or_Vertical)
{
	//������ʾ
		LCD_WriteReg(0x20,Ypos);
		LCD_WriteReg(0x21,319-Xpos);
}
		//������ʾ					   
else
{
	  LCD_WriteReg(0x20, Xpos);
		LCD_WriteReg(0x21, Ypos);	
}	

}



/*****************************************************************************
** ��������: LCD_Scan_Dir
** ��������: ����LCD���Զ�ɨ�跽��
*****************************************************************************/  
void LCD_Scan(void)
{
	u16 regval=0;
  regval|=L2R_U2D; //������,���ϵ���
	regval|=1<<12;  
	LCD_WriteReg(0X03,regval);
	
} 
/*****************************************************************************
** ��������: LCD_DrawPoint
** ��������: ����
** x:0~239
** y:0~319
** POINT_COLOR:�˵����ɫ
*****************************************************************************/


void LCD_DrawPoint(u16 x,u16 y,u16 Color)
{
	LCD_SetCursor(x,y);//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD_WR_DATA(Color); 
} 


/*****************************************************************************
** ��������: LCD_GPIO_Configuration
** ��������: LCD_IO������
*****************************************************************************/
void LCD_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;	   //GPIO_Pin_10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC	

	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB

	GPIO_SetBits(GPIOB,GPIO_Pin_All);
 }


/*****************************************************************************
** ��������: LCD_GPIOB_Configuration
** ��������: lcd���ݴ����õ���IO����
*****************************************************************************/

void LCD_GPIOB_Configuration(u8 sel)
{
  GPIO_InitTypeDef GPIO_InitStructure;
if(sel==1)
{
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB

	GPIO_SetBits(GPIOB,GPIO_Pin_All);
}
else 
{
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB
}
}

 

/*****************************************************************************
** ��������: LCD_Init
** ��������: LCD��ʼ��
*****************************************************************************/
void LCD_Init(void)
{  
	LCD_GPIO_Configuration();

	LCD_WriteReg(0x0000,0x0001);//����ȥ��

 
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
	LCD_LED;//��������	 
}  		  

/*****************************************************************************
** ��������: LCD_Clear
** ��������: ����Ļ����
** Color:Ҫ���������ɫ
*****************************************************************************/

void LCD_Clear(u16 Color)
{
	u32 index=0;      
	LCD_SetCursor(0x00,0x0000);//���ù��λ�� 
	LCD_WriteRAM_Prepare();     //��ʼд��GRAM	 	  
	for(index=0;index<76800;index++)
	{
		LCD_WR_DATA(Color);    
	}
} 






