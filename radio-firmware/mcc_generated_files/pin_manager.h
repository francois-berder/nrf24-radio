/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F1829
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above
        MPLAB 	          :  MPLAB X 6.00	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set SERVO_2_SIGNAL aliases
#define SERVO_2_SIGNAL_TRIS                 TRISAbits.TRISA2
#define SERVO_2_SIGNAL_LAT                  LATAbits.LATA2
#define SERVO_2_SIGNAL_PORT                 PORTAbits.RA2
#define SERVO_2_SIGNAL_WPU                  WPUAbits.WPUA2
#define SERVO_2_SIGNAL_ANS                  ANSELAbits.ANSA2
#define SERVO_2_SIGNAL_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define SERVO_2_SIGNAL_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define SERVO_2_SIGNAL_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define SERVO_2_SIGNAL_GetValue()           PORTAbits.RA2
#define SERVO_2_SIGNAL_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define SERVO_2_SIGNAL_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define SERVO_2_SIGNAL_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define SERVO_2_SIGNAL_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define SERVO_2_SIGNAL_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define SERVO_2_SIGNAL_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set SERVO_4_SIGNAL aliases
#define SERVO_4_SIGNAL_TRIS                 TRISAbits.TRISA4
#define SERVO_4_SIGNAL_LAT                  LATAbits.LATA4
#define SERVO_4_SIGNAL_PORT                 PORTAbits.RA4
#define SERVO_4_SIGNAL_WPU                  WPUAbits.WPUA4
#define SERVO_4_SIGNAL_ANS                  ANSELAbits.ANSA4
#define SERVO_4_SIGNAL_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define SERVO_4_SIGNAL_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define SERVO_4_SIGNAL_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define SERVO_4_SIGNAL_GetValue()           PORTAbits.RA4
#define SERVO_4_SIGNAL_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define SERVO_4_SIGNAL_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define SERVO_4_SIGNAL_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define SERVO_4_SIGNAL_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define SERVO_4_SIGNAL_SetAnalogMode()      do { ANSELAbits.ANSA4 = 1; } while(0)
#define SERVO_4_SIGNAL_SetDigitalMode()     do { ANSELAbits.ANSA4 = 0; } while(0)

// get/set SERVO_3_SIGNAL aliases
#define SERVO_3_SIGNAL_TRIS                 TRISAbits.TRISA5
#define SERVO_3_SIGNAL_LAT                  LATAbits.LATA5
#define SERVO_3_SIGNAL_PORT                 PORTAbits.RA5
#define SERVO_3_SIGNAL_WPU                  WPUAbits.WPUA5
#define SERVO_3_SIGNAL_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define SERVO_3_SIGNAL_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define SERVO_3_SIGNAL_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define SERVO_3_SIGNAL_GetValue()           PORTAbits.RA5
#define SERVO_3_SIGNAL_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define SERVO_3_SIGNAL_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define SERVO_3_SIGNAL_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define SERVO_3_SIGNAL_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)

// get/set SOFTSPI_MOSI_IO aliases
#define SOFTSPI_MOSI_IO_TRIS                 TRISBbits.TRISB4
#define SOFTSPI_MOSI_IO_LAT                  LATBbits.LATB4
#define SOFTSPI_MOSI_IO_PORT                 PORTBbits.RB4
#define SOFTSPI_MOSI_IO_WPU                  WPUBbits.WPUB4
#define SOFTSPI_MOSI_IO_ANS                  ANSELBbits.ANSB4
#define SOFTSPI_MOSI_IO_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define SOFTSPI_MOSI_IO_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define SOFTSPI_MOSI_IO_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define SOFTSPI_MOSI_IO_GetValue()           PORTBbits.RB4
#define SOFTSPI_MOSI_IO_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define SOFTSPI_MOSI_IO_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define SOFTSPI_MOSI_IO_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define SOFTSPI_MOSI_IO_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define SOFTSPI_MOSI_IO_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define SOFTSPI_MOSI_IO_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set RB5 procedures
#define RB5_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define RB5_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define RB5_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define RB5_GetValue()              PORTBbits.RB5
#define RB5_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define RB5_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define RB5_SetPullup()             do { WPUBbits.WPUB5 = 1; } while(0)
#define RB5_ResetPullup()           do { WPUBbits.WPUB5 = 0; } while(0)
#define RB5_SetAnalogMode()         do { ANSELBbits.ANSB5 = 1; } while(0)
#define RB5_SetDigitalMode()        do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set SOFTSPI_SCK_IO aliases
#define SOFTSPI_SCK_IO_TRIS                 TRISBbits.TRISB6
#define SOFTSPI_SCK_IO_LAT                  LATBbits.LATB6
#define SOFTSPI_SCK_IO_PORT                 PORTBbits.RB6
#define SOFTSPI_SCK_IO_WPU                  WPUBbits.WPUB6
#define SOFTSPI_SCK_IO_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define SOFTSPI_SCK_IO_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define SOFTSPI_SCK_IO_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define SOFTSPI_SCK_IO_GetValue()           PORTBbits.RB6
#define SOFTSPI_SCK_IO_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define SOFTSPI_SCK_IO_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define SOFTSPI_SCK_IO_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define SOFTSPI_SCK_IO_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)

// get/set RB7 procedures
#define RB7_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define RB7_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define RB7_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define RB7_GetValue()              PORTBbits.RB7
#define RB7_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define RB7_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define RB7_SetPullup()             do { WPUBbits.WPUB7 = 1; } while(0)
#define RB7_ResetPullup()           do { WPUBbits.WPUB7 = 0; } while(0)

// get/set SERVO_1_SIGNAL aliases
#define SERVO_1_SIGNAL_TRIS                 TRISCbits.TRISC0
#define SERVO_1_SIGNAL_LAT                  LATCbits.LATC0
#define SERVO_1_SIGNAL_PORT                 PORTCbits.RC0
#define SERVO_1_SIGNAL_WPU                  WPUCbits.WPUC0
#define SERVO_1_SIGNAL_ANS                  ANSELCbits.ANSC0
#define SERVO_1_SIGNAL_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define SERVO_1_SIGNAL_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define SERVO_1_SIGNAL_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define SERVO_1_SIGNAL_GetValue()           PORTCbits.RC0
#define SERVO_1_SIGNAL_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define SERVO_1_SIGNAL_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define SERVO_1_SIGNAL_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define SERVO_1_SIGNAL_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define SERVO_1_SIGNAL_SetAnalogMode()      do { ANSELCbits.ANSC0 = 1; } while(0)
#define SERVO_1_SIGNAL_SetDigitalMode()     do { ANSELCbits.ANSC0 = 0; } while(0)

// get/set RF_CS_IO aliases
#define RF_CS_IO_TRIS                 TRISCbits.TRISC1
#define RF_CS_IO_LAT                  LATCbits.LATC1
#define RF_CS_IO_PORT                 PORTCbits.RC1
#define RF_CS_IO_WPU                  WPUCbits.WPUC1
#define RF_CS_IO_ANS                  ANSELCbits.ANSC1
#define RF_CS_IO_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define RF_CS_IO_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define RF_CS_IO_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define RF_CS_IO_GetValue()           PORTCbits.RC1
#define RF_CS_IO_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define RF_CS_IO_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define RF_CS_IO_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define RF_CS_IO_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define RF_CS_IO_SetAnalogMode()      do { ANSELCbits.ANSC1 = 1; } while(0)
#define RF_CS_IO_SetDigitalMode()     do { ANSELCbits.ANSC1 = 0; } while(0)

// get/set RF_CE_IO aliases
#define RF_CE_IO_TRIS                 TRISCbits.TRISC2
#define RF_CE_IO_LAT                  LATCbits.LATC2
#define RF_CE_IO_PORT                 PORTCbits.RC2
#define RF_CE_IO_WPU                  WPUCbits.WPUC2
#define RF_CE_IO_ANS                  ANSELCbits.ANSC2
#define RF_CE_IO_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define RF_CE_IO_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define RF_CE_IO_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define RF_CE_IO_GetValue()           PORTCbits.RC2
#define RF_CE_IO_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define RF_CE_IO_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define RF_CE_IO_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define RF_CE_IO_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define RF_CE_IO_SetAnalogMode()      do { ANSELCbits.ANSC2 = 1; } while(0)
#define RF_CE_IO_SetDigitalMode()     do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set RF_IRQ aliases
#define RF_IRQ_TRIS                 TRISCbits.TRISC6
#define RF_IRQ_LAT                  LATCbits.LATC6
#define RF_IRQ_PORT                 PORTCbits.RC6
#define RF_IRQ_WPU                  WPUCbits.WPUC6
#define RF_IRQ_ANS                  ANSELCbits.ANSC6
#define RF_IRQ_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define RF_IRQ_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define RF_IRQ_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RF_IRQ_GetValue()           PORTCbits.RC6
#define RF_IRQ_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define RF_IRQ_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define RF_IRQ_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define RF_IRQ_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define RF_IRQ_SetAnalogMode()      do { ANSELCbits.ANSC6 = 1; } while(0)
#define RF_IRQ_SetDigitalMode()     do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set SOFTSPI_MISO_IO aliases
#define SOFTSPI_MISO_IO_TRIS                 TRISCbits.TRISC7
#define SOFTSPI_MISO_IO_LAT                  LATCbits.LATC7
#define SOFTSPI_MISO_IO_PORT                 PORTCbits.RC7
#define SOFTSPI_MISO_IO_WPU                  WPUCbits.WPUC7
#define SOFTSPI_MISO_IO_ANS                  ANSELCbits.ANSC7
#define SOFTSPI_MISO_IO_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define SOFTSPI_MISO_IO_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define SOFTSPI_MISO_IO_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define SOFTSPI_MISO_IO_GetValue()           PORTCbits.RC7
#define SOFTSPI_MISO_IO_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define SOFTSPI_MISO_IO_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define SOFTSPI_MISO_IO_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define SOFTSPI_MISO_IO_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define SOFTSPI_MISO_IO_SetAnalogMode()      do { ANSELCbits.ANSC7 = 1; } while(0)
#define SOFTSPI_MISO_IO_SetDigitalMode()     do { ANSELCbits.ANSC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/