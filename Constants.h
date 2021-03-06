#ifndef CONSTANTS_H
#define CONSTANTS_H

#define __CONSTANT const static unsigned int
#define __S_CONSTANT const static int
#define __FCONSTANT const static float

__CONSTANT MAX_NO_BUFFERS = 512;
__CONSTANT MAX_RENDERERS = 6;

// time constants
__CONSTANT ZERO_TIME = 0;
__CONSTANT ONE_SECOND_IN_MS = 1000;
__CONSTANT CLOSE_TIMEOUT_COUNT = 10;

// mask/bit constants
__CONSTANT  THIRTY_TWO_BIT_BIT_MASK = 0xFFFFFFFF;
__CONSTANT	UNKNOWN_BUFFER = 0xFFFFFFFF;

// buffer constants
__CONSTANT  NO_BUFFERS_PROCESSED = 0;
__CONSTANT  DEFAULT_BUFFER = 0;
__S_CONSTANT  ERROR_BUFFER = -1;
__CONSTANT  GET_ONE_BUFFER = 1;
__CONSTANT  NO_BUFFERS = 0;
__CONSTANT  EMPTY_THE_BUFFER = 0;
__CONSTANT  MINIMUM_BUFFERS = 2;
__CONSTANT  MINIMUM_BUFFER_SIZE = 1024 * 8;
__CONSTANT	MAXIMUM_BUFFER_SIZE = 8192;
__CONSTANT	TEMP_BUFFER_SIZE = MAXIMUM_BUFFER_SIZE * 2;
__CONSTANT	MAXIMUM_BUFFER_OFFSET = 1;
__CONSTANT	PREBUFFER_LIMIT = 3;

// audio constants
__CONSTANT  NO_SAMPLE_RATE = 0;
__CONSTANT  NO_BITS_PER_SAMPLE = 0;
__CONSTANT  NO_NUMBER_OF_CHANNELS = 0;

__CONSTANT  SHIFT_BITS_TO_BYTES = 3;
__CONSTANT	ONE_BYTE_SAMPLE = 1;
__CONSTANT	TWO_BYTE_SAMPLE = 2;
__CONSTANT	FOUR_BYTE_SAMPLE = 4;

__CONSTANT	EIGHT_BIT_PER_SAMPLE = 8;
__CONSTANT	SIXTEEN_BIT_PER_SAMPLE = 16;

__CONSTANT	DEFC_DEVICE = 0;
__CONSTANT	DEFC_BUFFER_LENGTH = 2000;
__CONSTANT	CONF_BUFFER_LENGTH_MIN = 250;
__CONSTANT	CONF_BUFFER_LENGTH_MAX = 6000;

__CONSTANT	VOLUME_DIVISOR = 255;
__FCONSTANT	VOLUME_MIN = 0.0;
__FCONSTANT	VOLUME_MAX = 1.0;

__CONSTANT IS_PLAYING = 1;
__CONSTANT IS_NOT_PLAYING = 0;

typedef struct
{
	float x;
	float y;
	float z;
} speaker_T;

typedef struct 
{
	speaker_T position;
	speaker_T direction;
	speaker_T speakers[MAX_RENDERERS];
} speaker_matrix_T;

static const speaker_matrix_T DEFAULT_MATRIX = 
{
	/*	 x.x	,y.y ,	z.z	*/
	{	// position
		0.0	,0.0,	0.0
	},
	{	// direction
		0.0	,0.0,	0.0
	},
	{	// speakers
		{-12.50	,0.0,	-10.0},		// FL
		{12.50	,0.0,	-10.0},		// FR
		{-12.50	,0.0,	10.0},		// RL
		{12.50	,0.0,	10.0},		// RR
		{0.0	,0.0,	-10.0},		// C
		{0.0	,0.0,	0.0}		// S
	}
};

#define MATRIX_BUFFER_SIZE 11u
#define MATRIX_AXIS_POSITION 7u
#define MATRIX_RENDERER_POSITION 9u

#endif