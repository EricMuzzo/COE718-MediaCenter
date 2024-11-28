#define FALCON9_WIDTH (320)
#define FALCON9_HEIGHT (240)
#define FALCON9_BYTES_PER_PIXEL (2) /* 2:RGB16, 3:RGB, 4:RGBA */
#define FALCON9_PIXEL_DATA ((unsigned char*) FALCON9_pixel_data)
extern const unsigned char FALCON9_pixel_data[320 * 240 * 2 + 1];
