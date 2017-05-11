/*!
	\file player_graphmode_sdl.c
	\brief Animation Player in Graphical Mode Object (SDL)
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#include <SDL.h>
#include <SDL_ttf.h>

#include "frame.h"
#include "player.h"
#include "player_graphmode_sdl.h"


#define PLAYER_GRAPHMODE_SDL_DESC                  "Standard 640x480x256 (SDL)"
#define PLAYER_GRAPHMODE_SDL_COLS_COUNT            (640)
#define PLAYER_GRAPHMODE_SDL_ROWS_COUNT            (480)
#define PLAYER_GRAPHMODE_SDL_FONT_SIZE             (8)
#define PLAYER_GRAPHMODE_SDL_FONT_FILE             "./felix.ttf"


struct player_graphmode_sdl_data_s
{
	SDL_Surface * screen;
	TTF_Font * font;
};

typedef struct player_graphmode_sdl_data_s player_graphmode_sdl_data_t;


static player_t * player_graphmode_sdl_create( player_t * parent );
static void player_graphmode_sdl_destroy( player_t * this );
static int player_graphmode_sdl_initialize( player_t * this );
static void player_graphmode_sdl_finish( player_t * this );
static void player_graphmode_sdl_set_palette( player_t * this, palette_t * pal );
static void player_graphmode_sdl_render_frame( player_t * this, frame_t * frm );
static void player_graphmode_sdl_refresh_console( player_t * this );


player_implementation_t * player_graphmode_sdl_get_implementation( void )
{
	static player_implementation_t impl;

	impl.create = player_graphmode_sdl_create;
	impl.destroy = player_graphmode_sdl_destroy;
	impl.screen_initialize = player_graphmode_sdl_initialize;
	impl.screen_finish = player_graphmode_sdl_finish;
	impl.set_palette = player_graphmode_sdl_set_palette;
	impl.render_frame = player_graphmode_sdl_render_frame;
	impl.refresh_console = player_graphmode_sdl_refresh_console;

	return &impl;
};


static player_t * player_graphmode_sdl_create( player_t * parent )
{
	player_graphmode_sdl_data_t * data = NULL;

	data = (player_graphmode_sdl_data_t*) calloc( 1, sizeof(player_graphmode_sdl_data_t) );

	if( !data )
		return NULL;

	if( TTF_Init() == -1 )
	{
		free( data );
		return NULL;
	}

	data->font = TTF_OpenFont( PLAYER_GRAPHMODE_SDL_FONT_FILE, PLAYER_GRAPHMODE_SDL_FONT_SIZE );

	if( !data->font )
	{
		TTF_Quit();
		free( data );

		return NULL;
	}

	player_set_data( parent, (void*) data );
	player_set_screen_format( parent, player_screen_format_graphic );
	player_set_screen_cols_count( parent, PLAYER_GRAPHMODE_SDL_COLS_COUNT );
	player_set_screen_rows_count( parent, PLAYER_GRAPHMODE_SDL_ROWS_COUNT );

	return parent;
}


static void player_graphmode_sdl_destroy( player_t * this )
{
	player_graphmode_sdl_data_t * data = player_get_data( this );

	TTF_CloseFont( data->font );
	TTF_Quit();

	free( data );
}


static int player_graphmode_sdl_initialize( player_t * this )
{
	int nlines = 0;
	int ncols = 0;
	int nrows = 0;
	int font_width = 0;
	int font_height = 0;
	player_graphmode_sdl_data_t * data = player_get_data( this );
	console_t * con = player_get_console( this );

	if(con)
		nlines = console_get_lines_count( con );

	TTF_SizeText(	data->font,
					"\x20",
					&font_width,
					&font_height );

	player_get_screen_dimensions( this, &ncols, &nrows );

	player_set_real_cols_count( this, ncols );
	player_set_real_rows_count( this, nrows + (font_height * nlines ) );

	if(con)
	{
		player_set_console_dimension( this, ncols, font_height * nlines );
		player_set_console_position( this, 0, nrows );
	}

	if(SDL_Init( SDL_INIT_VIDEO ))
		return -1;

	SDL_WM_SetCaption(	PLAYER_GRAPHMODE_SDL_DESC,
						"FelixTheCat" );

	data->screen = SDL_SetVideoMode( 	player_get_real_cols_count(this),
										player_get_real_rows_count(this),
										8,
										SDL_DOUBLEBUF | SDL_SWSURFACE | SDL_HWPALETTE );
	if( !data->screen )
	{
		SDL_Quit();
		return -1;
	}

	return 0;
}


static void player_graphmode_sdl_finish( player_t * this )
{
	player_graphmode_sdl_data_t * data = player_get_data( this );

	SDL_FreeSurface( data->screen );
	SDL_Quit();
}


static void player_graphmode_sdl_set_palette( player_t * this, palette_t * pal )
{
	int i = 0;
	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;
	int count = 0;
	SDL_Color * colors = NULL;
	player_graphmode_sdl_data_t * data = player_get_data( this );

	count = palette_get_color_count( pal );

	colors = (SDL_Color*) calloc( count, sizeof(SDL_Color) );

	if(!colors)
		return;

	for( i = 0; i < count; i++ )
	{
		palette_get_color( pal, i, &red, &green, &blue );

		colors[i].r = red;
		colors[i].g = green;
		colors[i].b = blue;
	}

	SDL_SetColors(	data->screen,
					colors,
					0,
					count );

	free( colors );
}


static void player_graphmode_sdl_render_frame( player_t * this, frame_t * frm )
{
	int col = 0;
	int row = 0;
	int ncols = 0;
	int nrows = 0;
	Uint8 * video_buffer = NULL;
	int i = 0;
	frame_point_t pt;
	player_graphmode_sdl_data_t * data = player_get_data( this );

	frame_get_dimensions( frm, &ncols, &nrows );

	video_buffer = (Uint8*) data->screen->pixels;

	for( row = 0; row < nrows; row++ )
	{
		for( col = 0; col < ncols; col++ )
		{
			frame_get_point( frm, col, row, &pt );
			video_buffer[ i++ ] = pt.color;
		}
	}

	SDL_Flip( data->screen );
}


static void player_graphmode_sdl_refresh_console( player_t * this )
{
	int i = 0;
	int txt_height = 0;
	int count = 0;
	int ydim = 0;
	int ypos = 0;
	int xdim = 0;
	int xpos = 0;
	SDL_Surface * text = NULL;
	SDL_Color color = { .r = 255, .g = 255, .b = 255 };
	SDL_Rect location = { .x = 0, .y = 0, .w = 0, .h = 0 };
	player_graphmode_sdl_data_t * data = player_get_data( this );
	console_t * con = player_get_console( this );

	if( !con )
		return;

	player_get_console_position( this, &xpos, &ypos );
	player_get_console_dimension( this, &xdim, &ydim );

	txt_height = TTF_FontHeight(data->font);
	count = console_get_lines_count( con );

	location.x = xpos;
	location.y = ypos;
	location.w = xdim;
	location.h = ydim;

	SDL_FillRect( data->screen, &location, 0 );

	for( i = 0; i < count; i++ )
	{
		text = TTF_RenderText_Solid(	data->font,
										console_get_line( con, i ),
										color );

		location.x = xpos;
		location.y = ypos + (txt_height * i);
		location.w = xdim;
		location.h = txt_height;

		SDL_BlitSurface(	text,
							NULL,
							data->screen,
							&location );
	}

	SDL_Flip( data->screen );
}

/* $Id: player_graphmode_sdl.c 302 2015-08-06 17:04:55Z tiago.ventura $ */
