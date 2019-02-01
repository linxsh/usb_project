#include "module.h"
#include "inc/hal_gc0308.h"
#include "inc/hal_systick.h"
#include "src/gd32f4xx_dci.h"
#include "src/gd32f4xx_i2c.h"

static uint32_t g_buf_addr = 0;
static uint32_t g_buf_len  = 0;

static int gc0308_mod_init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	//MCLK
	gpio_mode_set          (GPIOA, GPIO_MODE_AF,  GPIO_PUPD_PULLUP,   GPIO_PIN_8);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_8);
	rcu_ckout0_config(RCU_CKOUT0SRC_IRC16M, RCU_CKOUT0_DIV2);

	rcu_periph_clock_enable(RCU_GPIOC);
	//RESETN
	gpio_mode_set          (GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,    GPIO_PIN_10);
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP,    GPIO_OSPEED_50MHZ, GPIO_PIN_10);

	rcu_periph_clock_enable(RCU_GPIOD);
	//PWDN
	gpio_mode_set          (GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,    GPIO_PIN_2);
	gpio_output_options_set(GPIOD, GPIO_OTYPE_PP,    GPIO_OSPEED_50MHZ, GPIO_PIN_2);

	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_I2C1);
	gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_10);
	gpio_af_set(GPIOC, GPIO_AF_4, GPIO_PIN_12);
	//I2C_SCL
	gpio_mode_seti         (GPIOB, GPIO_MODE_AF,  GPIO_PUPD_NONE,    GPIO_PIN_10);
	gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	//I2C_SDA
	gpio_mode_set          (GPIOC, GPIO_MODE_AF,  GPIO_PUPD_NONE,    GPIO_PIN_12);
	gpio_output_options_set(GPIOC, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_12);

	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_DCI);
	nvic_irq_enable(DCI_IRQn, 0, 0);
	gpio_af_set(GPIOA, GPIO_AF_13, GPIO_PIN_4);
	gpio_af_set(GPIOA, GPIO_AF_13, GPIO_PIN_6);
	gpio_af_set(GPIOB, GPIO_AF_13, GPIO_PIN_6);
	gpio_af_set(GPIOB, GPIO_AF_13, GPIO_PIN_7);
	gpio_af_set(GPIOB, GPIO_AF_13, GPIO_PIN_8);
	gpio_af_set(GPIOB, GPIO_AF_13, GPIO_PIN_9);
	gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_6);
	gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_7);
	gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_8);
	gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_9);
	gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_11);
	//HSYNC
	gpio_mode_set          (GPIOA, GPIO_MODE_AF,  GPIO_PUPD_NONE,     GPIO_PIN_4);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_4);
	//VSYNC
	gpio_mode_set          (GPIOB, GPIO_MODE_AF,  GPIO_PUPD_NONE,     GPIO_PIN_7);
	gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_7);
	//PCLK
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ,GPIO_PIN_6);
	//LOCSC_D0 - LOCSC_D7
	gpio_mode_set          (GPIOC, GPIO_MODE_AF,  GPIO_PUPD_NONE,     GPIO_PIN_6);
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_6);
	gpio_mode_set          (GPIOC, GPIO_MODE_AF,  GPIO_PUPD_NONE,     GPIO_PIN_7);
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_7);
	gpio_mode_set          (GPIOC, GPIO_MODE_AF,  GPIO_PUPD_NONE,     GPIO_PIN_8);
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_8);
	gpio_mode_set          (GPIOC, GPIO_MODE_AF,  GPIO_PUPD_NONE,     GPIO_PIN_9);
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_9);
	gpio_mode_set          (GPIOC, GPIO_MODE_AF,  GPIO_PUPD_NONE,     GPIO_PIN_11);
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_11);
	gpio_mode_set          (GPIOB, GPIO_MODE_AF,  GPIO_PUPD_NONE,     GPIO_PIN_6);
	gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_6);
	gpio_mode_set          (GPIOB, GPIO_MODE_AF,  GPIO_PUPD_NONE,     GPIO_PIN_8);
	gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_8);
	gpio_mode_set          (GPIOB, GPIO_MODE_AF,  GPIO_PUPD_NONE,     GPIO_PIN_9);
	gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_9);

	rcu_periph_clock_enable(RCU_DMA1);

	return 0;
}

int gc0308_mod_deinit(void)
{
	return 0;
}

int hal_gc0308_config_buffer(uint8_t *buf, uint32_t len)
{
	dma_single_data_parameter_struct dma;
	dci_parameter_struct             dci;

	g_buf_addr = (uint32_t)(buf);
	g_buf_len  = (uint32_t)(len);

	i2c_deinit          (I2C1);
	i2c_clock_config    (I2C1, 100000, I2C_DTCY_2);
	i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0xFE);
	i2c_enable          (I2C1);
	i2c_ack_config      (I2C1, I2C_ACK_ENABLE);

	/* DCI DMA configuration */
	dma_deinit(DMA1, DMA_CH7);

	dma.periph_addr   = 0x50050028;
	dma.memory0_addr  = g_buf_addr;
	dma.direction     = DMA_PERIPH_TO_MEMORY;
	dma.number        = g_buf_len / 4;
	dma.periph_inc    = DMA_PERIPH_INCREASE_DISABLE;
	dma.memory_inc    = DMA_MEMORY_INCREASE_ENABLE;
	dma.priority      = DMA_PRIORITY_HIGH;
	dma.circular_mode = DMA_CIRCULAR_MODE_ENABLE;
	dma.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
	dma_single_data_mode_init       (DMA1, DMA_CH7, dma);
	dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI1);
	dma_switch_buffer_mode_config   (DMA1, DMA_CH7, g_buf_addr + g_buf_len, DMA_MEMORY_0);
	dma_switch_buffer_mode_enable   (DMA1, DMA_CH7, ENABLE);
	/* DMA interrupt and channel enable */
	// dma_interrupt_enable(DMA1, DMA_CH7,DMA_CHXCTL_FTFIE);

	/* DCI configuration */
	dci.capture_mode    = DCI_CAPTURE_MODE_SNAPSHOT;
	dci.clock_polarity  = DCI_CK_POLARITY_RISING;
	dci.hsync_polarity  = DCI_HSYNC_POLARITY_LOW;
	dci.vsync_polarity  = DCI_VSYNC_POLARITY_LOW;
	dci.frame_rate      = DCI_FRAME_RATE_ALL;
	dci.interface_format= DCI_INTERFACE_FORMAT_8BITS;
	dci_init(&dci);
	dci_interrupt_clear(DCI_INT_EF|DCI_INT_OVR|DCI_INT_ESE|DCI_INT_VS|DCI_INT_EL);

}

int hal_gc0308_set(CMD_SET_CAMERA_T cmd, void *args)
{
	uint16_t i;

	hal_systick_delayms(100);
	gpio_bit_reset(GPIOC, GPIO_PIN_10); //RST_L
	gpio_bit_set  (GPIOD, GPIO_PIN_2);  //PWDN_H
	hal_systick_delayms(400);
	gpio_bit_reset(GPIOD, GPIO_PIN_2);  //PWDN_L
	gpio_bit_set  (GPIOC, GPIO_PIN_10); //RST_H

	//
	//switch (cmd) {
	//}

	for (i = 0; i < sizeof(g_sGC0308_VGA)/2; i++) {
		 if (dci_byte_write(g_sGC0308_VGA[i][0], g_sGC0308_VGA[i][1]) != 0) {
		 }
	}
	hal_systick_delayms(200);

	return 0;
}

int hal_gc0308_enable(void)
{
	dci_enable();
	dci_interrupt_clear (DCI_INT_EF);
	dci_interrupt_enable(DCI_INT_EF);

	return 0;
}

int hal_gc0308_disable(void)
{
	dci_disable();
	dci_interrupt_clear  (DCI_INT_EF);
	dci_interrupt_disable(DCI_INT_EF);

	dma_flag_clear(DMA1, DMA_CH7, DMA_INTF_FTFIF);
	DMA_CHCNT   (DMA1, DMA_CH7) = g_buf_len / 4;
	DMA_CHM0ADDR(DMA1, DMA_CH7) = g_buf_addr;
	DMA_CHM1ADDR(DMA1, DMA_CH7) = g_buf_addr + g_buf_len;
	dma_channel_enable(DMA1, DMA_CH7);
	dci_capture_enable();

	return 0;
}

int hal_gc0308_finish(void)
{
	return (dma_flag_get(DMA1, DMA_CH7, DMA_INTF_FTFIF) == RESET) ? 0 : 1;
}

MODULE_OPS_S gd32f4xx_gc0308_ops = {
	.name   = "GD32F4XX GC0308",
	.type   = MOD_GC0308_TYPE,
	.init   = gc0308_mod_init,
	.deinit = gc0308_mod_deinit,
};
