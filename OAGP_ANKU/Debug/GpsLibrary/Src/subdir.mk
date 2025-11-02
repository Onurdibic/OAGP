################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../GpsLibrary/Src/Gps.cpp 

OBJS += \
./GpsLibrary/Src/Gps.o 

CPP_DEPS += \
./GpsLibrary/Src/Gps.d 


# Each subdirectory must supply rules for building sources it contributes
GpsLibrary/Src/%.o GpsLibrary/Src/%.su GpsLibrary/Src/%.cyclo: ../GpsLibrary/Src/%.cpp GpsLibrary/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/T_rab/OneDrive/Desktop/STM32/OAGP/OAGP_ANKU/ImuLibrary" -I"C:/Users/T_rab/OneDrive/Desktop/STM32/OAGP/OAGP_ANKU/ImuLibrary/Inc" -I"C:/Users/T_rab/OneDrive/Desktop/STM32/OAGP/OAGP_ANKU/GpsLibrary" -I"C:/Users/T_rab/OneDrive/Desktop/STM32/OAGP/OAGP_ANKU/GpsLibrary/Inc" -I"C:/Users/T_rab/OneDrive/Desktop/STM32/OAGP/OAGP_ANKU/PaketLibrary/Inc" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-GpsLibrary-2f-Src

clean-GpsLibrary-2f-Src:
	-$(RM) ./GpsLibrary/Src/Gps.cyclo ./GpsLibrary/Src/Gps.d ./GpsLibrary/Src/Gps.o ./GpsLibrary/Src/Gps.su

.PHONY: clean-GpsLibrary-2f-Src

