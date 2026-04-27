#ifndef LED_EFFECT_H
#define LED_EFFECT_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief  Khởi tạo các biến trạng thái ban đầu
 */
void LED_Effect_Init(void);

/**
 * @brief  Hàm được gọi TRONG ngắt Timer Update (Update Interrupt).
 */
void LED_Effect_UpdateISR(void);

/**
 * @brief  Thay đổi tốc độ chạy của hiệu ứng
 * @param  speed_multiplier Tốc độ (1-10)
 */
void LED_Effect_SetSpeed(uint8_t speed_multiplier);

/**
 * @brief  Bật/tắt hiệu ứng từ nút nhấn
 */
void LED_Effect_Toggle(void);

extern void Output_PWM_Ch1(uint16_t duty);
extern void Output_PWM_Ch2(uint16_t duty);
extern void Output_PWM_Ch3(uint16_t duty);

#endif
