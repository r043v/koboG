
#include ".\kobox.h"

void GpMain(void *arg)
{
	init_screen(8) ;
	GpFatInit() ;
	initSprites() ;
	//MsgBox("init finish.");
	while(1)
	{
		refresh_key() ;

		if(up_front(kStart)) GpAppExit();

		if(instant_key(kUp))
		{
			if(instant_key(kLeft))			ship.way = 7 ;
			else	{	if(instant_key(kRight))	ship.way = 1 ;
					else			ship.way = 0 ;
				}			
		} else	{	if(instant_key(kDown))
				{
					if(instant_key(kLeft))			ship.way = 5 ;
					else	{	if(instant_key(kRight))	ship.way = 3 ;
							else			ship.way = 4 ;
						}								
				} else	{
						if(instant_key(kLeft))		ship.way = 6 ;
						else	if(instant_key(kRight))	ship.way = 2 ;
					}
			}

	for(cpt=0;cpt<ship.speed;cpt++)
		switch(ship.way)
		{
			case 0 : shipUp break ;
			case 1 : shipUp shipRight break ;
			case 2 : shipRight break ;
			case 3 : shipDown shipRight break ;
			case 4 : shipDown break ;
			case 5 : shipDown shipLeft break ;
			case 6 : shipLeft break ;
			case 7 : shipLeft shipUp break ;
		} ;

		if(instant_key(kB))
			if(GpTickCountGet() > lastShoot + shootTime)
			{ lastShoot=GpTickCountGet(); addBullet(&ship,0); }

		clr_scr_color(0) ;
		folow_gfx(&bg,&ship,100,100,130,130);
		draw_map_animated(&bg,spriteSize,spriteSize) ;
		if(!((rand()%25) -1)) addMeteor();
		showAndMoveSprites() ;
		draw_gfx_on_map(&ship,&bg,0,ship.way);
		draw_int(gdl_font,ballNumber,10,10) ;
		draw_int(gdl_font,count_fps(),35,10) ;
		draw_int(gdl_font,bg.scrool_x,55,10) ;
		draw_int(gdl_font,bg.scrool_y,75,10) ;
		
		//draw_text(gdl_font,"KoboG Beta4 by rov",60,10);
		
		flip_screen() ;
	} ;
}
