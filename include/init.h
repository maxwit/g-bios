#pragma once

#define __init           __attribute__ ((__section__(".code.init")))
#define __init_data      __attribute__ ((__section__(".data.init")))

#define INIT_CALL_LEVEL(n) __attribute__ ((__section__(".Level" #n ".gbios_init")))

#define __INIT_ARCH__     INIT_CALL_LEVEL(0)
#define __INIT_PLAT__     INIT_CALL_LEVEL(1) // PLL and WDT
#define __INIT_STDIO__    INIT_CALL_LEVEL(2)
#define __POST_STDIO__    INIT_CALL_LEVEL(3)
#define __INIT_SDRAM__    INIT_CALL_LEVEL(4)
#define __INIT_DRV__      INIT_CALL_LEVEL(5)

typedef int (*init_func_t)(void);

#define arch_init(func) \
	static __USED__ __INIT_ARCH__ init_func_t __initcall_##func = func

#define plat_init(func) \
	static __USED__ __INIT_PLAT__ init_func_t __initcall_##func = func

#define stdio_init(func) \
	static __USED__ __INIT_STDIO__ init_func_t __initcall_##func = func

#define stdio_post(func) \
	static __USED__ __POST_STDIO__ init_func_t __initcall_##func = func

#define sdram_init(func) \
		static __USED__ __INIT_SDRAM__ init_func_t __initcall_##func = func

#define module_init(func) \
		static __USED__ __INIT_DRV__ init_func_t __initcall_##func = func
