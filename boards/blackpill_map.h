/*
  blackpill_map.h - driver code for STM32F4xx (Blackpill) breakout board

  Part of grblHAL

  Copyright (c) 2021 rvalotta

  grblHAL is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  grblHAL is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with grblHAL. If not, see <http://www.gnu.org/licenses/>.
*/

/* Pin Assignments:
 *
 *                               -----------
 *                           VB |           | +3V
 *                          C13 |           | GND
 *                          C14 |           | +5V
 *                          C15 | *     - * | B9   Safety door / Aux In 0
 *                          RST |      |K|  | B8   Cycle Start
 *         Y_DIRECTION_PIN   A0 |       -   | B7   Feed Hold
 *              Y_STEP_PIN   A1 |           | B6   Reset/EStop
 *              Z_STEP_PIN   A2 |           | B5   Вентилятор охлаждения (Fan) / AUXOUT4
 *         Z_DIRECTION_PIN   A3 |    / \    | B4   Доп. нагрузка / AUXOUT5
 *   Вакуумный клапан №1     A4 |   <MCU>   | B3   (свободен)
 *   Вакуумный клапан №2     A5 |    \ /    | A15  (свободен)
 *         X_DIRECTION_PIN   A6 |           | A12  USB D+
 *              X_STEP_PIN   A7 |   -   -   | A11  USB D-
 *      STEPPERS_ENABLE_PIN  B0 |  |R| |B|  | A10  Клапан сдува / AUXOUT3
 *     (Spindle ENA — не исп.) B1 |   -   -   | A9   Вакуумная помпа / AUXOUT2
 *     (Spindle DIR — не исп.) B2 |           | A8   Подсветка LED ШИМ (Spindle PWM)
 *                          B10 |           | B15  Probe
 *                          +3V |   -----   | B14  Z Limit
 *                          GND |  |     |  | B13  Y Limit
 *                          +5V |  | USB |  | B12  X Limit
 *                               -----------
 */

#if N_ABC_MOTORS > 1
#error "Axis configuration is not supported!"
#endif

#define BOARD_NAME "BlackPill"

// Define step pulse output pins.
#define STEP_PORT               GPIOA
#define X_STEP_PIN              7
#define Y_STEP_PIN              1
#define Z_STEP_PIN              2
#define STEP_OUTMODE            GPIO_MAP

#define DIRECTION_PORT          GPIOA
#define X_DIRECTION_PIN         6
#define Y_DIRECTION_PIN         0
#define Z_DIRECTION_PIN         3
#define DIRECTION_OUTMODE       GPIO_MAP

// Define stepper driver enable/disable output pin.
#define STEPPERS_ENABLE_PORT    GPIOB
#define STEPPERS_ENABLE_PIN     0
#define STEPPERS_ENABLE_MASK    STEPPERS_ENABLE_BIT

// Define homing/hard limit switch input pins.
#define LIMIT_PORT              GPIOB
#define X_LIMIT_PIN             12
#define Y_LIMIT_PIN             13
#define Z_LIMIT_PIN             14
#define LIMIT_INMODE            GPIO_SHIFT12

// Define ganged axis or A axis step pulse and step direction output pins.
// Отключено, чтобы освободить PA6, PA7 и PB15.
/*
#if N_ABC_MOTORS == 1
#define M3_AVAILABLE
#define M3_STEP_PORT            STEP_PORT
#define M3_STEP_PIN             6
#define M3_DIRECTION_PORT       DIRECTION_PORT
#define M3_DIRECTION_PIN        7
#if N_AUTO_SQUARED
#define M3_LIMIT_PORT           LIMIT_PORT
#define M3_LIMIT_PIN            15
#endif
#endif
*/

// Define driver spindle pins.
// PB1/PB2 формально заняты под ENA/DIR, но физически не разводятся.
// ENA/DIR оставлены, чтобы не менять my_machine.h и не бороться с DRIVER_SPINDLE_ENABLE.
#if DRIVER_SPINDLE_ENABLE & SPINDLE_ENA
#define SPINDLE_ENABLE_PORT     GPIOB
#define SPINDLE_ENABLE_PIN      1
#endif
#if DRIVER_SPINDLE_ENABLE & SPINDLE_PWM
#define SPINDLE_PWM_PORT        GPIOA   // PA8 = TIM1_CH1, аппаратный ШИМ
#define SPINDLE_PWM_PIN         8
#endif
#if DRIVER_SPINDLE_ENABLE & SPINDLE_DIR
#define SPINDLE_DIRECTION_PORT  GPIOB
#define SPINDLE_DIRECTION_PIN   2
#endif

// Auxiliary outputs — все реальные устройства платы.
// Определены последовательно 0..5, поэтому M62 Pn совпадает с номером AUXOUTPUTn.
#define AUXOUTPUT0_PORT         GPIOA   // Вакуумный клапан №1
#define AUXOUTPUT0_PIN          4

#define AUXOUTPUT1_PORT         GPIOA   // Вакуумный клапан №2
#define AUXOUTPUT1_PIN          5

#define AUXOUTPUT2_PORT         GPIOA   // Вакуумная помпа
#define AUXOUTPUT2_PIN          9

#define AUXOUTPUT3_PORT         GPIOA   // Клапан сдува
#define AUXOUTPUT3_PIN          10

#define AUXOUTPUT4_PORT         GPIOB   // Вентилятор охлаждения (Fan)
#define AUXOUTPUT4_PIN          5

#define AUXOUTPUT5_PORT         GPIOB   // Доп. нагрузка
#define AUXOUTPUT5_PIN          4

// Coolant — не назначаем пины, чтобы ничего не занимать.
// Управление клапанами делаем через M62/M63.

// Spindle encoder pins.
#if SPINDLE_ENCODER_ENABLE

#define RPM_COUNTER_N           2
#define RPM_TIMER_N             3
#define SPINDLE_INDEX_PORT      GPIOB
#define SPINDLE_INDEX_PIN       3
#define SPINDLE_PULSE_PORT      GPIOA
#define SPINDLE_PULSE_PIN       15

#endif

#define AUXINPUT0_PORT          GPIOB // Safety door
#define AUXINPUT0_PIN           9
#if !N_AUTO_SQUARED
#define AUXINPUT1_PORT          GPIOB // Probe
#define AUXINPUT1_PIN           15
#endif
#define AUXINPUT2_PORT          GPIOB // Reset/EStop
#define AUXINPUT2_PIN           6
#define AUXINPUT3_PORT          GPIOB // Feed hold
#define AUXINPUT3_PIN           7
#define AUXINPUT4_PORT          GPIOB // Cycle start
#define AUXINPUT4_PIN           8

// Define user-control controls (cycle start, reset, feed hold) input pins.
#if CONTROL_ENABLE & CONTROL_HALT
#define RESET_PORT              AUXINPUT2_PORT
#define RESET_PIN               AUXINPUT2_PIN
#endif
#if CONTROL_ENABLE & CONTROL_FEED_HOLD
#define FEED_HOLD_PORT          AUXINPUT3_PORT
#define FEED_HOLD_PIN           AUXINPUT3_PIN
#endif
#if CONTROL_ENABLE & CONTROL_CYCLE_START
#define CYCLE_START_PORT        AUXINPUT4_PORT
#define CYCLE_START_PIN         AUXINPUT4_PIN
#endif

#if PROBE_ENABLE && defined(AUXINPUT1_PIN)
#define PROBE_PORT              AUXINPUT1_PORT
#define PROBE_PIN               AUXINPUT1_PIN
#endif

#if SAFETY_DOOR_ENABLE
#define SAFETY_DOOR_PORT        AUXINPUT0_PORT
#define SAFETY_DOOR_PIN         AUXINPUT0_PIN
#elif MOTOR_FAULT_ENABLE
#define MOTOR_FAULT_PORT        AUXINPUT0_PORT
#define MOTOR_FAULT_PIN         AUXINPUT0_PIN
#endif

// NOT SUPPORTED
#if SDCARD_ENABLE
//#error SDcard not supported
#endif

/* EOF */
