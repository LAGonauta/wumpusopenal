#pragma once

#define __CONSTANT const static unsigned int
#define __S_CONSTANT const static int

__CONSTANT MAX_NO_BUFFERS = 255;

// time constants
__CONSTANT ZERO_TIME = 0;
__CONSTANT ONE_SECOND_IN_MS = 1000;
__CONSTANT PREBUFFER_WRITE_DELAY = 100;

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
//__CONSTANT	MAXIMUM_BUFFER_SIZE = 131072;
__CONSTANT	MAXIMUM_BUFFER_SIZE = 8192;
__CONSTANT	MAXIMUM_BUFFER_OFFSET = 1;

// audio constants
__CONSTANT  NO_SAMPLE_RATE = 0;
__CONSTANT  NO_BITS_PER_SAMPLE = 0;
__CONSTANT  NO_NUMBER_OF_CHANNELS = 0;

__CONSTANT  SHIFT_BITS_TO_BYTES = 3;
__CONSTANT	TWO_BYTE_SAMPLE = 2;
__CONSTANT	FOUR_BYTE_SAMPLE = 4;

__CONSTANT	EIGHT_BIT_PER_SAMPLE = 8;
__CONSTANT	SIXTEEN_BIT_PER_SAMPLE = 16;

__CONSTANT	DEFC_DEVICE = 0;
__CONSTANT	DEFC_BUFFER_LENGTH = 2000;
__CONSTANT	CONF_BUFFER_LENGTH_MIN = 1000;
__CONSTANT	CONF_BUFFER_LENGTH_MAX = 4000;
