#
# Makefile for ALSA
#

obj-$(CONFIG_SND_ARMAACI)	+= snd-aaci.o
snd-aaci-objs			:= aaci.o

obj-$(CONFIG_SND_PXA2XX_PCM)	+= snd-pxa2xx-pcm.o
snd-pxa2xx-pcm-objs		:= pxa2xx-pcm.o
obj-$(CONFIG_SND_FH81_PCM)	+= snd-fh81-pcm.o
snd-fh81-pcm-objs		:= fh81_pcm.o

obj-$(CONFIG_SND_PXA2XX_LIB)	+= snd-pxa2xx-lib.o
obj-$(CONFIG_SND_FH81_LIB)	+= snd-fh81-lib.o
snd-pxa2xx-lib-y		:= pxa2xx-pcm-lib.o
snd-fh81-lib-y		:= fh81-pcm-lib.o
snd-pxa2xx-lib-$(CONFIG_SND_PXA2XX_LIB_AC97)	+= pxa2xx-ac97-lib.o
snd-fh81-lib-$(CONFIG_SND_FH81_LIB_AC97)	+= fh81-ac97-lib.o
obj-$(CONFIG_SND_PXA2XX_AC97)	+= snd-pxa2xx-ac97.o
obj-$(CONFIG_SND_FH81_AC97)	+= snd-fh81-ac97.o
snd-pxa2xx-ac97-objs		:= pxa2xx-ac97.o
snd-fh81-ac97-objs		:= fh81-ac97.o