/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2013  Evidence Srl
 *
 * This file is part of ERIKA Enterprise.
 *
 * ERIKA Enterprise is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation,
 * (with a special exception described below).
 *
 * Linking this code statically or dynamically with other modules is
 * making a combined work based on this code.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this code with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this code, you may extend
 * this exception to your version of the code, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * ERIKA Enterprise is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 * ###*E*### */

#include "ee.h"
#include "stm32f4xx.h"
#include <stdio.h>

#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

GPIO_InitTypeDef  GPIO_InitStructure;


__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;


int recv_data(uint8_t *buf)
{
    int len = 0, len_cnt = 0;
    uint8_t c;
    len = VCP_get_char(&c);
    while (len != 0) {
        buf[len_cnt] = c;
        len_cnt++;
        len = VCP_get_char(&c);
    }
    return len_cnt;
}


/*
 * SysTick ISR2
 */
ISR2(systick_handler)
{
	/* count the interrupts, waking up expired alarms */
	CounterTick(myCounter);
}


TASK(BlinkLed_0)
{
    
}


TASK(BlinkLed_1)
{
 
    static EE_UINT8 blink_1 = 0;
	if(blink_1==0)
	{
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		blink_1=1;
	}
	else
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		blink_1 =0;
	}
        
    
}


TASK(BlinkLed_2)
{
    static EE_UINT8 blink_2 = 0;
    static EE_UINT8 usb_trigger = 0;
   // static int i=0;
   // uint8_t buf[9];
    
	if(blink_2==0)
	{
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		blink_2=1;
	}
	else
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		blink_2 =0;
	}
    
    
    
    
    static EE_UINT8 blink_x = 0;
	if(blink_x==0)
	{
		USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb);
		blink_x=1;
	}
	
    
    
   /*
    if(usb_trigger==0){
        usb_trigger=1;
        //USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb);
        
    }else{
        usb_trigger=1;
      //  buf[8] = '\n';
      //  sprintf(buf, "%i", i);
     //   VCP_send_buffer(buf, sizeof(buf));
      //  i++;
        
        
    }
    */
}

#define MAX_ROUND 5;
TASK(TaskIOToggle)
{
	static EE_UINT8 act = 0;

	switch (act) {
	case 0:
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		act = (act + 1) % MAX_ROUND;
	break;
	case 1:
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		act = (act + 1) % MAX_ROUND;
	break;
	case 2:
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		act = (act + 1) % MAX_ROUND;
	break;
	case 3:
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		act = (act + 1) % MAX_ROUND;
	break;
	case 4:
		GPIO_ResetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
		act = (act + 1) % MAX_ROUND;
	break;
	}
	
}

void timing_handler() {
    
}

int main(void)
{

	/* Preconfiguration before using DAC----------------------------------------*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*
	 * Setup the microcontroller system.
	 * Initialize the Embedded Flash Interface, the PLL and update the
	 * SystemFrequency variable.
	 * For default settings look at:
	 * pkg/mcu/st_stm32_stm32f4xx/src/system_stm32f4xx.c
	 */
	SystemInit();

	/*Initialize Erika related stuffs*/
	EE_system_init();

	/*Initialize systick */
	EE_systick_set_period(MILLISECONDS_TO_TICKS(1, SystemCoreClock));
	EE_systick_enable_int();
	EE_systick_start();

	/* GPIOD Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
									GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Program cyclic alarms which will fire after an initial offset,
	 * and after that periodically
	 * */
	SetRelAlarm(AlarmToggle_0, 10, 100);
	SetRelAlarm(AlarmToggle_1, 10, 100);
    SetRelAlarm(AlarmToggle_2, 10, 100);

	/* Forever loop: background activities (if any) should go here */
	for (;;);

}

