/*****************************************************************************
*
*                                  chip.h
*
*	Copyright (c) 2010 Shanghai Fullhan Microelectronics Co., Ltd.
*						All Rights Reserved. Confidential.
*
*	File Description:
*		Chip definition. Include the base address of each module, memory
*		address, memory size
*
*	Modification History:
*
******************************************************************************/
#ifndef _CHIP_H_
#define _CHIP_H_

#ifdef CONFIG_ARCH_FH81
#include <mach/fh81.h>
#endif
#ifdef CONFIG_ARCH_LINBAO
#include <mach/linbao.h>
#endif

#endif
