#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

extern u8 Fsh500mS;
extern u32 T10mS;
extern u16 PWMDuty;
extern u16 locktime;
extern u16 skidstime;

void TIM1_Int_Init(u16 arr,u16 psc);
void TIM2_Int_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM5_Cap_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);
#endif

