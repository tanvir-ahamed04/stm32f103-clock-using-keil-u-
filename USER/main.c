#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "lcd.h"
#include "text.h"
#include "stdio.h"

// Declare time and date variables
int year = 2024, month = 12, day = 20;
char hour = 22, min = 29, sec = 30;
char alarm_hour = 22, alarm_min = 30;
int alarm_active = 0; // 0: inactive, 1: active
int display_mode = 0; // 0: normal, 1: alarm setting
int adjust_mode = 0; // 0: adjust hour, 1: adjust minute
char str[20];
volatile uint32_t millis = 0; // Global milliseconds counter
volatile int led_state = 0;   // Tracks LED on/off state during blinking

// Timer Configuration
void Timer2_Config() {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1;
    TIM_TimeBaseStructure.TIM_Period = 2000 - 1; // 1Hz interrupt
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

void Timer3_Config() {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // 1ms interrupt
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

// NVIC Configuration
void NVIC_Config() {
    NVIC_InitTypeDef NVIC_InitStructure;
    // TIM2 Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // TIM3 Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
}

// LED Configuration
void LED_Init() {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Blink_LED() {
    if (led_state) {
        GPIO_SetBits(GPIOA, GPIO_Pin_8);
    } else {
        GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    }
    led_state = !led_state; // Toggle LED state
}

// Button Configuration
void Button_Config() {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // Configure Key 0 (Switch Mode)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure Key 1 (Increase/Toggle)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// Button Press Check
int isSwitchButtonPressed()
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13) == Bit_RESET;
}

int isIncreaseButtonPressed()
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) == Bit_RESET;
}

// Delay Function
void Delay(uint32_t ms)
{
    volatile uint32_t i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 10000; j++);
}

// LCD Display Updates
void updateRealClockDisplay() {
    sprintf(str, "Time: %02d:%02d:%02d", hour, min, sec);
    LCD_ShowString(30, 90, str, POINT_COLOR);
}

void updateAlarmClockDisplay() {
    sprintf(str, "Alarm: %02d:%02d", alarm_hour, alarm_min);
    LCD_ShowString(30, 70, str, POINT_COLOR);
}

void updateNormalDisplay() {
    LCD_Clear(WHITE);
    LCD_ShowString(30, 110, "NAME: Tanvir Ahamed", POINT_COLOR);
    LCD_ShowString(30, 130, "ID: https://github.com/tanvir-ahamed04", POINT_COLOR);
    LCD_ShowString(30, 150, "DATE: 2024/12/20", POINT_COLOR);
    LCD_ShowString(30, 170, "FRIDAY", POINT_COLOR);
}

// Toggle Adjustment Mode
void toggleAdjustmentMode() {
    adjust_mode = !adjust_mode;
    Delay(200); // Debounce delay
}

// Real Time Setting
void setRealTimeClock() {
    LCD_Clear(WHITE);
    LCD_ShowString(30, 70, "Set Real Time:", POINT_COLOR);

    while (!isSwitchButtonPressed()) { // Exit adjustment on switch button press
        if (isIncreaseButtonPressed()) 
        {
            if (adjust_mode == 0) {
                hour = (hour + 1) % 24; // Increment hour (wrap around after 23)
            } else {
                min = (min + 1) % 60; // Increment minute (wrap around after 59)
            }
            Delay(200);
        }

        updateRealClockDisplay(); // Update LCD with the new real-time values
    }
    toggleAdjustmentMode(); // Toggle between hour and minute adjustment
}

// Alarm Time Setting
void setAlarmTime() {
    LCD_Clear(WHITE);
    LCD_ShowString(30, 50, "Set Alarm Time:", POINT_COLOR);

    while (!isSwitchButtonPressed()) { // Exit adjustment on switch button press
        if (isIncreaseButtonPressed()) {
            if (adjust_mode == 0) {
                alarm_hour = (alarm_hour + 1) % 24; // Increment alarm hour
            } else {
                alarm_min = (alarm_min + 1) % 60; // Increment alarm minute
            }
            Delay(200);
        }

        updateAlarmClockDisplay(); // Update LCD with the new alarm time values
    }
    toggleAdjustmentMode(); // Toggle between hour and minute adjustment
}

// Real-Time Clock Update
void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        if (++sec == 60) {
            sec = 0;
            if (++min == 60) {
                min = 0;
                if (++hour == 24)
                    hour = 0;
            }
        }

        // Activate alarm when time matches
        if (hour == alarm_hour && min == alarm_min) {
            alarm_active = 1;
        }
    }
}

// Millisecond Timer Update
void TIM3_IRQHandler(void) {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

        millis++;
        if (millis >= 500) { // Blink every 500ms
            millis = 0;
            if (alarm_active) {
                Blink_LED(); // Blink LED while alarm is active
            }
        }
    }
}

// Main Function
int main(void)
{
    NVIC_Config();
    Timer2_Config();
    Timer3_Config();
    Button_Config();
    LED_Init();
    LCD_Init();
    POINT_COLOR = RED;
    LCD_Clear(WHITE);

    updateNormalDisplay();
    while (1)
    {
        if (isSwitchButtonPressed())
        {
            display_mode = !display_mode;
            while (isSwitchButtonPressed())
                Delay(80);

            if (display_mode)
                updateAlarmClockDisplay();
            else
                updateNormalDisplay();
        }

        if (display_mode && isIncreaseButtonPressed())
            setAlarmTime();
        else if (!display_mode && isIncreaseButtonPressed())
            setRealTimeClock();
        else {
            updateRealClockDisplay();
            if (alarm_active) {
                Blink_LED();
                LCD_ShowString(30, 200, "ALARM ACTIVE!", POINT_COLOR);
                Delay(100); // Implement a delay to control the blinking speed
            }
        }
    }
}
