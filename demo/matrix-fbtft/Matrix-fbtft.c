#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <linux/fb.h>

#define	RGB(r,g,b)		(((r << 11) & 0xF800) | ((g << 6) & 0x07E0) | (( b & 0x1F)))

static int xres, yres, bpp;
static unsigned char *fbaddr;


/* Draw point (x, y) with specific RGB888 color (r,g,b) */
static void lcd_point_rgb(int x, int y, int r, int g, int b)
{
	unsigned int v = 0;
	unsigned char *p = fbaddr;

	if ((x < 0 || x >= xres) || (y < 0 || y >= yres)) {
		/* out of LCD display area */
		return;
	}

	switch (bpp) {
		case 16:
			v |= ((b >> 3) & 0x1f);
			v |= ((g >> 2) & 0x3f) << 5;
			v |= ((r >> 3) & 0x1f) << 11;

			p += ((y * xres) + x) * 2;
			*p++ = v;
			*p++ = v >> 8;
			break;

		case 18:
			v |= ((b >> 2) & 0x3f);
			v |= ((g >> 2) & 0x3f) << 6;
			v |= ((r >> 2) & 0x3f) << 12;

			p += ((y * xres) + x) * 3;
			*p++ = v;
			*p++ = v >> 8;
			*p++ = v >> 16;
			break;

		case 24:
			p += ((y * xres) + x) * 3;
			*p++ = b;
			*p++ = g;
			*p++ = r;
			break;

		case 32:
			p += ((y * xres) + x) * 4;
			*p++ = b;
			*p++ = g;
			*p++ = r;
			break;
	}
}

/* Draw line (x0, y0)~(x1, y1) with specific RGB888 color (r,g,b) */
static void lcd_drawline(int x0, int y0, int x1, int y1, int r, int g, int b)
{
	int dx, dy, dm, dn, m, n, k, u, v, l, sum;

	dx = x1 - x0;
	dy = y1 - y0;
	dm = 1;
	dn = 1;

	if (dx < 0) {
		dx = -dx;
		dm = -1;
	}
	if (dy < 0) {
		dy = -dy;
		dn = -1;
	}

	m = dx;
	n = dy;
	k = 1;
	u = x0;
	v = y0;

	if (dx < dy) {
		m = dy;
		n = dx;
		k = dm;
		dm = dn;
		dn = k;
		k = 0;
		u = y0;
		v = x0;
	}

	l = 0;
	sum = m;

	lcd_point_rgb(x0, y0, r, g, b);

	while (sum != 0) {
		sum = sum - 1;
		l = l + n;
		u = u + dm;
		if (l >= m) {
			v = v + dn;
		}
		l = l % m;

		if (k == 1) {
			lcd_point_rgb(u, v, r, g, b);
		} else {
			lcd_point_rgb(v, u, r, g, b);
		}
	}
}

static int set_color(int bpp, unsigned int color) 
{
	int i;

	if (bpp == 16) {
		ushort *p = (ushort *) fbaddr;
		for (i = 0; i < (xres * yres); i++) {
			*p++ = color;
		}
	} else if (bpp == 32) {
		unsigned int *p = (unsigned int *) fbaddr;
		for (i = 0; i < (xres * yres); i++) {
			*p++ = color;
		}
	} else {
		printf("Unsupported BPP %d\n", bpp);
	}

	return 0;
}


int main(int argc, char *argv[]) 
{
	char *devname;
	struct fb_fix_screeninfo fix;
	struct fb_var_screeninfo var;
	int fb;
	int fbsize;
	int color;

	if ((devname = getenv("MATRIX_FB")) == NULL) {
		devname = "/dev/fb0";
	}
	printf("FB device:%s\n", devname);
	if ((fb = open(devname, O_RDWR)) < 0) {
		printf("Open FB '%s' failed: %d, %s\n", devname, errno, strerror(errno));
		return -1;
	}

	memset(&var, 0, sizeof(struct fb_var_screeninfo));
	if (ioctl(fb, FBIOGET_VSCREENINFO, &var)) {
		printf("Get var screeninfo failed, %d\n", errno);
		return -1;
	}

	/* initialize global vars */
	xres	= var.xres;
	yres	= var.yres;
	bpp		= var.bits_per_pixel;

	printf("Current setting(s):\n");
	printf("  X-res, Y-res ........ %4d,  %4d\n", var.xres, var.yres);
	printf("  bpp ................. %4d\n", bpp);
	printf("  pixclock ............ %4d\n", var.pixclock);
	printf("  left,  right ........ %4d,  %4d\n", var.left_margin, var.right_margin);
	printf("  upper, lower ........ %4d,  %4d\n", var.upper_margin, var.lower_margin);
	printf("  hsync, vsync ........ %4d,  %4d\n", var.hsync_len, var.vsync_len);
	printf("\n");

	memset( &fix, 0, sizeof(struct fb_fix_screeninfo) );
	if (ioctl(fb, FBIOGET_FSCREENINFO, &fix)) {
		printf("Get fix screeninfo failed, %d\n", errno);
		return -1;
	}

	fbsize = fix.smem_len;
	fbaddr = (unsigned char *)mmap(0, fbsize, PROT_READ | (1 * PROT_WRITE), MAP_SHARED, fb, 0);

	if (fbaddr == MAP_FAILED) {
		printf("Cannot map Frame Buffer, fbsize is %u, errno = %d\n", fbsize, errno);
		return -1;
	}

	if (argc == 6) {                    // draw boarder
		int x0, y0, x1, y1, color;
		int r, g, b;

		x0 = atoi(argv[1]);
		y0 = atoi(argv[2]);
		x1 = atoi(argv[3]);
		y1 = atoi(argv[4]);
		color = strtol(argv[5], NULL, 0);

		r = (color >> 16) & 0xff;
		g = (color >>  8) & 0xff;
		b = (color >>  0) & 0xff;
		lcd_drawline(x0, y0, x1, y1, r, g, b);
		printf("Line (%d, %d) --> (%d, %d), RGB(%02x, %02x, %02x)\n", x0, y0, x1, y1, r, g, b);
	} else if (argc == 2) {             // draw pure color
		color = strtoul(argv[1], 0, 0);
		set_color(bpp, color);
	} else {
	    switch(bpp) {
	    case 16:
	        // blue, bit[0-4]
	        set_color(bpp, 0x1f);
	        printf("outputting blue\n");
	        sleep(1);
            // green, bit[5-10]
            set_color(bpp, 0x7e0);
            printf("outputting green\n");
            sleep(1);
	        // red, bit[11-15]
            set_color(bpp, 0x1f800);
            printf("outputting red\n");
            sleep(1);
	        break;
	    case 32:
            // blue, bit[0-7]
            set_color(bpp, 0xff);
            printf("outputting blue\n");
            sleep(1);
            // green, bit[8-15]
            set_color(bpp, 0xff00);
            printf("outputting green\n");
            sleep(1);
            // red, bit[16-23]
            set_color(bpp, 0xff0000);
            printf("outputting red\n");
            sleep(1);
            break;
	    default:
	        printf("unsupported bpp\n");
	        break;
	    }
	}

	close(fb); 
	munmap(fbaddr, fbsize);

	return 0;
}

