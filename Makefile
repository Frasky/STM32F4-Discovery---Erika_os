#globals
PROJ_NAME=erika_uart
PROGPATH=../../programmers
COMPILER=$(PROGPATH)/gcc-arm-embedded/gcc-arm-none-eabi-4_8-2013q4
BINPATH=$(PROGPATH)/gcc-arm-embedded/gcc-arm-none-eabi-4_8-2013q4/bin


############################################################################
#
# STM32F4XX_SPD Options
#
############################################################################
EEOPT += __USE_STM32F4XX_SPD__



############################################################################
#
# STM32F4_DISCOVERY Options
#
############################################################################
EEOPT += __USE_STM32F4_DISCOVERY__

### Setting Erika's variables:
# ERIKA_FILES= ../../..
# APPBASE = .
# 
# EEOPT =


RTDRUID_CONFIGURATOR_NUMBER:=1277



############################################################################
#
# User options
#
############################################################################
EEOPT += DEBUG
EEOPT += __USE_SYSTICK__
EEOPT += __ADD_LIBS__


############################################################################
#
# Automatic options
#
############################################################################
EEOPT += __RTD_LINUX__
EEOPT += __STM32__
EEOPT += __STM32F4xx__
EEOPT += __CORTEX_MX__
EEOPT += __CORTEX_M4__
EEOPT += __GNU__
EEOPT += __FP__
EEOPT += __MONO__
EEOPT += __ALARMS__
EEOPT += __FP_NO_RESOURCE__
EEOPT += __DISABLE_EEOPT_DEFINES__


############################################################################
#
# Erika base directory
#
############################################################################
ifdef ERIKA_FILES
ifdef EEBASE
    $(warning EEBASE is set, but it has been overridden by ERIKA_FILES)
endif
EEBASE := ${ERIKA_FILES}

else # ERIKA_FILES

ifndef EEBASE
        EEBASE := erika_os
else
        $(warning The usage of EEBASE is deprecated. Please use ERIKA_FILES)
endif
endif # ERIKA_FILES
# ERIKA_FILES has fulfilled its role. Make sure it's not used inside Erika makefiles
ERIKA_FILES := 
$(info Using erika files in $(EEBASE))


############################################################################
#
# CORTEX M0
#
############################################################################
include $(EEBASE)/pkg/cfg/path_helper.mk
APPBASE := ..
OUTBASE := Debug

$(eval $(call check_and_set_linux_compiler_path,GNU_ARM_ROOT,$(COMPILER)))


############################################################################
#
# Init MCU for CORTEX Mx (STM32F4xx)
#
############################################################################
STM32_MODEL           := STM32F4xx


############################################################################
#
# Flags
#
############################################################################
CFLAGS  += $(INCLUDE) -g3
ASFLAGS += 
LDFLAGS += 
LDDEPS  += 
LIBS    += 
OPT_LIBS += libusbdevcore.a libusbdevcdc.a libusbcore.a 


LIB_OPT  += __LIB_ST_CMSIS__
LIB_OPT  += __LIB_STM32F4XX_SPD__
LIB_OPT  += __LIB_STM32F4_DISCOVERY__



############################################################################
#
# Sources
#
############################################################################


### ADDED USB STUFF:



INCLUDE+=-I$(CURDIR)/erika_os/contrib/st/USB_Host/Core/inc
INCLUDE+=-I$(CURDIR)/erika_os/contrib/st/USB_Device/Class/cdc/inc
INCLUDE+=-I$(CURDIR)/erika_os/contrib/st/USB_Host/Class/MSC/inc
INCLUDE+=-I$(CURDIR)/erika_os/contrib/st/USB_Device/Core/inc
INCLUDE+=-I$(CURDIR)/erika_os/contrib/st/USB_Host/Class/HID/inc

INCLUDE+=-I$(CURDIR)/erika_os/contrib/st/USB_OTG/inc
INCLUDE+=-I$(CURDIR)/erika_os/contrib/st/USB_Device/Core/inc
INCLUDE+=-I$(CURDIR)/erika_os/contrib/st/Conf
INCLUDE+=-I$(CURDIR)/

INCLUDE+=-I$(CURDIR)/erika_os/contrib/st/CMSIS/Device/ST/STM32F4xx/Include
INCLUDE+=-I$(CURDIR)/erika_os/contrib/st/CMSIS/Include
INCLUDE+=-I$(CURDIR)/erika_os/contrib/st/STM32F4-Discovery/inc
INCLUDE+=-I$(CURDIR)/erika_os/contrib/st/STM32F4xx_StdPeriph_Driver/inc
					 
vpath %.c $(CURDIR)/erika_os/contrib/st/STM32F4-Discovery/src
vpath %.c $(CURDIR)/erika_os/contrib/st/STM32F4xx_StdPeriph_Driver/src

APP_SRCS+=misc.c
APP_SRCS+=stm32f4xx_exti.c
APP_SRCS+=stm32f4xx_gpio.c
APP_SRCS+=stm32f4xx_i2c.c
APP_SRCS+=stm32f4xx_rcc.c
APP_SRCS+=stm32f4xx_tim.c
APP_SRCS+=stm32f4xx_usart.c
APP_SRCS+=stm32f4xx_rng.c
APP_SRCS+=stm32f4_discovery.c


APP_SRCS+=stm32f4xx_it.c 
APP_SRCS+=usbd_cdc_vcp.c
APP_SRCS+=usb_bsp.c
APP_SRCS+=usbd_desc.c
APP_SRCS+=usbd_usr.c

APP_SRCS += eecfg.c code.c 

############################################################################
#
# end
#
############################################################################
include $(EEBASE)/pkg/cfg/rules.mk


flash:
	st-flash write $(PROJ_NAME).bin 0x8000000
	
gdb:
	gnome-terminal --execute bash -c '$(PROGPATH)/st-util ""; bash'
	$(BINPATH)/arm-none-eabi-gdb $(PROJ_NAME).elf
