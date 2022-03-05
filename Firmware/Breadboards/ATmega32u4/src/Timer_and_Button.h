/*
 * Timer.h
 *
 * Created: 02/06/2018 18:42:55
 *  Author: Chris
 */ 


#ifndef TIMER_H_
#define TIMER_H_

void	Timer_And_Button_Init(void);

uint8_t Timer_WS2801_OV_Occurred(void);
void	Timer_WS2801_Restart(void);
uint8_t	Timer_WS2801_On_LED_Blick(void);

void	Timer_Button_Start(void);
void	Timer_Button_Stop(void);

uint8_t Button_Short_Pressed(void);
uint8_t	Button_Short_State(void);
uint8_t Button_Long_Pressed(void);

void	Timer_No_Data_Light_Reset(void);
uint8_t	Timer_No_Data_Light_On(void);



#endif /* TIMER_H_ */