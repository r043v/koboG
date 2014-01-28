
#define way	custom_short_var[0]
#define enable	custom_short_var[1]
#define speed	custom_short_var[2]
#define stFrm	custom_short_var[3]
#define speedx	custom_short_var[4]
#define speedy	custom_short_var[5]
#define dstruct	custom_short_var[6]
#define boomx	custom_short_var[2]
#define boomy	custom_short_var[4]
#define initfrm	custom_short_var[5]

#define createTime	custom_var[1]

#define ship	sprites[0]
#define BulletMax	50
#define delayDestructMax	100

#define kLeft	  1
#define kDown	  2
#define kRight	  4
#define kUp	  8
#define kL	 16
#define kB	 32
#define kA	 64
#define kR	128
#define kStart	256
#define kSelect	512

#define shipUp	if(ship.pos_y) ship.pos_y-- ;\
			if(get_map_tile(&bg,ship.pos_x+7,ship.pos_y)) GpAppExit();

#define shipDown	if(ship.pos_y+16 < bg.map_y_max+239) ship.pos_y++ ;\
			if(get_map_tile(&bg,ship.pos_x+7,ship.pos_y+15)) GpAppExit();
			
#define shipLeft	if(ship.pos_x) ship.pos_x-- ;\
			if(get_map_tile(&bg,ship.pos_x,ship.pos_y+7)) GpAppExit();

#define shipRight	if(ship.pos_x+16 < bg.map_x_max+320) ship.pos_x++ ;\
			if(get_map_tile(&bg,ship.pos_x+15,ship.pos_y+7)) GpAppExit();

#define blt	bullet[nb][type]

#define Time		||((type)&&(GpTickCountGet() - blt.custom_var[0] < 110))

#define bulletUp	colTmp=0;\
			if((!test_tile(&bg,get_map_tile(&bg,blt.pos_x + 7,blt.pos_y + 1),0x1))Time)\
			{	if(blt.pos_y) {	blt.pos_y-- ; colTmp=1 ; }\
			} else	if(!type) if(test_tile(&bg,get_map_tile(&bg,blt.pos_x + 7,blt.pos_y + 1),0x2))\
				destructTile(blt.pos_x + 7,blt.pos_y + 1,0,0);\
			if(!colTmp) blt.enable=0;

#define bulletDown	colTmp=0;\
			if((!test_tile(&bg,get_map_tile(&bg,blt.pos_x  + 7,blt.pos_y  + 14),0x1))Time)\
			{	if(blt.pos_y+spriteSize < bg.map_y_max+239) { blt.pos_y++ ; colTmp=1 ; }\
			} else	if(!type) if(test_tile(&bg,get_map_tile(&bg,blt.pos_x  + 7,blt.pos_y  + 14),0x2))\
				destructTile(blt.pos_x  + 7,blt.pos_y  + 14,0,0);\
			if(!colTmp) blt.enable=0;

#define bulletLeft	colTmp=0;\
			if(!test_tile(&bg,get_map_tile(&bg,blt.pos_x  + 1,blt.pos_y  + 7),0x1)Time)\
			{	if(blt.pos_x) { blt.pos_x-- ; colTmp=1 ; }\
			} else	if(!type) if(test_tile(&bg,get_map_tile(&bg,blt.pos_x  + 1,blt.pos_y  + 7),0x2))\
				destructTile(blt.pos_x  + 1,blt.pos_y  + 7,0,0);\
			if(!colTmp) blt.enable=0;

#define bulletRight	colTmp=0;\
			if(!test_tile(&bg,get_map_tile(&bg,blt.pos_x + 13 ,blt.pos_y  + 8),0x1)Time)\
			{	if(blt.pos_x+spriteSize < bg.map_x_max+320) { blt.pos_x++ ; colTmp=1 ; }\
			} else	if(!type) if(test_tile(&bg,get_map_tile(&bg,blt.pos_x + 13 ,blt.pos_y  + 8),0x2))\
				destructTile(blt.pos_x + 13 ,blt.pos_y  + 8,0,0);\
			if(!colTmp) blt.enable=0;

#define bulletUpRight	colTmp=0;\
			if(!test_tile(&bg,get_map_tile(&bg,blt.pos_x  + 12,blt.pos_y  + 2),0x1)Time)\
			{	if(blt.pos_y) if(blt.pos_x+spriteSize < bg.map_x_max+320) { blt.pos_y-- ;  blt.pos_x++ ; colTmp=1 ; }\
			} else	if(!type) if(test_tile(&bg,get_map_tile(&bg,blt.pos_x  + 12,blt.pos_y  + 2),0x2))\
				destructTile(blt.pos_x  + 12,blt.pos_y  + 2,0,0);\
			if(!colTmp) blt.enable=0;

#define bulletUpLeft	colTmp=0;\
			if(!test_tile(&bg,get_map_tile(&bg,blt.pos_x  + 2,blt.pos_y  + 2),0x1)Time)\
			{	if(blt.pos_y) if(blt.pos_x)  {	blt.pos_y-- ;  blt.pos_x-- ; colTmp=1 ; }\
			} else	if(!type) if(test_tile(&bg,get_map_tile(&bg,blt.pos_x  + 2,blt.pos_y  + 2),0x2))\
				destructTile(blt.pos_x  + 2,blt.pos_y  + 2,0,0);\
			if(!colTmp) blt.enable=0;

#define bulletDownLeft	colTmp=0;\
			if(!test_tile(&bg,get_map_tile(&bg,blt.pos_x + 1 ,blt.pos_y  + 13),0x1)Time)\
			{	if(blt.pos_y+spriteSize < bg.map_y_max+239) if(blt.pos_x) { blt.pos_y++ ;  blt.pos_x-- ; colTmp=1 ; }\
			} else	if(!type) if(test_tile(&bg,get_map_tile(&bg,blt.pos_x + 1 ,blt.pos_y  + 13),0x2))\
				destructTile(blt.pos_x + 1 ,blt.pos_y  + 13,0,0);\
			if(!colTmp) blt.enable=0;

#define bulletDownRight	colTmp=0;\
			if(!test_tile(&bg,get_map_tile(&bg,blt.pos_x + 12 ,blt.pos_y  + 13),0x1)Time)\
			{	if(blt.pos_y+spriteSize < bg.map_y_max+239) if(blt.pos_x+16 < bg.map_x_max+320) { blt.pos_y++ ;  blt.pos_x++ ; colTmp=1 ; }\
			} else	if(!type) if(test_tile(&bg,get_map_tile(&bg,blt.pos_x + 12 ,blt.pos_y  + 13),0x2))\
				destructTile(blt.pos_x + 12 ,blt.pos_y  + 13,0,0);\
			if(!colTmp) blt.enable=0;

#define moveShipBullet	switch(bullet[nb][type].way)\
			{\
				case 0 : bulletUp break ;\
				case 1 : bulletUpRight break ;\
				case 2 : bulletRight break ;\
				case 3 : bulletDownRight break ;\
				case 4 : bulletDown break ;\
				case 5 : bulletDownLeft break ;\
				case 6 : bulletLeft break ;\
				case 7 : bulletUpLeft break ;\
			} \

unsigned char GetAPixel(short x, short y, struct gfx * gfx,unsigned char frm)
{
	unsigned char *p_buf = (unsigned char *)gfx->img[frm] ;
 	return *(p_buf + x*gfx->size_y + (gfx->size_y-y));
}

short colpixel(struct gfx * sprite1, struct gfx * sprite2, char img_1, char img_2)
{
		//char buffer[30] ;
		
		char colision=0;
		
		// detection colision par zone simple
		if((sprite1->pos_y + sprite1->size_y) >= sprite2->pos_y)
			if(sprite1->pos_y <= (sprite2->pos_y + sprite2->size_y))
				if((sprite1->pos_x + sprite1->size_x) >= sprite2->pos_x)
					if(sprite1->pos_x <= (sprite2->pos_x + sprite2->size_x))
						colision=1 ;

		// si colision
		if(colision)
		{
			unsigned short cpt_x,cpt_y ;
			
			//char trace=0 ;
			
			short spr1_x=0,spr1_y=0,spr2_x=0,spr2_y=0,size_x,size_y ;

			// calcul des rectangles de colision dans les 2 sprites

			if(sprite1->pos_x < sprite2->pos_x) spr1_x = sprite2->pos_x - sprite1->pos_x ;
			else if(sprite2->pos_x < sprite1->pos_x) spr2_x = sprite1->pos_x - sprite2->pos_x ;
			
			if(sprite1->pos_y < sprite2->pos_y) spr1_y = sprite2->pos_y - sprite1->pos_y ;
			else if(sprite2->pos_y < sprite1->pos_y) spr2_y = sprite1->pos_y - sprite2->pos_y ;
			
			if(sprite1->pos_x + sprite1->size_x >= sprite2->pos_x + sprite2->size_x)
			{
				if(sprite1->pos_x > sprite2->pos_x) size_x = (sprite2->pos_x + sprite2->size_x) - (sprite1->pos_x) ;
				else size_x = sprite2->size_x ;
			}
			else if(sprite1->pos_x + sprite1->size_x < sprite2->pos_x + sprite2->size_x)	 size_x = (sprite1->pos_x + sprite1->size_x) - (sprite2->pos_x) ;
			
			
			if(sprite1->pos_y + sprite1->size_y >= sprite2->pos_y + sprite2->size_y)
			{
				if(sprite1->pos_y > sprite2->pos_y) size_y = (sprite2->pos_y + sprite2->size_y) - (sprite1->pos_y) ;
				else size_y = sprite2->size_y ;
			}
			else if(sprite1->pos_y + sprite1->size_y < sprite2->pos_y + sprite2->size_y) size_y = (sprite1->pos_y + sprite1->size_y) - (sprite2->pos_y) ;

			if(size_x > sprite1->size_x) size_x=sprite1->size_x ;
			if(size_y > sprite1->size_y) size_y=sprite1->size_y ;

			// calcul du nombre de pixels en contact

			for(cpt_x=0;cpt_x<size_x;cpt_x++)
				for(cpt_y=0;cpt_y<size_y;cpt_y++)
					if(GetAPixel(spr1_x + cpt_x,spr1_y + cpt_y,sprite1,img_1) != sprite1->transp_color)
						if(GetAPixel(spr2_x + cpt_x,spr2_y + cpt_y,sprite2,img_2) != sprite2->transp_color)
							return 1 ;

			return 0 ; // renvoi le nb de pixels en contact
		}
	return 0 ; // pas de colision
}

#define meteorGoUp	meteor[nb].pos_y = bg.scrool_y - rand()%50;
#define meteorGoDown	meteor[nb].pos_y = bg.scrool_y + 240 + rand()%50;
#define meteorGoLeft	meteor[nb].pos_x = bg.scrool_x - rand()%50;
#define meteorGoRight	meteor[nb].pos_x = bg.scrool_x + 320 + rand()%50;

