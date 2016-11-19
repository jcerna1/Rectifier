################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_15.12.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./source/ADC_drv.obj" \
"./source/BACK_loop.obj" \
"./source/DLOG_gen.obj" \
"./source/GPIO.obj" \
"./source/PER_int.obj" \
"./source/PWM_drv.obj" \
"./source/TEST_GPIO.obj" \
"./source/globals.obj" \
"./source/main.obj" \
"./source/device/F2806x_CSMPasswords.obj" \
"./source/device/F2806x_CodeStartBranch.obj" \
"./source/device/F2806x_DBGIER.obj" \
"./source/device/F2806x_DefaultIsr.obj" \
"./source/device/F2806x_DisInt.obj" \
"./source/device/F2806x_GlobalVariableDefs.obj" \
"./source/device/F2806x_Gpio.obj" \
"./source/device/F2806x_MemCopy.obj" \
"./source/device/F2806x_PieCtrl.obj" \
"./source/device/F2806x_PieVect.obj" \
"./source/device/F2806x_SysCtrl.obj" \
"./source/device/F2806x_TempSensorConv.obj" \
"./source/device/F2806x_usDelay.obj" \
"../cmd/2806x_lnk_RAM.cmd" \
"../cmd/F2806x_Headers_nonBIOS.cmd" \
"../lib/IQmath_fpu32.lib" \
"../lib/rts2800_fpu32.lib" \
"../lib/rts2800_fpu32_fast_supplement.lib" \
$(GEN_CMDS__FLAG) \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include cmd/subdir_vars.mk
-include lib/subdir_vars.mk
-include source/subdir_vars.mk
-include source/device/subdir_vars.mk
-include cmd/subdir_rules.mk
-include lib/subdir_rules.mk
-include source/subdir_rules.mk
-include source/device/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Rectifier.out \

EXE_OUTPUTS__QUOTED += \
"Rectifier.out" \

BIN_OUTPUTS += \
Rectifier.hex \

BIN_OUTPUTS__QUOTED += \
"Rectifier.hex" \


# All Target
all: Rectifier.out

# Tool invocations
Rectifier.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_15.12.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -g --diag_warning=225 --display_error_number --diag_wrap=off -z -m"Rectifier.map" --stack_size=0x300 --warn_sections -i"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_15.12.1.LTS/lib" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_15.12.1.LTS/include" --reread_libs --display_error_number --diag_wrap=off --xml_link_info="Rectifier_linkInfo.xml" --rom_model -o "Rectifier.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

Rectifier.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C2000 Hex Utility'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_15.12.1.LTS/bin/hex2000"  -o "Rectifier.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "source\ADC_drv.d" "source\BACK_loop.d" "source\DLOG_gen.d" "source\GPIO.d" "source\PER_int.d" "source\PWM_drv.d" "source\TEST_GPIO.d" "source\globals.d" "source\main.d" "source\device\F2806x_DefaultIsr.d" "source\device\F2806x_GlobalVariableDefs.d" "source\device\F2806x_Gpio.d" "source\device\F2806x_MemCopy.d" "source\device\F2806x_PieCtrl.d" "source\device\F2806x_PieVect.d" "source\device\F2806x_SysCtrl.d" "source\device\F2806x_TempSensorConv.d" 
	-$(RM) "source\ADC_drv.obj" "source\BACK_loop.obj" "source\DLOG_gen.obj" "source\GPIO.obj" "source\PER_int.obj" "source\PWM_drv.obj" "source\TEST_GPIO.obj" "source\globals.obj" "source\main.obj" "source\device\F2806x_CSMPasswords.obj" "source\device\F2806x_CodeStartBranch.obj" "source\device\F2806x_DBGIER.obj" "source\device\F2806x_DefaultIsr.obj" "source\device\F2806x_DisInt.obj" "source\device\F2806x_GlobalVariableDefs.obj" "source\device\F2806x_Gpio.obj" "source\device\F2806x_MemCopy.obj" "source\device\F2806x_PieCtrl.obj" "source\device\F2806x_PieVect.obj" "source\device\F2806x_SysCtrl.obj" "source\device\F2806x_TempSensorConv.obj" "source\device\F2806x_usDelay.obj" 
	-$(RM) "source\device\F2806x_CSMPasswords.d" "source\device\F2806x_CodeStartBranch.d" "source\device\F2806x_DBGIER.d" "source\device\F2806x_DisInt.d" "source\device\F2806x_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

