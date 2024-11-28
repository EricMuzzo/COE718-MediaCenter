#define STARSHIP_WIDTH (320)
#define STARSHIP_HEIGHT (240)
#define STARSHIP_BYTES_PER_PIXEL (2) /* 2:RGB16, 3:RGB, 4:RGBA */
#define STARSHIP_PIXEL_DATA ((unsigned char*) STARSHIP_pixel_data)
extern const unsigned char STARSHIP_pixel_data[320 * 240 * 2 + 1];
