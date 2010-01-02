#include "Out_Effects.h"
#include "Framework\Framework.h"

static const EAXREVERBPROPERTIES REVERB_LOOKUP_TABLE[NO_OF_EFFECTS] = 
{
	REVERB_PRESET_GENERIC,
	REVERB_PRESET_PADDEDCELL,
	REVERB_PRESET_ROOM,
	REVERB_PRESET_BATHROOM,
	REVERB_PRESET_LIVINGROOM,
	REVERB_PRESET_STONEROOM,
	REVERB_PRESET_AUDITORIUM,
	REVERB_PRESET_CONCERTHALL,
	REVERB_PRESET_CAVE,
	REVERB_PRESET_ARENA,
	REVERB_PRESET_HANGAR,
	REVERB_PRESET_CARPETTEDHALLWAY,
	REVERB_PRESET_HALLWAY,
	REVERB_PRESET_STONECORRIDOR,
	REVERB_PRESET_ALLEY,
	REVERB_PRESET_FOREST,
	REVERB_PRESET_CITY,
	REVERB_PRESET_MOUNTAINS,
	REVERB_PRESET_QUARRY,
	REVERB_PRESET_PLAIN,
	REVERB_PRESET_PARKINGLOT,
	REVERB_PRESET_SEWERPIPE,
	REVERB_PRESET_UNDERWATER,
	REVERB_PRESET_DRUGGED,
	REVERB_PRESET_DIZZY,
	REVERB_PRESET_PSYCHOTIC,
	REVERB_PRESET_OUTDOORS_BACKYARD,
	REVERB_PRESET_OUTDOORS_ROLLINGPLAINS,
	REVERB_PRESET_OUTDOORS_DEEPCANYON,
	REVERB_PRESET_OUTDOORS_CREEK,
	REVERB_PRESET_OUTDOORS_VALLEY,
	REVERB_PRESET_MOOD_HEAVEN,
	REVERB_PRESET_MOOD_HELL,
	REVERB_PRESET_MOOD_MEMORY,
	REVERB_PRESET_DRIVING_INCAR_RACER,
	REVERB_PRESET_DRIVING_INCAR_LUXURY,
	REVERB_PRESET_DRIVING_TUNNEL,
	REVERB_PRESET_CITY_STREETS,
	REVERB_PRESET_CITY_SUBWAY,
	REVERB_PRESET_CITY_MUSEUM,
	REVERB_PRESET_CITY_LIBRARY,
	REVERB_PRESET_CITY_UNDERPASS,
	REVERB_PRESET_CITY_ABANDONED,
	REVERB_PRESET_DUSTYROOM,
	REVERB_PRESET_CHAPEL,
	REVERB_PRESET_SMALLWATERROOM
};

namespace WinampOpenALOut {

	Output_Effects::Output_Effects()
	{
		is_on = false;
		is_loaded = false;
		status = EAX_NOT_LOADED;
		
		channels = 0;
		for(unsigned char c = 0; c < MAX_NO_CHANNELS ; c++ )
		{
			source[c] = 0;
		}

		effect = EFX_REVERB_PRESET_GENERIC;
		effect_slot = 0;
		the_effect = 0;

		memset(
			&efx_reverb,
			0,
			sizeof(efx_reverb));
	}

	Output_Effects::~Output_Effects()
	{
		if ( is_loaded == true )
		{
			this->OnClose();
		}
	}

	eax_load_status Output_Effects::Setup(void)
	{
		eax_load_status retval = EAX_NOT_SUPPORTED;

		if ( this->is_on == true )
		{
			if ( this->is_loaded == EAX_LOADED_OK )
			{
				// if we're setup already we need to shutdown
				// and start again with new slots for new sources
				this->OnClose();
			}

			alGetError();

			if ( Framework::getInstance()->ALFWIsEFXSupported() == AL_TRUE )
			{
				if (CreateAuxEffectSlot(&effect_slot) == AL_TRUE )
				{
					if (CreateEffect(&the_effect, AL_EFFECT_EAXREVERB))
					{

						/*
						 * once a reverb effect is loaded, use the configuration 
						 * from the table.
						 */

						EAXREVERBPROPERTIES eaxEffect = REVERB_LOOKUP_TABLE[effect];
						EFXEAXREVERBPROPERTIES efxReverb;
						ConvertReverbParameters(&eaxEffect, &efxReverb);

						if (SetEFXEAXReverbProperties(&efxReverb, the_effect))
						{
							alAuxiliaryEffectSloti(effect_slot, AL_EFFECTSLOT_EFFECT, the_effect);
							this->is_loaded = true;

							retval = EAX_LOADED_OK;
						}
						else
						{
							// failed to set the EAX properties
							retval = EAX_FAIL_PROPERTIES;
						}
					}
					else
					{
						// failed to create the effect
						retval = EAX_FAIL_CREATE_EFFECT;
					}
				}
				else
				{
					// failed to create an effect slot
					retval = EAX_FAIL_EFFECT_SLOT;
				}
			}
			else
			{
				// eax not supported
				retval = EAX_NOT_SUPPORTED;
			}
		}

		if ( retval != EAX_LOADED_OK )
		{
			this->CleanUp(retval);
		}

		return retval;
	}

	void Output_Effects::AddSource(ALuint the_source)
	{
		/*
		 * keep track of the source and add the source to the list of effects slots
		 */
		source[channels++] = the_source;
		alSource3i(the_source, AL_AUXILIARY_SEND_FILTER, effect_slot, 0, AL_FILTER_NULL);
	}

	void Output_Effects::OnClose()
	{
		/*
		 * remove each source from the effects slot
		 */
		for( unsigned char c = 0; c < channels ; c++ )
		{
			alSource3i(source[c], AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, 0, AL_FILTER_NULL);
		}
		channels = 0;

		this->CleanUp(status);
	}

	void Output_Effects::CleanUp(eax_load_status close)
	{
		switch(close)
		{
			case EAX_LOADED_OK:
				// nothing to do
				// intentional fall-through
			case EAX_FAIL_PROPERTIES:
				// nothing to remove, just remove the effect slot
				// intentional fall-through
			case EAX_FAIL_CREATE_EFFECT:
				alAuxiliaryEffectSloti(effect_slot, AL_EFFECTSLOT_EFFECT, AL_EFFECT_NULL);
				// intentional fall-through
			case EAX_FAIL_EFFECT_SLOT:
				alDeleteEffects(1, &the_effect);
				alDeleteAuxiliaryEffectSlots(1, &effect_slot);
				break;
			case EAX_NOT_SUPPORTED:
				// intentional fall-through
			case EAX_NOT_LOADED:
				// nothing to do
				break;
		};
			
		this->is_loaded = false;
	}

	effects_list Output_Effects::GetCurrentEffect(void)
	{
		return effect;
	}

	void Output_Effects::SetCurrentEffect(effects_list an_effect)
	{
		unsigned char old_channels = channels;
		
		this->OnClose();
		effect = an_effect;

		this->Setup();

		for(unsigned char c = 0 ; c < old_channels ; c++ )
		{
			this->AddSource(source[c]);
		}
	}

	bool Output_Effects::IsEnabled(void)
	{
		return is_on;
	}

	bool Output_Effects::Enable(bool enable)
	{
		bool retval = false;

		if ( enable == false &&
			is_loaded == true )
		{
			this->OnClose();
		}

		if ( enable != is_on )
		{
			unsigned char old_channels = channels;

			this->OnClose();
			is_on = enable;

			// failed to setup effects
			if ( this->Setup() != EAX_LOADED_OK )
			{
				is_on = false;
				is_loaded = false;
			}
			else
			{
				for(unsigned char c = 0 ; c < old_channels ; c++ )
				{
					this->AddSource(source[c]);
				}

				retval = true;
			}
		}
		else
		{
			retval = true;
		}

		return retval;
	}

	ALboolean Output_Effects::CreateAuxEffectSlot(ALuint *aux_effect_slot)
	{
		ALboolean bReturn = AL_FALSE;
		ALenum err = AL_NO_ERROR;

		// Clear AL Error state
		alGetError();

		// Generate an Auxiliary Effect Slot
		alGenAuxiliaryEffectSlots(1, aux_effect_slot);
		err = alGetError();
		if (err == AL_NO_ERROR)
		{
			bReturn = AL_TRUE;
		}

		return bReturn;
	}

	ALboolean Output_Effects::CreateEffect(ALuint *effect, ALenum effect_type)
	{
		ALboolean bReturn = AL_FALSE;

		if (effect)
		{
			// Clear AL Error State
			alGetError();

			// Generate an Effect
			alGenEffects(1, effect);
			if (alGetError() == AL_NO_ERROR)
			{
				// Set the Effect Type
				alEffecti(*effect, AL_EFFECT_TYPE, effect_type);
				if (alGetError() == AL_NO_ERROR)
					bReturn = AL_TRUE;
				else
					alDeleteEffects(1, effect);
			}
		}

		return bReturn;
	}

	ALboolean Output_Effects::SetEFXEAXReverbProperties(EFXEAXREVERBPROPERTIES *pEFXEAXReverb, ALuint uiEffect)
	{
		ALboolean bReturn = AL_FALSE;

		if (pEFXEAXReverb)
		{
			// Clear AL Error code
			alGetError();

			alEffectf(uiEffect, AL_EAXREVERB_DENSITY, pEFXEAXReverb->flDensity);
			alEffectf(uiEffect, AL_EAXREVERB_DIFFUSION, pEFXEAXReverb->flDiffusion);
			alEffectf(uiEffect, AL_EAXREVERB_GAIN, pEFXEAXReverb->flGain);
			alEffectf(uiEffect, AL_EAXREVERB_GAINHF, pEFXEAXReverb->flGainHF);
			alEffectf(uiEffect, AL_EAXREVERB_GAINLF, pEFXEAXReverb->flGainLF);
			alEffectf(uiEffect, AL_EAXREVERB_DECAY_TIME, pEFXEAXReverb->flDecayTime);
			alEffectf(uiEffect, AL_EAXREVERB_DECAY_HFRATIO, pEFXEAXReverb->flDecayHFRatio);
			alEffectf(uiEffect, AL_EAXREVERB_DECAY_LFRATIO, pEFXEAXReverb->flDecayLFRatio);
			alEffectf(uiEffect, AL_EAXREVERB_REFLECTIONS_GAIN, pEFXEAXReverb->flReflectionsGain);
			alEffectf(uiEffect, AL_EAXREVERB_REFLECTIONS_DELAY, pEFXEAXReverb->flReflectionsDelay);
			alEffectfv(uiEffect, AL_EAXREVERB_REFLECTIONS_PAN, pEFXEAXReverb->flReflectionsPan);
			alEffectf(uiEffect, AL_EAXREVERB_LATE_REVERB_GAIN, pEFXEAXReverb->flLateReverbGain);
			alEffectf(uiEffect, AL_EAXREVERB_LATE_REVERB_DELAY, pEFXEAXReverb->flLateReverbDelay);
			alEffectfv(uiEffect, AL_EAXREVERB_LATE_REVERB_PAN, pEFXEAXReverb->flLateReverbPan);
			alEffectf(uiEffect, AL_EAXREVERB_ECHO_TIME, pEFXEAXReverb->flEchoTime);
			alEffectf(uiEffect, AL_EAXREVERB_ECHO_DEPTH, pEFXEAXReverb->flEchoDepth);
			alEffectf(uiEffect, AL_EAXREVERB_MODULATION_TIME, pEFXEAXReverb->flModulationTime);
			alEffectf(uiEffect, AL_EAXREVERB_MODULATION_DEPTH, pEFXEAXReverb->flModulationDepth);
			alEffectf(uiEffect, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, pEFXEAXReverb->flAirAbsorptionGainHF);
			alEffectf(uiEffect, AL_EAXREVERB_HFREFERENCE, pEFXEAXReverb->flHFReference);
			alEffectf(uiEffect, AL_EAXREVERB_LFREFERENCE, pEFXEAXReverb->flLFReference);
			alEffectf(uiEffect, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, pEFXEAXReverb->flRoomRolloffFactor);
			alEffecti(uiEffect, AL_EAXREVERB_DECAY_HFLIMIT, pEFXEAXReverb->iDecayHFLimit);

			if (alGetError() == AL_NO_ERROR)
				bReturn = AL_TRUE;
		}

		return bReturn;
	}

}