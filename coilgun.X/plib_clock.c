/*******************************************************************************
 CLOCK PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_clock.c

  Summary:
    CLOCK PLIB Implementation File.

  Description:
    None

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#include "plib_clock.h"
#include "samc20j16a.h"




static void OSCCTRL_Initialize(void) {
    /****************** XOSC Initialization   ********************************/


    /* Configure External Oscillator */
    OSCCTRL_REGS->OSCCTRL_XOSCCTRL = (uint16_t)(
                                     OSCCTRL_XOSCCTRL_STARTUP(0UL)
                                   | OSCCTRL_XOSCCTRL_GAIN(3UL)
                                   | OSCCTRL_XOSCCTRL_XTALEN_Msk
                                   | OSCCTRL_XOSCCTRL_ENABLE_Msk
                                   );

    while ((OSCCTRL_REGS->OSCCTRL_STATUS & OSCCTRL_STATUS_XOSCRDY_Msk) != OSCCTRL_STATUS_XOSCRDY_Msk) {
        /* Waiting for the XOSC Ready state */
    }

#if 0
    uint32_t calibValue = (uint32_t)(((*(uint64_t*)0x806020UL) >> 41 ) & 0x3fffffUL);
    OSCCTRL_REGS->OSCCTRL_CAL48M = calibValue;


    /* Selection of the Division Value */
    OSCCTRL_REGS->OSCCTRL_OSC48MDIV = (uint8_t)OSCCTRL_OSC48MDIV_DIV(0UL);

    while((OSCCTRL_REGS->OSCCTRL_OSC48MSYNCBUSY & OSCCTRL_OSC48MSYNCBUSY_OSC48MDIV_Msk) == OSCCTRL_OSC48MSYNCBUSY_OSC48MDIV_Msk)
    {
        /* Waiting for the synchronization */
    }

    while((OSCCTRL_REGS->OSCCTRL_STATUS & OSCCTRL_STATUS_OSC48MRDY_Msk) != OSCCTRL_STATUS_OSC48MRDY_Msk)
    {
        /* Waiting for the OSC48M Ready state */
    }
#endif
}

static void OSC32KCTRL_Initialize(void) {
    OSC32KCTRL_REGS->OSC32KCTRL_OSC32K = 0x0UL;

    OSC32KCTRL_REGS->OSC32KCTRL_RTCCTRL = OSC32KCTRL_RTCCTRL_RTCSEL(0UL);
}

static void FDPLL_Initialize(void) {

    /****************** DPLL Initialization  *********************************/

    // 16MHz XOSC source
    // pre-divide by 2*(15 + 1) = 32 to get within DPLL's 32kHz-2MHz input range
    OSCCTRL_REGS->OSCCTRL_DPLLCTRLB = OSCCTRL_DPLLCTRLB_FILTER(0UL)
                                    | OSCCTRL_DPLLCTRLB_LTIME(0UL)
                                    | OSCCTRL_DPLLCTRLB_REFCLK_XOSC
                                    | OSCCTRL_DPLLCTRLB_DIV(15);


    // multiply 500 kHz by (95 + 1 + 0/16) = 96
    OSCCTRL_REGS->OSCCTRL_DPLLRATIO = OSCCTRL_DPLLRATIO_LDRFRAC(0UL)
                                    | OSCCTRL_DPLLRATIO_LDR(95UL);
    OSCCTRL_REGS->OSCCTRL_DPLLPRESC = OSCCTRL_DPLLPRESC_PRESC_DIV1;

    while ((OSCCTRL_REGS->OSCCTRL_DPLLSYNCBUSY & OSCCTRL_DPLLSYNCBUSY_DPLLRATIO_Msk) == OSCCTRL_DPLLSYNCBUSY_DPLLRATIO_Msk) {
        /* Waiting for the synchronization */
    }

    /* Selection of the DPLL Enable */
    OSCCTRL_REGS->OSCCTRL_DPLLCTRLA = (uint8_t)(OSCCTRL_DPLLCTRLA_ENABLE_Msk);

    while ((OSCCTRL_REGS->OSCCTRL_DPLLSYNCBUSY & OSCCTRL_DPLLSYNCBUSY_ENABLE_Msk) == OSCCTRL_DPLLSYNCBUSY_ENABLE_Msk) {
        /* Waiting for the DPLL enable synchronization */
    }

    while ((OSCCTRL_REGS->OSCCTRL_DPLLSTATUS & (OSCCTRL_DPLLSTATUS_LOCK_Msk | OSCCTRL_DPLLSTATUS_CLKRDY_Msk)) !=
                (OSCCTRL_DPLLSTATUS_LOCK_Msk | OSCCTRL_DPLLSTATUS_CLKRDY_Msk)) {
        /* Waiting for the Ready state */
    }
}

static void gclk_sync(int mask) {
    while (GCLK_REGS->GCLK_SYNCBUSY & mask);
}

static void GCLK_Initialize(void) {
    // 2 wait states needed for 48MHz operation
    NVMCTRL_REGS->NVMCTRL_CTRLB = NVMCTRL_CTRLB_MANW_Msk
                                | NVMCTRL_CTRLB_RWS_DUAL;

    GCLK_REGS->GCLK_GENCTRL[0] = GCLK_GENCTRL_DIVSEL_DIV1
                               | GCLK_GENCTRL_SRC_DPLL96M
                               | GCLK_GENCTRL_GENEN_Msk
                               | GCLK_GENCTRL_IDC_Msk
                               ;

    gclk_sync(GCLK_SYNCBUSY_GENCTRL0_Msk);

}

/**
 * Enable the peripheral clock using the source GCLK_PCHCTRL_GEN_GCLK* as a source
 */
static void pchctrl_enable(int i, uint32_t source) {
    GCLK_REGS->GCLK_PCHCTRL[i] = source | GCLK_PCHCTRL_CHEN_Msk;
    while ((GCLK_REGS->GCLK_PCHCTRL[i] & GCLK_PCHCTRL_CHEN_Msk) != GCLK_PCHCTRL_CHEN_Msk);
}

void CLOCK_Initialize (void) {
    /* Function to Initialize the Oscillators */
    OSCCTRL_Initialize();

    /* Function to Initialize the 32KHz Oscillators */
    OSC32KCTRL_Initialize();

    FDPLL_Initialize();
    GCLK_Initialize();

    /**
     * Enable peripheral clocks
     */

    pchctrl_enable(2, GCLK_PCHCTRL_GEN_GCLK0); // EIC
    pchctrl_enable(20, GCLK_PCHCTRL_GEN_GCLK0); // SERCOM1_CORE
    pchctrl_enable(21, GCLK_PCHCTRL_GEN_GCLK0); // SERCOM2_CORE
    pchctrl_enable(22, GCLK_PCHCTRL_GEN_GCLK0); // SERCOM3_CORE
    pchctrl_enable(28, GCLK_PCHCTRL_GEN_GCLK0); // TCC0/TCC1
    pchctrl_enable(30, GCLK_PCHCTRL_GEN_GCLK0); // TC0/TC1
    pchctrl_enable(31, GCLK_PCHCTRL_GEN_GCLK0); // TC2/TC3
    pchctrl_enable(33, GCLK_PCHCTRL_GEN_GCLK0); // ADC0

    /* Configure the APBC Bridge Clocks */
    MCLK_REGS->MCLK_APBAMASK &= ~(MCLK_APBAMASK_WDT_Msk | MCLK_APBAMASK_RTC_Msk);

    MCLK_REGS->MCLK_APBCMASK = MCLK_APBCMASK_SERCOM1_Msk
                             | MCLK_APBCMASK_SERCOM2_Msk
                             | MCLK_APBCMASK_SERCOM3_Msk
                             | MCLK_APBCMASK_TCC0_Msk
                             | MCLK_APBCMASK_TC0_Msk
                             | MCLK_APBCMASK_TC2_Msk
                             | MCLK_APBCMASK_TC3_Msk
                             | MCLK_APBCMASK_ADC0_Msk
                             ;

}