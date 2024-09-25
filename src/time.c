#include "main.h"

time_ms_t time_in_mills(void) {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

float get_seconds_from_millis(time_ms_t millis) {
	if (millis < 1) {
		return 0;
	}
	return (1000.0 / (float) millis);
}

time_ms_t get_millis_from_seconds(float sec) {
	if (sec < 0.0001) {
		return 0;
	}
	return (time_ms_t) (1.0 / sec);
}
