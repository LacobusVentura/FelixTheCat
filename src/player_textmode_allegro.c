/*!
	\file player_textmode_allegro.c
	\brief Animation Player Object in Emulated Text Mode (Allegro)
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "console.h"
#include "frame.h"
#include "player.h"
#include "player_textmode_allegro.h"


#define PLAYER_TEXTMODE_ALLEGRO_DESC                "Emulated Text Mode 80x60x256 (Allegro)"
#define PLAYER_TEXTMODE_ALLEGRO_COLS_COUNT          (640)
#define PLAYER_TEXTMODE_ALLEGRO_ROWS_COUNT          (480)
#define PLAYER_TEXTMODE_ALLEGRO_FONT_FILE           "./felix.ttf"
#define PLAYER_TEXTMODE_ALLEGRO_FONT_SIZE           (8)


/* Private Structures */
struct player_textmode_allegro_data_s
{
	ALLEGRO_DISPLAY * display;
	ALLEGRO_FONT * font;
	ALLEGRO_BITMAP * buffer;
	ALLEGRO_BITMAP * console_buffer;
};

/* Private Types */
typedef struct player_textmode_allegro_data_s player_textmode_allegro_data_t;


/* Abstract Functions Implementation Prototypes */
static player_t * player_textmode_allegro_create( player_t * parent );
static void player_textmode_allegro_destroy( player_t * this );
static int player_textmode_allegro_initialize( player_t * this );
static void player_textmode_allegro_finish( player_t * this );
static void player_textmode_allegro_set_palette( player_t * this, palette_t * pal );
static void player_textmode_allegro_render_frame( player_t * this, frame_t * frm );
static void player_textmode_allegro_refresh_console( player_t * this );


/* Implementation */
player_implementation_t * player_textmode_allegro_get_implementation( void )
{
	static player_implementation_t impl;

	impl.create = player_textmode_allegro_create;
	impl.destroy = player_textmode_allegro_destroy;
	impl.screen_initialize = player_textmode_allegro_initialize;
	impl.screen_finish = player_textmode_allegro_finish;
	impl.set_palette = player_textmode_allegro_set_palette;
	impl.render_frame = player_textmode_allegro_render_frame;
	impl.refresh_console = player_textmode_allegro_refresh_console;

	return &impl;
};


static player_t * player_textmode_allegro_create( player_t * parent )
{
	player_textmode_allegro_data_t * data = NULL;

	data = (player_textmode_allegro_data_t*) calloc( 1, sizeof(player_textmode_allegro_data_t) );

	if( !data )
		return NULL;

	if( !al_init() )
	{
		free(data);
		return NULL;
	}

	al_init_font_addon();
	al_init_ttf_addon();

	data->font = al_load_font( PLAYER_TEXTMODE_ALLEGRO_FONT_FILE, PLAYER_TEXTMODE_ALLEGRO_FONT_SIZE, 0 );

	if(!data->font)
	{
		free(data);
		return NULL;
	}

	player_set_screen_format( parent, player_screen_format_text );
	player_set_data( parent, (void*) data );

	return parent;
}


static void player_textmode_allegro_destroy( player_t * this )
{
	player_textmode_allegro_data_t * data = player_get_data( this );

	al_destroy_font( data->font );

	free( data );
}


static int player_textmode_allegro_initialize( player_t * this )
{
	int ret = 0;
	int nlines = 0;
	int font_height = 0;
	int font_length = 0;
	player_textmode_allegro_data_t * data = player_get_data( this );
	console_t * con = player_get_console( this );


	font_height =  al_get_font_line_height( data->font );
	font_length = al_get_text_width( data->font, "\x20" );

	if(con)
		nlines = console_get_lines_count( con );

	player_set_real_cols_count( this, PLAYER_TEXTMODE_ALLEGRO_COLS_COUNT );
	player_set_real_rows_count( this, PLAYER_TEXTMODE_ALLEGRO_ROWS_COUNT + (font_height * nlines) );

	player_set_screen_cols_count( this, PLAYER_TEXTMODE_ALLEGRO_COLS_COUNT / font_height );
	player_set_screen_rows_count( this, PLAYER_TEXTMODE_ALLEGRO_ROWS_COUNT / font_length );

	if(con)
	{
		player_set_console_dimension( this, PLAYER_TEXTMODE_ALLEGRO_COLS_COUNT, font_height * nlines );
		player_set_console_position( this, 0, PLAYER_TEXTMODE_ALLEGRO_ROWS_COUNT );
	}

	data->buffer = al_create_bitmap(	player_get_real_cols_count(this),
										player_get_real_rows_count(this) );

	if( !data->buffer )
		return -1;

	if( con )
	{
		int console_nrows = 0;
		int console_ncols = 0;

		player_get_console_dimension( this, &console_ncols, &console_nrows );

		data->console_buffer = al_create_bitmap(	console_ncols,
													console_nrows );

		if( !data->console_buffer )
		{
			al_destroy_bitmap( data->buffer );
			return -1;
		}
	}

	al_set_new_bitmap_format( ALLEGRO_PIXEL_FORMAT_SINGLE_CHANNEL_8 );

	al_set_new_display_flags( ALLEGRO_PROGRAMMABLE_PIPELINE | ALLEGRO_OPENGL );

	data->display = al_create_display(	player_get_real_cols_count(this),
										player_get_real_rows_count(this) );

	if( ret < 0 )
	{
		al_destroy_bitmap( data->buffer );
		al_destroy_bitmap( data->console_buffer );
		return -1;
	}

	al_set_window_title( data->display, PLAYER_TEXTMODE_ALLEGRO_DESC );

	return 0;
}


static void player_textmode_allegro_finish( player_t * this )
{
	player_textmode_allegro_data_t * data = player_get_data( this );

	al_destroy_bitmap( data->buffer );
	al_destroy_bitmap( data->console_buffer );
	al_destroy_display( data->display );
}


static void player_textmode_allegro_set_palette( player_t * this, palette_t * pal )
{
	/*
	int i = 0;
	int count = 0;
	PALETTE palette;

		palette[256];

	count = palette_get_color_count( pal );

	for( i = 0; i < count; i++ )
		palette_get_color( pal, i, &palette[i].r, &palette[i].g, &palette[i].b );

	al_wait_for_vsync();

	set_palette( palette );
	*/
}


static void player_textmode_allegro_render_frame( player_t * this, frame_t * frm )
{
	int col = 0;
	int row = 0;
	int txtrow = 0;
	int txtcol = 0;
	int ncols = 0;
	int nrows = 0;
	int font_height = 0;
	int font_length = 0;
	frame_point_t pt;
	player_textmode_allegro_data_t * data = player_get_data( this );

	frame_get_dimensions( frm, &ncols, &nrows );

	font_height =  al_get_font_line_height( data->font );
	font_length = al_get_text_width( data->font, "\x20" );

	for( row = 0, txtrow = 0; row < nrows; row++, txtrow += font_height )
	{
		for( col = 0, txtcol = 0; col < ncols; col++, txtcol += font_length )
		{
			frame_get_point( frm, col, row, &pt );

			if(!isprint(pt.chr))
				pt.chr = '\x20';

			/* Text Backgroud Color */
			al_draw_filled_rectangle( txtcol, txtrow, txtcol + font_length, txtrow + font_height, al_map_rgb( 0, 0, 0 ) );

			/* Text Draw */
			al_draw_textf( data->font, al_map_rgb( 0, 128, 0 ), txtcol, txtrow, ALLEGRO_ALIGN_LEFT, "%c", pt.chr );
		}
	}

	al_flip_display();
}


static void player_textmode_allegro_refresh_console( player_t * this )
{
	int i = 0;
	int txt_height = 0;
	int count = 0;
	int ydim = 0;
	int ypos = 0;
	int xdim = 0;
	int xpos = 0;
	console_t * con = player_get_console( this );
	player_textmode_allegro_data_t * data = player_get_data( this );


	if( !con )
		return;

	txt_height =  al_get_font_line_height( data->font );

	count = console_get_lines_count( con );

	//al_clear_bitmap( data->console_buffer );
	//al_clear_to_color( al_map_rgb( 0, 0, 0) )

	for( i = 0; i < count; i++ )
	{
		al_draw_textf(	data->font,
						al_map_rgb( 255, 255, 255),
						0,
						txt_height * i,
						ALLEGRO_ALIGN_LEFT,
						"%s",
						console_get_line( con, i ) );
	}

	player_get_console_position( this, &xpos, &ypos );
	player_get_console_dimension( this, &xdim, &ydim );

	//al_draw_bitmap_region( data->console_buffer, 0, 0, float sw, float sh, float dx, float dy, int flags );
/*
	blit(	data->console_buffer,
			screen,
			0,
			0,
			xpos,
			ypos,
			xdim,
			ydim );
*/
}

/* $Id: player_textmode_allegro.c 307 2015-08-13 20:18:23Z tiago.ventura $ */
