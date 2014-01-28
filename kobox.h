#include ".\Gdl.h"
#include ".\data\FONT8.h"
#include ".\data\array.h"
#include ".\define.h"

struct ball
{
	int px, py;
	long lastShootTime;
	int enabled ;
	int purple ;
};

struct	delayDestruct
{
	int delay ;
	int mx, my ;
	int enabled ;
	int purple ;
};

struct	font font8 ;
uint cpt ;	
uint pal[256] ;
uchar * gfx[25] ;
struct gfx sprites[2] ;
struct gfx bullet[BulletMax][6] ;
struct gfx boom[60] ;
struct map bg;	
int nbBullet[6];
int shootTime = 130 ;
int lastShoot=0 ;
int spriteSize ;
int colTmp;
uchar tile_colision[24] = {0x0,0xFF,0xFF,0xFF,0xFF,0xFF,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1} ;
short atan[360] ;
int ballNumber=0;
struct ball balls[100] ;
struct delayDestruct delayDestruct[delayDestructMax] ;
struct gfx enemi[50];
struct gfx meteor[50];

/*
	 way.
	  0
	 7 1
	6   2
	 5 3
	  4
*/

void scanMap(void)
{
	int cptx, cpty, cpt, value ;
	ballNumber=0;
	
	for(cptx=0;cptx<64;cptx++)
	 for(cpty=0;cpty<64;cpty++)
	 {
	 	value = bg.map[cptx + cpty*64] ;
	 	if(value== 5)	balls[ballNumber].purple = 1 ;
	 	else		balls[ballNumber].purple = 0 ;
	 	if((value > 0)&&(value < 6)) { balls[ballNumber].px = cptx*spriteSize+6 ; balls[ballNumber].py = cpty*spriteSize+6 ; balls[ballNumber].lastShootTime=0; balls[ballNumber].enabled=1 ; ballNumber++ ; }
	 };

	for(cpt=ballNumber;cpt<99;cpt++) balls[cpt].enabled=0 ;
}

int onScreen(int x, int y)
{
	return ((x-bg.scrool_x < 320 + spriteSize)&&(y-bg.scrool_y < 240 + spriteSize)&&(x>0)&&(y>0));
}

void refreshTile(int x, int y, int tile)
{
	switch(tile)
	{
		case 16:
			if(!get_map_tile(&bg,x,y+spriteSize)) set_map_tile(&bg,x,y,7) ;
			else	if(!get_map_tile(&bg,x,y-spriteSize)) set_map_tile(&bg,x,y,9) ;
				else	if(!get_map_tile(&bg,x-spriteSize,y)) set_map_tile(&bg,x,y,11) ;
		break; 
		case 17:
			if(!get_map_tile(&bg,x,y+spriteSize)) set_map_tile(&bg,x,y,6) ;
			else	if(!get_map_tile(&bg,x,y-spriteSize)) set_map_tile(&bg,x,y,8) ;
				else	if(!get_map_tile(&bg,x+spriteSize,y)) set_map_tile(&bg,x,y,11) ;
		break; 
		case 18:
			if(!get_map_tile(&bg,x-spriteSize,y)) set_map_tile(&bg,x,y,8) ;
			else	if(!get_map_tile(&bg,x+spriteSize,y)) set_map_tile(&bg,x,y,9) ;
				else	if(!get_map_tile(&bg,x,y+spriteSize)) set_map_tile(&bg,x,y,10) ;
		break; 
		case 19:
			if(!get_map_tile(&bg,x-spriteSize,y)) set_map_tile(&bg,x,y,6) ;
			else	if(!get_map_tile(&bg,x+spriteSize,y)) set_map_tile(&bg,x,y,7) ;
				else	if(!get_map_tile(&bg,x,y-spriteSize)) set_map_tile(&bg,x,y,10) ;
		break; 
		case 20:
			if(!get_map_tile(&bg,x,y-spriteSize)) set_map_tile(&bg,x,y,18) ;
			else	if(!get_map_tile(&bg,x,y+spriteSize)) set_map_tile(&bg,x,y,19) ;
				else	if(!get_map_tile(&bg,x-spriteSize,y)) set_map_tile(&bg,x,y,17) ;
					else	if(!get_map_tile(&bg,x+spriteSize,y)) set_map_tile(&bg,x,y,16) ;
		break;
	};
}

int	testTile(int x, int y, int max, int min, int purple)
{
	int tile = get_map_tile(&bg,x,y);
	
	if(!purple) { if((tile > min)&&(tile < max)) return 1 ; }
	else	if(tile) return 0 ;
	return 0 ;
}

void	addBoom(int x, int y)
{
	int nb=0 ;
	short px, py,tx,ty ;

	while((boom[nb].enable)&&(nb<49)) nb++ ;
	boom[nb].enable = 1 ;
	px = bg.scrool_x ;
	py = bg.scrool_y ;
	bg.scrool_x = x ;
	bg.scrool_y = y ;
	get_start_pos_of_a_tile(&bg,0,0,&tx,&ty);
	boom[nb].pos_x = tx ;
	boom[nb].pos_y = ty ;
	bg.scrool_x = px ;
	bg.scrool_y = py ;
}

void	addDestruct(int mx, int my, int purple, int delay)
{
	int nb=delayDestructMax ;	
	while( --nb && delayDestruct[nb].enabled ) ;
	delayDestruct[nb].enabled	= 1	;
	delayDestruct[nb].mx	= mx	;
	delayDestruct[nb].my	= my	;
	delayDestruct[nb].delay	= delay	;
	delayDestruct[nb].purple	= purple	;
}

void	(*destruct)(int up,int down,int left,int right,int x,int y,int purple, int chainTime) ;
void	(*destroyTile)(int mx, int my, int purple) ;

void	destructTile(int mx, int my, int purple, int delay)
{
	if(delay)	addDestruct(mx,my,purple,delay) ;
	else	destroyTile(mx,my,purple) ;
}

void	_destroyTile(int mx, int my, int purple)
{
	int chainTime = 5 ;
	int tile = get_map_tile(&bg,mx,my) ;
	int x=mx, y=my ;

     if(tile)
     {	addBoom(x,y);
     	if(tile < 6) // ball
	{
		int nb ;

		// destruct the tile
		set_map_tile(&bg,x,y,0) ;

		// add entry in destruct list.
		switch(tile)
		{
			case 1:	destructTile(x,y-spriteSize,purple,chainTime) ; break;
			case 2:	destructTile(x+spriteSize,y,purple,chainTime) ; break;
			case 3:	destructTile(x,y+spriteSize,purple,chainTime) ; break;
			case 4:	destructTile(x-spriteSize,y,purple,chainTime) ; break;
			case 5:
				destructTile(x+spriteSize,y,1,chainTime) ;
				destructTile(x+spriteSize,y+spriteSize,1,chainTime) ;
				destructTile(x-spriteSize,y,1,chainTime) ;
				destructTile(x-spriteSize,y-spriteSize,1,chainTime) ;
				destructTile(x+spriteSize,y-spriteSize,1,chainTime) ;
				destructTile(x-spriteSize,y+spriteSize,1,chainTime) ;
				destructTile(x,y+spriteSize,1,chainTime) ;
				destructTile(x,y-spriteSize,1,chainTime) ;
			break;
		};

		// unable the ball fire
		for(nb=0;nb<99;nb++)
		 if(balls[nb].enabled)
		  if(abs(balls[nb].px-x) < 10)
		   if(abs(balls[nb].py-y)< 10)
		   { balls[nb].enabled=0 ; ballNumber-- ; nb=99; }

	} else	{	if(tile < 12) // wall
			{
				switch(tile)
				{
					case  6: destruct(1,0,0,1,x,y,purple,chainTime); break;
					case  7: destruct(1,0,1,0,x,y,purple,chainTime); break;
					case  8: destruct(0,1,0,1,x,y,purple,chainTime); break;
					case  9: destruct(0,1,1,0,x,y,purple,chainTime); break;
					case 10: destruct(0,0,1,1,x,y,purple,chainTime); break;
					case 11: destruct(1,1,0,0,x,y,purple,chainTime); break;
				};
			} else {	if(tile < 16) { set_map_tile(&bg,x,y,0) ; return ; } // wall end
				else 
				{
					if(purple)
					{
						switch(tile)
						{
							case  16: destruct(1,1,1,0,x,y,1,chainTime); break;
							case  17: destruct(1,1,0,1,x,y,1,chainTime); break;
							case  18: destruct(0,1,1,1,x,y,1,chainTime); break;
							case  19: destruct(1,0,1,1,x,y,1,chainTime); break;
							case  20: destruct(1,1,1,1,x,y,1,chainTime); break;
						} ;
					} else refreshTile(x,y,tile) ;
				}
			     }
		}
     }
}

void	readDestruct(void)
{	
	int nb=0, max=delayDestructMax ;
	for(;nb<max;nb++)	if(delayDestruct[nb].enabled )
				{
					if(delayDestruct[nb].delay) delayDestruct[nb].delay-- ;
					else {	destroyTile(delayDestruct[nb].mx,delayDestruct[nb].my,delayDestruct[nb].purple) ; delayDestruct[nb].enabled = 0 ; }
				}
}

void _destruct(int up,int down,int left,int right,int x,int y,int purple, int chainTime)
{	set_map_tile(&bg,x,y,0) ;
	if((!up)||(!testTile(x,y-spriteSize,6,0,purple)))
		if((!down)||(!testTile(x,y+spriteSize,6,0,purple)))
			if((!left)||(!testTile(x-spriteSize,y,6,0,purple)))
				if((!right)||(!testTile(x+spriteSize,y,6,0,purple)))
				{
					if(up)	destructTile(x,y-spriteSize,purple,chainTime) ;
					if(down)  destructTile(x,y+spriteSize,purple,chainTime) ;
					if(left)     destructTile(x-spriteSize,y,purple,chainTime) ;
					if(right)   destructTile(x+spriteSize,y,purple,chainTime) ;
				}
}

void initSprites(void)
{
	int nb, type;

	destruct = _destruct ;
	destroyTile= _destroyTile ;
	load_font(&font8,(uchar *)FONT8,8,8,0x16,91) ;
	set_system_font(&font8) ;

	gfx[0] = load_pcx("gp:\\gpmm\\kobog\\gfx\\ship.pcx")	;
	gfx[1] = load_pcx("gp:\\gpmm\\kobog\\gfx\\bullet.pcx")	;
	gfx[2] = load_pcx("gp:\\gpmm\\kobog\\gfx\\enemiG.pcx")	;
	gfx[3] = load_pcx("gp:\\gpmm\\kobog\\gfx\\tileset.pcx")	;
	gfx[4] = load_pcx("gp:\\gpmm\\kobog\\gfx\\boom.pcx")	;
	gfx[5] = load_pcx("gp:\\gpmm\\kobog\\gfx\\ball.pcx")	;
	gfx[6] = load_pcx("gp:\\gpmm\\kobog\\gfx\\meteor.pcx")	;

	spriteSize = getPcxSy("gp:\\gpmm\\kobog\\gfx\\ship.pcx");
	load_font_from_pcx(&font8,"gp:\\gpmm\\kobog\\gfx\\font.pcx");
	set_system_font(&font8) ;

	charge_palette_pcx("gp:\\gpmm\\kobog\\gfx\\tileset.pcx", pal) ;
	fade_to_palette(0,pal) ;
	set_map(&bg,(short*)mapArray,64,64,0,0,gfx[3],21,spriteSize,spriteSize,*gfx[3]);
	set_walkable_on_tiles(&bg,tile_colision,21) ;

	load_anim(&ship,0,0,8,gfx[0],120,2,0,spriteSize,spriteSize,100,100,*gfx[0]) ;

	ship.way = 4 ;
	ship.speed = 2*spriteSize>>4 ;

	type=0;
	for(nb=0;nb<BulletMax-1;nb++)
	{
		load_anim(&bullet[nb][type],0,0,8,gfx[1],120,2,0,spriteSize,spriteSize,100,100,*gfx[1]) ;
		bullet[nb][type].enable=0 ;
		bullet[nb][type].speed = 4*(spriteSize>>4) ;
	};

	type=1;
	for(nb=0;nb<BulletMax-1;nb++)
	{
		load_anim(&bullet[nb][type],0,0,8,gfx[5],120,2,0,spriteSize,spriteSize,100,100,*gfx[5]) ;
		bullet[nb][type].enable=0 ;
		bullet[nb][type].speed = 2;//1*(spriteSize>>4) + 1  ;
	};

	for(type=2;type<6;type++)
		for(nb=0;nb<BulletMax-1;nb++)
		{
			load_anim(&bullet[nb][type],0,0,8,gfx[1],120,2,0,spriteSize,spriteSize,100,100,*gfx[1]) ;
			bullet[nb][type].enable=0 ;
			bullet[nb][type].speed = 1*(spriteSize>>4) ;
		};

	for(nb=0;nb<6;nb++)
	{	
		nbBullet[nb] = BulletMax ;
	} ;

	for(nb=0;nb<50;nb++)
	{	
		load_anim(&enemi[nb],0,0,8,gfx[2],120,2,0,spriteSize,spriteSize,100,100,*gfx[2]) ;
		enemi[nb].enable = 0 ;
		enemi[nb].speed  = 1 ;
		enemi[nb].way = rand()%8 ;
	} ;
	
	for(nb=0;nb<50;nb++)
	{	
		load_anim(&meteor[nb],0,0,3,gfx[6],120,2,0,spriteSize,spriteSize,100,100,*gfx[6]) ;
		meteor[nb].enable = 0 ;
		meteor[nb].speed  = rand()%3 ;
		meteor[nb].way = rand()%8 ;
	} ;	

	for(nb=0;nb<BulletMax-1;nb++)
	{
		load_anim(&boom[nb],0,0,7,gfx[4],80,0,0,spriteSize,spriteSize,100,100,*gfx[4]) ;
		boom[nb].stFrm  = 0 ;
		boom[nb].enable = 0 ;
	};

	for(nb=0;nb<delayDestructMax;nb++) delayDestruct[nb].enabled=0;
	scanMap();
}

void	addenemi(struct gfx * gfx, int type)
{
	int nb = 49 ;
	while(nb && enemi[nb].enable) nb-- ;
	enemi[nb].enable= 1 ;
	enemi[nb].pos_x = gfx->pos_x ;
	enemi[nb].pos_y = gfx->pos_y ;
	enemi[nb].way = gfx->way ;
}

int	go_on_ship(int px, int py, int spx, int spy, int sway, int preShootBoxPawa)
{
	int up,down,left,right ;

	switch(sway) // change ship pos, with ship pos + 1 tile
	{
		case 0: spy-=spriteSize ; break ;
		case 1: spy-=spriteSize ; spx+=spriteSize; break ;
		case 2: spy-=spriteSize ; spx+=spriteSize; break ;
		case 3: spy+=spriteSize ; spx+=spriteSize; break ;
		case 4: spy+=spriteSize ; break ;
		case 5: spy+=spriteSize ; spx-=spriteSize; break ;
		case 6: spy+=spriteSize ; spx-=spriteSize; break ;
		case 7: spy-=spriteSize ; spx-=spriteSize; break ;				
	};
			
	up	= (spy < py)&&(py-spy > preShootBoxPawa);
	down	= (spy > py)&&(spy-py > preShootBoxPawa);
	right	= (spx > px)&&(spx-px > preShootBoxPawa);
	left	= (spx < px)&&(px-spx > preShootBoxPawa);

	// if ship not near, fire behind him

	if(up)
	{
	  if(left) return 7;
	  else	if(right) return 1;
	 	else	return 0;
	} else	if(down)
		{
		 if(left) return 5;
		 else	if(right) return 3;
		 	else	return 4;				
		} else	{
			 if(left) return 6;
			 else	if(right) return 2;
			 	else
			 	{	// if ship is near, shoot on him !
			 		
	 				up	= (spy < py);
					down	= (spy > py);
					right	= (spx > px);
					left	= (spx < px);
		
					if(up)
					{
					 if(left) return 7;
					 else	if(right) return 1;
					 	else	return 0;
					} else	if(down)
						{
						 if(left) return 5;
						 else	if(right) return 3;
						 	else	return 4;				
						} else	{
							 if(left) return 6;
							 else	if(right) return 2; // else, random shoot way, maybe autodestruct in future.
							 	else	return rand()%8;
							}
				}
			}
}


void	addMeteor(void)
{
	int nb = 49 ;
	int px, py ;
	while(nb && meteor[nb].enable) nb-- ;
	meteor[nb].enable= 1 ;
	meteor[nb].way = rand()%8 ;
	px = meteor[nb].pos_x ;
	py = meteor[nb].pos_y ;
	
	switch(meteor[nb].way)
	{
		case 0: meteorGoUp  break;
		case 1: meteorGoUp meteorGoRight break;
		case 7: meteorGoUp  break;
		case 2: meteorGoRight break;
		case 6: meteorGoLeft break;
		case 3: meteorGoRight meteorGoDown break;
		case 5: meteorGoLeft meteorGoDown break;
		case 4: meteorGoDown  break;
	};
	
	meteor[nb].way = go_on_ship(px,py,ship.pos_x,ship.pos_y,ship.way,32) ;
}

void	addBullet(struct gfx * gfx, int type)
{
	int nb = nbBullet[type]-1 ;

	while(nb && (bullet[nb][type]).enable) nb-- ;
	bullet[nb][type].enable= 1 ;
	bullet[nb][type].pos_x = gfx->pos_x ;
	bullet[nb][type].pos_y = gfx->pos_y ;
	bullet[nb][type].way = gfx->way ;
	//bullet[nb][type].way = rand()%8;

	if(!type)
	{
		nb = nbBullet[type]-1 ;
		while(nb && (bullet[nb][type]).enable) nb-- ;
		bullet[nb][type].enable= 1 ;
		bullet[nb][type].pos_x = gfx->pos_x ;
		bullet[nb][type].pos_y = gfx->pos_y ;
	
		switch(gfx->way) // (gfx->way+4)%8 ;
		{
			case 0 : bullet[nb][type].way = 4 ; break ;
			case 1 : bullet[nb][type].way = 5 ; break ;
			case 2 : bullet[nb][type].way = 6 ; break ;
			case 3 : bullet[nb][type].way = 7 ; break ;
			case 4 : bullet[nb][type].way = 0 ; break ;
			case 5 : bullet[nb][type].way = 1 ; break ;
			case 6 : bullet[nb][type].way = 2 ; break ;
			case 7 : bullet[nb][type].way = 3 ; break ;
		} ;
		//bullet[nb][type].way = rand()%8;
	}
	
	bullet[nb][type].custom_var[0] = GpTickCountGet() ;
}
/*
	 way.
	  0
	 7 1
	6   2
	 5 3
	  4
*/

void	showAndMoveSprites(void)
{
	int type, nb,nb2, px, py, cpt;
	static struct gfx tmp ;

	readDestruct();

	cpt=0 ;
	for(nb=0;nb<99;nb++)
	{
		px = balls[nb].px;
		py = balls[nb].py;

		if (balls[nb].enabled)
		{  if((balls[nb].lastShootTime + 1500 + rand()%750 < GpTickCountGet())&&(onScreen(px,py)))
		   {
			int spx = ship.pos_x ;
			int spy = ship.pos_y ;

			tmp.way = go_on_ship(px,py,spx,spy,ship.way,64) ;

			tmp.pos_x = px ;
			tmp.pos_y = py ;
			if(!balls[nb].purple)		addBullet(&tmp,1);
			else			addenemi(&tmp,1);
			balls[nb].lastShootTime = GpTickCountGet() ;
		   }
		   if(++cpt>=ballNumber)nb=BulletMax;
		}
	};

	for(type=0;type<2;type++)
		for(nb=0;nb<(BulletMax-1);nb++)
		{
			if(bullet[nb][type].enable)
			{
				if(!type)	draw_gfx_on_map(&bullet[nb][type],&bg,0,bullet[nb][type].way);
				else		play_anim_on_map(&bullet[nb][type],&bg,0,0) ;

				for(nb2=0;nb2<50;nb2++)
					if(meteor[nb2].enable)	if(test_colision_pixel(&bullet[nb][type],&meteor[nb2],0,0,0,0)) { bullet[nb][type].enable=0; addBoom(bullet[nb][type].pos_x,bullet[nb][type].pos_y); } ;
				
				for(nb2=0;nb2<50;nb2++)
					if(enemi[nb2].enable)	if(colpixel(&enemi[nb2],&bullet[nb][type],enemi[nb2].way,bullet[nb][type].way)) { addBoom(enemi[nb2].pos_x,enemi[nb2].pos_y); enemi[nb2].enable=0; bullet[nb][type].enable=0; } ;
				
				if(type != 0)
					if(colpixel(&ship,&bullet[nb][type],ship.way,bullet[nb][type].way)) { wait_key(kA); }
				
				for(cpt=0;cpt<bullet[nb][type].speed;cpt++)
				{	switch(type)
					{
						case 0:	moveShipBullet ; break;
						case 1:	moveShipBullet ; break;
					};
					
					if((bullet[nb][type].way%2))
						if(cpt >= bullet[nb][type].speed>>1) cpt++ ;
				};
				
				if(bullet[nb][type].pos_x - bg.scrool_x < -spriteSize) bullet[nb][type].enable=0 ;
				else	if(bullet[nb][type].pos_x - bg.scrool_x > 320+spriteSize) bullet[nb][type].enable=0 ;
				
				if(bullet[nb][type].pos_y - bg.scrool_y < -spriteSize) bullet[nb][type].enable=0 ;
				else	if(bullet[nb][type].pos_y - bg.scrool_y > 240+spriteSize) bullet[nb][type].enable=0 ;
			}
		};

	for(nb=0;nb<BulletMax-1;nb++)
	{
		if(boom[nb].enable)
		{	if(!boom[nb].animation[0].finish) play_anim_on_map(&boom[nb],&bg,0,0) ;
			else { reset_anim(&boom[nb],0); boom[nb].enable=0; }
		}
	};
	
	for(nb=0;nb<50;nb++)
	{
		if(enemi[nb].enable)
		{

			for(nb2=0;nb2<50;nb2++)
				if(meteor[nb2].enable)	if(colpixel(&enemi[nb],&meteor[nb2],enemi[nb].way,meteor[nb2].animation[0].curent_frame)) { enemi[nb].enable=0; addBoom(enemi[nb].pos_x,enemi[nb].pos_y); } ;

			if(colpixel(&enemi[nb],&ship,enemi[nb].way,ship.way)) { addBoom(enemi[nb].pos_x,enemi[nb].pos_y); enemi[nb].enable=0; wait_key(kA); } ;

			switch(enemi[nb].way)
			{
				case 0: enemi[nb].pos_y-=1; break;
				case 1: enemi[nb].pos_y-=1; enemi[nb].pos_x+=1; break;
				case 2: enemi[nb].pos_x+=1; break;
				case 3: enemi[nb].pos_y+=1; enemi[nb].pos_x+=1; break;
				case 4: enemi[nb].pos_y-=1; break;
				case 5: enemi[nb].pos_y+=1; enemi[nb].pos_x-=1; break;
				case 6: enemi[nb].pos_x-=1; break;
				case 7: enemi[nb].pos_y-=1; enemi[nb].pos_x-=1; break;
			};

			if(!onScreen(enemi[nb].pos_x,enemi[nb].pos_y))	enemi[nb].enable = 0 ;

			draw_gfx_on_map(&enemi[nb],&bg,0,enemi[nb].way);
		}
	};

	for(nb=0;nb<50;nb++)
	{
		if(meteor[nb].enable)
		{
			if(GpTickCountGet() - meteor[nb].createTime > 1000)	if(!onScreen(meteor[nb].pos_x,meteor[nb].pos_y))	meteor[nb].enable = 0 ;
			
			switch(meteor[nb].way)
			{
				case 0: meteor[nb].pos_y-=1; break;
				case 1: meteor[nb].pos_y-=1; meteor[nb].pos_x+=1; break;
				case 2: meteor[nb].pos_x+=1; break;
				case 3: meteor[nb].pos_y+=1; meteor[nb].pos_x+=1; break;
				case 4: meteor[nb].pos_y-=1; break;
				case 5: meteor[nb].pos_y+=1; meteor[nb].pos_x-=1; break;
				case 6: meteor[nb].pos_x-=1; break;
				case 7: meteor[nb].pos_y-=1; meteor[nb].pos_x-=1; break;
			};

			play_anim_on_map(&meteor[nb],&bg,0,0);

			if(colpixel(&ship,&meteor[nb],ship.way,meteor[nb].animation[0].curent_frame)) { wait_key(kA); }
		}
	};
}
