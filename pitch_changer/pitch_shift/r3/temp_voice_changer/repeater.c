
#include <alsa/asoundlib.h>

snd_pcm_t *pcm_c;			//capture device
snd_pcm_t *pcm_p;			//playback device

char *name = "default";			//name of device

snd_pcm_hw_params_t *params_c, *params_p;


int
main(int argc, char *argv[])
{
	int dir, res, size;
	unsigned int val;
	float *ibuff;
	
	snd_pcm_uframes_t frames;
	
	res = snd_pcm_open(&pcm_c, name, SND_PCM_STREAM_CAPTURE, 0);
	if (res != 0) {
		printf("open for capture err\n");
		exit(1);
	}
	
	res = snd_pcm_open(&pcm_p, name, SND_PCM_STREAM_PLAYBACK, 0);
	if (res != 0) {
		printf("open for playback err\n");
		exit(1);
	}
	
	snd_pcm_hw_params_alloca(&params_c);
	
	snd_pcm_hw_params_any(pcm_c, params_c);

	snd_pcm_hw_params_set_access(pcm_c, params_c,
                      SND_PCM_ACCESS_RW_INTERLEAVED);

	snd_pcm_hw_params_set_format(pcm_c, params_c, SND_PCM_FORMAT_FLOAT_LE);
	
	snd_pcm_hw_params_set_channels(pcm_c, params_c, 1);

	val = 44100;
	snd_pcm_hw_params_set_rate_near(pcm_c, params_c, &val, &dir);
	
	frames = 1024;
	snd_pcm_hw_params_set_period_size_near(pcm_c, params_c, &frames, &dir);
	
	res = snd_pcm_hw_params(pcm_c, params_c);
	if (res < 0) {
		fprintf(stderr, "unable to set hw params(: %s\n", snd_strerror(res));
		exit(1);
	}
	
	snd_pcm_hw_params_get_period_size(params_c, &frames, &dir);
	size = frames * 4;

	ibuff = malloc(size);

	snd_pcm_hw_params_alloca(&params_p);
	snd_pcm_hw_params_copy(params_p, params_c);

	res = snd_pcm_hw_params(pcm_p, params_p);
	if (res < 0) {
		fprintf(stderr, "unable to set hw params(: %s\n", snd_strerror(res));
		exit(1);
	}
	
	snd_pcm_prepare(pcm_c);
	snd_pcm_prepare(pcm_p);
	
	int loop = 200;
	while (loop-- != 0) {
		
		res = snd_pcm_readi(pcm_c, ibuff, frames);

		if (res == -EPIPE) {
			// EPIPE means overrun
			fprintf(stderr, "overrun occurred\n");
			snd_pcm_prepare(pcm_c);
		} else if (res < 0)
			fprintf(stderr, "error from read: %s\n", snd_strerror(res));
		else if (res != (int)frames)
			fprintf(stderr, "short read, read %d frames\n", res);

		res = snd_pcm_writei(pcm_p, ibuff, frames);

		if (res == -EPIPE) {
			// EPIPE means underrun
			fprintf(stderr, "underrun occurred\n");
			snd_pcm_prepare(pcm_p);
		} else if (res < 0)
			fprintf(stderr, "error from writei: %s\n", snd_strerror(res));
		else if (res != (int)frames)
			fprintf(stderr, "short write, write %d frames\n", res);
		printf("loop %d\n", loop);
	}

	printf("the end\n");
	snd_pcm_drain(pcm_c);
	snd_pcm_drain(pcm_p);

	snd_pcm_close(pcm_c);
	snd_pcm_close(pcm_p);
	
	free(ibuff);

	return 0;
}
