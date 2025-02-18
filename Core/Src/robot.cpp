#include <stdio.h>
#include "robot.h"
#include <math.h>

Robot robot;

void Robot::init()
{
    joystick_uart.init();
    for (int i = 0; i < 4; ++i)
    {
        motors[i].init();
    }
    HAL_Delay(100);
}

void Robot::run_motors(Twist twist)
{
    float vel = twist.v;
    float omega = twist.w;
        printf("%f   ", vel);
        printf("%f   ", omega);


    float v_left = vel + omega * (wheel_base / 2);
    float v_right = vel - omega * (wheel_base / 2);

        printf("%f   ", v_left);
        printf("%f   \n", v_right);


    // Normalize the wheel velocities to range [-1, 1]
    float max_speed = fmaxf(fabs(v_left), fabs(v_right));
    if (max_speed > 1.0f)
    {
        v_left /= max_speed;
        v_right /= max_speed;
    }
    vel = vel / 3.0;
    motors[0].set_speed(v_left);
    motors[1].set_speed(v_right);
    motors[2].set_speed(v_left);
    motors[3].set_speed(v_right);
    // printf("%f\n", vel);
}

void Robot::run()
{
    if (HAL_GetTick() - loop_tick < 5)
        return;

    joystick_uart.get_received_data((uint8_t *)&twist);
    // print_twist();
    if (!joystick_uart.connected()) {
        printf("Comms Stopped\n");
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
        Twist twist = Twist{0.0,0.0};
        run_motors(twist);
        joystick_uart.init();
    }
    else{
        run_motors(twist);
    }

    loop_tick = HAL_GetTick();
}

void Robot::print_twist()
{
    printf("velocity: %f   omega:  %f\n", twist.v, twist.w);
}

void UART::UART_RxErrorCallBack(UART_HandleTypeDef *huart, UARTReceiveStatus status)
{
    if (HAL_GetTick() - robot.get_err_led_tick() > 10)
    {
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
        robot.set_err_led_tick(HAL_GetTick());
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    Twist twist = Twist{0.0, 0.0};
    robot.run_motors(twist);
    robot.joystick_uart .init();
    if (HAL_GetTick() - robot.get_err_led_tick() > 10)
    {
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
        robot.set_err_led_tick(HAL_GetTick());
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (HAL_GetTick() - robot.get_rx_led_tick() > 200)
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