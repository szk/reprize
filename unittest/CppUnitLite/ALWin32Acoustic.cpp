#include "Common.hpp"

#include "ALWin32Acoustic.hpp"
#include "Speaker.hpp"
#include "resource/Loader/ALSndLoader.hpp"

using namespace reprize;
using namespace res;
using namespace aud;
using namespace std;

HINSTANCE g_hOpenALDLL = NULL;

// AL 1.0 did not define the ALchar and ALCchar types, so define them here
// if they don't exist

#ifndef ALchar
#define ALchar char
#endif

#ifndef ALCchar
#define ALCchar char
#endif

// Complete list of functions available in AL 1.0 implementations
typedef void (ALAPIENTRY* LPALENABLE)(ALenum capability);
typedef void (ALAPIENTRY* LPALDISABLE)(ALenum capability);
typedef ALboolean (ALAPIENTRY* LPALISENABLED)(ALenum capability);
typedef const ALchar* (ALAPIENTRY* LPALGETSTRING)(ALenum param);
typedef void (ALAPIENTRY* LPALGETBOOLEANV)(ALenum param, ALboolean* data);
typedef void (ALAPIENTRY* LPALGETINTEGERV)(ALenum param, ALint* data);
typedef void (ALAPIENTRY* LPALGETFLOATV)(ALenum param, ALfloat* data);
typedef void (ALAPIENTRY* LPALGETDOUBLEV)(ALenum param, ALdouble* data);
typedef ALboolean (ALAPIENTRY* LPALGETBOOLEAN)(ALenum param);
typedef ALint (ALAPIENTRY* LPALGETINTEGER)(ALenum param);
typedef ALfloat (ALAPIENTRY* LPALGETFLOAT)(ALenum param);
typedef ALdouble (ALAPIENTRY* LPALGETDOUBLE)(ALenum param);
typedef ALenum (ALAPIENTRY* LPALGETERROR)(void);
typedef ALboolean (ALAPIENTRY* LPALISEXTENSIONPRESENT)(const ALchar* extname);
typedef void* (ALAPIENTRY* LPALGETPROCADDRESS)(const ALchar* fname);
typedef ALenum (ALAPIENTRY* LPALGETENUMVALUE)(const ALchar* ename);
typedef void (ALAPIENTRY* LPALLISTENERF)(ALenum param, ALfloat value);
typedef void (ALAPIENTRY* LPALLISTENER3F)(ALenum param, ALfloat value1,
                                          ALfloat value2, ALfloat value3);
typedef void (ALAPIENTRY* LPALLISTENERFV)(ALenum param, const ALfloat* values);
typedef void (ALAPIENTRY* LPALLISTENERI)(ALenum param, ALint value);
typedef void (ALAPIENTRY* LPALGETLISTENERF)(ALenum param, ALfloat* value);
typedef void (ALAPIENTRY* LPALGETLISTENER3F)(ALenum param, ALfloat* value1,
                                             ALfloat* value2, ALfloat* value3);
typedef void (ALAPIENTRY* LPALGETLISTENERFV)(ALenum param, ALfloat* values);
typedef void (ALAPIENTRY* LPALGETLISTENERI)(ALenum param, ALint* value);
typedef void (ALAPIENTRY* LPALGENSOURCES)(ALsizei n, ALuint* sources);
typedef void (ALAPIENTRY* LPALDELETESOURCES)(ALsizei n, const ALuint* sources);
typedef ALboolean (ALAPIENTRY* LPALISSOURCE)(ALuint sid);
typedef void (ALAPIENTRY* LPALSOURCEF)(ALuint sid, ALenum param,
                                       ALfloat value);
typedef void (ALAPIENTRY* LPALSOURCE3F)(ALuint sid, ALenum param,
                                        ALfloat value1, ALfloat value2,
                                        ALfloat value3);
typedef void (ALAPIENTRY* LPALSOURCEFV)(ALuint sid, ALenum param,
                                        const ALfloat* values);
typedef void (ALAPIENTRY* LPALSOURCEI)(ALuint sid, ALenum param, ALint value);
typedef void (ALAPIENTRY* LPALGETSOURCEF)(ALuint sid, ALenum param,
                                          ALfloat* value);
typedef void (ALAPIENTRY* LPALGETSOURCE3F)(ALuint sid, ALenum param,
                                           ALfloat* value1, ALfloat* value2,
                                           ALfloat* value3);
typedef void (ALAPIENTRY* LPALGETSOURCEFV)(ALuint sid, ALenum param,
                                           ALfloat* values);
typedef void (ALAPIENTRY* LPALGETSOURCEI)(ALuint sid, ALenum param,
                                          ALint* value);
typedef void (ALAPIENTRY* LPALSOURCEPLAYV)(ALsizei ns, const ALuint* sids);
typedef void (ALAPIENTRY* LPALSOURCESTOPV)(ALsizei ns, const ALuint* sids);
typedef void (ALAPIENTRY* LPALSOURCEREWINDV)(ALsizei ns, const ALuint* sids);
typedef void (ALAPIENTRY* LPALSOURCEPAUSEV)(ALsizei ns, const ALuint* sids);
typedef void (ALAPIENTRY* LPALSOURCEPLAY)(ALuint sid);
typedef void (ALAPIENTRY* LPALSOURCESTOP)(ALuint sid);
typedef void (ALAPIENTRY* LPALSOURCEREWIND)(ALuint sid);
typedef void (ALAPIENTRY* LPALSOURCEPAUSE)(ALuint sid);
typedef void (ALAPIENTRY* LPALSOURCEQUEUEBUFFERS)(ALuint sid,
                                                  ALsizei numEntries,
                                                  const ALuint* bids);
typedef void (ALAPIENTRY* LPALSOURCEUNQUEUEBUFFERS)(ALuint sid,
                                                    ALsizei numEntries,
                                                    ALuint* bids);
typedef void (ALAPIENTRY* LPALGENBUFFERS)(ALsizei n, ALuint* buffers);
typedef void (ALAPIENTRY* LPALDELETEBUFFERS)(ALsizei n, const ALuint* buffers);
typedef ALboolean (ALAPIENTRY* LPALISBUFFER)(ALuint bid);
typedef void (ALAPIENTRY* LPALBUFFERDATA)(ALuint bid, ALenum format,
                                          const ALvoid* data, ALsizei size,
                                          ALsizei freq);
typedef void (ALAPIENTRY* LPALGETBUFFERF)(ALuint bid, ALenum param,
                                          ALfloat* value);
typedef void (ALAPIENTRY* LPALGETBUFFERI)(ALuint bid, ALenum param,
                                          ALint* value);
typedef void (ALAPIENTRY* LPALDOPPLERFACTOR)(ALfloat value);
typedef void (ALAPIENTRY* LPALDOPPLERVELOCITY)(ALfloat value);
typedef void (ALAPIENTRY* LPALDISTANCEMODEL)(ALenum distanceModel);

typedef ALCcontext* (ALCAPIENTRY* LPALCCREATECONTEXT) (ALCdevice* device,
                                                       const ALCint* attrlist);
typedef ALCboolean (ALCAPIENTRY* LPALCMAKECONTEXTCURRENT)(ALCcontext* context);
typedef void (ALCAPIENTRY* LPALCPROCESSCONTEXT)(ALCcontext* context);
typedef void (ALCAPIENTRY* LPALCSUSPENDCONTEXT)(ALCcontext* context);
typedef void (ALCAPIENTRY* LPALCDESTROYCONTEXT)(ALCcontext* context);
typedef ALCcontext* (ALCAPIENTRY* LPALCGETCURRENTCONTEXT)(ALCvoid);
typedef ALCdevice* (ALCAPIENTRY* LPALCGETCONTEXTSDEVICE)(ALCcontext* context);
typedef ALCdevice* (ALCAPIENTRY* LPALCOPENDEVICE)(const ALCchar* devicename);
typedef ALCboolean  (ALCAPIENTRY* LPALCCLOSEDEVICE)(ALCdevice* device);
typedef ALCenum (ALCAPIENTRY* LPALCGETERROR)(ALCdevice* device);
typedef ALCboolean (ALCAPIENTRY* LPALCISEXTENSIONPRESENT)(ALCdevice* device,
                                                          const ALCchar* extname);
typedef void* (ALCAPIENTRY* LPALCGETPROCADDRESS)(ALCdevice* device,
                                                 const ALCchar* funcname);
typedef ALCenum (ALCAPIENTRY* LPALCGETENUMVALUE)(ALCdevice* device,
                                                 const ALCchar* enumname);
typedef const ALCchar* (ALCAPIENTRY* LPALCGETSTRING)(ALCdevice* device,
                                                     ALCenum param);
typedef void (ALCAPIENTRY* LPALCGETINTEGERV)(ALCdevice* device, ALCenum param,
                                             ALCsizei size, ALCint* dest);
typedef struct
{
    LPALENABLE alEnable;
    LPALDISABLE alDisable;
    LPALISENABLED alIsEnabled;
    LPALGETBOOLEAN alGetBoolean;
    LPALGETINTEGER alGetInteger;
    LPALGETFLOAT alGetFloat;
    LPALGETDOUBLE alGetDouble;
    LPALGETBOOLEANV alGetBooleanv;
    LPALGETINTEGERV alGetIntegerv;
    LPALGETFLOATV alGetFloatv;
    LPALGETDOUBLEV alGetDoublev;
    LPALGETSTRING alGetString;
    LPALGETERROR alGetError;
    LPALISEXTENSIONPRESENT alIsExtensionPresent;
    LPALGETPROCADDRESS alGetProcAddress;
    LPALGETENUMVALUE alGetEnumValue;
    LPALLISTENERI alListeneri;
    LPALLISTENERF alListenerf;
    LPALLISTENER3F alListener3f;
    LPALLISTENERFV alListenerfv;
    LPALGETLISTENERI alGetListeneri;
    LPALGETLISTENERF alGetListenerf;
    LPALGETLISTENER3F alGetListener3f;
    LPALGETLISTENERFV alGetListenerfv;
    LPALGENSOURCES alGenSources;
    LPALDELETESOURCES alDeleteSources;
    LPALISSOURCE alIsSource;
    LPALSOURCEI alSourcei;
    LPALSOURCEF alSourcef;
    LPALSOURCE3F alSource3f;
    LPALSOURCEFV alSourcefv;
    LPALGETSOURCEI alGetSourcei;
    LPALGETSOURCEF alGetSourcef;
    LPALGETSOURCEFV alGetSourcefv;
    LPALSOURCEPLAYV alSourcePlayv;
    LPALSOURCESTOPV alSourceStopv;
    LPALSOURCEPLAY alSourcePlay;
    LPALSOURCEPAUSE alSourcePause;
    LPALSOURCESTOP alSourceStop;
    LPALGENBUFFERS alGenBuffers;
    LPALDELETEBUFFERS alDeleteBuffers;
    LPALISBUFFER alIsBuffer;
    LPALBUFFERDATA alBufferData;
    LPALGETBUFFERI alGetBufferi;
    LPALGETBUFFERF alGetBufferf;
    LPALSOURCEQUEUEBUFFERS alSourceQueueBuffers;
    LPALSOURCEUNQUEUEBUFFERS alSourceUnqueueBuffers;
    LPALDISTANCEMODEL alDistanceModel;
    LPALDOPPLERFACTOR alDopplerFactor;
    LPALDOPPLERVELOCITY alDopplerVelocity;
    LPALCGETSTRING alcGetString;
    LPALCGETINTEGERV alcGetIntegerv;
    LPALCOPENDEVICE alcOpenDevice;
    LPALCCLOSEDEVICE alcCloseDevice;
    LPALCCREATECONTEXT alcCreateContext;
    LPALCMAKECONTEXTCURRENT alcMakeContextCurrent;
    LPALCPROCESSCONTEXT alcProcessContext;
    LPALCGETCURRENTCONTEXT alcGetCurrentContext;
    LPALCGETCONTEXTSDEVICE alcGetContextsDevice;
    LPALCSUSPENDCONTEXT alcSuspendContext;
    LPALCDESTROYCONTEXT alcDestroyContext;
    LPALCGETERROR alcGetError;
    LPALCISEXTENSIONPRESENT alcIsExtensionPresent;
    LPALCGETPROCADDRESS alcGetProcAddress;
    LPALCGETENUMVALUE alcGetEnumValue;
} OPENALFNTABLE,* LPOPENALFNTABLE;

#define ALCHECK(N, n) if (!(ALfuncTbl->n=(N)GetProcAddress(g_hOpenALDLL,#n))) \
    {                                                                   \
        OutputDebugString("Failed to retrieve '" #n "' function address\n");\
        return AL_FALSE;                                               \
    }

// for eax
typedef ALboolean (__cdecl* LPEAXSETBUFFERMODE)(ALsizei n, ALuint* buffers,
                                                ALint value);
typedef ALenum (__cdecl* LPEAXGETBUFFERMODE)(ALuint buffer, ALint* value);

LPEAXSETBUFFERMODE eaxSetBufferMode = NULL;
LPEAXGETBUFFERMODE eaxGetBufferMode = NULL;
// extern LPEAXSETBUFFERMODE eaxSetBufferMode;
// extern LPEAXGETBUFFERMODE eaxGetBufferMode;


ALSndLoader sndloader(NULL);

bool LoadOAL10Library(char* szOALFullPathName, LPOPENALFNTABLE ALfuncTbl)
{
    if (!ALfuncTbl) { return false;}

    if (szOALFullPathName) { g_hOpenALDLL = LoadLibrary(szOALFullPathName);}
    else { g_hOpenALDLL = LoadLibrary("openal32.dll");}

    if (!g_hOpenALDLL) { return false;}

    memset(ALfuncTbl, 0, sizeof(OPENALFNTABLE));

    ALCHECK(LPALENABLE, alEnable);
    ALCHECK(LPALDISABLE, alDisable);
    ALCHECK(LPALISENABLED, alIsEnabled);
    ALCHECK(LPALGETBOOLEAN, alGetBoolean);
    ALCHECK(LPALGETINTEGER, alGetInteger);
    ALCHECK(LPALGETFLOAT, alGetFloat);
    ALCHECK(LPALGETDOUBLE, alGetDouble);
    ALCHECK(LPALGETBOOLEANV, alGetBooleanv);
    ALCHECK(LPALGETINTEGERV, alGetIntegerv);
    ALCHECK(LPALGETFLOATV, alGetFloatv);
    ALCHECK(LPALGETDOUBLEV, alGetDoublev);
    ALCHECK(LPALGETSTRING, alGetString);
    ALCHECK(LPALGETERROR, alGetError);
    ALCHECK(LPALISEXTENSIONPRESENT, alIsExtensionPresent);
    ALCHECK(LPALGETPROCADDRESS, alGetProcAddress);
    ALCHECK(LPALGETENUMVALUE, alGetEnumValue);
    ALCHECK(LPALLISTENERI, alListeneri);
    ALCHECK(LPALLISTENERF, alListenerf);
    ALCHECK(LPALLISTENER3F, alListener3f);
    ALCHECK(LPALLISTENERFV, alListenerfv);
    ALCHECK(LPALGETLISTENERI, alGetListeneri);
    ALCHECK(LPALGETLISTENERF, alGetListenerf);
    ALCHECK(LPALGETLISTENER3F, alGetListener3f);
    ALCHECK(LPALGETLISTENERFV, alGetListenerfv);
    ALCHECK(LPALGENSOURCES, alGenSources);
    ALCHECK(LPALDELETESOURCES, alDeleteSources);
    ALCHECK(LPALISSOURCE, alIsSource);
    ALCHECK(LPALSOURCEI, alSourcei);
    ALCHECK(LPALSOURCEF, alSourcef);
    ALCHECK(LPALSOURCE3F, alSource3f);
    ALCHECK(LPALSOURCEFV, alSourcefv);
    ALCHECK(LPALGETSOURCEI, alGetSourcei);
    ALCHECK(LPALGETSOURCEF, alGetSourcef);
    ALCHECK(LPALGETSOURCEFV, alGetSourcefv);
    ALCHECK(LPALSOURCEPLAYV, alSourcePlayv);
    ALCHECK(LPALSOURCESTOPV, alSourceStopv);
    ALCHECK(LPALSOURCEPLAY, alSourcePlay);
    ALCHECK(LPALSOURCEPAUSE, alSourcePause);
    ALCHECK(LPALSOURCESTOP, alSourceStop);
    ALCHECK(LPALGENBUFFERS, alGenBuffers);
    ALCHECK(LPALDELETEBUFFERS, alDeleteBuffers);
    ALCHECK(LPALISBUFFER, alIsBuffer);
    ALCHECK(LPALBUFFERDATA, alBufferData);
    ALCHECK(LPALGETBUFFERI, alGetBufferi);
    ALCHECK(LPALGETBUFFERF, alGetBufferf);
    ALCHECK(LPALSOURCEQUEUEBUFFERS, alSourceQueueBuffers);
    ALCHECK(LPALSOURCEUNQUEUEBUFFERS, alSourceUnqueueBuffers);
    ALCHECK(LPALDISTANCEMODEL, alDistanceModel);
    ALCHECK(LPALDOPPLERFACTOR, alDopplerFactor);
    ALCHECK(LPALDOPPLERVELOCITY, alDopplerVelocity);
    ALCHECK(LPALCGETSTRING, alcGetString);
    ALCHECK(LPALCGETINTEGERV, alcGetIntegerv);
    ALCHECK(LPALCOPENDEVICE, alcOpenDevice);
    ALCHECK(LPALCCLOSEDEVICE, alcCloseDevice);
    ALCHECK(LPALCCREATECONTEXT, alcCreateContext);
    ALCHECK(LPALCMAKECONTEXTCURRENT, alcMakeContextCurrent);
    ALCHECK(LPALCPROCESSCONTEXT, alcProcessContext);
    ALCHECK(LPALCGETCURRENTCONTEXT, alcGetCurrentContext);
    ALCHECK(LPALCGETCONTEXTSDEVICE, alcGetContextsDevice);
    ALCHECK(LPALCSUSPENDCONTEXT, alcSuspendContext);
    ALCHECK(LPALCDESTROYCONTEXT, alcDestroyContext);
    ALCHECK(LPALCGETERROR, alcGetError);
    ALCHECK(LPALCISEXTENSIONPRESENT, alcIsExtensionPresent);
    ALCHECK(LPALCGETPROCADDRESS, alcGetProcAddress);
    ALCHECK(LPALCGETENUMVALUE, alcGetEnumValue);

    return true;
}

ALvoid UnloadOAL10Library(void)
{
    // Unload the dll
    if (g_hOpenALDLL)
    {
        FreeLibrary(g_hOpenALDLL);
        g_hOpenALDLL = NULL;
    }
}

OPENALFNTABLE ALfunc;

ALWin32Acoustic::ALWin32Acoustic(EnvDepInfo* depinfo_)
    : Acoustic(depinfo_)
{
}

ALWin32Acoustic::~ALWin32Acoustic(void)
{
    ALCcontext* pContext;
    ALCdevice* pDevice;

    pContext = alcGetCurrentContext();
    pDevice = alcGetContextsDevice(pContext);

    alcMakeContextCurrent(NULL);
    alcDestroyContext(pContext);
    alcCloseDevice(pDevice);
}

const bool ALWin32Acoustic::init(void)
{
//     ALDeviceList* pDeviceList = NULL;
    ALCcontext* pContext = NULL;
    ALCdevice* pDevice = NULL;
    Int32 i;
    bool bReturn = false;

//     pDeviceList = new ALDeviceList();
    ALDeviceListInitializer();

    if (//(pDeviceList) &&
        (GetNumDevices()))
    {
//         ALFWprintf("\nSelect OpenAL Device:\n");
//         for (i = 0; i < GetNumDevices(); i++) 
//         {
//             ALFWprintf("%d. %s%s\n", i + 1, GetDeviceName(i),
//                        i == GetDefaultDevice() ? "(DEFAULT)" : "");
//         }
//         do
//         {
//             Char ch = _getch();
//             i = atoi(&ch);
//         }
//         while ((i < 1) || (i > GetNumDevices()));

        pDevice = alcOpenDevice(GetDeviceName(0));
//         pDevice = alcOpenDevice(GetDeviceName(i - 1));
        if (pDevice)
        {
            pContext = alcCreateContext(pDevice, NULL);
            if (pContext)
            {
                std::cerr << "Opened "
                          << alcGetString(pDevice, ALC_DEVICE_SPECIFIER)
                          << "Device(s)\n" << std::endl;

                alcMakeContextCurrent(pContext);
                bReturn = true;
            }
            else { alcCloseDevice(pDevice); }
        }

        ALDeviceListDestroy();
    }

    std::cerr << "OpenAL failed";

    return bReturn;
/*
    ALDeviceList* pDeviceList = NULL;
    ALCcontext* pContext = NULL;
    ALCdevice* pDevice = NULL;
    ALint i;
    ALboolean bReturn = AL_FALSE;

    pDeviceList = new ALDeviceList();
    if ((pDeviceList) && (pDeviceList->GetNumDevices()))
    {
        ALFWprintf("\nSelect OpenAL Device:\n");
        for (i = 0; i < pDeviceList->GetNumDevices(); i++)
            ALFWprintf("%d. %s%s\n", i + 1, pDeviceList->GetDeviceName(i), i == pDeviceList->GetDefaultDevice() ? "(DEFAULT)" : "");

        do {
            ALchar ch = _getch();
            i = atoi(&ch);
        } while ((i < 1) || (i > pDeviceList->GetNumDevices()));

        pDevice = alcOpenDevice(pDeviceList->GetDeviceName(i - 1));
        if (pDevice)
        {
            pContext = alcCreateContext(pDevice, NULL);
            if (pContext)
            {
                ALFWprintf("\nOpened %s Device\n", alcGetString(pDevice, ALC_DEVICE_SPECIFIER));
                alcMakeContextCurrent(pContext);
                bReturn = AL_TRUE;
            }
            else
            {
                alcCloseDevice(pDevice);
            }
        }

        delete pDeviceList;
    }
*/
//     return bReturn;
}

/*
void ALWin32Acoustic::appear(void)
{

    ALint iDeviceIndex = 0;
    const ALchar* pDeviceNames = NULL;
    const ALchar* pDefaultDevice = NULL;

    printf("\n\nAll Available OpenAL Devices\n\n");

    if(!(LoadOAL10Library(NULL, &ALfunc))) { return; }

    printf("Devices available with standard ALC_ENUMERATION_EXT\n");
    printf("===================================================\n");
    printf("     %-40s   OpenAL Spec/Ext Support\n", "           ");
    printf("#    %-40s  Default  1.0   1.1   EFX\n\n", "Device Name");
    if(ALfunc.alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"))
    {
        pDeviceNames = ALfunc.alcGetString(NULL, ALC_DEVICE_SPECIFIER);
        pDefaultDevice = ALfunc.alcGetString(NULL,
                                             ALC_DEFAULT_DEVICE_SPECIFIER);
        if(!strlen(pDeviceNames)) { printf("None found.\n"); }
        else
        {
            while(pDeviceNames &&* pDeviceNames)
            {
                printf("%-2d - %-40s", iDeviceIndex, pDeviceNames);

                if(strlen(pDeviceNames) > 40) { printf("\n%-45s", ""); }

                // Open and query for extensions
                ALCdevice* pDevice = ALfunc.alcOpenDevice(pDeviceNames);

                if(pDevice)
                {
                    ALint iMajorVersion, iMinorVersion;
                    ALboolean bSpec10Support = AL_FALSE;
                    ALboolean bSpec11Support = AL_FALSE;
                    ALboolean bEFXSupport = AL_FALSE;

                    ALfunc.alcGetIntegerv(pDevice, ALC_MAJOR_VERSION,
                                          sizeof(ALint), &iMajorVersion);
                    ALfunc.alcGetIntegerv(pDevice, ALC_MINOR_VERSION,
                                          sizeof(ALint), &iMinorVersion);

                    if ((iMajorVersion == 1) && (iMinorVersion == 0))
                    { bSpec10Support = AL_TRUE; }
                    else if ((iMajorVersion > 1)
                             || ((iMajorVersion == 1) && (iMinorVersion >= 1)))
                    { bSpec11Support = AL_TRUE; }

                    if(ALfunc.alcIsExtensionPresent(pDevice, "ALC_EXT_EFX"))
                    { bEFXSupport = AL_TRUE; }

                    // Print default flag
                    printf("   %-8c",
                           !strcmp(pDeviceNames, pDefaultDevice) ? 'X' : ' ');
                    // Print Spec version flags
                    printf(" %-5c %-5c", bSpec10Support ? 'X' : ' ',
                           bSpec11Support ? 'X' : ' ');
                    // Print XRAM ext support flag
                    printf(" %-5c", bEFXSupport ? 'X' : ' ');

                    ALfunc.alcCloseDevice(pDevice);
                }
                else { printf("ERROR: Doesn't open!\n"); }
                printf("\n");
                iDeviceIndex++;
                pDeviceNames += strlen(pDeviceNames) + 1;
            }
        }
    }

    printf("\n\n");
    iDeviceIndex = 0;

    printf("Devices available with ALC_ENUMERATE_ALL_EXT\n");
    printf("============================================\n");
    printf("     %-40s   OpenAL Spec/Ext Support\n", "           ");
    printf("#    %-40s  Default  1.0   1.1   EFX\n\n", "Device Name");
    if(!(ALfunc.alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT")))
    { return; }

    pDeviceNames = ALfunc.alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
    pDefaultDevice = ALfunc.alcGetString(NULL,
                                         ALC_DEFAULT_ALL_DEVICES_SPECIFIER);
    if(!strlen(pDeviceNames)) { printf("None found.\n"); return; }

    while(pDeviceNames &&* pDeviceNames)
    {
        printf("%-2d - %-40s", iDeviceIndex, pDeviceNames);

        if(strlen(pDeviceNames) > 40) { printf("\n%-45s", ""); }

        // Open and query for extensions
        ALCdevice* pDevice = ALfunc.alcOpenDevice(pDeviceNames);

        if(pDevice)
        {
            ALint iMajorVersion, iMinorVersion;
            ALboolean bSpec10Support = AL_FALSE;
            ALboolean bSpec11Support = AL_FALSE;
            ALboolean bEFXSupport = AL_FALSE;

            ALfunc.alcGetIntegerv(pDevice, ALC_MAJOR_VERSION, sizeof(ALint),
                                  &iMajorVersion);
            ALfunc.alcGetIntegerv(pDevice, ALC_MINOR_VERSION,
                                  sizeof(ALint), &iMinorVersion);

            if ((iMajorVersion == 1) && (iMinorVersion == 0))
            { bSpec10Support = AL_TRUE; }
            else if ((iMajorVersion > 1) ||
                     ((iMajorVersion == 1) && (iMinorVersion >= 1)))
            { bSpec11Support = AL_TRUE; }

            if(ALfunc.alcIsExtensionPresent(pDevice, "ALC_EXT_EFX"))
            { bEFXSupport = AL_TRUE; }

            // Print default flag
            printf("   %-8c", !strcmp(pDeviceNames,
                                      pDefaultDevice) ? 'X' : ' ');
            // Print Spec version flags
            printf(" %-5c %-5c", bSpec10Support ? 'X' : ' ',
                   bSpec11Support ? 'X' : ' ');
            // Print XRAM ext support flag
            printf(" %-5c", bEFXSupport ? 'X' : ' ');
            ALfunc.alcCloseDevice(pDevice);
        }
        else { printf("ERROR: Doesn't open!\n"); }
        printf("\n");
        iDeviceIndex++;
        pDeviceNames += strlen(pDeviceNames) + 1;
    }

    printf("\nSeeya.\n");

}
*/

void ALWin32Acoustic::begin_play(void)
{
    return;
    ALuint uiBuffer, uiSource;
    ALint iState;

    // plz get uibuffer from whereever
    alGenSources(1, &uiSource);
    // Attach Source to Buffer
    alSourcei(uiSource, AL_BUFFER, uiBuffer);
    alSourcePlay(uiSource);
    do
    {
        Sleep(100);
        // Get Source State
        alGetSourcei( uiSource, AL_SOURCE_STATE, &iState);
    } while (iState == AL_PLAYING);
    alSourceStop(uiSource);
    alDeleteSources(1, &uiSource);
}

void ALWin32Acoustic::finish_play(void)
{
}

void ALWin32Acoustic::release(void)
{
}

void ALWin32Acoustic::test(void)
{
    ALuint uiBuffer, uiSource;
    ALint iState;

//     if (!init())
//     {
//         std::cerr << "Failed to initialize OpenAL" << std::endl;
//         return;
//     }

    // Generate an AL Buffer
    alGenBuffers(1, &uiBuffer );

    // Load Wave file into OpenAL Buffer
    if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath("footsteps.wav"),
                              uiBuffer))
    {
        std::cerr << "Failed to load"
                  << ALFWaddMediaPath("footsteps.wav") << std::endl;
    }

    // Generate a Source to playback the Buffer
    alGenSources( 1, &uiSource );

    // Attach Source to Buffer
    alSourcei( uiSource, AL_BUFFER, uiBuffer );

    // Play Source
    alSourcePlay( uiSource );
    std::cerr << "Playing Source " << std::endl;

    do
    {
        Sleep(100);
        // Get Source State
        alGetSourcei( uiSource, AL_SOURCE_STATE, &iState);
    } while (iState == AL_PLAYING);

    // Clean up by deleting Source(s) and Buffer(s)
    alSourceStop(uiSource);
    alDeleteSources(1, &uiSource);
    alDeleteBuffers(1, &uiBuffer);

    ALFWShutdownOpenAL();
}

bool ALWin32Acoustic::ALFWShutdownOpenAL(void)
{
    ALCcontext* pContext;
    ALCdevice* pDevice;

    pContext = alcGetCurrentContext();
    pDevice = alcGetContextsDevice(pContext);

    alcMakeContextCurrent(NULL);
    alcDestroyContext(pContext);
    alcCloseDevice(pDevice);

    return true;
}

bool ALWin32Acoustic::ALFWLoadWaveToBuffer(const char* szWaveFile,
                                           ALuint uiBufferID,
                                           ALenum eXRAMBufferMode)
{
    WAVEID WaveID;
    Int32 iDataSize, iFrequency;
    ALenum eBufferFormat;
    Char* pData;
    bool bReturn;

    bReturn = false;
//     if (!g_pWaveLoader) { return bReturn; }

    if (!SUCCEEDED(sndloader.LoadWaveFile(szWaveFile, &WaveID)))
    {
        std::cerr << "LoadWaveFile is failed" << std::endl;
        return bReturn;
    }

    if ((SUCCEEDED(sndloader.GetWaveSize(WaveID, (unsigned long*)&iDataSize)))
        && (SUCCEEDED(sndloader.GetWaveData(WaveID, (void**)&pData)))
        && (SUCCEEDED(sndloader.GetWaveFrequency(WaveID,
                                                 (unsigned long*)&iFrequency)))
        && (SUCCEEDED(sndloader.GetWaveALBufferFormat(WaveID, &alGetEnumValue,
                                                      (unsigned long*)&eBufferFormat))))
    {
        // Set XRAM Mode (if application)
        if (eaxSetBufferMode && eXRAMBufferMode)
        { eaxSetBufferMode(1, &uiBufferID, eXRAMBufferMode); }

        if (alGetError() == AL_NO_ERROR)
        {
            std::cerr << "no_error before buffer data" << std::endl;
        }

        alBufferData(uiBufferID, eBufferFormat, pData, iDataSize, iFrequency);
        ALenum e = alGetError();

        if (e == AL_NO_ERROR)
        {
            std::cerr << "al_no_error" << std::endl;
            bReturn = true;
        }
        std::cerr << e << ": al_an_error" << std::endl;
        sndloader.DeleteWaveFile(WaveID);
    }

    return bReturn;
}

Char fullPath[_MAX_PATH];
Char* ALWin32Acoustic::ALFWaddMediaPath(const ALchar* filename)
{
    sprintf(fullPath, "%s%s", "..\\..\\Media\\", filename);
    return fullPath;
}

// Extension Queries
#ifdef UNDEF
ALboolean ALWin32Acoustic::ALFWIsXRAMSupported(void)
{
    ALboolean bXRAM = AL_FALSE;

    if (alIsExtensionPresent("EAX-RAM") == AL_TRUE)
    {
        // Get X-RAM Function pointers
        eaxSetBufferMode = (EAXSetBufferMode)alGetProcAddress("EAXSetBufferMode");
        eaxGetBufferMode = (EAXGetBufferMode)alGetProcAddress("EAXGetBufferMode");

        if (eaxSetBufferMode && eaxGetBufferMode)
        {
            eXRAMSize = alGetEnumValue("AL_EAX_RAM_SIZE");
            eXRAMFree = alGetEnumValue("AL_EAX_RAM_FREE");
            eXRAMAuto = alGetEnumValue("AL_STORAGE_AUTOMATIC");
            eXRAMHardware = alGetEnumValue("AL_STORAGE_HARDWARE");
            eXRAMAccessible = alGetEnumValue("AL_STORAGE_ACCESSIBLE");

            if (eXRAMSize && eXRAMFree && eXRAMAuto && eXRAMHardware && eXRAMAccessible)
            { bXRAM = AL_TRUE;}
        }
    }

    return bXRAM;
}

ALboolean ALWin32Acoustic::ALFWIsEFXSupported(void)
{
    ALCdevice* pDevice = NULL;
    ALCcontext* pContext = NULL;
    ALboolean bEFXSupport = AL_FALSE;

    pContext = alcGetCurrentContext();
    pDevice = alcGetContextsDevice(pContext);

    if (alcIsExtensionPresent(pDevice, (ALCchar*)ALC_EXT_EFX_NAME))
    {
        // Get function pointers
        alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
        alDeleteEffects = (LPALDELETEEFFECTS )alGetProcAddress("alDeleteEffects");
        alIsEffect = (LPALISEFFECT )alGetProcAddress("alIsEffect");
        alEffecti = (LPALEFFECTI)alGetProcAddress("alEffecti");
        alEffectiv = (LPALEFFECTIV)alGetProcAddress("alEffectiv");
        alEffectf = (LPALEFFECTF)alGetProcAddress("alEffectf");
        alEffectfv = (LPALEFFECTFV)alGetProcAddress("alEffectfv");
        alGetEffecti = (LPALGETEFFECTI)alGetProcAddress("alGetEffecti");
        alGetEffectiv = (LPALGETEFFECTIV)alGetProcAddress("alGetEffectiv");
        alGetEffectf = (LPALGETEFFECTF)alGetProcAddress("alGetEffectf");
        alGetEffectfv = (LPALGETEFFECTFV)alGetProcAddress("alGetEffectfv");
        alGenFilters = (LPALGENFILTERS)alGetProcAddress("alGenFilters");
        alDeleteFilters = (LPALDELETEFILTERS)alGetProcAddress("alDeleteFilters");
        alIsFilter = (LPALISFILTER)alGetProcAddress("alIsFilter");
        alFilteri = (LPALFILTERI)alGetProcAddress("alFilteri");
        alFilteriv = (LPALFILTERIV)alGetProcAddress("alFilteriv");
        alFilterf = (LPALFILTERF)alGetProcAddress("alFilterf");
        alFilterfv = (LPALFILTERFV)alGetProcAddress("alFilterfv");
        alGetFilteri = (LPALGETFILTERI )alGetProcAddress("alGetFilteri");
        alGetFilteriv= (LPALGETFILTERIV )alGetProcAddress("alGetFilteriv");
        alGetFilterf = (LPALGETFILTERF )alGetProcAddress("alGetFilterf");
        alGetFilterfv= (LPALGETFILTERFV )alGetProcAddress("alGetFilterfv");
        alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
        alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
        alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");
        alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");
        alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV)alGetProcAddress("alAuxiliaryEffectSlotiv");
        alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)alGetProcAddress("alAuxiliaryEffectSlotf");
        alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV)alGetProcAddress("alAuxiliaryEffectSlotfv");
        alGetAuxiliaryEffectSloti = (LPALGETAUXILIARYEFFECTSLOTI)alGetProcAddress("alGetAuxiliaryEffectSloti");
        alGetAuxiliaryEffectSlotiv = (LPALGETAUXILIARYEFFECTSLOTIV)alGetProcAddress("alGetAuxiliaryEffectSlotiv");
        alGetAuxiliaryEffectSlotf = (LPALGETAUXILIARYEFFECTSLOTF)alGetProcAddress("alGetAuxiliaryEffectSlotf");
        alGetAuxiliaryEffectSlotfv = (LPALGETAUXILIARYEFFECTSLOTFV)alGetProcAddress("alGetAuxiliaryEffectSlotfv");

        if (alGenEffects && alDeleteEffects && alIsEffect && alEffecti
            && alEffectiv && alEffectf && alEffectfv && alGetEffecti
            && alGetEffectiv && alGetEffectf && alGetEffectfv && alGenFilters
            && alDeleteFilters && alIsFilter && alFilteri && alFilteriv
            && alFilterf && alFilterfv && alGetFilteri && alGetFilteriv
            && alGetFilterf && alGetFilterfv && alGenAuxiliaryEffectSlots
            && alDeleteAuxiliaryEffectSlots && alIsAuxiliaryEffectSlot
            && alAuxiliaryEffectSloti && alAuxiliaryEffectSlotiv
            && alAuxiliaryEffectSlotf && alAuxiliaryEffectSlotfv &&
            alGetAuxiliaryEffectSloti && alGetAuxiliaryEffectSlotiv
            && alGetAuxiliaryEffectSlotf && alGetAuxiliaryEffectSlotfv)
        { bEFXSupport = AL_TRUE; }
    }

    return bEFXSupport;
}

#endif

/*
 *  Init call
 */
void ALWin32Acoustic::ALDeviceListInitializer(void)
{
    ALDEVICEINFO ALDeviceInfo;
    char* devices;
    int index;
    const char* defaultDeviceName;
    const char* actualDeviceName;

    // DeviceInfo vector stores, for each enumerated device, it's device name, selection status, spec version #, and extension support
    vDeviceInfo.empty();
    vDeviceInfo.reserve(10);

    defaultDeviceIndex = 0;

    // grab function pointers for 1.0-API functions,
    // and if successful proceed to enumerate all devices
    if (LoadOAL10Library(NULL, &ALfunc) == FALSE)
    { ResetFilters(); return; }

    if (!ALfunc.alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"))
    { ResetFilters(); return; }

    devices = (char*)ALfunc.alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    defaultDeviceName = (char*)ALfunc.alcGetString(NULL,
                                                       ALC_DEFAULT_DEVICE_SPECIFIER);
    index = 0;
    // go through device list (each device terminated with a single NULL,
    // list terminated with double NULL)
    while (*devices != NULL)
    {
        if (strcmp(defaultDeviceName, devices) == 0)
        { defaultDeviceIndex = index; }
        ALCdevice* device = ALfunc.alcOpenDevice(devices);
        if (device)
        {
            ALCcontext* context = ALfunc.alcCreateContext(device, NULL);
            if (context)
            {
                ALfunc.alcMakeContextCurrent(context);
                // if new actual device name isn't already in the list, then add it...
                actualDeviceName = ALfunc.alcGetString(device,
                                                       ALC_DEVICE_SPECIFIER);
                bool bNewName = true;
                for (Size32 i = 0; i < GetNumDevices(); i++)
                {
                    if (strcmp(GetDeviceName(i), actualDeviceName) == 0)
                    { bNewName = false; }
                }
                if ((bNewName) && (actualDeviceName != NULL)
                    && (strlen(actualDeviceName) > 0))
                {
                    memset(&ALDeviceInfo, 0, sizeof(ALDEVICEINFO));
                    ALDeviceInfo.bSelected = true;
                    ALDeviceInfo.strDeviceName = actualDeviceName;
                    ALfunc.alcGetIntegerv(device, ALC_MAJOR_VERSION,
                                          sizeof(Int32),
                                          &ALDeviceInfo.iMajorVersion);
                    ALfunc.alcGetIntegerv(device, ALC_MINOR_VERSION,
                                          sizeof(Int32),
                                          &ALDeviceInfo.iMinorVersion);

                    ALDeviceInfo.pvstrExtensions = new vector<string>;
                    // Check for ALC Extensions
                    if (ALfunc.alcIsExtensionPresent(device,
                                                     "ALC_EXT_CAPTURE") == AL_TRUE)
                    { ALDeviceInfo.pvstrExtensions->push_back("ALC_EXT_CAPTURE"); }
                    if (ALfunc.alcIsExtensionPresent(device,
                                                     "ALC_EXT_EFX") == AL_TRUE)
                    { ALDeviceInfo.pvstrExtensions->push_back("ALC_EXT_EFX"); }
                    // Check for AL Extensions
                    if (ALfunc.alIsExtensionPresent("AL_EXT_OFFSET") == AL_TRUE)
                    { ALDeviceInfo.pvstrExtensions->push_back("AL_EXT_OFFSET"); }

                    if (ALfunc.alIsExtensionPresent("AL_EXT_LINEAR_DISTANCE") == AL_TRUE)
                    { ALDeviceInfo.pvstrExtensions->push_back("AL_EXT_LINEAR_DISTANCE"); }
                    if (ALfunc.alIsExtensionPresent("AL_EXT_EXPONENT_DISTANCE") == AL_TRUE)
                    { ALDeviceInfo.pvstrExtensions->push_back("AL_EXT_EXPONENT_DISTANCE"); }

                    if (ALfunc.alIsExtensionPresent("EAX2.0") == AL_TRUE)
                        ALDeviceInfo.pvstrExtensions->push_back("EAX2.0");
                    if (ALfunc.alIsExtensionPresent("EAX3.0") == AL_TRUE)
                        ALDeviceInfo.pvstrExtensions->push_back("EAX3.0");
                    if (ALfunc.alIsExtensionPresent("EAX4.0") == AL_TRUE)
                        ALDeviceInfo.pvstrExtensions->push_back("EAX4.0");
                    if (ALfunc.alIsExtensionPresent("EAX5.0") == AL_TRUE)
                        ALDeviceInfo.pvstrExtensions->push_back("EAX5.0");
                    if (ALfunc.alIsExtensionPresent("EAX-RAM") == AL_TRUE)
                        ALDeviceInfo.pvstrExtensions->push_back("EAX-RAM");
                    // Get Source Count
                    ALDeviceInfo.uiSourceCount = GetMaxNumSources();
                    vDeviceInfo.push_back(ALDeviceInfo);
                }
                ALfunc.alcMakeContextCurrent(NULL);
                ALfunc.alcDestroyContext(context);
            }
            ALfunc.alcCloseDevice(device);
        }
        devices += strlen(devices) + 1;
        index += 1;
    }

    ResetFilters();
}

/*
 *  Exit call
 */
void ALWin32Acoustic::ALDeviceListDestroy(void)
{
    for (unsigned int i = 0; i < vDeviceInfo.size(); i++)
    {
        if (vDeviceInfo[i].pvstrExtensions)
        {
            vDeviceInfo[i].pvstrExtensions->empty();
            delete vDeviceInfo[i].pvstrExtensions;
        }
    }

    vDeviceInfo.empty();

    UnloadOAL10Library();
}

/*
 *  Returns the number of devices in the complete device list
 */
Size32 ALWin32Acoustic::GetNumDevices(void)
{
    return vDeviceInfo.size();
}

/*
 *  Returns the device name at an index in the complete device list
 */
char*  ALWin32Acoustic::GetDeviceName(Size32 idx_)
{
    if (idx_ < GetNumDevices())
    { return (char* )vDeviceInfo[idx_].strDeviceName.c_str(); }
    return NULL;
}

/*
 *  Returns the major and minor version numbers for a device at a specified index in the complete list
 */
void ALWin32Acoustic::GetDeviceVersion(Size32 idx_, int* major, int* minor)
{
    if (idx_ < GetNumDevices())
    {
        if (major) {* major = vDeviceInfo[idx_].iMajorVersion; }
        if (minor) {* minor = vDeviceInfo[idx_].iMinorVersion; }
    }
    return;
}

/*
 *  Returns the maximum number of Sources that can be generate on the given device
 */
unsigned int ALWin32Acoustic::GetMaxNumSources(Size32 idx_)
{
    if (idx_ < GetNumDevices()) { return vDeviceInfo[idx_].uiSourceCount; }
    return 0;
}

/*
 *  Checks if the extension is supported on the given device
 */
bool ALWin32Acoustic::IsExtensionSupported(Size32 idx_, char* szExtName)
{
    bool bReturn = false;

    if (idx_ < GetNumDevices())
    {
        for (unsigned int i = 0; i < vDeviceInfo[idx_].pvstrExtensions->size();
             i++)
        {
            if (!_stricmp(vDeviceInfo[idx_].pvstrExtensions->at(i).c_str(),
                          szExtName))
            {
                bReturn = true;
                break;
            }
        }
    }

    return bReturn;
}

/*
 *  returns the index of the default device in the complete device list
 */
int ALWin32Acoustic::GetDefaultDevice(void)
{
    return defaultDeviceIndex;
}

/*
 *  Deselects devices which don't have the specified minimum version
 */
void ALWin32Acoustic::FilterDevicesMinVer(int major, int minor)
{
    Int32 dMajor, dMinor;
    for (unsigned int i = 0; i < vDeviceInfo.size(); i++)
    {
        GetDeviceVersion(i, &dMajor, &dMinor);
        if ((dMajor < major) || ((dMajor == major) && (dMinor < minor)))
        { vDeviceInfo[i].bSelected = false; }
    }
}

/*
 *  Deselects devices which don't have the specified maximum version
 */
void ALWin32Acoustic::FilterDevicesMaxVer(int major, int minor)
{
    Int32 dMajor, dMinor;
    for (unsigned int i = 0; i < vDeviceInfo.size(); i++)
    {
        GetDeviceVersion(i, &dMajor, &dMinor);
        if ((dMajor > major) || ((dMajor == major) && (dMinor > minor)))
        { vDeviceInfo[i].bSelected = false; }
    }
}

/*
 *  Deselects device which don't support the given extension name
 */
void ALWin32Acoustic::FilterDevicesExtension(char* szExtName)
{
    bool bFound;

    for (unsigned int i = 0; i < vDeviceInfo.size(); i++)
    {
        bFound = false;
        for (unsigned int j = 0; j < vDeviceInfo[i].pvstrExtensions->size();
             j++)
        {
            if (!_stricmp(vDeviceInfo[i].pvstrExtensions->at(j).c_str(),
                          szExtName))
            {
                bFound = true;
                break;
            }
        }
        if (!bFound) { vDeviceInfo[i].bSelected = false; }
    }
}

/*
 *  Resets all filtering, such that all devices are in the list
 */
void ALWin32Acoustic::ResetFilters(void)
{
    for (Size32 i = 0; i < GetNumDevices(); i++)
    { vDeviceInfo[i].bSelected = true; }
    filterIndex = 0;
}

/*
 *  Gets index of first filtered device
 */
Size32 ALWin32Acoustic::GetFirstFilteredDevice(void)
{
    Size32 i;
    for (i = 0; i < GetNumDevices(); i++)
    {
        if (vDeviceInfo[i].bSelected == true) { break; }
    }
    filterIndex = i + 1;
    return i;
}

/*
 *  Gets index of next filtered device
 */
Size32 ALWin32Acoustic::GetNextFilteredDevice(void)
{
    Size32 i;
    for (i = filterIndex; i < GetNumDevices(); i++)
    {
        if (vDeviceInfo[i].bSelected == true) { break; }
    }
    filterIndex = i + 1;
    return i;
}

/*
 *  Internal function to detemine max number of Sources that can be generated
 */
unsigned int ALWin32Acoustic::GetMaxNumSources(void)
{
    ALuint uiSources[256];
    unsigned int iSourceCount = 0;

    // Clear AL Error Code
    ALfunc.alGetError();

    // Generate up to 256 Sources, checking for any errors
    for (iSourceCount = 0; iSourceCount < 256; iSourceCount++)
    {
        ALfunc.alGenSources(1, &uiSources[iSourceCount]);
        if (ALfunc.alGetError() != AL_NO_ERROR) { break; }
    }

    // Release the Sources
    ALfunc.alDeleteSources(iSourceCount, uiSources);
    if (ALfunc.alGetError() != AL_NO_ERROR)
    {
        for (unsigned int i = 0; i < 256; i++)
        { ALfunc.alDeleteSources(1, &uiSources[i]); }
    }

    return iSourceCount;
}
