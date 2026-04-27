#include "led_effect.h"

#define LUT_SIZE 120

/*
 * Công thức: (sin(i * 2 * PI / 120) + 1.0) * (MAX_PWM / 2)
 */
static const uint16_t pwm_lut[LUT_SIZE] = {
    500,526,552,578,603,629,654,679,703,726,750,772,793,814,834,853,871,888,904,919,
    933,945,956,966,975,982,989,993,997,999,1000,999,997,993,989,982,975,966,956,945,
    933,919,904,888,871,853,834,814,793,772,750,726,703,679,654,629,603,578,552,526,
    500,473,447,421,396,370,345,320,296,273,250,227,206,185,165,146,128,111,95,80,
    66,54,43,33,24,17,10,6,2,0,0,0,2,6,10,17,24,33,43,54,
    66,80,95,111,128,146,165,185,206,227,250,273,296,320,345,370,396,421,447,473
};

/* Phase offsets cho 3 LED (đều nhau 120 độ) */
#define PHASE_OFFSET_A 0
#define PHASE_OFFSET_B 40
#define PHASE_OFFSET_C 80

static const uint8_t speed_to_ticks[10] = {6,5,5,4,4,3,3,2,2,1};

// Các biến trạng thái phải được khai báo 'volatile' vì có sử dụng trong Interrupt
static volatile uint16_t current_index = 0;
static volatile bool is_effect_running = true;
static volatile uint8_t effect_speed = 1;
static volatile uint16_t tick_counter = 0;

void LED_Effect_Init(void) {
    current_index = 0;
    is_effect_running = true;
    effect_speed = 1;
    tick_counter = 0;
}

void LED_Effect_UpdateISR(void) {
    if (!is_effect_running) {
        // Nếu tắt hiệu ứng, đưa PWM về 0
        Output_PWM_Ch1(0);
        Output_PWM_Ch2(0);
        Output_PWM_Ch3(0);
        return;
    }

    tick_counter++;

    uint8_t wait_ticks = speed_to_ticks[effect_speed - 1];

    if (tick_counter < wait_ticks) {
        return;
    }

    tick_counter = 0;

    uint16_t idxA = current_index;

    uint16_t idxB = (current_index + PHASE_OFFSET_B);
    if (idxB >= LUT_SIZE) idxB -= LUT_SIZE;

    uint16_t idxC = (current_index + PHASE_OFFSET_C);
    if (idxC >= LUT_SIZE) idxC -= LUT_SIZE;
    // Cập nhật giá trị PWM
    Output_PWM_Ch1(pwm_lut[idxA]);
    Output_PWM_Ch2(pwm_lut[idxB]);
    Output_PWM_Ch3(pwm_lut[idxC]);

    current_index++;
    if (current_index >= LUT_SIZE) {
        current_index = 0;
    }
}

void LED_Effect_SetSpeed(uint8_t speed_multiplier) {
    // Ràng buộc tốc độ từ 1 đến 10
    if (speed_multiplier > 0 && speed_multiplier <= 10) {
        effect_speed = speed_multiplier;
    }
}

void LED_Effect_Toggle(void) {
    is_effect_running = !is_effect_running;
}
