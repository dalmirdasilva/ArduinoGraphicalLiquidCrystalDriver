################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../GlcdStraight/GlcdStraight.cpp 

OBJS += \
./GlcdStraight/GlcdStraight.o 

CPP_DEPS += \
./GlcdStraight/GlcdStraight.d 


# Each subdirectory must supply rules for building sources it contributes
GlcdStraight/%.o: ../GlcdStraight/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I/usr/share/arduino/libraries/Wire/utility -I/usr/share/arduino/libraries/Wire -I/usr/share/arduino/hardware/arduino/variants/standard -I/usr/share/arduino/hardware/arduino/cores/arduino -I"/home/dalmir/storage/microcontroller/arduino/driver/glcd" -I"/home/dalmir/storage/microcontroller/arduino/driver/glcd/GlcdWire" -I"/home/dalmir/storage/microcontroller/arduino/driver/glcd/Glcd" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


