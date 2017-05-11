# **************************************************************
# *                                                            *
# *                       Makefile                             *
# *                                                            *
# **************************************************************

SRC_PATH=./src
EXECUTABLE=felix

#General Flags
GENERAL_CFLAGS= -c -Wall -I./src
GENERAL_LDFLAGS= -lpthread -lm

#Debug Flags
DEBUG_CFLAGS= -g -D_DEBUG=1
DEBUG_LDFLAGS=

#SDL - Simple DirectMedia Layer Library Flags
SDL_CFLAGS= $(shell sdl-config --cflags)
SDL_LDFLAGS= $(shell sdl-config --libs) -lSDL_ttf

#Allegro 5 Library Flags
ALLEGRO5_CFLAGS=
ALLEGRO5_LDFLAGS= -lallegro -lallegro_font -lallegro_ttf -lallegro_primitives

# Compilation and Linking Flags
CFLAGS= $(GENERAL_CFLAGS) $(SDL_CFLAGS) $(ALLEGRO5_CFLAGS)
LDFLAGS= $(GENERAL_LDFLAGS) $(SDL_LDFLAGS) $(ALLEGRO5_LDFLAGS)

#Debug Mode Control
ifeq ($(DEBUG),1)
    CFLAGS += $(DEBUG_CFLAGS)
    LDFLAGS += $(DEBUG_LDFLAGS)
endif

#Source Files
SOURCES=$(SRC_PATH)/main.c                             \
        $(SRC_PATH)/frame.c                            \
        $(SRC_PATH)/palette.c                          \
        $(SRC_PATH)/console.c                          \
        $(SRC_PATH)/filter.c                           \
        $(SRC_PATH)/filter_blur.c                      \
        $(SRC_PATH)/filter_noise.c                     \
        $(SRC_PATH)/animation.c                        \
        $(SRC_PATH)/player.c                           \
        $(SRC_PATH)/player_textmode_allegro.c          \
        $(SRC_PATH)/player_graphmode_sdl.c             \
        $(SRC_PATH)/animation_tvstatic.c               \
        $(SRC_PATH)/animation_lifegame.c               \
        $(SRC_PATH)/animation_fernfractal.c            \
        $(SRC_PATH)/animation_fire.c                   \
        $(SRC_PATH)/animation_starfield.c              \
        $(SRC_PATH)/animation_lissajous.c              \
        $(SRC_PATH)/animation_spirograph.c             \
        $(SRC_PATH)/animation_matrix.c                 
        #$(SRC_PATH)/animation_swarm.c


#        $(SRC_PATH)/player_graphmode_allegro.c         \
#        $(SRC_PATH)/player_graphmode_modex_allegro.c   \


OBJECTS=$(SOURCES:.c=.o)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(SRC_PATH)/*.o ./$(EXECUTABLE)

# $Id: Makefile 551 2016-09-30 22:09:22Z tiago.ventura $
