################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/capteurCouleur.cpp \
../src/capteurInfra.cpp \
../src/capteurLigne.cpp \
../src/capteurSon.cpp \
../src/capteurs.cpp \
../src/main.cpp 

OBJS += \
./src/capteurCouleur.o \
./src/capteurInfra.o \
./src/capteurLigne.o \
./src/capteurSon.o \
./src/capteurs.o \
./src/main.o 

CPP_DEPS += \
./src/capteurCouleur.d \
./src/capteurInfra.d \
./src/capteurLigne.d \
./src/capteurSon.d \
./src/capteurs.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking ARM-Linux C++ Compiler'
	arm-softfloat-linux-gnueabi-g++ -c -I"C:/Users/Utilisateur/Dropbox/UNI GI/solutions/GrandeCourse/include" -I"C:/Program Files (x86)/ArmusIDE/librairie/include" -ggdb --sysroot="C:/Program Files (x86)/ArmusIDE/toolchain/sysroot" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


