#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<unistd.h>
#include <sys/time.h>

#include "smi_leds.h"

#define NUM_LEDS 144
#define NUM_STRIPS 8
#define NUM_FRAMES 1000

// This code benchmarks how fast the smi_leds can push frames out. Far faster
// than the strips can handle them!

int main(int argc, char *argv[])
{
    struct  timeval t1, t2;
    uint8_t buffer[NUM_LEDS * NUM_STRIPS * 3];
    uint8_t *ptr;
    double  elapsed, per_frame;

    // initialize the smi_leds module, starting with a 25% brightness
    leds_init(NUM_LEDS, 25);

    memset(buffer, 128, NUM_LEDS * NUM_STRIPS * 3);

    gettimeofday(&t1, NULL);
    for(int i = 0; i < NUM_FRAMES; i++)
    {
        // Send the buffer to the SMI buffer
        leds_set(buffer);

        // Actually send them to the LEDs:
        leds_send();
    }

    gettimeofday(&t2, NULL);
    elapsed = (t2.tv_sec - t1.tv_sec);
    elapsed += (t2.tv_usec - t1.tv_usec) / 1000000.0;   // us to ms
    per_frame = elapsed / NUM_FRAMES;
    printf("%.4fs total, %.4fs per frame, %.2fFPS\n", elapsed, per_frame, 1.0 / per_frame);

    memset(buffer, 128, NUM_LEDS * NUM_STRIPS * 3);
    leds_set(buffer);
    leds_send();
}
