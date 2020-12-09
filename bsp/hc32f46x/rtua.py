
def GetCPPPATH(BSP_ROOT, RTT_ROOT):
	CPPPATH=[
		RTT_ROOT + "/bsp/hc32f46x",
		RTT_ROOT + "/bsp/hc32f46x/applications",
		RTT_ROOT + "/bsp/hc32f46x/drivers",
		RTT_ROOT + "/bsp/hc32f46x/libraries",
		RTT_ROOT + "/bsp/hc32f46x/libraries/CMSIS/include",
		RTT_ROOT + "/bsp/hc32f46x/libraries/driver/inc",
		RTT_ROOT + "/bsp/hc32f46x/libraries/mcu/common",
		RTT_ROOT + "/bsp/hc32f46x/packages/CmBacktrace-latest",
		RTT_ROOT + "/bsp/hc32f46x/packages/EasyFlash-master/inc",
		RTT_ROOT + "/bsp/hc32f46x/packages/fal-latest/inc",
		RTT_ROOT + "/bsp/hc32f46x/packages/fal-latest/porting",
		RTT_ROOT + "/components/dfs/filesystems/devfs",
		RTT_ROOT + "/components/dfs/filesystems/elmfat",
		RTT_ROOT + "/components/dfs/include",
		RTT_ROOT + "/components/drivers/include",
		RTT_ROOT + "/components/drivers/spi",
		RTT_ROOT + "/components/drivers/spi/sfud/inc",
		RTT_ROOT + "/components/finsh",
		RTT_ROOT + "/components/libc/compilers/newlib",
		RTT_ROOT + "/components/libc/libdl",
		RTT_ROOT + "/components/utilities/ulog",
		RTT_ROOT + "/components/utilities/ymodem",
		RTT_ROOT + "/include",
		RTT_ROOT + "/libcpu/arm/common",
		RTT_ROOT + "/libcpu/arm/cortex-m4",
	]

	return CPPPATH

def GetCPPDEFINES():
	CPPDEFINES=['HC32F46x', 'RT_USING_NEWLIB', '__DEBUG', 'USE_DEVICE_DRIVER_LIB']
	return CPPDEFINES

