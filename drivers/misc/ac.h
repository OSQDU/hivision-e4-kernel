#ifndef __AC_H
#define __AC_H

#define	FH_AUDIO_IOCTL_MEM_BASE	'M'
#define	AC_INIT_CAPTURE_MEM    	   _IOWR(FH_AUDIO_IOCTL_MEM_BASE, 0, int)
#define	AC_INIT_PLAYBACK_MEM         _IOWR(FH_AUDIO_IOCTL_MEM_BASE, 1, int)

#define	FH_AUDIO_IOCTL_PARAM_BASE	'P'
#define AC_SET_VOL                                   _IOWR(FH_AUDIO_IOCTL_PARAM_BASE, 0, int)
#define AC_SET_INPUT_MODE                _IOWR(FH_AUDIO_IOCTL_PARAM_BASE, 1, int)
#define AC_SET_OUTPUT_MODE            _IOWR(FH_AUDIO_IOCTL_PARAM_BASE, 2, int)

#define	FH_AUDIO_IOCTL_ENA_BASE	'E'
#define AC_AI_EN                                        _IOWR(FH_AUDIO_IOCTL_ENA_BASE, 0, int)
#define AC_AO_EN                                      _IOWR(FH_AUDIO_IOCTL_ENA_BASE, 1, int)
#define AC_AI_DISABLE                            _IOWR(FH_AUDIO_IOCTL_ENA_BASE, 2, int)
#define AC_AO_DISABLE       			         _IOWR(FH_AUDIO_IOCTL_ENA_BASE, 3, int)
#define AC_AI_PAUSE               					 _IOWR(FH_AUDIO_IOCTL_ENA_BASE, 4, int)
#define AC_AI_RESUME          				     _IOWR(FH_AUDIO_IOCTL_ENA_BASE, 5, int)
#define AC_AO_PAUSE              			     _IOWR(FH_AUDIO_IOCTL_ENA_BASE, 6, int)
#define AC_AO_RESUME          			     _IOWR(FH_AUDIO_IOCTL_ENA_BASE, 7, int)

enum io_select{
	mic_in = 0,
	line_in = 1,
	speaker_out = 2,
	line_out = 3,
};

struct fh_audio_cfg_arg{
	enum io_select io_type;
	int volume;
	int rate;
	int frame_bit;
	int channels;
	int buffer_size;
	int period_size;
};

#endif

