#include <io.h>
#include <arm/omap4.h>

#if 0
#define OSC_SYS_CLK  MHz(26)
#define OSC_DIV      2
#define SYS_CLK      (OSC_SYS_CLK / SYS_CLK)
#endif

struct dpll_desc {
	__u32 m, n;
	__u32 m2;
	__u32 m3;
	__u32 m4;
	__u32 m5;
	__u32 m6;
	__u32 m7;
};

static const struct dpll_desc g_mpu_dpll = {
	0x7d, 0x07, 0x1, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const struct dpll_desc g_per_dpll = {
	0x14, 0x00, 0x08, 0x06, 0x0c, 0x09, 0x04, 0x05,
};

static const struct dpll_desc g_core_dpll = {
	0x7d, 0x05, 0x01, 0x05, 0x08, 0x04, 0x06, 0x05
};

static void mpu_dpll_init(const struct dpll_desc *dpll)
{
	bits_writel(VA(CM_CLKMODE_DPLL_MPU), 0x7, PLL_MP_BYPASS);
	bits_writel(VA(CM_AUTOIDLE_DPLL_MPU), 0x7, 0x0);
	bits_writel(VA(CM_CLKSEL_DPLL_MPU), 0x7FF << 8, dpll->m << 8);
	bits_writel(VA(CM_CLKSEL_DPLL_MPU), 0x3F, dpll->n);
	bits_writel(VA(CM_DIV_M2_DPLL_MPU), 0x1F, dpll->m2);
	bits_writel(VA(CM_DIV_M2_DPLL_MPU), 0x1 << 8, 0x1 << 8);
	bits_writel(VA(CM_CLKMODE_DPLL_MPU), 0x7, PLL_LOCK);
}

static void per_dpll_init(const struct dpll_desc *dpll)
{
	bits_writel(VA(CM_CLKMODE_DPLL_PER), 0x7, PLL_MP_BYPASS);
	bits_writel(VA(CM_AUTOIDLE_DPLL_PER), 0x7, 0x0);
	bits_writel(VA(CM_CLKSEL_DPLL_PER), 0x7FF << 8, dpll->m << 8);
	bits_writel(VA(CM_CLKSEL_DPLL_PER), 0x3F, dpll->n);
	bits_writel(VA(CM_DIV_M2_DPLL_PER), 0x1F, dpll->m2);
	bits_writel(VA(CM_DIV_M2_DPLL_PER), 0x1 << 8, 0x1 << 8);
	bits_writel(VA(CM_DIV_M3_DPLL_PER), 0x1F, dpll->m3);
	bits_writel(VA(CM_DIV_M3_DPLL_PER), 0x1 << 8, 0x1 << 8);
	bits_writel(VA(CM_DIV_M4_DPLL_PER), 0x1F, dpll->m4);
	bits_writel(VA(CM_DIV_M4_DPLL_PER), 0x1 << 8, 0x1 << 8);
	bits_writel(VA(CM_DIV_M5_DPLL_PER), 0x1F, dpll->m5);
	bits_writel(VA(CM_DIV_M5_DPLL_PER), 0x1 << 8, 0x1 << 8);
	bits_writel(VA(CM_DIV_M6_DPLL_PER), 0x1F, dpll->m6);
	bits_writel(VA(CM_DIV_M6_DPLL_PER), 0x1 << 8, 0x1 << 8);
	bits_writel(VA(CM_DIV_M7_DPLL_PER), 0x1F, dpll->m7);
	bits_writel(VA(CM_DIV_M7_DPLL_PER), 0x1 << 8, 0x1 << 8);
	bits_writel(VA(CM_CLKMODE_DPLL_PER), 0x7, PLL_LOCK);
}

static void core_dpll_init(const struct dpll_desc *dpll)
{
	__u32 val;

	writel(VA(CM_CLKSEL_CORE), 0x110);
	bits_writel(VA(CM_CLKMODE_DPLL_CORE), 0x7, PLL_MP_BYPASS);
	bits_writel(VA(CM_AUTOIDLE_DPLL_CORE), 0x7, 0x0);
	bits_writel(VA(CM_CLKSEL_DPLL_CORE), 0x7FF << 8, dpll->m << 8);
	bits_writel(VA(CM_CLKSEL_DPLL_CORE), 0x3F, dpll->n);
	bits_writel(VA(CM_DIV_M2_DPLL_CORE), 0x1F, dpll->m2);
	bits_writel(VA(CM_DIV_M3_DPLL_CORE), 0x1F, dpll->m3);
	bits_writel(VA(CM_DIV_M4_DPLL_CORE), 0x1F, dpll->m4);
	bits_writel(VA(CM_DIV_M5_DPLL_CORE), 0x1F, dpll->m5);
	bits_writel(VA(CM_DIV_M6_DPLL_CORE), 0x1F, dpll->m6);
	bits_writel(VA(CM_DIV_M7_DPLL_CORE), 0x1F, dpll->m7);

	// lock
	val = readl(VA(CM_MEMIF_CLKSTCTRL));
	val &= ~3;
	val |= 2;
	writel(VA(CM_MEMIF_CLKSTCTRL), val);

	while(readl(VA(CM_MEMIF_EMIF_1_CLKCTRL)) & 0x30000);
	while(readl(VA(CM_MEMIF_EMIF_2_CLKCTRL)) & 0x30000);

	writel(VA(0x4A004120), 0x0A);
	writel(VA(0x4A004260), 0x70D | (dpll->m2 << 11));
	while((readl(VA(0x4A004260)) & 0x1) == 0x1);

	// while(readl(VA(CM_MEMIF_EMIF_1_CLKCTRL)) & 0x30000);
	// while(readl(VA(CM_MEMIF_EMIF_2_CLKCTRL)) & 0x30000);

	writel(VA(CM_MEMIF_CLKSTCTRL), val | 3);
}

int soc_init(void)
{
#if 0
	// disable WDT
	writel(VA(CM_FCLKEN_WKUP), 1 << 5);
	writel(VA(CM_ICLKEN_WKUP), 1 << 5);
	while (readl(VA(CM_IDLEST_WKUP)) & (1 << 5));

	writel(VA(WDTTIMER2 + WSPR), 0xaaaa);
	while (readl(VA(WDTTIMER2 + WWPS)));
	writel(VA(WDTTIMER2 + WSPR), 0x5555);
#endif

	// SYS_CLK
	writel(VA(CM_SYS_CLKSEL), 0x7);

	mpu_dpll_init(&g_mpu_dpll);
	per_dpll_init(&g_per_dpll);
	core_dpll_init(&g_core_dpll);

	// fixme: to be moved
	// GPIO clocks
	writel(VA(CM_L4PER_GPIO2_CLKCTRL), 0x1);
	writel(VA(CM_L4PER_GPIO3_CLKCTRL), 0x1);
	writel(VA(CM_L4PER_GPIO4_CLKCTRL), 0x1);
	bits_writel(VA(CM_L4PER_GPIO4_CLKCTRL), 0x1 << 8, 0x1 << 8);
	writel(VA(CM_L4PER_GPIO5_CLKCTRL), 0x1);
	writel(VA(CM_L4PER_GPIO6_CLKCTRL), 0x1);

	// UART clocks
	writel(VA(CM_L4PER_UART1_CLKCTRL), 0x2);
	writel(VA(CM_L4PER_UART2_CLKCTRL), 0x2);
	writel(VA(CM_L4PER_UART3_CLKCTRL), 0x2);
	writel(VA(CM_L4PER_UART4_CLKCTRL), 0x2);

	return 0;
}
