/*****************************************************************************
* Product: PELICAN crossing example, QK-nano kernel, LPCXpresso-1343, GNU
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Mar 01, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qpn_port.h"
#include "bsp.h"
#include "pelican.h"

#include "LPC13xx.h"                                 /* LPC13xx definitions */
#include "timer16.h"
#include "clkconfig.h"
#include "gpio.h"

#define LED_PORT    0
#define LED_BIT     7
#define LED_ON      1
#define LED_OFF     0

enum ISR_Priorities {   /* ISR priorities starting from the highest urgency */
    PIOINT0_PRIO,
    SYSTICK_PRIO,
    /* ... */
};

/*..........................................................................*/
void SysTick_Handler(void) __attribute__((__interrupt__));
void SysTick_Handler(void) {
    QK_ISR_ENTRY();                       /* inform QK-nano about ISR entry */
    QF_tickISR();
    QK_ISR_EXIT();                         /* inform QK-nano about ISR exit */
}
/*..........................................................................*/
void PIOINT0_IRQHandler(void) __attribute__((__interrupt__));
void PIOINT0_IRQHandler(void) {
    QK_ISR_ENTRY();                       /* inform QK-nano about ISR entry */
    QActive_postISR((QActive *)&AO_Ped, PEDS_WAITING_SIG, 0);
    QK_ISR_EXIT();                         /* inform QK-nano about ISR exit */
}
/*..........................................................................*/
void BSP_init(void) {
    SystemInit();                         /* initialize the clocking system */
    QK_init();                                        /* initialize QK-nano */
    GPIOInit();                                          /* initialize GPIO */
    GPIOSetDir(LED_PORT, LED_BIT, 1);         /* set port for LED to output */
}
/*..........................................................................*/
void QF_onStartup(void) {
    /* Set up and enable the SysTick timer. It will be used as a reference
    * for delay loops in the interrupt handlers. The SysTick timer period
    * will be set up for BSP_TICKS_PER_SEC.
    */
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

           /* enable EINT0 interrupt, which is used for testing preemptions */
    NVIC_EnableIRQ(EINT0_IRQn);

                       /* set priorities of all interrupts in the system... */
    NVIC_SetPriority(SysTick_IRQn, SYSTICK_PRIO);
    NVIC_SetPriority(EINT0_IRQn,   PIOINT0_PRIO);
}
/*..........................................................................*/
void QK_onIdle(void) {
                         /* toggle the User LED on and then off, see NOTE01 */
    //QF_INT_DISABLE();
    //GPIOSetValue(LED_PORT, LED_BIT, LED_ON);                     /* LED on  */
    //GPIOSetValue(LED_PORT, LED_BIT, LED_OFF);                    /* LED off */
    //QF_INT_ENABLE();

#ifdef NDEBUG
    /* put the CPU and peripherals to the low-power mode */
    __WFI();                /* stop clocking the CPU and wait for interrupt */
#endif
}
/*..........................................................................*/
/* error routine that is called if the STM32 library encounters an error    */
void assert_failed(char const *file, int line) {
    Q_onAssert(file, line);
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    QF_INT_DISABLE();         /* make sure that all interrupts are disabled */
    for (;;) {       /* NOTE: replace the loop with reset for final version */
    }
}
/*..........................................................................*/
void BSP_showState(uint8_t prio, char const *state) {
}
/*..........................................................................*/
void BSP_signalCars(enum BSP_CarsSignal sig) {
}
/*..........................................................................*/
void BSP_signalPeds(enum BSP_PedsSignal sig) {
    if (sig == PEDS_DONT_WALK) {
        GPIOSetValue(LED_PORT, LED_BIT, LED_ON);                 /* LED on  */
    }
    else {
        GPIOSetValue(LED_PORT, LED_BIT, LED_OFF);                /* LED off */
    }
}

/*****************************************************************************
* NOTE01:
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts disabled, so no
* interrupt execution time contributes to the brightness of the User LED.
*/