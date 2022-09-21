#include <avr/io.h>
#include <avr/interrupt.h>

//estados
#define pasocarro 0 //auto detenido luz roja
#define parpCarro 1 //auto detenido luz parpadea
#define altoCarro 2 //auto en movimiento luz verde
#define pasoPeaton 3 //peaton en movimiento luz verde
#define parpPeaton 4 //peaton detenido luz parpadea
#define altoPeaton 5 //peaton detenido luz roja

//variables globales

int tiempo = 0;     //segundos 
int botonPulsado = 0;  //toque
int contadores = 0;     //contador para ciclo
int d = 2;  
int seg = 30;
int estado = 0;

int main(void)
{
  //PB3 o PB2 o PB1 o PB0
  DDRB |= (1 << PB3)|(1 << PB2)|(1 << PB1) |(1 << PB0);

  PORTB = (0 << PB3)|(1 << PB2)|(0 << PB1)|(1 << PB0); 
  
  estado = pasocarro; 
  
  //inicializa

  botonPulsado = 0;
  
  tiempo = 0;

  //interrumpe todo 
  sei(); 
  
  GIMSK |= (1 << INT0); 

  MCUCR |= (1 << ISC00); 
  
  MCUCR |= (1 << ISC01); 

  //interrupcion global para el Atiiny85 con timer

  TCCR0A = 0x00; 
  TCCR0B = 0x00;

  TCCR0B |= (1 << CS00) | (1 << CS02); 
  
  sei (); 

  TCNT0 = 0; 
  
  TIMSK |= (1 << TOIE0); 

  //loop infinito

  while (1) 
  {
    //inicio de estados
    switch (estado){
    case (pasocarro):
        
        PORTB = (0<<PB3)|(1<<PB2)|(0<<PB1)|(1<<PB0); 
        
        if(botonPulsado == 1)
        {
            if(tiempo >= 10)
            {
                contadores = 0; 
                tiempo = 0;
                estado = parpCarro;
            }
        }
        
        else
        
        {
            estado = pasocarro;
        }
        
        break;
    
    case (parpCarro):
        
        if(tiempo >= 3)
        {
            contadores = 0; 
            tiempo = 0;
            estado = altoCarro;
        }
        
        else
        {
            estado = parpCarro;
        }
        
        break;
    
    case (altoCarro):
        
        PORTB = (0<<PB3)|(1<<PB2)|(1<<PB1)|(0<<PB0); 
        
        if (tiempo >= 1)
        {
            contadores = 0; 
            tiempo = 0;
            estado = pasoPeaton;
        }
        
        else
        {
            estado = altoCarro;
        }

        break;
    
    case (pasoPeaton):
        PORTB = (1<<PB3)|(0<<PB2)|(1<<PB1)|(0<<PB0); 
        if(tiempo >= 10)
        {
            contadores = 0; 
            tiempo = 0;
            estado = parpPeaton;
        }
        
        else
        {
            estado = pasoPeaton;
        }

        break;

    case (parpPeaton):
        if(tiempo >= 3)
        {
            contadores = 0; 
            tiempo = 0;
            estado = altoPeaton;
        }

        else
        {
            estado = parpPeaton;
        }
        
        break;

    case (altoPeaton):
        PORTB = (0<<PB3)|(1<<PB2)|(1<<PB1)|(0<<PB0); 
        
        if(tiempo >= 1)
        {
            contadores = 0; 
            tiempo = 0;
            botonPulsado = 0; 
            estado = pasocarro;
        }

        else
        {
            estado = altoPeaton;
        }

        break;

    default:
        estado = pasocarro; 
        
        break;
    }
  }
}


    ISR(INT0_vect){
    botonPulsado = 1; 
    }

    ISR (TIMER0_OVF_vect)
    {
        int aux =d*30;
        switch (estado){ 


            case parpCarro:
                if(contadores == seg || contadores == aux){ 
                PORTB ^= (1<<PB0); 
                }
                break;
            
            case parpPeaton:
                if(contadores == seg || contadores == aux){ 
                PORTB ^= (1<<PB3); 
                }
                break;
            
            default:
                break;
            } 

        if(contadores == aux+3)
        {
            tiempo++; 
            contadores = 0;
        }
        else
        {
            contadores++; 
        } 
}





