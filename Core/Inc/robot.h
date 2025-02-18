#ifndef robot_h
#define robot_h

#ifdef __cplusplus

#include "stm32f4xx_hal.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "robotlib/communication/uart.hpp"
#include "robotlib/actuators/motor.hpp"
#include "robotlib/sensors/encoder.hpp"
#include "Robot_config.h"

#define JOYSTICK_UART huart2
#define wheel_base 1.0f


struct Twist
{
    float v;
    float w;
};
class Robot
{
public:
    Robot() = default;  
    void init();
    void run();
    void print_twist();
    void run_motors(Twist twist);

    USART_TypeDef* get_joystick_handle() { return joystick_uart.get_uart_instance(); }
    void process_joystick_callback() { joystick_uart.process_receive_callback(); }
    uint32_t get_rx_led_tick() { return rx_led_tick; }
    void set_rx_led_tick(uint32_t tick) { rx_led_tick = tick; }
    uint32_t get_err_led_tick() { return err_led_tick; }
    void set_err_led_tick(uint32_t tick) { err_led_tick = tick; }

    ~Robot() = default;

    UART joystick_uart{&JOYSTICK_UART, UART_BOTH, sizeof(Twist)};
    private:
    Motor motors[4] = {Motor(&M1P_TIMER, M1P_TIMER_CHANNEL, M1D_GPIO_Port, M1D_Pin),
                       Motor(&M2P_TIMER, M2P_TIMER_CHANNEL, M2D_GPIO_Port, M2D_Pin),
                       Motor(&M3P_TIMER, M3P_TIMER_CHANNEL, M3D_GPIO_Port, M3D_Pin),
                       Motor(&M4P_TIMER, M4P_TIMER_CHANNEL, M4D_GPIO_Port, M4D_Pin)};

    const Encoder motor_encoders[4] = {Encoder(&ENC5_TIMER, -537),
                                       Encoder(&ENC3_TIMER, -537),
                                       Encoder(&ENC2_TIMER, -537),
                                       Encoder(&ENC1_TIMER, -537)};

    Twist twist{0.0f, 0.0f};
    uint32_t loop_tick = 0;
    uint32_t rx_led_tick = 0;
    uint32_t err_led_tick = 0;
};

extern Robot robot;

#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

void  run_robot();

#ifdef __cplusplus
}
#endif

#endif // robot_h










