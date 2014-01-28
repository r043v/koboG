/* this file is a part of Gdl 0.4f and was writen by rov, http://procvor.free.fr */

//-----------------------------------------------------------------------

//  pixel function - 8bit

//-----------------------------------------------------------------------

unsigned char GetPixel(short x, short y, struct gfx * gfx, unsigned char img_id)
{
	unsigned char *p_buf = (unsigned char *)gfx->img[img_id] ;
 	return *(p_buf + x*gfx->size_y + (gfx->size_y-y));
}

//-----------------------------------------------------------------------

void SetPixel(short x, short y, struct gfx * gfx, unsigned char img_id, unsigned char color)
{
	unsigned char *p_buf = (unsigned char *)gfx->img[img_id] ;
 	*(p_buf + x*gfx->size_y + (gfx->size_y-y)) = color ;
}

//-----------------------------------------------------------------------

unsigned char GetAnimatedPixel(short x, short y, struct gfx * gfx,unsigned char anim)
{
	unsigned char *p_buf = (unsigned char *)gfx->img[gfx->animation[anim].curent_frame] ;
 	return *(p_buf + x*gfx->size_y + (gfx->size_y-y));
}

//-----------------------------------------------------------------------

void PutPixel(unsigned char color, short x, short y)
{
	unsigned char *p_buf = (unsigned char *) (gpDraw[nflip].ptbuffer);
	*(p_buf + x*240 + (239-y)) = color ;
}

//-----------------------------------------------------------------------

void PutPixel_transp(unsigned char color, short x, short y, unsigned char transp_color)
{
	unsigned char *p_buf = (unsigned char *) (gpDraw[nflip].ptbuffer);
	if(transp_color != color) *(p_buf + x*240 + (239-y)) = color ;
}

//-----------------------------------------------------------------------

unsigned char get_screen_pixel(short x, short y)
{
	unsigned char *screen = (unsigned char *) (gpDraw[nflip].ptbuffer);

	return *(screen + x*240 + (239-y)) ;
}

//-----------------------------------------------------------------------
//pas de blur en 8bit, effet chelou :D
void blur_pixel(short x, short y)
{
	unsigned char color = ( get_screen_pixel(x-1,y) + get_screen_pixel(x+1,y) + get_screen_pixel(x,y-1) + get_screen_pixel(x,y+1) ) >>2 ;
	PutPixel(color,x,y) ;
}

//-----------------------------------------------------------------------

// pixel function - 16bit

//-----------------------------------------------------------------------

unsigned short GetPixel_16(short x, short y, struct gfx * gfx, unsigned char img_id)
{
	unsigned short *p_buf = (unsigned short *)gfx->img[img_id] ;
 	return *(p_buf + x*gfx->size_y + (gfx->size_y-y));
}

//-----------------------------------------------------------------------

void SetPixel_16(short x, short y, struct gfx * gfx, unsigned char img_id, unsigned short color)
{
	unsigned short *p_buf = (unsigned short *)gfx->img[img_id] ;

 	*(p_buf + x*gfx->size_y + (gfx->size_y-y)) = color ;
}

//-----------------------------------------------------------------------

unsigned short GetAnimatedPixel_16(short x, short y, struct gfx * gfx,unsigned char anim)
{
	unsigned short *p_buf = (unsigned short *)gfx->img[gfx->animation[anim].curent_frame] ;
 	return *(p_buf + x*gfx->size_y + (gfx->size_y-y));
}

//-----------------------------------------------------------------------

void PutPixel_16(unsigned short color, short x, short y)
{
	unsigned short *p_buf = (unsigned short *) (gpDraw[nflip].ptbuffer);
	*(p_buf + x*240 + (239-y)) = color ;
}

//-----------------------------------------------------------------------

void PutPixel_transp_16(unsigned short color, short x, short y, unsigned short transp_color)
{
	unsigned short *p_buf = (unsigned short *) (gpDraw[nflip].ptbuffer);
	if(transp_color != color) *(p_buf + x*240 + (239-y)) = color ;
}

//-----------------------------------------------------------------------

unsigned short get_screen_pixel_16(short x, short y)
{
	unsigned short *screen = (unsigned short *) (gpDraw[nflip].ptbuffer);

	return *(screen + x*240 + (239-y)) ;
}

//-----------------------------------------------------------------------

void blur_pixel_16(short x, short y)
{
	unsigned short color = ( get_screen_pixel(x-1,y) + get_screen_pixel(x+1,y) + get_screen_pixel(x,y-1) + get_screen_pixel(x,y+1) ) >>2 ;
	PutPixel_16(color,x,y) ;
}

//-----------------------------------------------------------------------

unsigned short get_color_buffer_16(ulong x,ulong y, ulong size_y, unsigned short *buffer)
{
	return *(buffer + x*size_y + (size_y-1-y)) ;
}

void set_color_buffer_16(short x, short y, short size_y, unsigned short *buffer, unsigned short color)
{
	*(buffer + x*size_y + (size_y-1-y)) = color ;
}

uchar get_color_buffer(short x, short y, short size_y, uchar *buffer)
{
	return *(buffer + x*size_y + (size_y-1-y)) ;
}

void set_color_buffer(short x, short y, short size_y, unsigned char *buffer, unsigned char color)
{
	*(buffer + x*size_y + (size_y-1-y)) = color ;
}

