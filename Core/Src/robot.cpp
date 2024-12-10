#include <stdio.h>

#include "robot.h"

Robot robot;

void Robot::init()
{
    joystick_uart.init();

    HAL_Delay(100);
}

void Robot::run()
{
    if (HAL_GetTick() - loop_tick < 10)
        return;

    joystick_uart.get_received_data((uint8_t *)&twist);
    print_twist();

    loop_tick = HAL_GetTick();
}

void Robot::print_twist()
{
    printf("twist(v,w): %f %f\n", twist.v, twist.w);
}

void UART::UART_RxErrorCallBack(UART_HandleTypeDef *huart, UARTReceiveStatus status)
{
    if (HAL_GetTick() - robot.get_err_led_tick() > 50)
    {
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
        robot.set_err_led_tick(HAL_GetTick());
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (HAL_GetTick() - robot.get_err_led_tick() > 50)
    {
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
        robot.set_err_led_tick(HAL_GetTick());
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (HAL_GetTick() - robot.get_rx_led_tick() > 50)
    {
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
        robot.set_rx_led_tick(HAL_GetTick());
    }

    if (huart->Instance == robot.get_joystick_handle())
    {
        robot.process_joystick_callback();
    }
}

void run_robot()
{
    robot.init();

    while (1)
        robot.run();
}