################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../generator.c \
../grammar.tab.c \
../lex.yy.c \
../main.c \
../symbols.c \
../test.c 

OBJS += \
./generator.o \
./grammar.tab.o \
./lex.yy.o \
./main.o \
./symbols.o \
./test.o 

C_DEPS += \
./generator.d \
./grammar.tab.d \
./lex.yy.d \
./main.d \
./symbols.d \
./test.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


