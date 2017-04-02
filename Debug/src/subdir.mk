################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Boid.cpp \
../src/BoidScene.cpp \
../src/ParamaterFileReader.cpp \
../src/PhysicsObject.cpp \
../src/SceneShader.cpp \
../src/Shader.cpp \
../src/main.cpp 

OBJS += \
./src/Boid.o \
./src/BoidScene.o \
./src/ParamaterFileReader.o \
./src/PhysicsObject.o \
./src/SceneShader.o \
./src/Shader.o \
./src/main.o 

CPP_DEPS += \
./src/Boid.d \
./src/BoidScene.d \
./src/ParamaterFileReader.d \
./src/PhysicsObject.d \
./src/SceneShader.d \
./src/Shader.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ugd/bemayer/CPSC587/A3/libraries/trimesh/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


