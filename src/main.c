/*!
	\file main.c
	\brief Felix The Cat Entry Point

	\mainpage Felix The Cat Project v1.00

	\image html felix.jpg

	Felix the Cat is a funny animal cartoon character created in the
	silent film era. The anthropomorphic black cat with his black body, white
	eyes, and giant grin, coupled with the surrealism of the situations in which
	his cartoons place him, combine to make Felix one of the most recognized
	cartoon characters in film history. Felix was the first character from
	animation to attain a level of popularity sufficient to draw movie audiences.
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "felix.h"


/* ************************************************************************** */
/* *                              GLOBALS                                   * */
/* ************************************************************************** */

filter_implementation_t * g_filter_impl = NULL;
player_implementation_t * g_player_impl = NULL;
animation_implementation_t * g_animation_impl = NULL;


/* ************************************************************************** */
/* *                          PRIVATE PROTOTYPES                            * */
/* ************************************************************************** */

static int main_initialize( int argc, char * argv[] );
static void main_signal_handler( int sig );
static int show_syntax( char ** argv );


/* ************************************************************************** */
/* *                            IMPLEMENTATION                              * */
/* ************************************************************************** */

static void main_signal_handler( int sig )
{
	player_t * p = NULL;

	fprintf( stdout, "\nSignal Received: %s (%d)\n\n", strsignal(sig), sig );

	switch( sig )
	{
		case SIGWINCH :
		{
			break;
		}

		case SIGABRT  :
		case SIGINT   :
		case SIGQUIT  :
		case SIGTERM  :
		{
			p = player_get_instance();

			if( p )
				player_stop( p );

			break;
		}

		default:
		{
			break;
		}
	}
}


static int show_syntax( char ** argv )
{
	printf( "usage:\n" );
	printf( "	%s\n", argv[0] );
	printf("		-a	life, tvstatic, fire, fern, spirograph, lissajous, starfield, matrix, swarm\n");
	printf("		-p	sdl, allegro, modex, text\n");
	printf("		-f	blur, noise\n");
	printf("\n");

	return 0;
}


static int main_initialize( int argc, char * argv[] )
{
	int parm = 0;
	int syntax_error = 0;

	if( argc <= 1 )
	{
		show_syntax( argv );
		exit(1);
	}

	signal( SIGWINCH, main_signal_handler );
	signal( SIGABRT, main_signal_handler );
	signal( SIGINT, main_signal_handler );
	signal( SIGQUIT, main_signal_handler );
	signal( SIGTERM, main_signal_handler );

	opterr = 0;

	while( ( parm = getopt ( argc, argv, "p:a:f:ch" ) ) != -1 )
	{
		switch( parm )
		{
			case 'p': /* Player */
			{
				if( !strcmp("allegro",optarg) )
				{
					//g_player_impl = player_graphmode_allegro_get_implementation();
				}
				else if( !strcmp("sdl",optarg) )
				{
					g_player_impl = player_graphmode_sdl_get_implementation();
				}
				else if( !strcmp("text",optarg) )
				{
					g_player_impl = player_textmode_allegro_get_implementation();
				}
				else if( !strcmp("modex",optarg) )
				{
					//g_player_impl = player_graphmode_modex_allegro_get_implementation();
				}
				else
				{
					syntax_error = 1;
				}

				break;
			}

			case 'a': /* Animation */
			{
				if( !strcmp("life",optarg) )
				{
					g_animation_impl = animation_lifegame_get_implementation();
				}
				else if( !strcmp("tvstatic",optarg) )
				{
					g_animation_impl = animation_tvstatic_get_implementation();
				}
				else if( !strcmp("fire",optarg) )
				{
					g_animation_impl = animation_fire_get_implementation();
				}
				else if( !strcmp("fern",optarg) )
				{
					g_animation_impl = animation_fern_fractal_get_implementation();
				}
				else if( !strcmp("spirograph",optarg) )
				{
					g_animation_impl = animation_spirograph_get_implementation();
				}
				else if( !strcmp("lissajous",optarg) )
				{
					g_animation_impl = animation_lissajous_get_implementation();
				}
				else if( !strcmp("starfield",optarg) )
				{
					g_animation_impl = animation_starfield_get_implementation();
				}
				else if( !strcmp("matrix",optarg) )
				{
					g_animation_impl = animation_matrix_get_implementation();
				}
				else if( !strcmp("swarm",optarg) )
				{
					//g_animation_impl = animation_swarm_get_implementation();
				}
				else
				{
					syntax_error = 1;
				}

				break;
			}

			case 'f': /* Filter */
			{
				if( !strcmp("blur",optarg) )
				{
					g_filter_impl = filter_blur_get_implementation();
				}
				else if( !strcmp("noise",optarg) )
				{
					g_filter_impl = filter_noise_get_implementation();
				}
				else
				{
					syntax_error = 1;
				}

				break;
			}

			case 'c': /* Console */
			{
				console_create( 10 );
				break;
			}

			case 'h': /* Help/Syntax */
			{
				syntax_error = 1;
				break;
			}

			case '?':
			default :
			{
				syntax_error = 1;
				break;
			}
		}
	}

	if( syntax_error )
	{
		show_syntax( argv );
		exit(1);
	}

	return 0;
}


/* ************************************************************************** */
/* *                              main()                                    * */
/* ************************************************************************** */

int main( int argc , char * argv[] )
{
	filter_t * f = NULL;
	animation_t * a = NULL;
	player_t * p = NULL;
	console_t * c = NULL;

	main_initialize( argc, argv );

	p = player_create( g_player_impl );
	f = filter_create( g_filter_impl );
	a = animation_create( g_animation_impl );
	c = console_get_instance();

	player_set_animation( p, a );
	player_set_filter( p, f );
	player_set_console( p, c );

	animation_set_console( a, c );

	player_screen_initialize( p );
	player_play( p );
	player_screen_finish( p );

	console_destroy( c );
	animation_destroy( a );
	filter_destroy( f );
	player_destroy( p );

	return EXIT_SUCCESS;
}

/* $Id: main.c 551 2016-09-30 22:09:22Z tiago.ventura $ */
