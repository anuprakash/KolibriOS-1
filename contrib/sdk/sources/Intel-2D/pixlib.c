
// -kr -i4 -ts4 -bls -bl -bli0

#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <pixlib2.h>
#include "pixdriver.h"

#include <kos32sys.h>

void* load_library(const char *name);


#define DISPLAY_VERSION         0x0200	/*      2.00     */

#define SRV_GETVERSION              0
#define SRV_GET_CAPS                3

#define BUFFER_SIZE(n) ((n)*sizeof(uint32_t))
#define __ALIGN_MASK(x,mask)  (((x)+(mask))&~(mask))
#define ALIGN(x,a)            __ALIGN_MASK(x,(typeof(x))(a)-1)

#define to_surface(x) (surface_t*)((x)->handle)

typedef struct
{
	uint32_t width;
	uint32_t height;
	void *data;
	uint32_t pitch;
	uint32_t bo;
	uint32_t bo_size;
	uint32_t flags;
} surface_t;

static uint32_t service;
static uint32_t hw_caps;
static struct pix_driver pix_driver;

uint32_t init_pixlib(uint32_t caps)
{
    void *lib;
    uint32_t (*drventry)(uint32_t service, struct pix_driver *driver);

	uint32_t api_version;
	ioctl_t io;

	if (service != 0)
		return caps & hw_caps;

	service = get_service("DISPLAY");
	if (service == 0)
		goto fail;

	io.handle = service;
	io.io_code = SRV_GETVERSION;
	io.input = NULL;
	io.inp_size = 0;
	io.output = &api_version;
	io.out_size = BUFFER_SIZE(1);

	if (call_service(&io) != 0)
		goto fail;

	if ((DISPLAY_VERSION > (api_version & 0xFFFF)) ||
		(DISPLAY_VERSION < (api_version >> 16)))
		goto fail;

    lib = load_library("intel-sna.drv");
    if(lib == 0)
        lib = load_library("intel-uxa.drv");
    if(lib == 0)
        goto fail;

    drventry = get_proc_address(lib, "DrvInit");

    if( drventry == NULL)
        goto fail;

    hw_caps = drventry(service, &pix_driver);

	if (hw_caps)
		printf("2D caps %s%s%s\n",
			   (hw_caps & HW_BIT_BLIT) != 0 ? "HW_BIT_BLIT " : "",
			   (hw_caps & HW_TEX_BLIT) != 0 ? "HW_TEX_BLIT " : "",
			   (hw_caps & HW_VID_BLIT) != 0 ? "HW_VID_BLIT " : "");

	return caps & hw_caps;

  fail:
	service = 0;
	return 0;
};

void done_pixlib()
{
	if (hw_caps != 0)
        pix_driver.fini();
};

int create_bitmap(bitmap_t * bitmap)
{
	uint32_t size, bo_size;
	uint32_t pitch, max_pitch;
	void *buffer;
	surface_t *sf;

	bitmap->handle = -1;
	bitmap->data = (void *) -1;
	bitmap->pitch = -1;

    if (bitmap->flags &= hw_caps)
       return pix_driver.create_bitmap(bitmap);

	pitch = ALIGN(bitmap->width * 4, 16);
	max_pitch = ALIGN(bitmap->max_width * 4, 16);

	size = ALIGN(pitch * bitmap->height, 4096);
	bo_size = ALIGN(max_pitch * bitmap->max_height, 4096);

	if (bo_size < size)
		bo_size = size;

	sf = malloc(sizeof(*sf));
	if (sf == NULL)
		return -1;

	buffer = user_alloc(bo_size);

	if (buffer == NULL)
	{
		free(sf);
		return -1;
	};

	sf->width = bitmap->width;
	sf->height = bitmap->height;
	sf->data = buffer;
	sf->pitch = pitch;
	sf->bo = 0;
	sf->bo_size = bo_size;
	sf->flags = bitmap->flags;

	bitmap->handle = (uint32_t) sf;

//    printf("create bitmap %p handle %p data %p  w %d h%d\n",
//            bitmap, bitmap->handle, bitmap->data, bitmap->width, bitmap->height);

	return 0;
};

int destroy_bitmap(bitmap_t * bitmap)
{
	surface_t *sf = to_surface(bitmap);

    if (sf->flags & hw_caps)
        return pix_driver.destroy_bitmap(bitmap);

	user_free(sf->data);
	free(sf);

	bitmap->handle = -1;
	bitmap->data = (void *) -1;
	bitmap->pitch = -1;

	return 0;
};

int lock_bitmap(bitmap_t * bitmap)
{
	surface_t *sf = to_surface(bitmap);

	if (bitmap->data != (void *) -1)
		return 0;

    if (sf->flags & hw_caps)
       return pix_driver.lock_bitmap(bitmap);

	bitmap->data = sf->data;
	bitmap->pitch = sf->pitch;

	return 0;
};

int blit_bitmap(bitmap_t * bitmap, int dst_x, int dst_y,
                int w, int h, int src_x, int src_y)
{
	struct blit_call bc;
	int ret;

	surface_t *sf = to_surface(bitmap);

    if (sf->flags & hw_caps & HW_BIT_BLIT)
        return pix_driver.blit(bitmap, 0, 0, dst_x, dst_y, w, h, src_x, src_y);

	bc.dstx     = dst_x;
	bc.dsty     = dst_y;
	bc.w        = w;
	bc.h        = h;
	bc.srcx     = 0;
	bc.srcy     = 0;
	bc.srcw     = w;
	bc.srch     = h;
	bc.stride   = sf->pitch;
	bc.bitmap   = sf->data;

	__asm__ __volatile__(
    "int $0x40":"=a"(ret):"a"(73), "b"(0x00),
	"c"(&bc):"memory");

	bitmap->data = (void *) -1;
	bitmap->pitch = -1;

	return ret;
};

int fplay_blit_bitmap(bitmap_t * bitmap, int dst_x, int dst_y, int w, int h)
{
	struct blit_call bc;
	int ret;

	surface_t *sf = to_surface(bitmap);

    if (sf->flags & hw_caps & HW_TEX_BLIT)
        return pix_driver.blit(bitmap, 1, 1, dst_x, dst_y, w, h, 0, 0);

	bc.dstx = dst_x;
	bc.dsty = dst_y;
	bc.w = w;
	bc.h = h;
	bc.srcx = 0;
	bc.srcy = 0;
	bc.srcw = w;
	bc.srch = h;
	bc.stride = sf->pitch;
	bc.bitmap = sf->data;

	__asm__ __volatile__(
    "int $0x40":"=a"(ret):"a"(73), "b"(0x00),
	"c"(&bc):"memory");

	bitmap->data = (void *) -1;
	bitmap->pitch = -1;

	return ret;
};

int resize_bitmap(bitmap_t * bitmap)
{
	uint32_t size;
	uint32_t pitch;

//    printf("%s\n", __FUNCTION__);

	surface_t *sf = to_surface(bitmap);

    if (sf->flags & hw_caps)
        return pix_driver.resize_bitmap(bitmap);

	pitch = ALIGN(bitmap->width * 4, 16);
	size = ALIGN(pitch * bitmap->height, 4096);

	bitmap->pitch = -1;
	bitmap->data = (void *) -1;

	if (size > sf->bo_size)
	{
		sf->data = user_realloc(sf->data, size);	/* grow buffer */
		if (sf->data == NULL)
			return -1;

		sf->bo_size = size;
	} else if (size < sf->bo_size)
		user_unmap(sf->data, size, sf->bo_size - size);	/* unmap unused pages */

	sf->width  = bitmap->width;
	sf->height = bitmap->height;
	sf->pitch  = pitch;

	return 0;
};

int sna_create_mask()
{
    return 0;
}

