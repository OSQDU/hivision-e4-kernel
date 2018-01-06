#ifndef __FH81_AUDIO_MISC_H
#define __FH81_AUDIO_MISC_H
#include <linux/types.h>
#include <linux/poll.h>
#include <linux/ioctl.h>
#define	FH_AUDIO__IOCTL_BASE_BASE  	'B'
#define FH_AUDIO_WRITE_RATE    _IOWR(FH_AUDIO__IOCTL_BASE_BASE, 0, int)
#define FH_AUDIO_WRITE_CHANNELS    _IOWR(FH_AUDIO__IOCTL_BASE_BASE, 1, int)
#define  FH_AUDIO_WRITE_BIT   _IOWR(FH_AUDIO__IOCTL_BASE_BASE, 2, int)

#define	FH_AUDIO__IOCTL_MEM_BASE	'M'
#define	AC_INIT_MEM    	   _IOWR(FH_AUDIO__IOCTL_MEM_BASE, 0, int)

#define	FH_AUDIO__IOCTL_SEL_BASE	'S'
#define	AC_SET_MIC_IN	    _IOWR(FH_AUDIO__IOCTL_SEL_BASE, 0, int)
#define	AC_SET_SPK_OUT 	    _IOWR(FH_AUDIO__IOCTL_SEL_BASE, 1, int)
#define	FH_AUDIO__IOCTL_AIO_BASE	'A'
#define	AC_SET_AI_ATTR	   _IOWR(FH_AUDIO__IOCTL_AIO_BASE, 0, int)
#define AC_GET_AI_ATTR     _IOWR(FH_AUDIO__IOCTL_AIO_BASE, 1, int)
#define AC_SET_AO_ATTR     _IOWR(FH_AUDIO__IOCTL_AIO_BASE, 2, int)
#define AC_GET_AO_ATTR     _IOWR(FH_AUDIO__IOCTL_AIO_BASE, 3, int)


#define	FH_AUDIO__IOCTL_VOL_BASE	'V'
#define AC_SET_MIC_VOL      _IOWR(FH_AUDIO__IOCTL_VOL_BASE, 0, int)
#define AC_SET_LINEIN_VOL   _IOWR(FH_AUDIO__IOCTL_VOL_BASE, 1, int)
#define AC_SET_LINEOUT_VOL  _IOWR(FH_AUDIO__IOCTL_VOL_BASE, 2, int)
#define AC_SET_SPKOUT_VOL   _IOWR(FH_AUDIO__IOCTL_VOL_BASE, 3, int)
#define AC_GET_MIC_VOL      _IOWR(FH_AUDIO__IOCTL_VOL_BASE, 4, int)
#define AC_GET_LINEIN_VOL   _IOWR(FH_AUDIO__IOCTL_VOL_BASE, 5, int)
#define AC_GET_LINEOUT_VOL  _IOWR(FH_AUDIO__IOCTL_VOL_BASE, 6, int)
#define AC_GET_SPKOUT_VOL   _IOWR(FH_AUDIO__IOCTL_VOL_BASE, 7, int)

#define	FH_AUDIO__IOCTL_ENA_BASE	'E'
#define AC_AI_EN  _IOWR(FH_AUDIO__IOCTL_ENA_BASE, 0, int)
#define AC_AO_EN  _IOWR(FH_AUDIO__IOCTL_ENA_BASE, 1, int)

#define	FH_AUDIO__IOCTL_FRM_BASE	'F'
#define AC_AI_GET_FRAME      _IOWR(FH_AUDIO__IOCTL_FRM_BASE, 0, int)
#define AC_AI_RELEASE_FRAME  _IOWR(FH_AUDIO__IOCTL_FRM_BASE, 1, int)
#define AC_AO_SEND_FRAME     _IOWR(FH_AUDIO__IOCTL_FRM_BASE, 2, int)



#define I2S_FIFO_LEN_RX  40
#define I2S_FIFO_LEN_TX  40
typedef enum{
	AC_SR_8K   = 8000,
	AC_SR_16K  = 16000,
	AC_SR_441K = 44100,
	AC_SR_48K  = 48000,
	AC_SR_96K  = 96000,
} AC_SAMPLE_RATE_E;

typedef enum{
	AC_BW_8  = 8,  //8bit/sample
	AC_BW_16 = 16, //16bit/sample
	AC_BW_24 = 24, //24bit/sample
} AC_BIT_WIDTH_E;

typedef struct{
	unsigned int	vol;
	int 		mute;
} AC_VOL_CTRL_S;

typedef struct {
	AC_SAMPLE_RATE_E	sample_rate;
	AC_BIT_WIDTH_E		bit_width;
	unsigned int		frame_num;
} AC_AIO_ATTR_S;

typedef enum{
	ENCODE_PCM,
	ENCODE_ACC
} AC_ENCODE_TYPE_E;

typedef struct{
	AC_SAMPLE_RATE_E	sample_rate;
	AC_BIT_WIDTH_E		bit_width;
	AC_ENCODE_TYPE_E	encode_type;
	unsigned int		frame_no;
	unsigned int		len;
	unsigned long long	pts;
	unsigned char		*data;
	unsigned int        flag_done;
} AC_FRAME_S;

typedef struct
{
	unsigned int	base;
	void		*vbase;
	unsigned int	size;
	unsigned int	align;
} MEM_DESC;

enum audio_type{
	capture = 0,
	playback,
};
struct fh_i2s_dev{
	int irq;
	u8 name[10];
};
enum audio_state{
	normal = 0,
	xrun,
	stopping,
	running,
};
struct audio_ptr_t
{
	enum audio_state state;
	u8 * buffer_base;
	u8 * rw_buff;
	long size;
	int hw_ptr;
	int appl_ptr;
	u8 * mmap_addr;
};
struct fh_audio_cfg{
	int rate;
	int frame_bit;
	int channels;
	int buffer_size;
	int period_size;
	int start_threshold;
	int stop_threshold;
	struct audio_ptr_t capture;
	struct audio_ptr_t playback;
	wait_queue_head_t readqueue;
	wait_queue_head_t writequeue;
};
struct pwm_device {
	struct device *dev;
	struct list_head node;
	const char *label;
	unsigned int pwm_id;
};
//new












MEM_DESC mem_info;

typedef struct{
	 AC_FRAME_S *AC_FRAM_S_KE;
	unsigned int ptr;
	wait_queue_head_t readqueue;
	wait_queue_head_t writequeue;
}AC_FRAME_S_DRV;
MEM_DESC mem_info;
#endif
