################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/PlatformGAME.cpp 

OBJS += \
./src/PlatformGAME.o 

CPP_DEPS += \
./src/PlatformGAME.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/user/Desktop/eclipse_ws_eliav/PlatformSDKClient/src" -I"/home/user/Desktop/eclipse_ws_eliav/lab32/Utils/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


