#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#define BAUD 9600
#define PRESCALLER (((F_CPU / (BAUD*16UL))) -1)

//Declaração das funções
void USART_init(void);
unsigned char USART_receive(void);
void USART_send(unsigned char data);
void USART_putstring(char* StringPtr);
void adc_init(void);
uint16_t read_adc(uint8_t channel);

//char String[] = "Teste \n";
char String1[] = "\nVariável de Controle: ";
char String2[] = "Tensão CA: ";
char String3[] = "\tCorrente: ";
char BN[] = "\n";
String string4;
char m2[4], m3[6];
int m;
double n, current, control, amps, VRMS, AmpsRMS;

void setup() {
  USART_init();
  adc_init();
}

void loop() {
  //USART_putstring(String);
  //_delay_ms(5000);
  //Recebendo o valor do pino A0 para o cálculo da tensão
  m = read_adc(0);
  n = (round((m*100)/100) * .247);

  //Recebendo o valor do pino A1 para o cálculo da corrente
  current = getCurrent();
  VRMS = (current/2.0)*.707;
  AmpsRMS = (VRMS *1000)/295;

  //USART_putstring(String2);
  string4 = String(n, 2);
  string4.toCharArray(m2, 7);
  USART_putstring(m2);
  USART_putstring(" , ");
  //Imprimindo os valores: Corrente
  //USART_putstring(String3);
  string4 = String(AmpsRMS, 3);
  string4.toCharArray(m3, 7);
  USART_putstring(m3);
  

  USART_putstring("\n");
  _delay_ms(500);
}

void USART_init(void){
  UBRR0H = (uint8_t)(PRESCALLER>>8);
  UBRR0L = (uint8_t)(PRESCALLER);
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  UCSR0C = (3<<UCSZ00);
}


unsigned char USART_receive(void){
  while(!(UCSR0A & (1<<RXC0)));
  return UDR0;
}

void USART_send(unsigned char data){
  while(!(UCSR0A & (1<<UDRE0)));
  UDR0 = data;
}


void USART_putstring(char* StringPtr){
  while(*StringPtr != 0x00){
    USART_send(*StringPtr);
    StringPtr++;
  }
}

void adc_init(void){
  //Para clock de 16MHz, 125kHz de clock de referência para o ADC
  ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
  ADMUX |= (1<<REFS0);
  ADCSRA |= (1<<ADEN);
  ADCSRA |= (1<<ADSC);
}


uint16_t read_adc(uint8_t channel){
  ADMUX &= 0xE0;
  ADMUX |= channel&0x07;
  ADCSRB = channel&(1<<3);
  ADCSRA |= (1<<ADSC);
  while(ADCSRA & (1<<ADSC));
  return ADCW;
}


float getCurrent(){
  float result;
  int readValue, maxValue = 0, minValue = 1024;
  uint32_t startTime = millis();

  while((millis()-startTime)<500){
    readValue = read_adc(1);
    if(readValue > maxValue){
      maxValue = readValue;
    } else{
      minValue = readValue;
    }
  }

  result = ((maxValue - minValue)*5.0)/1024.0;

  return result;
}
