#pragma once

#include "Constants.h"
#include "Framework\Framework.h"
#include "Clock.h"

namespace WinampOpenALOut {

	public class Output_Wumpus {

		typedef struct
		{
			ALuint buffer_id;
			bool available;
			unsigned int size;
		} buffer_type;

	public:
		Output_Wumpus();
		~Output_Wumpus();
		void Config(HWND hwnd);
		void About(HWND hwnd);
		void Initialise(HWND window);
		void Quit();
		int	Open(int samplerate, int numchannels, int bitspersamp, int bufferlenms, int prebufferms);
		void Close();
		int Write(char *buf, int len);
		int CanWrite();
		int IsPlaying();
		int Pause(int pause);
		void SetVolume(int newVolume);
		void SetPan(int newPan);
		void Flush(int newTimeMs);
		int inline	GetOutputTime();
		int GetWrittenTime();

		void SwitchOutputDevice(int device);
		

		inline bool IsStreamOpen()						{ return streamOpen; }

		inline unsigned int	GetSampleRate()				{ return sampleRate; }
		inline unsigned int GetBitsPerSample()			{ return bitsPerSample; }
		inline unsigned int GetNumberOfChannels()		{ return numberOfChannels; }
		inline int GetLastOutputTime()					{ return lastOutputTime; }
		inline int GetLastWrittenTime()					{ return lastWrittenTime; }

		inline unsigned int GetNumberOfBuffers()		{ return noBuffers; }

		inline int GetConfBufferLength() { return c_bufferLength;}
		inline void SetConfBufferLength( unsigned int i) { c_bufferLength = i;}

		inline bool IsStereoExpanded() { return stereoExpand;}
		inline void SetStereoExpanded(bool expanded);

		inline bool IsMonoExpanded() { return monoExpand;}
		inline void SetMonoExpanded(bool expanded);

		inline bool IsXRAMEnabled() { return xram_enabled; }
		inline void SetXRAMEnabled( bool enabled );

	protected:

		inline void onError();
		void Relocate(int device, int currentPosition);

		void CheckProcessedBuffers();
		void CheckPlayState();
		int SetBufferTime(int tMs);

		void SetVolumeInternal(ALfloat newVolume);

			// semaphore for the right access to buffers/open_al api
		CRITICAL_SECTION criticalSection;

		int inline get_current_output_time(__int64 currentOutputTime, unsigned int sampleRate);
		int inline get_current_written_time(__int64 currentOutputTime, unsigned int sampleRate);

		// boolean to store if winamp is allowed to write more data
		bool			canWrite;
		// boolean to store internal playing state
		bool			isPlaying;
		// boolean to store if the file steam is open and
		// thread is running
		bool			streamOpen;
		// boolean for prebuffering state at the start to get as much data
		// as possible
		bool			preBuffer;
		unsigned int	preBufferNumber;

		unsigned int	buffer_free;

		bool			xram_enabled;

		// integer to store the sample rate
		unsigned int	sampleRate;
		// integer to store the number of channels
		unsigned int	numberOfChannels;
		// integer to store the bits per second
		unsigned int	bitsPerSample;
		// integer to store the number of buffers we'll use
		unsigned int	noBuffers;
		// integer to store bytes per sample (optimisation
		unsigned int	bytesPerSampleChannel;
		// integer to store the last pause state
		int				lastPause;

		// floating point value to store the volume
		ALfloat			volume;

		// store the format
		unsigned long	ulFormat;

		// long int to store time written
		__int64		total_written;
		// long int to store time played
		__int64		total_played;
		// integer to store the where we are right now
		__int64		currentOutputTime;
		// integer to store the where we've buffered to
		__int64		currentWrittenTime;

		int			lastOutputTime;
		int			lastWrittenTime;

		// the open al buffers themselves
		buffer_type	    uiBuffers[MAX_NO_BUFFERS];
		// integer used to reference the open al source
		ALuint		    uiSource;

		Time_Type		lastCheckBuffers;
		Time_Type		lastCheckDelay;

		// used to store the configuration buffer length
		unsigned int	c_bufferLength;

		bool			stereoExpand;
		bool			monoExpand;

	private:

		void log_debug_msg(char* msg);
	};

}
