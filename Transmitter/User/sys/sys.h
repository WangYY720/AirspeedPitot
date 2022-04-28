#ifndef __SYS_H
#define __SYS_H	
#include "stm32f10x.h"	 

#ifndef STM32F10X_MD
#define STM32F10X_MD
#define USE_STDPERIPH_DRIVER
#endif
																	    
#define SYSTEM_SUPPORT_OS		1		//锟斤拷锟斤拷系统锟侥硷拷锟斤拷锟角凤拷支锟斤拷OS

//位锟斤拷锟斤拷锟斤拷,实锟斤拷51锟斤拷锟狡碉拷GPIO锟斤拷锟狡癸拷锟斤拷
//锟斤拷锟斤拷实锟斤拷思锟斤拷,锟轿匡拷<<CM3权锟斤拷指锟斤拷>>锟斤拷锟斤拷锟斤拷(87页~92页).
//IO锟节诧拷锟斤拷锟疥定锟斤拷
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO锟节碉拷址映锟斤拷
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO锟节诧拷锟斤拷,只锟皆碉拷一锟斤拷IO锟斤拷!
//确锟斤拷n锟斤拷值小锟斤拷16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //锟斤拷锟? 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //锟斤拷锟斤拷 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //锟斤拷锟? 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //锟斤拷锟斤拷 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //锟斤拷锟? 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //锟斤拷锟斤拷 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //锟斤拷锟? 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //锟斤拷锟斤拷 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //锟斤拷锟? 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //锟斤拷锟斤拷

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //锟斤拷锟? 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //锟斤拷锟斤拷

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //锟斤拷锟? 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //锟斤拷锟斤拷



void NVIC_Configuration(void); //嵌锟斤拷锟叫断匡拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
void RCC_Configuration(void); //RCC时锟斤拷锟斤拷锟斤拷锟斤拷锟?


#endif
