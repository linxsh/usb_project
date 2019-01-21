#include "inc/hal_gc0308.h"
#include "inc/hal_systick.h"
#include "src/gd32f4xx_dci.h"
#include "src/gd32f4xx_i2c.h"

void hal_gc0308_init(void)
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
}

void hal_gc0308_deinit(void)
{
}

int hal_gc0308_config_buffer(uint8_t *buf, uint32_t len)
{
	dma_single_data_parameter_struct dma;
	dci_parameter_struct             dci;

	i2c_deinit          (I2C1);
	i2c_clock_config    (I2C1, 100000, I2C_DTCY_2);
	i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0xFE);
	i2c_enable          (I2C1);
	i2c_ack_config      (I2C1, I2C_ACK_ENABLE);

	/* DCI DMA configuration */
	dma_deinit(DMA1, DMA_CH7);

	dma.periph_addr   = 0x50050028;
	dma.memory0_addr  = (uint32_t)(buf);
	dma.direction     = DMA_PERIPH_TO_MEMORY;
	dma.number        = len / 4;
	dma.periph_inc    = DMA_PERIPH_INCREASE_DISABLE;
	dma.memory_inc    = DMA_MEMORY_INCREASE_ENABLE;
	dma.priority      = DMA_PRIORITY_HIGH;
	dma.circular_mode = DMA_CIRCULAR_MODE_ENABLE;
	dma.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
	dma_single_data_mode_init       (DMA1, DMA_CH7, dma);
	dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI1);
	dma_switch_buffer_mode_config   (DMA1, DMA_CH7, ((uint32_t)(buf)) + len, DMA_MEMORY_0);
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
}

int hal_gc0308_enable(void)
{
}
