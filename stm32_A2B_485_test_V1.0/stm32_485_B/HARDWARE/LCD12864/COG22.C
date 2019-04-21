//12864cog-1 / 12864-cog-lcd1  ������������
//��Ʒ�ں�ST7565R    


#include "COG22.h"


void COG22_IOInit()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PB,PE�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	GPIO_SetBits(GPIOA,GPIO_Pin_1);						 //PB.5 �����
	GPIO_SetBits(GPIOA,GPIO_Pin_2);						 //PB.5 �����
	GPIO_SetBits(GPIOA,GPIO_Pin_3);						 //PB.5 �����
	GPIO_SetBits(GPIOA,GPIO_Pin_4);						 //PB.5 �����
	GPIO_SetBits(GPIOA,GPIO_Pin_5);						 //PB.5 �����
}



//=============transfer command to LCM===============����
//ֱ�Ӻ͵ײ�Ӳ�����
void transfer_command(int data1)

{
	char i;
	cs1=0;
	rs=0;
	for(i=0;i<8;i++)
	{
		lcd_sclk=0;
		//delay_us(10); //��������ʱ
		if(data1&0x80) 
			lcd_sid=1;
		else lcd_sid=0;
			lcd_sclk=1;
		//delay_us(10); //��������ʱ
		data1=data1<<=1;
	}
	cs1=1;
}



//ֱ�Ӻ͵ײ�Ӳ�����
//-----------transfer data to LCM---------------
void transfer_data(int data1)
{
	char i;

	cs1=0;
	rs=1;
	for(i=0;i<8;i++)
	{
		lcd_sclk=0;
		if(data1&0x80) 
			lcd_sid=1;
		else lcd_sid=0;
			lcd_sclk=1;
		data1=data1<<=1;
	}
	cs1=1;
}



//=============transfer command to LCM===============

/*��ʱ*/
void delay(int i)
{
	int j,k;
	for(j=0;j<i;j++)
	for(k=0;k<310;k++);
}


//ֱ�Ӻ͵ײ�Ӳ�����
/*LCD ģ���ʼ��*/
void initial_lcd()
{
	COG22_IOInit();
	delay(100);
	cs1=0;
	reset=0; /*�͵�ƽ��λ*/
	delay(20);
	reset=1; /*��λ���*/
	delay(20);
	transfer_command(0xe2); /*����λ*/
	delay(50);
	transfer_command(0x2c); /*��ѹ����1*/
	delay(50);
	transfer_command(0x2e); /*��ѹ����2*/
	delay(50);
	transfer_command(0x2f); /*��ѹ����3*/
	delay(50);
	transfer_command(0x24); /*�ֵ��Աȶȣ������÷�Χ0x20��0x27*/
	transfer_command(0x81); /*΢���Աȶ�*/
	transfer_command(0x23); /*0x1a,΢���Աȶȵ�ֵ�������÷�Χ0x00��0x3f 1f*/
	transfer_command(0xa2); /*1/9 ƫѹ�ȣ�bias��*/
	transfer_command(0xc8); /*��ɨ��˳�򣺴��ϵ���*/
	transfer_command(0xa0); /*��ɨ��˳�򣺴�����*/
	transfer_command(0x40); /*��ʼ�У���һ�п�ʼ*/
	transfer_command(0xaf); /*����ʾ*/
	cs1=1;
}


void lcd_address(uchar page,uchar column)
{
	cs1=0;
	column=column; //
	page=page-1;
	transfer_command(0xb0+page); 
	transfer_command(((column>>4)&0x0f)+0x10); //�����е�ַ�ĸ�4 λ
	transfer_command(column&0x0f); //�����е�ַ�ĵ�4 λ
}


/*ȫ������*/
void clear_screen()
{
	unsigned char i,j;
	cs1=0;
	for(i=0;i<9;i++)
	{
		lcd_address(1+i,0);
		for(j=0;j<132;j++)
		{
			transfer_data(0x00);
		}
	}
	cs1=1;
}


void test_display(uchar data1,uchar data2)
{
	int i,j;
	for(j=0;j<8;j++)
	{
		cs1=0;
		lcd_address(j+1,0);//ҳ���е�ַ
		for(i=0;i<64;i++)
		{
			transfer_data(data1);
			transfer_data(data2);
		}
	}
}


void display_string_8x16(uchar fb,uint page,uint column,uchar *text)
{
	uint i=0,j,k,n;
	cs1=0;
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;

			for(n=0;n<2;n++)
			{
				lcd_address(page+n,column);
				for(k=0;k<8;k++)
				{
					if (fb==1) 
						transfer_data(ascii_table_8x16[j][k+8*n]);
					else 
						transfer_data(~(ascii_table_8x16[j][k+8*n]));
				}
			}
			i++;
			column+=8;
		}
		else
		i++;
	}
}


void display_string_8x16_t(uchar fb,uint page,uint column,uchar text)
{
	uint j,k,n;
	j=text+16;
	for(n=0;n<2;n++)
	{				
	lcd_address(page+n,column);
		for(k=0;k<8;k++)
		{
		if (fb==1) 
			transfer_data(ascii_table_8x16[j][k+8*n]);
		else 
			transfer_data(~(ascii_table_8x16[j][k+8*n]));
		}
	}
		
} 
 
 
void display_string_5x8(uint page,uint column,uchar *text)
{
	uint i=0,j,k;
	cs1=0;
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<0x7e))	
		{
			j=text[i]-0x20;
			lcd_address(page,column);
			for(k=0;k<5;k++)					
			{
				transfer_data(ascii_table_5x8[j][k]);
			}
			i++;
			column+=6;							
		}
		else
		i++;
	}
}




void display_graphic_16x16(uchar fb,uchar page,uchar column,uchar *dp)
{
	uint i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<16;i++)
		{
			if(fb==1) 
				transfer_data(*dp);
			else 
				transfer_data(~(*dp)); 
			dp++;
		}
	}
}


//��ʾ128x64 ����߿�

void display_bk()
{
	uint i,j;

	//���
	for(j=0;j<8;j++)
	{
		lcd_address(j+1,0);
		transfer_data(0xff); 
	}

	//�ҿ�
	for(j=0;j<8;j++)
	{
		lcd_address(j+1,127);
		transfer_data(0xff); 
	}

	//�Ͽ�
	lcd_address(1,1);
	for (i=0;i<126;i++)
	{
		transfer_data(0x01);
	}

	//�¿�
	lcd_address(8,1);
	for (i=0;i<126;i++)
	{
		transfer_data(0x80); 
	}

}
/*

//~~~~~~~~~~~~~~~~~~~~~@~~~~~~~~&~~~~~~~~~~@~~~~~~~~~~~~����ʼ
void main1(void)
{

		initial_lcd();//ȥ��ʼ����ʾģ��ļĴ���

		clear_screen(); //clear all dots//��ʾ������
		
		clear_screen(); //clear all dots
		display_string_8x16(0,1,8," HUIJINGKEJI");
		display_string_8x16(1,3,1,"WELCOME TO HUIJI");
		display_string_5x8(5,1,"TEL: 0755-23146001");
		display_string_5x8(6,1,"FAX: 0755-23146002");
		display_string_5x8(7,1,"www.huijinglcm.com");
		display_string_5x8(8,1,"huijinglcm@sina.com");
		delay(700);
		//~~~~~~~~~~~~~~~~~~~~~@~~~~~~~~&~~~~~~~~~~@~~~~~~~~~~~~8�����Խ��ַ���ʾ

		clear_screen(); //clear all dots
		display_bk();
		display_graphic_16x16(1,3,33,hui1); 	
		display_graphic_16x16(1,3,49,jing1); 	
		display_graphic_16x16(1,3,65,ke1); 	
		display_graphic_16x16(1,3,81,ji1); 	
		display_string_8x16(1,5,40,"128*64");
		delay(700);
		//~~~~~~~~~~~~~~~~~~~~~@~~~~~~~~&~~~~~~~~~~@~~~~~~~~~~~~9�����Խ�����ͼ��
		clear_screen(); //clear all dots

		transfer_command(0xa7);//����

		//~~~~~~~~~~~~~~~~~~~~~@~~~~~~~~&~~~~~~~~~~@~~~~~~~~~~~~10����������ʾ
		clear_screen(); //clear all dots
		transfer_command(0xa6);
		}
		initial_lcd();
		clear_screen(); //clear all dots
		display_graphic_16x16(0,1,0,hui1); 	
		display_graphic_16x16(0,3,0,jing1); 
		display_string_8x16(0,6,0,"TEL:755-23146001");
		display_string_5x8(8,4,".WWW.HUIJINGLCM.COM.");
		display_string_5x8(1,40,"~~~~~~~~~~~~~~");
		display_string_5x8(4,40,"~~~~~~~~~~~~~~");
	
	//~~~~~~~~~~~~~~~~~~~~~@~~~~~~~~&~~~~~~~~~~@~~~~~~~~~~~~11������̬����ʱ����ʾ
	
}
*/

