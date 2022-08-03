################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RTT/SEGGER_RTT.c \
../RTT/SEGGER_RTT_Syscalls_GCC.c \
../RTT/SEGGER_RTT_printf.c 

OBJS += \
./RTT/SEGGER_RTT.o \
./RTT/SEGGER_RTT_Syscalls_GCC.o \
./RTT/SEGGER_RTT_printf.o 

C_DEPS += \
./RTT/SEGGER_RTT.d \
./RTT/SEGGER_RTT_Syscalls_GCC.d \
./RTT/SEGGER_RTT_printf.d 


# Each subdirectory must supply rules for building sources it contributes
RTT/%.o: ../RTT/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@RTT/SEGGER_RTT.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


