#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "usbdrv.h"
#include "osc_cal.h"

/*
 * TOOD:
 * - Watchdog? Brownout?
 */

/*
 * PB0: MOSI (ISP)
 * PB1: MISO (ISP)
 * PB2: SCK (ISP) / Button
 * PB3: D-
 * PB4: D+
 * PB5: RESET (ISP)
 */

#define EEPROM_ADDR_DEVICE_ID       ((uint8_t *) 1)
uint8_t device_id = 0x00;

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t *rq = (void *)data;
    static uchar replyBuf[1];

    usbMsgPtr = (usbMsgPtr_t) replyBuf;
    if(rq->bRequest == 0){      /* Get/Set Device ID */
        /* Reserved id 0x00 is used to just read the id.
         * All other values set the device id.
         */
        if (rq->wValue.bytes[0] != 0x00)
        {
            device_id = rq->wValue.bytes[0]; 
        	eeprom_update_byte(EEPROM_ADDR_DEVICE_ID, device_id);
        }
        replyBuf[0] = device_id;
        return 1;
    }

    return 0;
}


/**
 * Button interrupt.
 * Debouncing is realized through USB (poll interval 25ms).
 * Everything else can be done later in software.
 */
ISR (INT0_vect)
{
    static uchar data[1] = {0xa5};  /* marker 0xa5 to detect transmission problems. */

    /* Just update the interrupt data, regardless of current transferstate.
     * Either the data is overwritten, or another interrupt is send.
     */
    usbSetInterrupt(data, 1);

    GIMSK |= (1 << INTF0);
}



int main(void)
{
    /* read device id */
    device_id = eeprom_read_byte(EEPROM_ADDR_DEVICE_ID);

    /* Button with pull up */
    DDRB  &= ~(1 << PB2);
    PORTB |=  (1 << PB2);

    /* Some flank interrupt of the button. */
    MCUCR &= ~(1 << ISC00);
    MCUCR |= (1 << ISC01);
    GIMSK |= (1 << INTF0);

    usbInit();

    usbDeviceDisconnect();
    _delay_ms(500.0);
    usbDeviceConnect();

    sei();

    for(;;){    /* main event loop */
        usbPoll();
    }

    return 0;
}









