/*!
	\file player_graphmode_allegro.c
	\brief Animation Player Object in Graphical Mode Object (Allegro)
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#include <stdio.h>
#include <stdlib.h>

#include <allegro.h>

#include "frame.h"
#include "player.h"
#include "player_graphmode_allegro.h"

#define PLAYER_GRAPHMODE_ALLEGRO_DESC                  "Standard 640x480x256 (Allegro)"
#define PLAYER_GRAPHMODE_ALLEGRO_COLS_COUNT            (640)
#define PLAYER_GRAPHMODE_ALLEGRO_ROWS_COUNT            (480)


/* Private Structures */
struct player_graphmode_allegro_data_s
{
	BITMAP * buffer;
	BITMAP * console_buffer;
};

/* Private Types */
typedef struct player_graphmode_allegro_data_s player_graphmode_allegro_data_t;


/* Abstract Functions Implementation Prototypes */
static player_t * player_graphmode_allegro_create( player_t * parent );
static void player_graphmode_allegro_destroy( player_t * this );
static int player_graphmode_allegro_initialize( player_t * this );
static void player_graphmode_allegro_finish( player_t * this );
static void player_graphmode_allegro_set_palette( player_t * this, palette_t * pal );
static void player_graphmode_allegro_render_frame( player_t * this, frame_t * frm );
static void player_graphmode_allegro_refresh_console( player_t * this );


/* Implementation */
player_implementation_t * player_graphmode_allegro_get_implementation( void )
{
	static player_implementation_t impl;

	impl.create = player_graphmode_allegro_create;
	impl.destroy = player_graphmode_allegro_destroy;
	impl.screen_initialize = player_graphmode_allegro_initialize;
	impl.screen_finish = player_graphmode_allegro_finish;
	impl.set_palette = player_graphmode_allegro_set_palette;
	impl.render_frame = player_graphmode_allegro_render_frame;
	impl.refresh_console = player_graphmode_allegro_refresh_console;

	return &impl;
};


static player_t * player_graphmode_allegro_create( player_t * parent )
{
	player_graphmode_allegro_data_t * data = NULL;

	data = (player_graphmode_allegro_data_t*) calloc( 1, sizeof(player_graphmode_allegro_data_t) );

	if( !data )
		return NULL;

	data->buffer = NULL;

	player_set_screen_format( parent, player_screen_format_graphic );

	player_set_data( parent, (void*) data );

	return parent;
}


static void player_graphmode_allegro_destroy( player_t * this )
{
	free( player_get_data( this ) );
}


static int player_graphmode_allegro_initialize( player_t * this )
{
	int ret = 0;
	int nlines = 0;
	int font_height = 0;
	int font_length = 0;
	player_graphmode_allegro_data_t * data = player_get_data( this );
	console_t * con = player_get_console( this );


	font_height = text_height( font );
	font_length = text_length( font, "\x20" );

	if(con)
		nlines = console_get_lines_count( con );

	player_set_real_cols_count( this, PLAYER_GRAPHMODE_ALLEGRO_COLS_COUNT );
	player_set_real_rows_count( this, PLAYER_GRAPHMODE_ALLEGRO_ROWS_COUNT + (font_height * nlines) );

	player_set_screen_cols_count( this, PLAYER_GRAPHMODE_ALLEGRO_COLS_COUNT );
	player_set_screen_rows_count( this, PLAYER_GRAPHMODE_ALLEGRO_ROWS_COUNT );

	if(con)
	{
		player_set_console_dimension( this, PLAYER_GRAPHMODE_ALLEGRO_COLS_COUNT, font_height * nlines );
		player_set_console_position( this, 0, PLAYER_GRAPHMODE_ALLEGRO_ROWS_COUNT );
	}

	ret = allegro_init();

	if( ret != 0 )
		return -1;

	set_color_depth( 8 );

	set_window_title( PLAYER_GRAPHMODE_ALLEGRO_DESC );

	data->buffer = create_bitmap(	player_get_real_cols_count(this),
									player_get_real_rows_count(this) );

	if( !data->buffer )
	{
		allegro_exit();
		return -1;
	}

	if( con )
	{
		int console_nrows = 0;
		int console_ncols = 0;

		player_get_console_dimension( this, &console_ncols, &console_nrows );

		data->console_buffer = create_bitmap(	console_ncols,
												console_nrows );

		if( !data->console_buffer )
		{
			allegro_exit();
			return -1;
		}
	}

	ret = set_gfx_mode( GFX_AUTODETECT_WINDOWED,
						player_get_real_cols_count(this),
						player_get_real_rows_count(this),
						0,
						0 );
	if( ret < 0 )
	{
		destroy_bitmap( data->buffer );
		allegro_exit();
		return -1;
	}

	return 0;
}


static void player_graphmode_allegro_finish( player_t * this )
{
	player_graphmode_allegro_data_t * data = player_get_data( this );

	destroy_bitmap( data->buffer );
	destroy_bitmap( data->console_buffer );
	allegro_exit();
}


static void player_graphmode_allegro_set_palette( player_t * this, palette_t * pal )
{
	int i = 0;
	int count = 0;
	PALETTE palette;

	count = palette_get_color_count( pal );

	for( i = 0; i < count; i++ )
		palette_get_color( pal, i, &palette[i].r, &palette[i].g, &palette[i].b );

	vsync();

	set_palette( palette );
}


static void player_graphmode_allegro_render_frame( player_t * this, frame_t * frm )
{
	int col = 0;
	int row = 0;
	int ncols = 0;
	int nrows = 0;
	frame_point_t pt;


	player_graphmode_allegro_data_t * data = player_get_data( this );

	frame_get_dimensions( frm, &ncols, &nrows );

	for( row = 0; row < nrows; row++ )
	{
		for( col = 0; col < ncols; col++ )
		{
			frame_get_point( frm, col, row, &pt );

			putpixel(	data->buffer,
						col,
						row,
						pt.color );
		}
	}

	blit(	data->buffer,
			screen,
			0,
			0,
			0,
			0,
			ncols,
			nrows );
}


static void player_graphmode_allegro_refresh_console( player_t * this )
{
	int i = 0;
	int txt_height = 0;
	int count = 0;
	int ydim = 0;
	int ypos = 0;
	int xdim = 0;
	int xpos = 0;
	console_t * con = player_get_console( this );
	player_graphmode_allegro_data_t * data = player_get_data( this );


	if( !con )
		return;

	txt_height = text_height( font );

	count = console_get_lines_count( con );

	clear_bitmap( data->console_buffer );

	for( i = 0; i < count; i++ )
	{
		textprintf_ex(	data->console_buffer,
						font,
						0,
						txt_height * i,
						makecol( 255, 255, 255 ),
						makecol( 0, 0, 0 ),
						"%s",
						console_get_line( con, i ) );
	}

	player_get_console_position( this, &xpos, &ypos );
	player_get_console_dimension( this, &xdim, &ydim );

	blit(	data->console_buffer,
			screen,
			0,
			0,
			xpos,
			ypos,
			xdim,
			ydim );
}

/* $Id: player_graphmode_allegro.c 304 2015-08-08 00:57:58Z tiago.ventura $ */
