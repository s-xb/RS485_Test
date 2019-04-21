//12864cog-1 / 12864-cog-lcd1  串口驱动程序
//产品内核ST7565R    


#include "COG22.h"


void COG22_IOInit()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_SetBits(GPIOA,GPIO_Pin_1);						 //PB.5 输出高
	GPIO_SetBits(GPIOA,GPIO_Pin_2);						 //PB.5 输出高
	GPIO_SetBits(GPIOA,GPIO_Pin_3);						 //PB.5 输出高
	GPIO_SetBits(GPIOA,GPIO_Pin_4);						 //PB.5 输出高
	GPIO_SetBits(GPIOA,GPIO_Pin_5);						 //PB.5 输出高
}



//=============transfer command to LCM===============串口
//直接和底层硬件相关
void transfer_command(int data1)

{
	char i;
	cs1=0;
	rs=0;
	for(i=0;i<8;i++)
	{
		lcd_sclk=0;
		//delay_us(10); //加少量延时
		if(data1&0x80) 
			lcd_sid=1;
		else lcd_sid=0;
			lcd_sclk=1;
		//delay_us(10); //加少量延时
		data1=data1<<=1;
	}
	cs1=1;
}



//直接和底层硬件相关
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

/*延时*/
void delay(int i)
{
	int j,k;
	for(j=0;j<i;j++)
	for(k=0;k<310;k++);
}


//直接和底层硬件相关
/*LCD 模块初始化*/
void initial_lcd()
{
	COG22_IOInit();
	delay(100);
	cs1=0;
	reset=0; /*低电平复位*/
	delay(20);
	reset=1; /*复位完毕*/
	delay(20);
	transfer_command(0xe2); /*软复位*/
	delay(50);
	transfer_command(0x2c); /*升压步聚1*/
	delay(50);
	transfer_command(0x2e); /*升压步聚2*/
	delay(50);
	transfer_command(0x2f); /*升压步聚3*/
	delay(50);
	transfer_command(0x24); /*粗调对比度，可设置范围0x20～0x27*/
	transfer_command(0x81); /*微调对比度*/
	transfer_command(0x23); /*0x1a,微调对比度的值，可设置范围0x00～0x3f 1f*/
	transfer_command(0xa2); /*1/9 偏压比（bias）*/
	transfer_command(0xc8); /*行扫描顺序：从上到下*/
	transfer_command(0xa0); /*列扫描顺序：从左到右*/
	transfer_command(0x40); /*起始行：第一行开始*/
	transfer_command(0xaf); /*开显示*/
	cs1=1;
}


void lcd_address(uchar page,uchar column)
{
	cs1=0;
	column=column; //
	page=page-1;
	transfer_command(0xb0+page); 
	transfer_command(((column>>4)&0x0f)+0x10); //设置列地址的高4 位
	transfer_command(column&0x0f); //设置列地址的低4 位
}


/*全屏清屏*/
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
		lcd_address(j+1,0);//页，列地址
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


//显示128x64 点阵边框

void display_bk()
{
	uint i,j;

	//左框
	for(j=0;j<8;j++)
	{
		lcd_address(j+1,0);
		transfer_data(0xff); 
	}

	//右框
	for(j=0;j<8;j++)
	{
		lcd_address(j+1,127);
		transfer_data(0xff); 
	}

	//上框
	lcd_address(1,1);
	for (i=0;i<126;i++)
	{
		transfer_data(0x01);
	}

	//下框
	lcd_address(8,1);
	for (i=0;i<126;i++)
	{
		transfer_data(0x80); 
	}

}
/*

//~~~~~~~~~~~~~~~~~~~~~@~~~~~~~~&~~~~~~~~~~@~~~~~~~~~~~~程序开始
void main1(void)
{

		initial_lcd();//去初始化显示模块的寄存器

		clear_screen(); //clear all dots//显示屏清屏
		
		clear_screen(); //clear all dots
		display_string_8x16(0,1,8," HUIJINGKEJI");
		display_string_8x16(1,3,1,"WELCOME TO HUIJI");
		display_string_5x8(5,1,"TEL: 0755-23146001");
		display_string_5x8(6,1,"FAX: 0755-23146002");
		display_string_5x8(7,1,"www.huijinglcm.com");
		display_string_5x8(8,1,"huijinglcm@sina.com");
		delay(700);
		//~~~~~~~~~~~~~~~~~~~~~@~~~~~~~~&~~~~~~~~~~@~~~~~~~~~~~~8屏，自建字符演示

		clear_screen(); //clear all dots
		display_bk();
		display_graphic_16x16(1,3,33,hui1); 	
		display_graphic_16x16(1,3,49,jing1); 	
		display_graphic_16x16(1,3,65,ke1); 	
		display_graphic_16x16(1,3,81,ji1); 	
		display_string_8x16(1,5,40,"128*64");
		delay(700);
		//~~~~~~~~~~~~~~~~~~~~~@~~~~~~~~&~~~~~~~~~~@~~~~~~~~~~~~9屏，自建汉字图形
		clear_screen(); //clear all dots

		transfer_command(0xa7);//反显

		//~~~~~~~~~~~~~~~~~~~~~@~~~~~~~~&~~~~~~~~~~@~~~~~~~~~~~~10屏，反白演示
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
	
	//~~~~~~~~~~~~~~~~~~~~~@~~~~~~~~&~~~~~~~~~~@~~~~~~~~~~~~11屏，动态变量时钟演示
	
}
*/


