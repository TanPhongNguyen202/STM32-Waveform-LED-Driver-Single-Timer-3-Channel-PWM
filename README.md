# STM32-Waveform-LED-Driver-Single-Timer-3-Channel-PWM
Điều khiển LED dạng sóng dùng 1 Timer duy nhất
# 3-Channel LED Waveform Control on STM32

Solution for the "3-Channel LED Waveform Control" firmware test.  
Continuously fades 3 LEDs with a smooth sinusoidal brightness pattern, precisely 120° out of phase, using **a single hardware timer**.

## Problem Summary
- Drive 3 LEDs (A, B, C) with a flowing, jitter‑free brightness wave.
- Phase offset must remain exactly equal (120°) at all times.
- The system must run stably, with no flicker, glitches, or phase drift.

## Mandatory Technical Constraints
- **One timer only**, generating 3 PWM channels **and** an Update interrupt.
- PWM frequency ≥ 1 kHz, Center‑Aligned preferred.
- Lookup Table with ≥ 100 elements, pre‑computed (no `sin()` in runtime).
- PWM duty updates **exclusively inside the Update ISR**, every 1–5 ms.
- **No `float` / `double` in ISR, no `HAL_Delay` or blocking code.**
- ISR code must be short, optimal, and safe (no array out‑of‑bounds).

## Solution Overview
| Feature | Implementation |
|--------|----------------|
| **Timer** | TIM1, Center‑Aligned, 1.2 kHz PWM, ARR = 1000 |
| **Lookup Table** | 120‑element `pwm_lut[]` (scaled sine, 0–1000) |
| **Phase Offset** | A: index + 0, B: index + 40, C: index + 80 (120° each) |
| **PWM Update** | Inside `HAL_TIM_PeriodElapsedCallback` for TIM1 |
| **Speed Control** | 10 levels (1 = slowest, 10 = fastest) via tick‑skip counter |
| **User Interaction** | Button (PC13) toggles effect ON/OFF |

> The Lookup Table, offsets, and speed values are all static constants.  
> No dynamic memory, no float math, no blocking delays anywhere.

## File Structure
├── main.c # Hardware init, PWM outputs, ISR callback
├── led_effect.h # Public API (init, update, set speed, toggle)
└── led_effect.c # Lookup Table, phase logic, speed management

text
All effect logic is isolated in `led_effect.*`. The hardware abstraction (`Output_PWM_Chx()`) is implemented in `main.c`.

## Quick Start
1. **Hardware**: STM32F103C8T6 (Blue Pill)  
   - LED A → PA8 (TIM1_CH1)  
   - LED B → PA9 (TIM1_CH2)  
   - LED C → PA10 (TIM1_CH3)  
   - Button → PC13 (pull‑up, active low)
2. **IDE**: STM32CubeIDE. Import the project and build.
3. **Flash** the binary.
4. The effect starts automatically. Press the button to toggle ON/OFF.
5. To change speed, call `LED_Effect_SetSpeed(1‑10)` from your code (e.g., on another button press).

## Why This Works (Key Optimisations)
- Update ISR is **only a few integer operations** – no heavy math, no branches that could cause jitter.
- Tick‑skip mechanism (via `speed_to_ticks` array) changes update cadence without disturbing the smooth waveform.
- All variables shared with ISR are marked `volatile`.
- The Lookup Table is crafted for exactly 1000 ARR, so no runtime scaling is needed.

## Demo
[[Video link](https://youtu.be/...)](https://youtu.be/p81u3_nZmBY?si=iFBh3d6COld8og9N) – showing smooth LED fading, speed change, and toggle.

## Test Compliance
✔ PWM ≥ 1 kHz Center‑Aligned  
✔ LUT size 120 (>100)  
✔ Three‑phase offset (120°)  
✔ Update rate 1–5 ms (adjustable)  
✔ No `float`, no `sin()`, no `HAL_Delay`  
✔ Modular code, clear separation of init and runtime  
✔ Button toggle (advanced) – working  
✔ Speed control 1–10 (advanced) – working

*(DMA circular update is not implemented – it is an optional advanced feature.)*
