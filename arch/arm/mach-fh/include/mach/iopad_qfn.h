
#include "pinctrl.h"
#include "pinctrl_osdep.h"

PINCTRL_FUNC(MAC_RMII_CLK, 15, FUNC0);
PINCTRL_FUNC(SD1_CLK, 15, FUNC1);
PINCTRL_FUNC(MAC_REF_CLK, 16, FUNC0);

PINCTRL_FUNC(MAC_RXD_0, 22, FUNC0);
PINCTRL_FUNC(SD1_DATA_2, 22, FUNC1);
PINCTRL_FUNC(UART1_TX, 22, FUNC2);
PINCTRL_FUNC(GPIO68, 22, FUNC3);
PINCTRL_FUNC(MAC_RXD_1, 23, FUNC0);
PINCTRL_FUNC(SD1_DATA_3, 23, FUNC1);
PINCTRL_FUNC(UART1_RX, 23, FUNC2);
PINCTRL_FUNC(GPIO69, 23, FUNC3);
PINCTRL_FUNC(DA_I2S_CLK, 23, FUNC4);
PINCTRL_FUNC(PWM6, 23, FUNC5);

PINCTRL_FUNC(MAC_RXDV, 26, FUNC0);
PINCTRL_FUNC(SD1_CD, 26, FUNC1);
PINCTRL_FUNC(UART2_TX, 26, FUNC2);
PINCTRL_FUNC(GPIO39, 26, FUNC3);

PINCTRL_FUNC(MAC_TXD_0, 28, FUNC0);
PINCTRL_FUNC(SD1_DATA_0, 28, FUNC1);
PINCTRL_FUNC(GPIO70, 28, FUNC2);
PINCTRL_FUNC(MAC_TXD_1, 29, FUNC0);
PINCTRL_FUNC(SD1_DATA_1, 29, FUNC1);
PINCTRL_FUNC(GPIO71, 29, FUNC2);

PINCTRL_FUNC(MAC_TXEN, 32, FUNC0);
PINCTRL_FUNC(SD1_CMD_RSP, 32, FUNC1);
PINCTRL_FUNC(GPIO44, 32, FUNC2);
PINCTRL_FUNC(MAC_RXER, 33, FUNC0);
PINCTRL_FUNC(SD1_WP, 33, FUNC1);
PINCTRL_FUNC(UART2_RX, 33, FUNC2);
PINCTRL_FUNC(GPIO40, 33, FUNC3);

PINCTRL_FUNC(ARM_JTAG_TRSTN, 35, FUNC0);
PINCTRL_FUNC(SSI1_TXD, 35, FUNC1);
PINCTRL_FUNC(SSI2_TXD, 35, FUNC2);
PINCTRL_FUNC(PWM0, 35, FUNC3);
PINCTRL_FUNC(GPIO0, 35, FUNC4);
PINCTRL_FUNC(ARM_JTAG_TMS, 36, FUNC0);
PINCTRL_FUNC(SSI1_RXD, 36, FUNC1);
PINCTRL_FUNC(SSI2_RXD, 36, FUNC2);
PINCTRL_FUNC(PWM1, 36, FUNC3);
PINCTRL_FUNC(GPIO1, 36, FUNC4);
PINCTRL_FUNC(ARM_JTAG_TCK, 37, FUNC0);
PINCTRL_FUNC(SSI1_CLK, 37, FUNC1);
PINCTRL_FUNC(SSI2_CLK, 37, FUNC2);
PINCTRL_FUNC(PWM2, 37, FUNC3);
PINCTRL_FUNC(GPIO2, 37, FUNC4);
PINCTRL_FUNC(ARM_JTAG_TDI, 38, FUNC0);
PINCTRL_FUNC(SSI1_CSN_0, 38, FUNC1);
PINCTRL_FUNC(SSI2_CSN, 38, FUNC2);
PINCTRL_FUNC(PWM3, 38, FUNC3);
PINCTRL_FUNC(GPIO3, 38, FUNC4);
PINCTRL_FUNC(MAC_MDC, 38, FUNC5);
PINCTRL_FUNC(ARM_JTAG_TDO, 39, FUNC0);
PINCTRL_FUNC(SSI1_CSN_1, 39, FUNC1);
PINCTRL_FUNC(DA_I2S_WS, 39, FUNC2);
PINCTRL_FUNC(PWM4, 39, FUNC3);
PINCTRL_FUNC(GPIO4, 39, FUNC4);
PINCTRL_FUNC(MAC_MDIO, 39, FUNC5);

PINCTRL_FUNC(CIS_CLK, 47, FUNC0);
PINCTRL_FUNC(GPIO12, 47, FUNC1);
PINCTRL_FUNC(GPIO13, 48, FUNC1);
PINCTRL_FUNC(GPIO14, 49, FUNC1);
PINCTRL_FUNC(UART0_RX, 50, FUNC0);
PINCTRL_FUNC(I2C1_SDA, 50, FUNC1);
PINCTRL_FUNC(GPIO62, 50, FUNC2);
PINCTRL_FUNC(UART0_TX, 51, FUNC0);
PINCTRL_FUNC(I2C1_SCL, 51, FUNC1);
PINCTRL_FUNC(GPIO63, 51, FUNC2);
PINCTRL_FUNC(I2C0_SCL, 52, FUNC0);
PINCTRL_FUNC(GPIO16, 52, FUNC1);
PINCTRL_FUNC(I2C0_SDA, 53, FUNC0);
PINCTRL_FUNC(GPIO15, 53, FUNC1);

PINCTRL_FUNC(SSI0_CLK, 56, FUNC0);
PINCTRL_FUNC(SSI0_TXD, 57, FUNC0);
PINCTRL_FUNC(SSI0_CSN_0, 58, FUNC0);
PINCTRL_FUNC(GPIO54, 58, FUNC1);
PINCTRL_FUNC(SSI0_CSN_1, 59, FUNC0);
PINCTRL_FUNC(GPIO55, 59, FUNC1);
PINCTRL_FUNC(AC_MCLK, 59, FUNC2);
PINCTRL_FUNC(SSI3_CSN_1, 59, FUNC3);
PINCTRL_FUNC(PWM5, 59, FUNC4);
PINCTRL_FUNC(SSI0_RXD, 60, FUNC0);
PINCTRL_FUNC(SD0_CD, 61, FUNC0);
PINCTRL_FUNC(SSI3_CLK, 61, FUNC1);
PINCTRL_FUNC(SSI2_CLK, 61, FUNC2);
PINCTRL_FUNC(AD_I2S_CLK, 61, FUNC3);
PINCTRL_FUNC(I2S_CLK, 61, FUNC4);
PINCTRL_FUNC(GPIO46, 61, FUNC5);
PINCTRL_FUNC(SD0_WP, 62, FUNC0);
PINCTRL_FUNC(SSI3_TXD, 62, FUNC1);
PINCTRL_FUNC(SSI2_TXD, 62, FUNC2);
PINCTRL_FUNC(AD_I2S_WS, 62, FUNC3);
PINCTRL_FUNC(I2S_WS, 62, FUNC4);
PINCTRL_FUNC(GPIO47, 62, FUNC5);
PINCTRL_FUNC(SD0_CLK, 63, FUNC0);
PINCTRL_FUNC(GPIO50, 63, FUNC1);
PINCTRL_FUNC(ARC_JTAG_TRSTN, 63, FUNC2);
PINCTRL_FUNC(PAE_JTAG_TRSTN, 63, FUNC3);
PINCTRL_FUNC(SD0_CMD_RSP, 64, FUNC0);
PINCTRL_FUNC(GPIO51, 64, FUNC1);
PINCTRL_FUNC(ARC_JTAG_TMS, 64, FUNC2);
PINCTRL_FUNC(PAE_JTAG_TMS, 64, FUNC3);
PINCTRL_FUNC(SD0_DATA_0, 65, FUNC0);
PINCTRL_FUNC(GPIO52, 65, FUNC1);
PINCTRL_FUNC(ARC_JTAG_TCK, 65, FUNC2);
PINCTRL_FUNC(PAE_JTAG_TCK, 65, FUNC3);
PINCTRL_FUNC(SD0_DATA_1, 66, FUNC0);
PINCTRL_FUNC(GPIO53, 66, FUNC1);
PINCTRL_FUNC(ARC_JTAG_TDI, 66, FUNC2);
PINCTRL_FUNC(PAE_JTAG_TDI, 66, FUNC3);
PINCTRL_FUNC(SD0_DATA_2, 67, FUNC0);
PINCTRL_FUNC(SSI3_RXD, 67, FUNC1);
PINCTRL_FUNC(SSI2_RXD, 67, FUNC2);
PINCTRL_FUNC(DA_I2S_DO, 67, FUNC3);
PINCTRL_FUNC(I2S_DO, 67, FUNC4);
PINCTRL_FUNC(GPIO48, 67, FUNC5);
PINCTRL_FUNC(PWM7, 67, FUNC6);
PINCTRL_FUNC(SD0_DATA_3, 68, FUNC0);
PINCTRL_FUNC(SSI3_CSN_0, 68, FUNC1);
PINCTRL_FUNC(SSI2_CSN, 68, FUNC2);
PINCTRL_FUNC(AD_I2S_DI, 68, FUNC3);
PINCTRL_FUNC(I2S_DI, 68, FUNC4);
PINCTRL_FUNC(GPIO49, 68, FUNC5);
PINCTRL_FUNC(ARC_JTAG_TDO, 68, FUNC6);
PINCTRL_FUNC(PAE_JTAG_TDO, 68, FUNC7);
PINCTRL_FUNC(SD1_CD, 69, FUNC0);
PINCTRL_FUNC(GPIO64, 69, FUNC1);
PINCTRL_FUNC(DA_I2S_CLK, 69, FUNC2);

PINCTRL_FUNC(SD1_CLK, 71, FUNC0);
PINCTRL_FUNC(GPIO66, 71, FUNC1);
PINCTRL_FUNC(SD1_CMD_RSP, 72, FUNC0);
PINCTRL_FUNC(GPIO67, 72, FUNC1);
PINCTRL_FUNC(SD1_DATA_0, 73, FUNC0);
PINCTRL_FUNC(GPIO56, 73, FUNC1);

PINCTRL_MUX(AC_MCLK, 0, &PAD59_AC_MCLK);

PINCTRL_MUX(ACI2S_AD_CLK, 0, &PAD61_AD_I2S_CLK);
PINCTRL_MUX(ACI2S_AD_DI, 0, &PAD68_AD_I2S_DI);
PINCTRL_MUX(ACI2S_AD_WS, 0, &PAD62_AD_I2S_WS);
PINCTRL_MUX(ACI2S_DA_CLK, 0, &PAD23_DA_I2S_CLK, &PAD69_DA_I2S_CLK);
PINCTRL_MUX(ACI2S_DA_DO, 0, &PAD67_DA_I2S_DO);
PINCTRL_MUX(ACI2S_DA_WS, 0, &PAD39_DA_I2S_WS);

PINCTRL_MUX(DWI2S_CLK, 0, &PAD61_I2S_CLK);
PINCTRL_MUX(DWI2S_DI, 0, &PAD68_I2S_DI);
PINCTRL_MUX(DWI2S_DO, 0, &PAD67_I2S_DO);
PINCTRL_MUX(DWI2S_WS, 0, &PAD62_I2S_WS);

PINCTRL_MUX(ARCJTAG_TCK, 0, &PAD65_ARC_JTAG_TCK);
PINCTRL_MUX(ARCJTAG_TDI, 0, &PAD66_ARC_JTAG_TDI);
PINCTRL_MUX(ARCJTAG_TDO, 0, &PAD68_ARC_JTAG_TDO);
PINCTRL_MUX(ARCJTAG_TMS, 0, &PAD64_ARC_JTAG_TMS);
PINCTRL_MUX(ARCJTAG_TRSTN, 0, &PAD63_ARC_JTAG_TRSTN);

PINCTRL_MUX(ARMJTAG_TCK, 0, &PAD37_ARM_JTAG_TCK);
PINCTRL_MUX(ARMJTAG_TDI, 0, &PAD38_ARM_JTAG_TDI);
PINCTRL_MUX(ARMJTAG_TDO, 0, &PAD39_ARM_JTAG_TDO);
PINCTRL_MUX(ARMJTAG_TMS, 0, &PAD36_ARM_JTAG_TMS);
PINCTRL_MUX(ARMJTAG_TRSTN, 0, &PAD35_ARM_JTAG_TRSTN);

PINCTRL_MUX(PAEJTAG_TCK, 0, &PAD65_PAE_JTAG_TCK);
PINCTRL_MUX(PAEJTAG_TDI, 0, &PAD66_PAE_JTAG_TDI);
PINCTRL_MUX(PAEJTAG_TDO, 0, &PAD68_PAE_JTAG_TDO);
PINCTRL_MUX(PAEJTAG_TMS, 0, &PAD64_PAE_JTAG_TMS);
PINCTRL_MUX(PAEJTAG_TRSTN, 0, &PAD63_PAE_JTAG_TRSTN);

PINCTRL_MUX(CIS_CLK, 0, &PAD47_CIS_CLK);

PINCTRL_MUX(I2C0_SCL, 0, &PAD52_I2C0_SCL);
PINCTRL_MUX(I2C0_SDA, 0, &PAD53_I2C0_SDA);

PINCTRL_MUX(I2C1_SCL, 0, &PAD51_I2C1_SCL);
PINCTRL_MUX(I2C1_SDA, 0, &PAD50_I2C1_SDA);

PINCTRL_MUX(MAC_REF_CLK, 0, &PAD16_MAC_REF_CLK);
PINCTRL_MUX(MAC_RMII_CLK, 0, &PAD15_MAC_RMII_CLK);
PINCTRL_MUX(MAC_MDC, 0, &PAD38_MAC_MDC);
PINCTRL_MUX(MAC_MDIO, 0, &PAD39_MAC_MDIO);
PINCTRL_MUX(MAC_RXD_0, 0, &PAD22_MAC_RXD_0);
PINCTRL_MUX(MAC_RXD_1, 0, &PAD23_MAC_RXD_1);
PINCTRL_MUX(MAC_RXDV, 0, &PAD26_MAC_RXDV);
PINCTRL_MUX(MAC_RXER, 0, &PAD33_MAC_RXER);
PINCTRL_MUX(MAC_TXD_0, 0, &PAD28_MAC_TXD_0);
PINCTRL_MUX(MAC_TXD_1, 0, &PAD29_MAC_TXD_1);
PINCTRL_MUX(MAC_TXEN, 0, &PAD32_MAC_TXEN);

PINCTRL_MUX(PWM0, 0, &PAD35_PWM0);
PINCTRL_MUX(PWM1, 0, &PAD36_PWM1);
PINCTRL_MUX(PWM2, 0, &PAD37_PWM2);
PINCTRL_MUX(PWM3, 0, &PAD38_PWM3);
PINCTRL_MUX(PWM4, 0, &PAD39_PWM4);
PINCTRL_MUX(PWM5, 0, &PAD59_PWM5);
PINCTRL_MUX(PWM6, 0, &PAD23_PWM6);
PINCTRL_MUX(PWM7, 0, &PAD67_PWM7);

PINCTRL_MUX(SD0_CLK, 0, &PAD63_SD0_CLK);
PINCTRL_MUX(SD0_CD, 0, &PAD61_SD0_CD);
PINCTRL_MUX(SD0_CMD_RSP, 0, &PAD64_SD0_CMD_RSP);
PINCTRL_MUX(SD0_WP, 0, &PAD62_SD0_WP);
PINCTRL_MUX(SD0_DATA_0, 0, &PAD65_SD0_DATA_0);
PINCTRL_MUX(SD0_DATA_1, 0, &PAD66_SD0_DATA_1);
PINCTRL_MUX(SD0_DATA_2, 0, &PAD67_SD0_DATA_2);
PINCTRL_MUX(SD0_DATA_3, 0, &PAD68_SD0_DATA_3);

PINCTRL_MUX(SD1_CLK, 1, &PAD15_SD1_CLK, &PAD71_SD1_CLK);
PINCTRL_MUX(SD1_CD, 1, &PAD26_SD1_CD, &PAD69_SD1_CD);
PINCTRL_MUX(SD1_CMD_RSP, 1, &PAD32_SD1_CMD_RSP, &PAD72_SD1_CMD_RSP);
PINCTRL_MUX(SD1_WP, 1, &PAD33_SD1_WP);
PINCTRL_MUX(SD1_DATA_0, 1, &PAD28_SD1_DATA_0, &PAD73_SD1_DATA_0);
PINCTRL_MUX(SD1_DATA_1, 1, &PAD29_SD1_DATA_1);
PINCTRL_MUX(SD1_DATA_2, 1, &PAD22_SD1_DATA_2);
PINCTRL_MUX(SD1_DATA_3, 1, &PAD23_SD1_DATA_3);

PINCTRL_MUX(SSI0_CLK, 0, &PAD56_SSI0_CLK);
PINCTRL_MUX(SSI0_RXD, 0, &PAD60_SSI0_RXD);
PINCTRL_MUX(SSI0_TXD, 0, &PAD57_SSI0_TXD);
PINCTRL_MUX(SSI0_CSN_0, 0, &PAD58_SSI0_CSN_0);
PINCTRL_MUX(SSI0_CSN_1, 0, &PAD59_SSI0_CSN_1);

PINCTRL_MUX(SSI1_CLK, 0, &PAD37_SSI1_CLK);
PINCTRL_MUX(SSI1_RXD, 0, &PAD36_SSI1_RXD);
PINCTRL_MUX(SSI1_TXD, 0, &PAD35_SSI1_TXD);
PINCTRL_MUX(SSI1_CSN_0, 0, &PAD38_SSI1_CSN_0);
PINCTRL_MUX(SSI1_CSN_1, 0, &PAD39_SSI1_CSN_1);

PINCTRL_MUX(SSI2_CLK, 0, &PAD37_SSI2_CLK, &PAD61_SSI2_CLK);
PINCTRL_MUX(SSI2_RXD, 0, &PAD36_SSI2_RXD, &PAD67_SSI2_RXD);
PINCTRL_MUX(SSI2_TXD, 0, &PAD35_SSI2_TXD, &PAD62_SSI2_TXD);
PINCTRL_MUX(SSI2_CSN, 0, &PAD38_SSI2_CSN, &PAD68_SSI2_CSN);

PINCTRL_MUX(SSI3_CLK, 0, &PAD61_SSI3_CLK);
PINCTRL_MUX(SSI3_RXD, 0, &PAD67_SSI3_RXD);
PINCTRL_MUX(SSI3_TXD, 0, &PAD62_SSI3_TXD);
PINCTRL_MUX(SSI3_CSN_0, 0, &PAD68_SSI3_CSN_0);
PINCTRL_MUX(SSI3_CSN_1, 0, &PAD59_SSI3_CSN_1);

PINCTRL_MUX(UART0_RX, 0, &PAD50_UART0_RX);
PINCTRL_MUX(UART0_TX, 0, &PAD51_UART0_TX);
PINCTRL_MUX(UART1_RX, 0, &PAD23_UART1_RX);
PINCTRL_MUX(UART1_TX, 0, &PAD22_UART1_TX);
PINCTRL_MUX(UART2_RX, 0, &PAD33_UART2_RX);
PINCTRL_MUX(UART2_TX, 0, &PAD26_UART2_TX);

PINCTRL_MUX(GPIO0, 0, &PAD35_GPIO0);
PINCTRL_MUX(GPIO1, 0, &PAD36_GPIO1);
PINCTRL_MUX(GPIO2, 0, &PAD37_GPIO2);
PINCTRL_MUX(GPIO3, 0, &PAD38_GPIO3);
PINCTRL_MUX(GPIO4, 0, &PAD39_GPIO4);

PINCTRL_MUX(GPIO12, 0, &PAD47_GPIO12);
PINCTRL_MUX(GPIO13, 0, &PAD48_GPIO13);
PINCTRL_MUX(GPIO14, 0, &PAD49_GPIO14);
PINCTRL_MUX(GPIO15, 0, &PAD53_GPIO15);
PINCTRL_MUX(GPIO16, 0, &PAD52_GPIO16);

PINCTRL_MUX(GPIO39, 0, &PAD26_GPIO39);
PINCTRL_MUX(GPIO40, 0, &PAD33_GPIO40);

PINCTRL_MUX(GPIO44, 0, &PAD32_GPIO44);

PINCTRL_MUX(GPIO46, 0, &PAD61_GPIO46);
PINCTRL_MUX(GPIO47, 0, &PAD62_GPIO47);
PINCTRL_MUX(GPIO48, 0, &PAD67_GPIO48);
PINCTRL_MUX(GPIO49, 0, &PAD68_GPIO49);
PINCTRL_MUX(GPIO50, 0, &PAD63_GPIO50);
PINCTRL_MUX(GPIO51, 0, &PAD64_GPIO51);
PINCTRL_MUX(GPIO52, 0, &PAD65_GPIO52);
PINCTRL_MUX(GPIO53, 0, &PAD66_GPIO53);
PINCTRL_MUX(GPIO54, 0, &PAD58_GPIO54);
PINCTRL_MUX(GPIO55, 0, &PAD59_GPIO55);
PINCTRL_MUX(GPIO56, 0, &PAD73_GPIO56);

PINCTRL_MUX(GPIO64, 0, &PAD69_GPIO64);

PINCTRL_MUX(GPIO66, 0, &PAD71_GPIO66);
PINCTRL_MUX(GPIO67, 0, &PAD72_GPIO67);
PINCTRL_MUX(GPIO68, 0, &PAD22_GPIO68);
PINCTRL_MUX(GPIO69, 0, &PAD23_GPIO69);
PINCTRL_MUX(GPIO70, 0, &PAD28_GPIO70);
PINCTRL_MUX(GPIO71, 0, &PAD29_GPIO71);


PINCTRL_DEVICE(AC, 1, &MUX_AC_MCLK);
PINCTRL_DEVICE(ACI2S, 6, &MUX_ACI2S_AD_CLK, &MUX_ACI2S_AD_DI, &MUX_ACI2S_AD_WS,
        &MUX_ACI2S_DA_CLK, &MUX_ACI2S_DA_DO, &MUX_ACI2S_DA_WS);
PINCTRL_DEVICE(DWI2S, 4, &MUX_DWI2S_CLK, &MUX_DWI2S_DI, &MUX_DWI2S_DO, &MUX_DWI2S_WS);
PINCTRL_DEVICE(ARCJTAG, 5, &MUX_ARCJTAG_TCK, &MUX_ARCJTAG_TDI, &MUX_ARCJTAG_TDO,
        &MUX_ARCJTAG_TMS, &MUX_ARCJTAG_TRSTN);
PINCTRL_DEVICE(ARMJTAG, 5, &MUX_ARMJTAG_TCK, &MUX_ARMJTAG_TDI, &MUX_ARMJTAG_TDO,
        &MUX_ARMJTAG_TMS, &MUX_ARMJTAG_TRSTN);
PINCTRL_DEVICE(PAEJTAG, 5, &MUX_PAEJTAG_TCK, &MUX_PAEJTAG_TDI, &MUX_PAEJTAG_TDO,
        &MUX_PAEJTAG_TMS, &MUX_PAEJTAG_TRSTN);
PINCTRL_DEVICE(CIS, 1, &MUX_CIS_CLK);
PINCTRL_DEVICE(I2C0, 2, &MUX_I2C0_SCL, &MUX_I2C0_SDA);
PINCTRL_DEVICE(I2C1, 2, &MUX_I2C1_SCL, &MUX_I2C1_SDA);
PINCTRL_DEVICE(RMII, 11, &MUX_MAC_REF_CLK, &MUX_MAC_RMII_CLK,
        &MUX_MAC_MDC, &MUX_MAC_MDIO, &MUX_MAC_RXD_0,
        &MUX_MAC_RXD_1, &MUX_MAC_RXDV, &MUX_MAC_RXER, &MUX_MAC_TXD_0,
        &MUX_MAC_TXD_1, &MUX_MAC_TXEN);
PINCTRL_DEVICE(PWM0, 1, &MUX_PWM0);
PINCTRL_DEVICE(PWM1, 1, &MUX_PWM1);
PINCTRL_DEVICE(PWM2, 1, &MUX_PWM2);
PINCTRL_DEVICE(PWM3, 1, &MUX_PWM3);
PINCTRL_DEVICE(PWM4, 1, &MUX_PWM4);
PINCTRL_DEVICE(PWM5, 1, &MUX_PWM5);
PINCTRL_DEVICE(PWM6, 1, &MUX_PWM6);
PINCTRL_DEVICE(PWM7, 1, &MUX_PWM7);
PINCTRL_DEVICE(SD0, 8, &MUX_SD0_CLK, &MUX_SD0_CD, &MUX_SD0_CMD_RSP,
        &MUX_SD0_WP, &MUX_SD0_DATA_0, &MUX_SD0_DATA_1, &MUX_SD0_DATA_2, &MUX_SD0_DATA_3);
PINCTRL_DEVICE(SD1, 8, &MUX_SD1_CLK, &MUX_SD1_CD, &MUX_SD1_CMD_RSP,
        &MUX_SD1_WP, &MUX_SD1_DATA_0, &MUX_SD1_DATA_1, &MUX_SD1_DATA_2, &MUX_SD1_DATA_3);
PINCTRL_DEVICE(SSI0, 5, &MUX_SSI0_CLK, &MUX_SSI0_RXD, &MUX_SSI0_TXD,
        &MUX_GPIO54, &MUX_GPIO55);
PINCTRL_DEVICE(SSI1, 5, &MUX_SSI1_CLK, &MUX_SSI1_RXD, &MUX_SSI1_TXD,
        &MUX_SSI1_CSN_0, &MUX_SSI1_CSN_1);
PINCTRL_DEVICE(SSI2, 4, &MUX_SSI2_CLK, &MUX_SSI2_RXD, &MUX_SSI2_TXD,
        &MUX_SSI2_CSN);
PINCTRL_DEVICE(SSI3, 5, &MUX_SSI3_CLK, &MUX_SSI3_RXD, &MUX_SSI3_TXD,
        &MUX_SSI3_CSN_0, &MUX_SSI3_CSN_1);
PINCTRL_DEVICE(UART0, 2, &MUX_UART0_RX, &MUX_UART0_TX);
PINCTRL_DEVICE(UART1, 2, &MUX_UART1_RX, &MUX_UART1_TX);
PINCTRL_DEVICE(UART2, 2, &MUX_UART2_RX, &MUX_UART2_TX);
PINCTRL_DEVICE(GPIO0, 1, &MUX_GPIO0);
PINCTRL_DEVICE(GPIO1, 1, &MUX_GPIO1);
PINCTRL_DEVICE(GPIO2, 1, &MUX_GPIO2);
PINCTRL_DEVICE(GPIO3, 1, &MUX_GPIO3);
PINCTRL_DEVICE(GPIO4, 1, &MUX_GPIO4);

PINCTRL_DEVICE(GPIO12, 1, &MUX_GPIO12);
PINCTRL_DEVICE(GPIO13, 1, &MUX_GPIO13);
PINCTRL_DEVICE(GPIO14, 1, &MUX_GPIO14);
PINCTRL_DEVICE(GPIO15, 1, &MUX_GPIO15);
PINCTRL_DEVICE(GPIO16, 1, &MUX_GPIO16);

PINCTRL_DEVICE(GPIO39, 1, &MUX_GPIO39);
PINCTRL_DEVICE(GPIO40, 1, &MUX_GPIO40);

PINCTRL_DEVICE(GPIO44, 1, &MUX_GPIO44);

PINCTRL_DEVICE(GPIO46, 1, &MUX_GPIO46);
PINCTRL_DEVICE(GPIO47, 1, &MUX_GPIO47);
PINCTRL_DEVICE(GPIO48, 1, &MUX_GPIO48);
PINCTRL_DEVICE(GPIO49, 1, &MUX_GPIO49);
PINCTRL_DEVICE(GPIO50, 1, &MUX_GPIO50);
PINCTRL_DEVICE(GPIO51, 1, &MUX_GPIO51);
PINCTRL_DEVICE(GPIO52, 1, &MUX_GPIO52);
PINCTRL_DEVICE(GPIO53, 1, &MUX_GPIO53);
PINCTRL_DEVICE(GPIO54, 1, &MUX_GPIO54);
PINCTRL_DEVICE(GPIO55, 1, &MUX_GPIO55);
PINCTRL_DEVICE(GPIO56, 1, &MUX_GPIO56);

PINCTRL_DEVICE(GPIO64, 1, &MUX_GPIO64);

PINCTRL_DEVICE(GPIO66, 1, &MUX_GPIO66);
PINCTRL_DEVICE(GPIO67, 1, &MUX_GPIO67);
PINCTRL_DEVICE(GPIO68, 1, &MUX_GPIO68);
PINCTRL_DEVICE(GPIO69, 1, &MUX_GPIO69);
PINCTRL_DEVICE(GPIO70, 1, &MUX_GPIO70);
PINCTRL_DEVICE(GPIO71, 1, &MUX_GPIO71);


void fh_pinctrl_init_devicelist(OS_LIST *list)
{
    OS_LIST_EMPTY(list);

    PINCTRL_ADD_DEVICE(AC);
    PINCTRL_ADD_DEVICE(ACI2S);
    PINCTRL_ADD_DEVICE(DWI2S);
    PINCTRL_ADD_DEVICE(ARCJTAG);
    PINCTRL_ADD_DEVICE(ARMJTAG);
    PINCTRL_ADD_DEVICE(PAEJTAG);
    PINCTRL_ADD_DEVICE(CIS);
    PINCTRL_ADD_DEVICE(I2C0);
    PINCTRL_ADD_DEVICE(I2C1);

    PINCTRL_ADD_DEVICE(RMII);
    PINCTRL_ADD_DEVICE(PWM0);
    PINCTRL_ADD_DEVICE(PWM1);
    PINCTRL_ADD_DEVICE(PWM2);
    PINCTRL_ADD_DEVICE(PWM3);
    PINCTRL_ADD_DEVICE(PWM4);
    PINCTRL_ADD_DEVICE(PWM5);
    PINCTRL_ADD_DEVICE(PWM6);
    PINCTRL_ADD_DEVICE(PWM7);
    PINCTRL_ADD_DEVICE(SD0);
    PINCTRL_ADD_DEVICE(SD1);
    PINCTRL_ADD_DEVICE(SSI0);
    PINCTRL_ADD_DEVICE(SSI1);
    PINCTRL_ADD_DEVICE(SSI2);
    PINCTRL_ADD_DEVICE(SSI3);
    PINCTRL_ADD_DEVICE(UART0);
    PINCTRL_ADD_DEVICE(UART1);
    PINCTRL_ADD_DEVICE(UART2);
    PINCTRL_ADD_DEVICE(GPIO0);
    PINCTRL_ADD_DEVICE(GPIO1);
    PINCTRL_ADD_DEVICE(GPIO2);
    PINCTRL_ADD_DEVICE(GPIO3);
    PINCTRL_ADD_DEVICE(GPIO4);

    PINCTRL_ADD_DEVICE(GPIO12);
    PINCTRL_ADD_DEVICE(GPIO13);
    PINCTRL_ADD_DEVICE(GPIO14);
    PINCTRL_ADD_DEVICE(GPIO15);
    PINCTRL_ADD_DEVICE(GPIO16);

    PINCTRL_ADD_DEVICE(GPIO39);
    PINCTRL_ADD_DEVICE(GPIO40);

    PINCTRL_ADD_DEVICE(GPIO44);

    PINCTRL_ADD_DEVICE(GPIO46);
    PINCTRL_ADD_DEVICE(GPIO47);
    PINCTRL_ADD_DEVICE(GPIO48);
    PINCTRL_ADD_DEVICE(GPIO49);
    PINCTRL_ADD_DEVICE(GPIO50);
    PINCTRL_ADD_DEVICE(GPIO51);
    PINCTRL_ADD_DEVICE(GPIO52);
    PINCTRL_ADD_DEVICE(GPIO53);
    PINCTRL_ADD_DEVICE(GPIO54);
    PINCTRL_ADD_DEVICE(GPIO55);
    PINCTRL_ADD_DEVICE(GPIO56);

    PINCTRL_ADD_DEVICE(GPIO64);

    PINCTRL_ADD_DEVICE(GPIO66);
    PINCTRL_ADD_DEVICE(GPIO67);
    PINCTRL_ADD_DEVICE(GPIO68);
    PINCTRL_ADD_DEVICE(GPIO69);
    PINCTRL_ADD_DEVICE(GPIO70);
    PINCTRL_ADD_DEVICE(GPIO71);
}



char* fh_pinctrl_selected_devices[] =
{
    "CIS", "RMII", "UART0",
    "I2C0", "SSI0", "SD0",
    "GPIO0", "GPIO1", "GPIO2", "GPIO13", "GPIO14",
    "GPIO64", "GPIO66", "GPIO67"
};
