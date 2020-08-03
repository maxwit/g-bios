#include <io.h>
#include <delay.h>
#include <stdio.h>

#define EMIF_READL(reg) \
	readl(emif_base + (reg))

#define EMIF_WRITEL(reg, val) \
	writel(emif_base + (reg), (val))

struct omap4_ddr_config {
	__u32 tim1;
	__u32 tim2;
	__u32 tim3;
	__u32 phy_ctrl_1;
	__u32 ref_ctrl;
	__u32 config_init;
	__u32 config_final;
	__u32 zq_config;
	__u8 mr1;
	__u8 mr2;
};

static void emif_init_one(int emif, const struct omap4_ddr_config *ddr)
{
	__u32 val;
	void *emif_base, *clk_ctrl;

	if (emif == 1) {
		emif_base = VA(EMIF1_BASE);
		clk_ctrl = VA(CM_MEMIF_EMIF_1_CLKCTRL);
	} else {
		emif_base = VA(EMIF2_BASE);
		clk_ctrl = VA(CM_MEMIF_EMIF_2_CLKCTRL);
	}

	EMIF_WRITEL(EMIF_PWR_MGMT_CTRL, 0x0);

	val = EMIF_READL(EMIF_LPDDR2_NVM_CONFIG);
	EMIF_WRITEL(EMIF_LPDDR2_NVM_CONFIG, val & 0xBFFFFFFF);
	EMIF_WRITEL(EMIF_SDRAM_CONFIG, ddr->config_init);

	EMIF_WRITEL(EMIF_DDR_PHY_CTRL_1, 0x849FFFF5);
	EMIF_WRITEL(EMIF_DDR_PHY_CTRL_1_SHDW, ddr->phy_ctrl_1);

	EMIF_WRITEL(EMIF_READ_IDLE_CTRL, READ_IDLE_CTRL);
	EMIF_WRITEL(EMIF_READ_IDLE_CTRL_SHDW, READ_IDLE_CTRL);

	EMIF_WRITEL(EMIF_SDRAM_TIM_1, ddr->tim1);
	EMIF_WRITEL(EMIF_SDRAM_TIM_1_SHDW, ddr->tim1);

	EMIF_WRITEL(EMIF_SDRAM_TIM_2, ddr->tim2);
	EMIF_WRITEL(EMIF_SDRAM_TIM_2_SHDW, ddr->tim2);

	EMIF_WRITEL(EMIF_SDRAM_TIM_3, ddr->tim3);
	EMIF_WRITEL(EMIF_SDRAM_TIM_3_SHDW, ddr->tim3);

	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_CFG, MR0_ADDR);
	do {
		val = EMIF_READL(EMIF_LPDDR2_MODE_REG_DATA);
	} while ((val & 0x1) != 0);

	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_CFG, CS1_MR(MR0_ADDR));
	do {
		val = EMIF_READL(EMIF_LPDDR2_MODE_REG_DATA);
	} while ((val & 0x1) != 0);

	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_CFG, MR10_ADDR);
	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_DATA, MR10_ZQINIT);
	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_CFG, CS1_MR(MR10_ADDR));
	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_DATA, MR10_ZQINIT);

	udelay(0x100);

	EMIF_WRITEL(EMIF_ZQ_CONFIG, ddr->zq_config);

	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_CFG, MR1_ADDR);
	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_DATA, ddr->mr1);
	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_CFG, CS1_MR(MR1_ADDR));
	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_DATA, ddr->mr1);

	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_CFG, MR2_ADDR);
	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_DATA, ddr->mr2);
	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_CFG, CS1_MR(MR2_ADDR));
	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_DATA, ddr->mr2);

	EMIF_WRITEL(EMIF_SDRAM_CONFIG, ddr->config_final);
	EMIF_WRITEL(EMIF_DDR_PHY_CTRL_1, ddr->phy_ctrl_1);

	EMIF_WRITEL(EMIF_SDRAM_REF_CTRL, ddr->ref_ctrl);
	EMIF_WRITEL(EMIF_SDRAM_REF_CTRL_SHDW, ddr->ref_ctrl);

	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_CFG, MR16_ADDR | REF_EN);
	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_DATA, 0);
	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_CFG, CS1_MR(MR16_ADDR | REF_EN));
	EMIF_WRITEL(EMIF_LPDDR2_MODE_REG_DATA, 0);

	writel(VA(CM_DLL_CTRL), 0x0);
	while (((EMIF_READL(EMIF_STATUS)) & 0x04) != 0x04);
	writel(clk_ctrl, 0x1);

	EMIF_WRITEL(EMIF_PWR_MGMT_CTRL, 0x80000000);
	EMIF_WRITEL(EMIF_L3_CONFIG, 0x0A0000FF);

	val = EMIF_READL(IODFT_TLGC);
	EMIF_WRITEL(IODFT_TLGC, val | (1 << 10));
}

static const struct omap4_ddr_config elpida2G_400M = {
	.tim1		= 0x10eb0662,
	.tim2		= 0x20370dd2,
	.tim3		= 0x00b1c33f,
	.phy_ctrl_1	= 0x049FF408,
	.ref_ctrl	= 0x00000618,
	.config_init	= 0x80800eb9,
	.config_final	= 0x80801ab9,
	.zq_config	= 0xd00b3215,
	.mr1		= 0x83,
	.mr2		= 0x4
};

int sdram_init(void)
{
	writel(VA(DMM_BASE + DMM_LISA_MAP_0), 0x80720100);
	writel(VA(DMM_BASE + DMM_LISA_MAP_2), 0x00000000);
	writel(VA(DMM_BASE + DMM_LISA_MAP_1), 0xFF020100);
	writel(VA(DMM_BASE + DMM_LISA_MAP_3), 0x80640300);

	emif_init_one(1, &elpida2G_400M);
	emif_init_one(2, &elpida2G_400M);

	return SDRAM_BASE + SDRAM_SIZE;
}
