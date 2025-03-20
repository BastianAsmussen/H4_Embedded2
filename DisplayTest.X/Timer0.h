/*
 * Timer0.h
 *
 * Created: 16-01-2018 05:41:56
 *  Author: ltpe
 */ 


#ifndef TIMER0_H_
#define TIMER0_H_

extern void Setup_Timer0_Overflow_Interrupt(uint16_t ValueToVariable, 
                                            Function_Pointer_With_One_Uint32_t_Parameter Function_Pointer);
extern void Enable_Timer0_Overflow_Interrupt();
extern void Disable_Timer0_Overflow_Interrupt();



#endif /* TIMER0_H_ */