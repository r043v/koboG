void initBg(struct bg * bg, pixelsize tr_color)
{
	bg->frm_number=0 ;
}

void freeBg(struct bg * bg)
{
	uchar cpt ;
	for(cpt=0;cpt<bg->frm_number;cpt++)
	{
		_free(bg->frm[cpt]->img) ;
		_free(bg->frm[cpt]) ;
	} ;
}

void add_frm_bg(struct bg *bg, uchar **img, uint sizex, uint sizey, pixelsize tr_color, uchar frm_number, uint time_anim)
{
	char cpt_frm, cptx, cpty, last_nb, nb=0, suceed_line=0, line_state ;

	bg->frm_number++ ;
	bg->frm[bg->frm_number-1]= malloc(sizeof(struct frm_bg)) ;
	bg->frm[bg->frm_number-1]->tr_color = tr_color ;
	bg->frm[bg->frm_number-1]->sizex = sizex ;
	bg->frm[bg->frm_number-1]->sizey = sizey ;
	bg->frm[bg->frm_number-1]->img = img ;
	bg->frm[bg->frm_number-1]->frm_number = frm_number ;
	bg->frm[bg->frm_number-1]->zone_number = 0 ;
	bg->frm[bg->frm_number-1]->time_anim = time_anim ;
	bg->frm[bg->frm_number-1]->zone_number = 0 ;

	// search zone (line scan, up to down)

	for(cpt_frm=0;cpt_frm<frm_number;cpt_frm++)
	{	for(cpty=0;cpty<sizey;cpty++)
		{
			last_nb=nb ;
			nb=0 ;
			for(cptx=0;cptx<sizex;cptx++)
			{
				if(get_color_buffer(cptx, cpty, sizey, img[cpt_frm]) != tr_color) nb++ ;
				else	if(nb) // if mixed line detected
					{	// 0: empty 1: mixed 2: full
						if(cpty) // if not first line scan
						{	if(line_state==1) suceed_line++ ;
							else	{	// col scan
									ushort cpt_col, cpt_ligne, cmpt=0 ;

									for(cpt_col=0;cpt_col<sizey;cpt_col++)
										for(cpt_ligne=cpty-nb-1;cpt_ligne<cpty;cpt_ligne++)
										{
											if(get_color_buffer(cptx, cpty, sizey, img[cpt_frm]) != tr_color) cmpt++ ;
											else	if(cmpt) // if mixed col detected
												{
													if(cpt_col) // if not first col scan
													{
														if(line_state==1) suceed_line++ ;
														else
														{
															struct zone *** pointer ;
															struct zone ** pointer2 ;
															struct zone * zone_pointer ;

															pointer = bg->frm[bg->frm_number-1]->zone ;
															*pointer2 = *(pointer[cpt_frm]) ;
															zone_pointer = malloc(sizeof(struct zone)) ;
															pointer2[bg->frm[bg->frm_number-1]->zone_number] = zone_pointer ;

															zone_pointer->type = 2 ;
															zone_pointer->posx = 0 ;
															zone_pointer->posy = cpt_ligne-(cmpt+1) ;
															zone_pointer->sizex = sizex ;
															zone_pointer->sizey = cmpt+1 ;

															bg->frm[bg->frm_number-1]->zone_number++ ;
															suceed_line=0 ;
														}
													}
												}
										} ;
								}
						}
						line_state = 1 ; // set as mixed
						break ;
					}
			} ;

			if(nb==sizex) // full line
			{
				if(cpty) // if not first line scan
				{	if(line_state==2) suceed_line++ ;
					else { /* create new full zone */
						struct zone *** pointer ;
						struct zone * zone_pointer ;

						*pointer = bg->frm[bg->frm_number-1]->zone[(bg->frm_number-1)] ;
						zone_pointer = malloc(sizeof(struct zone)) ;
						*pointer[cpt_frm] = zone_pointer ;

						zone_pointer->type = 2 ;
						zone_pointer->posx = 0 ;
						zone_pointer->posy = cpty-suceed_line-1 ;
						zone_pointer->sizex = sizex ;
						zone_pointer->sizey = suceed_line+1 ;

						bg->frm[bg->frm_number-1]->zone_number++ ;
						suceed_line=0 ;
					 }
				}
				line_state = 2 ; // set as full
			}
			else
				if(!nb) // if empty line
				{
					if(cpty)
					{	if(line_state==0) suceed_line++ ;
						else { suceed_line=0 ; /* no zone need, blanck img part */ }
					}
					line_state = 0 ; // set as empty
				}
		} ;
	} ;
}


