#ifndef SSE_BUILD
namespace WinampOpenALOut {
#endif

#define CONF_TRUE "true"
#define CONF_FALSE "false"

#define CONF_DEVICE "DefaultDevice"
#define CONF_EFX "EffectsEnabled"
#define CONF_BUFFER_LENGTH "BufferLength"
#define CONF_MONO_EXPAND "MonoExpansion"
#define CONF_STEREO_EXPAND "StereoExpansion"

#ifndef SSE_BUILD
	public class ConfigFile {
#else
	class ConfigFile {
#endif

	private:
		static char INI_FILE[MAX_PATH];
	public:
		static void Initialise(HWND hMainWindow);

		static void ReadString(char *name,char *data, int mlen);
		static void WriteString(char *name,char *data);
		
		static int ReadInteger(char *name);
		static void WriteInteger(char *name, int i);

		static bool ReadBoolean(char *name);
		static void WriteBoolean(char* name, bool v);
	};
#ifndef SSE_BUILD
}
#endif