/* 
 * @file TAD_leds.h
 * @author alberto.marquillas & alba.falcon
 * @date 27 de febrer de 2025
 * @brief Definicio? de les funcions i macros per al control dels LEDs.
 */

#ifndef TAD_LEDS_H
#define TAD_LEDS_H

 void initLed();
 void led_valorTecla(unsigned char tecla, unsigned char valor);
void motorLed(unsigned char led);
#endif /* TAD_LEDS_H */