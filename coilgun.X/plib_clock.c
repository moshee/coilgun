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




static void OSCCTRL_Initialize(void)
{
    /****************** XOSC Initialization   ********************************/


    /* Configure External Oscillator */
    OSCCTRL_REGS->OSCCTRL_XOSCCTRL = (uint16_t)(OSCCTRL_XOSCCTRL_STARTUP(0UL) | OSCCTRL_XOSCCTRL_GAIN(3UL) | OSCCTRL_XOSCCTRL_XTALEN_Msk | OSCCTRL_XOSCCTRL_ENABLE_Msk);

    while((OSCCTRL_REGS->OSCCTRL_STATUS & OSCCTRL_STATUS_XOSCRDY_Msk) != OSCCTRL_STATUS_XOSCRDY_Msk)
    {
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

static void OSC32KCTRL_Initialize(void)
{
    OSC32KCTRL_REGS->OSC32KCTRL_OSC32K = 0x0UL;

    OSC32KCTRL_REGS->OSC32KCTRL_RTCCTRL = OSC32KCTRL_RTCCTRL_RTCSEL(0UL);
}

static void FDPLL_Initialize(void)
{

    /****************** DPLL Initialization  *********************************/

    // 16MHz XOSC source
    // pre-divide by 2*(7 + 1) = 16 to get within DPLL's 32kHz-2MHz input range
    OSCCTRL_REGS->OSCCTRL_DPLLCTRLB = OSCCTRL_DPLLCTRLB_FILTER(0UL)
                                    | OSCCTRL_DPLLCTRLB_LTIME(0UL)
                                    | OSCCTRL_DPLLCTRLB_REFCLK_XOSC
                                    | OSCCTRL_DPLLCTRLB_DIV(7);


    // multiply 1 MHz by (47 + 1 + 0/16) = 48
    OSCCTRL_REGS->OSCCTRL_DPLLRATIO = OSCCTRL_DPLLRATIO_LDRFRAC(0UL)
                                    | OSCCTRL_DPLLRATIO_LDR(47UL);
    OSCCTRL_REGS->OSCCTRL_DPLLPRESC = OSCCTRL_DPLLPRESC_PRESC_DIV2;

    while ((OSCCTRL_REGS->OSCCTRL_DPLLSYNCBUSY & OSCCTRL_DPLLSYNCBUSY_DPLLRATIO_Msk) == OSCCTRL_DPLLSYNCBUSY_DPLLRATIO_Msk) {
        /* Waiting for the synchronization */
    }

    /* Selection of the DPLL Enable */
    OSCCTRL_REGS->OSCCTRL_DPLLCTRLA = (uint8_t)(OSCCTRL_DPLLCTRLA_ENABLE_Msk);

    while((OSCCTRL_REGS->OSCCTRL_DPLLSYNCBUSY & OSCCTRL_DPLLSYNCBUSY_ENABLE_Msk) == OSCCTRL_DPLLSYNCBUSY_ENABLE_Msk ) {
        /* Waiting for the DPLL enable synchronization */
    }

    while((OSCCTRL_REGS->OSCCTRL_DPLLSTATUS & (OSCCTRL_DPLLSTATUS_LOCK_Msk | OSCCTRL_DPLLSTATUS_CLKRDY_Msk)) !=
                (OSCCTRL_DPLLSTATUS_LOCK_Msk | OSCCTRL_DPLLSTATUS_CLKRDY_Msk)) {
        /* Waiting for the Ready state */
    }
}


static void GCLK0_Initialize(void)
{

    GCLK_REGS->GCLK_GENCTRL[0] = GCLK_GENCTRL_DIVSEL_DIV1
                               | GCLK_GENCTRL_SRC_DPLL96M
                               | GCLK_GENCTRL_GENEN_Msk;

    while((GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL0_Msk) == GCLK_SYNCBUSY_GENCTRL0_Msk)
    {
        /* wait for the Generator 0 synchronization */
    }
}

void CLOCK_Initialize (void) {
    /* Function to Initialize the Oscillators */
    OSCCTRL_Initialize();

    /* Function to Initialize the 32KHz Oscillators */
    OSC32KCTRL_Initialize();

    FDPLL_Initialize();
    GCLK0_Initialize();


    /* Selection of the Generator and write Lock for SERCOM1_CORE */
    GCLK_REGS->GCLK_PCHCTRL[20] = GCLK_PCHCTRL_GEN(0x0UL)  | GCLK_PCHCTRL_CHEN_Msk;

    while ((GCLK_REGS->GCLK_PCHCTRL[20] & GCLK_PCHCTRL_CHEN_Msk) != GCLK_PCHCTRL_CHEN_Msk)
    {
        /* Wait for synchronization */
    }
    /* Selection of the Generator and write Lock for SERCOM2_CORE */
    GCLK_REGS->GCLK_PCHCTRL[21] = GCLK_PCHCTRL_GEN(0x0UL)  | GCLK_PCHCTRL_CHEN_Msk;

    while ((GCLK_REGS->GCLK_PCHCTRL[21] & GCLK_PCHCTRL_CHEN_Msk) != GCLK_PCHCTRL_CHEN_Msk)
    {
        /* Wait for synchronization */
    }
    /* Selection of the Generator and write Lock for SERCOM3_CORE */
    GCLK_REGS->GCLK_PCHCTRL[22] = GCLK_PCHCTRL_GEN(0x0UL)  | GCLK_PCHCTRL_CHEN_Msk;

    while ((GCLK_REGS->GCLK_PCHCTRL[22] & GCLK_PCHCTRL_CHEN_Msk) != GCLK_PCHCTRL_CHEN_Msk)
    {
        /* Wait for synchronization */
    }
    /* Selection of the Generator and write Lock for TCC0 TCC1 */
    GCLK_REGS->GCLK_PCHCTRL[28] = GCLK_PCHCTRL_GEN(0x0UL)  | GCLK_PCHCTRL_CHEN_Msk;

    while ((GCLK_REGS->GCLK_PCHCTRL[28] & GCLK_PCHCTRL_CHEN_Msk) != GCLK_PCHCTRL_CHEN_Msk)
    {
        /* Wait for synchronization */
    }
    /* Selection of the Generator and write Lock for TC2 TC3 */
    GCLK_REGS->GCLK_PCHCTRL[31] = GCLK_PCHCTRL_GEN(0x0UL)  | GCLK_PCHCTRL_CHEN_Msk;

    while ((GCLK_REGS->GCLK_PCHCTRL[31] & GCLK_PCHCTRL_CHEN_Msk) != GCLK_PCHCTRL_CHEN_Msk)
    {
        /* Wait for synchronization */
    }
    /* Selection of the Generator and write Lock for ADC0 */
    GCLK_REGS->GCLK_PCHCTRL[33] = GCLK_PCHCTRL_GEN(0x0UL)  | GCLK_PCHCTRL_CHEN_Msk;

    while ((GCLK_REGS->GCLK_PCHCTRL[33] & GCLK_PCHCTRL_CHEN_Msk) != GCLK_PCHCTRL_CHEN_Msk)
    {
        /* Wait for synchronization */
    }



    /* Configure the APBC Bridge Clocks */
    MCLK_REGS->MCLK_APBCMASK = 0x2c21cU;


}

#if 0
void CfgDPLLfor48MhzRunCpu(void) {
    /* change the flash wait states */
    NVMCTRL->CTRLB.bit.RWS = 3;
    /* change the OSC48M to run at 24MHz */
    //OSCCTRL->OSC48MDIV.reg = 1;

    ///*  configure GCLK1 to be the source for DPLL */
    GCLK_GENCTRL_Type genctrl_1;
    genctrl_1.bit.DIV = 16,      /* Division Factor get a 1MHz clock */
    //genctrl_1.bit.DIV = 24,      /* Division Factor get a 1MHz clock */
    genctrl_1.bit.RUNSTDBY = 0;  /* Run in Standby */
    genctrl_1.bit.DIVSEL = 0;    /* Divide Selection */
    genctrl_1.bit.OE = 0;        /* Output Enable */
    genctrl_1.bit.OOV = 0;       /* Output Off Value */
    genctrl_1.bit.IDC = 1;       /* Improve Duty Cycle */
    genctrl_1.bit.GENEN = 1;     /* Generator Enable */
    //genctrl_1.bit.SRC = GCLK_GENCTRL_SRC_OSC48M_Val; /* Generator Clock Source */
    genctrl_1.bit.SRC = GCLK_GENCTRL_SRC_XOSC_Val;
    GCLK->GENCTRL[1].reg = genctrl_1.reg;

    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL


    ///* FDPLL96M input clock source for reference */
    GCLK_PCHCTRL_Type pchctrl_0;
    pchctrl_0.bit.WRTLOCK = 0;
    pchctrl_0.bit.CHEN = 1;
    pchctrl_0.bit.GEN = GCLK_PCHCTRL_GEN_GCLK1;
    GCLK->PCHCTRL[0].reg = pchctrl_0.reg;



    /* set the ratio to 48MHz */
    OSCCTRL_DPLLRATIO_Type dpllratio;
    dpllratio.bit.LDRFRAC = 0;
    dpllratio.bit.LDR = 47;
    //dpllratio.bit.LDR = 2;
    OSCCTRL->DPLLRATIO.reg = dpllratio.reg;



    /* configure the DPLL and use GCLK as the reference clock */
    OSCCTRL_DPLLCTRLB_Type dpllctrlb;
    dpllctrlb.bit.DIV = 0;       /* XOSC clock division factor */
    dpllctrlb.bit.LBYPASS = 0;   /* DPLL Lock signal drives the DPLL controller internal logic */
    dpllctrlb.bit.LTIME = 0;     /* No time-out. Automatic lock */
    //dpllctrlb.bit.REFCLK = 2;  /* GCLK clock reference */
    dpllctrlb.bit.REFCLK = 1;    /* XOSC clock reference */
    dpllctrlb.bit.WUF = 0;       /* DPLL clock is output after startup and lock time */
    dpllctrlb.bit.LPEN = 0;      /* low-power mode is disabled. Time to Digital Converter is enabled */
    dpllctrlb.bit.FILTER = 0;    /* Default filter mode */
    OSCCTRL->DPLLCTRLB.reg = dpllctrlb.reg;



    /* leave the DPLL output at 96MHz, can be used to the TCC blocks */
    OSCCTRL->DPLLPRESC.reg = 0;



    /* Enable the DPLL */
    OSCCTRL_DPLLCTRLA_Type dpllctrla;
    dpllctrla.bit.ONDEMAND = 0, /* The DPLL is disabled if no peripheral is requesting the clock source */
    dpllctrla.bit.RUNSTDBY = 0, /* The DPLL is disabled in standby sleep mode if no peripheral requests the clock */
    dpllctrla.bit.ENABLE = 1,   /* The DPLL is enabled */
    OSCCTRL->DPLLCTRLA.reg = dpllctrla.reg;

    while (true) {
        bool ready = ((OSCCTRL->DPLLSTATUS.reg &
               (OSCCTRL_DPLLSTATUS_CLKRDY | OSCCTRL_DPLLSTATUS_LOCK)) ==
               (OSCCTRL_DPLLSTATUS_CLKRDY | OSCCTRL_DPLLSTATUS_LOCK));

        if (ready) break;
    }
}
#endif