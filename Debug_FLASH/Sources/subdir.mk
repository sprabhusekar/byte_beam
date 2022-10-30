################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/app_main.c \
../Sources/bb_memory.c \
../Sources/can.c \
../Sources/can_app.c \
../Sources/can_queue.c \
../Sources/can_rx_task.c \
../Sources/can_tx_task.c \
../Sources/clock.c \
../Sources/core_main.c \
../Sources/diagnostic_task.c \
../Sources/gpio.c \
../Sources/internal_flash.c \
../Sources/internal_flash_app.c \
../Sources/main.c \
../Sources/operating_system.c \
../Sources/rr_nvic.c \
../Sources/timer.c \
../Sources/uart.c \
../Sources/uart_queue.c \
../Sources/uart_rx_task.c \
../Sources/uart_state_machine.c \
../Sources/uart_tx_task.c 

OBJS += \
./Sources/app_main.o \
./Sources/bb_memory.o \
./Sources/can.o \
./Sources/can_app.o \
./Sources/can_queue.o \
./Sources/can_rx_task.o \
./Sources/can_tx_task.o \
./Sources/clock.o \
./Sources/core_main.o \
./Sources/diagnostic_task.o \
./Sources/gpio.o \
./Sources/internal_flash.o \
./Sources/internal_flash_app.o \
./Sources/main.o \
./Sources/operating_system.o \
./Sources/rr_nvic.o \
./Sources/timer.o \
./Sources/uart.o \
./Sources/uart_queue.o \
./Sources/uart_rx_task.o \
./Sources/uart_state_machine.o \
./Sources/uart_tx_task.o 

C_DEPS += \
./Sources/app_main.d \
./Sources/bb_memory.d \
./Sources/can.d \
./Sources/can_app.d \
./Sources/can_queue.d \
./Sources/can_rx_task.d \
./Sources/can_tx_task.d \
./Sources/clock.d \
./Sources/core_main.d \
./Sources/diagnostic_task.d \
./Sources/gpio.d \
./Sources/internal_flash.d \
./Sources/internal_flash_app.d \
./Sources/main.d \
./Sources/operating_system.d \
./Sources/rr_nvic.d \
./Sources/timer.d \
./Sources/uart.d \
./Sources/uart_queue.d \
./Sources/uart_rx_task.d \
./Sources/uart_state_machine.d \
./Sources/uart_tx_task.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/app_main.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


