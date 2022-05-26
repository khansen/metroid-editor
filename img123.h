typedef struct {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD          bmiColors[256];
    } IMAGEINFO123;

typedef struct {
void *bmptr;
void *bmhnd;
IMAGEINFO123 bminfo;
} Image123;
