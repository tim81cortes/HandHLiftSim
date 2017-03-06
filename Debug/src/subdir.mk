################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/lift.cpp \
../src/main.cpp \
../src/pass_event.cpp \
../src/splitter.cpp \
../src/timer.cpp \
../src/treeSrtNode.cpp 

OBJS += \
./src/lift.o \
./src/main.o \
./src/pass_event.o \
./src/splitter.o \
./src/timer.o \
./src/treeSrtNode.o 

CPP_DEPS += \
./src/lift.d \
./src/main.d \
./src/pass_event.d \
./src/splitter.d \
./src/timer.d \
./src/treeSrtNode.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


