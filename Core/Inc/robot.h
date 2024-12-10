#ifndef robot_h
#define robot_h

#ifdef __cplusplus

#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "usart.h"

#include "robotlib/communication/uart.hpp"

#define JOYSTICK_UART huart4


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

    USART_TypeDef* get_joystick_handle() { return joystick_uart.get_uart_instance(); }
    void process_joystick_callback() { joystick_uart.process_receive_callback(); }
    uint32_t get_rx_led_tick() { return rx_led_tick; }
    void set_rx_led_tick(uint32_t tick) { rx_led_tick = tick; }
    uint32_t get_err_led_tick() { return err_led_tick; }
    void set_err_led_tick(uint32_t tick) { err_led_tick = tick; }

    ~Robot() = default;

private:
    UART joystick_uart{&JOYSTICK_UART, UART_BOTH, sizeof(Twist)};
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