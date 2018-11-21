#include <msp430.h> 


/**
 * main.c
 */

int serial_init();
int RC4_code(char *ret,char *text,char *key,int text_len,int key_len);
int RC4_init(char *key,int key_len);
int keygen(char *key);

char base16[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
char input[40];
char output[80];
unsigned char s[256];
unsigned int index = 0;
char flag = 0;
char flagtx = 1;
char key[]="LCTF0000";

int main(void)
{
    char tmp;
    unsigned int n;

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    //======Timer Setting=====
    DCOCTL  = 0;             // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;   // Set range
    DCOCTL  = CALDCO_1MHZ;   // Set DCO step + modulation

    P1DIR |= BIT0 + BIT6;
    P1OUT |= BIT0 + BIT6;

    serial_init();
    keygen(key);
    index = 0;
	while(1)
	{
	    if(flag)
	    {
	        tmp = UCA0RXBUF;
	        if(tmp != '\r')
	        {
	            input[index++] = tmp;
	            flag = 0;
	        }
	        else
	        {
	            RC4_code(output,input,key,index,8);
	            for(n=0;n<index*2;n+=2)
	            {
	                output[n+1] = base16[ output[n]&0xf ];
	                output[n] = base16[ output[n]>>4 ];
	            }
	            for(n=0 ; n<index*2; n++)
	            {
	                while (!(IFG2&UCA0TXIFG));              // USCI_A0 TX buffer ready?
	                UCA0TXBUF = output[n];
	            }
	            index = 0;
	        }
	    }
	}

}

	//-----------------------------------------------------------------------//

	  //                Transmit and Receive interrupts                        //

	  //-----------------------------------------------------------------------//

int serial_init()
{
    P1SEL  =  BIT1 + BIT2;  // P1.1 UCA0RXD input

    P1SEL2 =  BIT1 + BIT2;  // P1.2 UCA0TXD output


    //------------UART(USCI_A0) Setting----------------//

    //UCA0CTL1 |=  UCSSEL_2 + UCSWRST;  // USCI Clock = SMCLK,USCI_A0 disabled
    UCA0CTL1 |=  UCSSEL_2;
    UCA0BR0   =  104;                 // 104 From datasheet table-
    UCA0BR1   =  0;                   // -selects baudrate =9600,clk = SMCLK
    UCA0MCTL  =  UCBRS0;             // Modulation value = 1 from datasheet
    //UCA0STAT |=  UCLISTEN;            // loop back mode enabled
    UCA0CTL1 &= ~UCSWRST;             // Clear UCSWRST to enable USCI_A0
    //IE2 |= UCA0TXIE;                  // Enable the Transmit interrupt
    IE2 |= UCA0RXIE;                  // Enable the Receive  interrupt
    _BIS_SR(GIE);                     // Enable the global interrupt

    return 0;
}


int keygen(char *key)
{
    unsigned char tmp = 0;
    P2DIR &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5);
    P2REN |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5);
    tmp = P2IN;

    key[4] = tmp * 3;
    key[5] = tmp << 1;
    key[6] = tmp & 0x74;
    key[7] = tmp + 0x50;
}

int RC4_code(char *ret,char *text,char *key,int text_len,int key_len)
{
    unsigned int n = 0;
    unsigned char i=0,j=0,t=0,k=0,tmp;

    RC4_init(key,key_len);

    while(n < text_len)
    {
        i = (i+1) % 256;
        j = (j + s[i]) % 256;
        tmp = s[j];
        s[j] = s[i];
        s[i] = tmp;
        t = ( s[i] + s[j] ) % 256;
        k = s[t];

        ret[n*2] = text[n] ^ k;
        n++;
    }
    return 0;
}

int RC4_init(char *key,int key_len)
{
    unsigned char tmp;
    unsigned int i,j;

    for(i=0;i<256;i++)
    {
        s[i] = i;
    }

    j = 0;
    for(i=0;i<256;i++)
    {
        j = (j+s[i]+key[i%key_len]) % 256;
        tmp = s[j];
        s[j] = s[i];
        s[i] = tmp;
    }

    return 0;
}

/*
#pragma vector = USCIAB0TX_VECTOR
__interrupt void TransmitInterrupt(void)
{
    P1OUT  ^= BIT0;//light up P1.0 Led on Tx
    //IFG2 &= ~UCA0TXIFG;
    //flagtx = 1;
}*/


#pragma vector = USCIAB0RX_VECTOR
__interrupt void ReceiveInterrupt(void)
{
    P1OUT  ^= BIT6;     // light up P1.6 LED on RX
    IFG2 &= ~UCA0RXIFG; // Clear RX flag
    flag = 1;
}

