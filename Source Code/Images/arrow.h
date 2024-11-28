#define ARROW_WIDTH (16)
#define ARROW_HEIGHT (16)
#define ARROW_BYTES_PER_PIXEL (2) /* 2:RGB16, 3:RGB, 4:RGBA */
#define ARROW_PIXEL_DATA ((unsigned char*) ARROW_pixel_data)
extern const unsigned char ARROW_pixel_data[16 * 16 * 2 + 1];

