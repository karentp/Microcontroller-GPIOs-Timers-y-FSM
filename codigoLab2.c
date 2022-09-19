//INCLUSION DE BIBLIOTECAS
#include <avr/io.h>
#include <avr/interrupt.h>

//DECLARACION DE estadoOS
// FSM

#define pasocarro 0
#define parpCarro 1
#define altoCarro 2
#define pasoPeaton 3
#define parpPeaton 4
#define altoPeaton 5
#define zero 0


//uso de variables importantes
int tiempo = 0;
int estado = 0;
int botonPulsado = 0; 
int contadores = 0; 
int d = 2;
int seg = 30;
//Protocolo interrupciones ISR

ISR( INT0_vect ){
  botonPulsado = 1; 
}


ISR ( TIMER0_OVF_vect ){
    
    switch (estado)
    { 
        case parpCarro:

            if( contadores == seg || contadores == d*seg )
            { 
                PORTB ^= ( 1<< PB0 ); 
            }
        break;
      
        case parpPeaton :

            if(contadores == 30 || contadores == 60)
            { 
                PORTB ^= (1<<PB3);
            }
        break;
      
        
        default: 
        break;
    }  

    
    if(contadores  == d*seg+3)
    {
        tiempo++; 
        contadores = 0;
    }
    
    else
    {
        contadores++; 
    }
}


int main(void)
{
  DDRB |= ( 1 << PB0 )|( 1 << PB1 )|( 1 << PB2 ) |( 1 << PB3 );
  
  PORTB = ( 0<<PB0 )|( 1<<PB1 )|( 0<<PB2 )|( 1<<PB3 ); 
  
  estado = pasocarro; 
  
  botonPulsado = 0;
  
  tiempo = 0;

  sei(); //interrupcion global

  GIMSK |= ( 1 << INT0 ); 
  MCUCR |= ( 1 << ISC00 ); 
  MCUCR |= ( 1 << ISC01 ); 

  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0B |= (1 << CS00) | (1 << CS02); 
  
  sei (); //interrupcion global
  
  TCNT0 = 0; 
  TIMSK |= ( 1 << TOIE0 ); 
  
  while (1) 
  {
    
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
        if( tiempo >= 3 )
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
        PORTB = ( 0<<PB0 )|(1 << PB1 )|( 1 << PB2)|( 0 << PB3 ); 
        if ( tiempo >= 1 )
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
        PORTB = ( 1 << PB0 )|( 0 << PB1 )|( 1 << PB2 )|( 0 << PB3 ); 
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
        PORTB = ( 0 << PB0 )|( 1 << PB1 )|( 1 << PB2 )|( 0 << PB3 ); 
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



