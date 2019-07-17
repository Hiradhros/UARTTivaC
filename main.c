#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "tm4c_cmsis.h"
#include <intrinsics.h>


//define easy to read names for registers
#define SYSCTL_RCGCUART (*((volatile unsigned long *) 0x400FE618 ))     //RCGCUART Base 0x400FE000
#define SYSCTL_RCGCGPIO (*((volatile unsigned long *) 0x400FE608 ))     //RCGCCGPIO  0x400FE000  0x608
#define SYS_CLOCK_HZ 16000000U

// 2. Declarations Section
//   Global Variables
unsigned long SW1; // input from PF4
unsigned long SW2; // input from PF0
unsigned long Green; // output to PF3 (green LED)
int say = 0;
int clock = 0;
unsigned long  periyod_sayi = 0; // Variable that will be used as period number.
int LED_1_delay = 0;
int LED_2_delay = 0;
int LED_3_delay = 0;

//   Function Prototypes
void PortF_Init(void);
void FlashSOS(void);
void delay(unsigned long halfsecs);
char readChar(void);
void printChar(char c);
void printString(char * string);
void UART_Init(void);
void _pushbutton(void);
void GPIO_Init(void);
void LED_RED(void);
void LED_BLUE(void);
void LED_GREEN(void);
void LED_OFF(void);
void _harakete_gecirme_1(int p, int d);
void _harakete_gecirme_2(int p, int d);
void _harakete_gecirme_3(int p, int d);
int readInt(void);
void printInt(int i);
void _arayuzbaslangic(void);
void _periyod_sorgulama(void);
void _yanmasuresi_sorgulama_1(void);
void _yanmasuresi_sorgulama_2(void);
void _yanmasuresi_sorgulama_3(void);
void _anamenuyedonme(void);

// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){
  unsigned long SOS;
  PortF_Init();
  UART_Init();  
  _arayuzbaslangic();
  _periyod_sorgulama();
  _yanmasuresi_sorgulama_1();
  _yanmasuresi_sorgulama_2();
  _yanmasuresi_sorgulama_3();
  printString("TEBRIKLER KONFIGURASYONLARI BASARI ILE YUKLEDINIZ... \n\r");
  while(1){
    SW1 =   GPIOF->DATA  &0x10; // read PF4 into SW1
    
    if (SW1) { //SW1 is Not pressed: SW1 is 1 (Negative logic)
      SOS = 0;
    } else {
      SOS = 1;
    }
    while (SOS == 1) {
      FlashSOS();
      delay(20); // Delay for 5 secs in between flashes
      SW2 =   GPIOF->DATA  &0x01; // read PF0 into SW2
      if (!SW2) { //SW1 is Pressed: SW2 is 0 (Negative logic)
        SOS = 0;
        GPIOF->DATA   &= ~0x08; //turn LED off
      }
    }
    //_anamenuyedonme();
  }
}

void _yanmasuresi_sorgulama_1(void) {
  printString(" 1. LED icin girmek istediginiz yanma suresini girin: \n\r");
    printString(" Komut 1, 5, 8 mevcuttur: \n\r");
  char c = readChar();
  printChar(c);
  printString("\n\r");
  switch(c)
  {
  case '1':
    LED_1_delay = 1;
    printString("Yanma suresi  10 ms  olarak atandi... \n\r");
    printString("Komut Basarili! \n\r");
    break;
  case '5':
    LED_1_delay = 5;
    printString("Yanma suresi 500 ms olarak atandi... \n\r");
    printString("Komut Basarili! \n\r");
    break;
  case '8':
    LED_1_delay = 8;
    printString("Yanma suresi 800 ms  olarak atandi... \n\r");
    printString("Komut Basarili! \n\r");
    break;
  default:
    LED_OFF();
    printString("Yanlis bir komut girdiniz.... \n\r");
    break;
  }
}
void _yanmasuresi_sorgulama_2(void) {
  printString(" 2. LED icin girmek istediginiz yanma suresini girin: \n\r");
  printString(" Komut 1, 5, 8 mevcuttur: \n\r");
  char c = readChar();
  printChar(c);
  printString("\n\r");
  switch(c)
  {
  case '1':
    LED_2_delay = 1;
    printString("Yanma suresi  10 ms  olarak atandi... \n\r");
    printString("Komut Basarili! \n\r");
    break;
  case '5':
    LED_2_delay = 5;
    printString("Yanma suresi 500 ms olarak atandi... \n\r");
    printString("Komut Basarili! \n\r");
    break;
  case '8':
    LED_2_delay = 8;
    printString("Yanma suresi 800 ms  olarak atandi... \n\r");
    printString("Komut Basarili! \n\r");
    break;
  default:
    LED_OFF();
    printString("Yanlis bir komut girdiniz.... \n\r");
    break;
  }
}
void _yanmasuresi_sorgulama_3(void) {
  printString(" 3. LED icin girmek istediginiz yanma suresini girin: \n\r");
    printString(" Komut 1, 5, 8 mevcuttur: \n\r");
  char c = readChar();
  printChar(c);
  printString("\n\r");
  switch(c)
  {
  case '1':
    LED_3_delay = 1;
    printString("Yanma suresi  10 ms  olarak atandi... \n\r");
    printString("Komut Basarili! \n\r");
    break;
  case '5':
    LED_3_delay = 5;
    printString("Yanma suresi 500 ms olarak atandi... \n\r");
    printString("Komut Basarili! \n\r");
    break;
  case '8':
    LED_3_delay = 8;
    printString("Yanma suresi 800 ms  olarak atandi... \n\r");
    printString("Komut Basarili! \n\r");
    break;
  default:
    LED_OFF();
    printString("Yanlis bir komut girdiniz.... \n\r");
    break;
  }
}

// Subroutine to initialize port F pins for input and output
// PF4 is input SW1 and PF2 is output Blue LED
// Inputs: None
// Outputs: None
// Notes: ...
void PortF_Init(void){ volatile unsigned long delay;
SYSCTL_RCGCGPIO |= (1<<5);
GPIOF->LOCK = 0x4C4F434B;
GPIOF->CR = 0xff;
GPIOF->DIR = (1<<1)|(1<<2)|(1<<3)|(0<<4);  // make LED pins (PF1, PF2, and PF3) outputs & PF4 as input
GPIOF->PUR = 0x10;    
GPIOF->DATA &= ~((1<<1)|(1<<2)|(1<<3)); // turn off leds
GPIOF->DEN = (1<<1)|(1<<2)|(1<<3)|(1<<4); // enable digital function on LED pins
SW1 = GPIOF->DATA & 0x11;  //GPIO_PORTF_DATA_R & 0x10;  // read PF4 into SW1
}
// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E

// Subroutine to Flash a green LED SOS once
// PF3 is green LED: SOS 
//    S: Toggle light 3 times with 1/2 sec gap between ON....1/2sec....OFF
//    O: Toggle light 3 times with 2 sec gap between ON....2sec....OFF
//    S: Toggle light 3 times with 1/2 sec gap between ON....1/2sec....OFF
// Inputs: None
// Outputs: None
// Notes: ...
void FlashSOS(void){
  _harakete_gecirme_1(periyod_sayi, LED_1_delay);
  _harakete_gecirme_2(periyod_sayi, LED_2_delay);
   _harakete_gecirme_3(periyod_sayi, LED_3_delay);
}
void _harakete_gecirme_1(int p, int d) {
  //p -> period
  //d -> delay 
  // c -> character
  for (int yakala = 0; yakala < p ; yakala++) {
    GPIOF->DATA |= 0x02; // Green = GPIO_PORTF_DATA_R | 0x08;
    // GPIO_PORTF_DATA_R = Green
    delay(d);
    GPIOF->DATA &= ~0x02;
    delay(d);// Green = GPIO_PORTF_DATA_R | 0x08;
  }
}
void _harakete_gecirme_2(int p, int d) {
  //p -> period
  //d -> delay 
  // c -> character
  for (int yakala = 0; yakala < p ; yakala++) {
    GPIOF->DATA |= 0x04; // Green = GPIO_PORTF_DATA_R | 0x08;
    delay(d);
    GPIOF->DATA &= ~0x04;
    delay(d);
  }
}
void _harakete_gecirme_3(int p, int d) {
  //p -> period
  //d -> delay 
  // c -> character
  for (int yakala = 0; yakala < p ; yakala++) {
    GPIOF->DATA |= 0x08; // Green = GPIO_PORTF_DATA_R | 0x08;
    delay(d);
    GPIOF->DATA &= ~0x08;
    delay(d);
  }
}
void LED_RED(void) {
  GPIOF->DATA = 0x02;
}
void LED_BLUE(void) {
  GPIOF->DATA = 0x04;
}
void LED_GREEN(void) {
  GPIOF->DATA = 0x08;
}
void LED_OFF(void) {
  GPIOF->DATA &= ~((1<<1)|(1<<2)|(1<<3)); // LED OFF
}
void _pushbutton(void) {
  if (SW1 == 0x00){   // zero means SW1 is pressed
    say++;
    SysTick->LOAD = SYS_CLOCK_HZ / 2U - 1U;
    SysTick->VAL = 0;
    SysTick->CTRL = (1U << 2) | (1U <<1) | 1U;
    __enable_interrupt();
  }
}
void _arayuzbaslangic(void) {
  for(int uzunluk = 0; uzunluk < 50; uzunluk++) {
    printString("*");
  }
  printString("\n\r");
  printString("Cihazin Arayuzune hos geldiniz \n\r");
  for(int uzunluk = 0; uzunluk < 50; uzunluk++) {
    printString("*");
  }  
  printString("\n\r");
}

void _periyod_sorgulama (void) {
  PortF_Init();
  printString("Girmek istediginiz Periyodu girin: \n\r");
  char c = readChar();
  printChar(c);
  printString("\n\r");
  
  switch(c)
  {
  case '1':
    periyod_sayi = 1;
    printString("Periyod sayiniz 1 olarak atandi... \n\r");
    printString("Komut Basarili! \n\r");
    break;
  case '2':
    periyod_sayi = 2;
    printString("Periyod sayiniz 2 olarak atandi... \n\r");
    printString("Komut Basarili! \n\r");
    break;
  case '3':
    periyod_sayi = 3;
    printString("Periyod sayiniz 3 olarak atandi... \n\r");
    printString("Komut Basarili! \n\r");
    break;
  case '5':
    periyod_sayi = 5;  
    printString("Periyod sayiniz 5 olarak atandi... \n\r");
    printString("Komut Basarili! \n\r");
    break;
  default:
    LED_OFF();
    break;
  }
}
void _anamenuyedonme(void) {
  printString("Komut listersi bitti ...  \n\r");
  printString("Ana menuye donmek ister misiniz ?[e/h] \n\r");
  char c = readChar();
  printChar(c);
  printString("\n\r");
  switch(c)
  {
  case 'e':
    for(int Baslangic = 0 ; Baslangic < 60; Baslangic++) {
      printString("\n\r");
    }
    _periyod_sorgulama();
    break;
  case 'h':
    LED_OFF();
    printString("Pekala iyi gunler !");
    printString("\n\r");
    break;
  default:
    LED_OFF();
    break;
  }
}

void UART_Init(void) {
  // 1. Enable the UART module using the RCGCUART register (see page 344).
  SYSCTL_RCGCUART |= (1<<0); 
  
  // 2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register (see page 340).
  // To find out which GPIO port to enable, refer to Table 23-5 on page 1351.
  SYSCTL_RCGCGPIO |= (1<<0); 
  
  // 3. Set the GPIO AFSEL bits for the appropriate pins (see page 671). To determine which GPIOs to
  // configure, see Table 23-4 on page 1344
  GPIOA->AFSEL = (1<<1)|(1<<0); 
  
  // 4. Configure the GPIO current level and/or slew rate as specified for the mode selected (see
  // page 673 and page 681
  
  // 5. Configure the PMCn fields in the GPIOPCTL register to assign the UART signals to the appropriate
  // pins (see page 688 and Table 23-5 on page 1351).
  GPIOA->PCTL = (1<<0)|(1<<4);  
  
  GPIOA->DEN = (1<<0)|(1<<1); 
  
  // Find  the Baud-Rate Divisor
  // BRD = 16,000,000 / (16 * 9600) = 104.16666666666666666666666666666666666666666666666666
  // UARTFBRD[DIVFRAC] = integer(0.166667 * 64 + 0.5) = 11
  
  
  // With the BRD values in hand, the UART configuration is written to the module in the following order
  
  // 1. Disable the UART by clearing the UARTEN bit in the UARTCTL register
  UART0->CTL &= ~(1<<0);
  
  // 2. Write the integer portion of the BRD to the UARTIBRD register
  UART0->IBRD = 104;      
  // 3. Write the fractional portion of the BRD to the UARTFBRD register.
  UART0->FBRD = 11; 
  
  // 4. Write the desired serial parameters to the UARTLCRH register (in this case, a value of 0x0000.0060)
  UART0->LCRH = (0x3<<5)|(1<<4);     // 8-bit, no parity, 1-stop bit
  
  // 5. Configure the UART clock source by writing to the UARTCC register
  //UART0_CC = 0x0;          
  
  // 6. Optionally, configure the ?DMA channel (see ?Micro Direct Memory Access (?DMA)? on page 585)
  // and enable the DMA option(s) in the UARTDMACTL register
  
  // 7. Enable the UART by setting the UARTEN bit in the UARTCTL register.
  UART0->CTL = (1<<0)|(1<<8)|(1<<9); 
  
}

// Subroutine to delay in units of half seconds
// We will make a precise estimate later: 
//   For now we assume it takes 1/2 sec to count down
//   from 2,000,000 down to zero
// Inputs: Number of half seconds to delay
// Outputs: None
// Notes: ...
void delay(unsigned long halfsecs){
  unsigned long count;
  
  while(halfsecs > 0 ) { // repeat while there are still halfsecs to delay
    count=200000; // Estimate that it takes 1/2 sec to count down to zero
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    halfsecs--;
  }
}
char readChar(void)  
{
  char c;
  while((UART0->FR & (1<<4)) != 0);
  c = UART0->DR;                 
  return c;                    
}

void printChar(char c)  
{
  while((UART0->FR & (1<<5)) != 0);
  UART0->DR = c;          
}

void printString(char * string)
{
  while(*string)
  {
    printChar(*(string++));
  }
}