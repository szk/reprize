#include "Common.hpp"

#include <limits.h>

#include "ALUNIXAcoustic.hpp"
#include "Speaker.hpp"
#include "resource/loader/ALSndLoader.hpp"

using namespace reprize;
using namespace res;
using namespace aud;
using namespace std;

ALSndLoader sndloader(NULL);

ALUNIXAcoustic::ALUNIXAcoustic(EnvDepInfo* depinfo_)
    : Acoustic(depinfo_)
{
}

ALUNIXAcoustic::~ALUNIXAcoustic(void)
{
    ALCcontext* pContext;
    ALCdevice* pDevice;

    pContext = alcGetCurrentContext();
    pDevice = alcGetContextsDevice(pContext);

    alcMakeContextCurrent(NULL);
    alcDestroyContext(pContext);
    alcCloseDevice(pDevice);
}

const bool ALUNIXAcoustic::init(void)
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
                          << "Device\n" << std::endl;

                alcMakeContextCurrent(pContext);
                bReturn = true;
            }
            else { alcCloseDevice(pDevice); }
        }

        ALDeviceListDestroy();
    }

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
void ALUNIXAcoustic::appear(void)
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

void ALUNIXAcoustic::begin_play(void)
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
        sleep(100);
        // Get Source State
        alGetSourcei( uiSource, AL_SOURCE_STATE, &iState);
    }
    while (iState == AL_PLAYING);
    alSourceStop(uiSource);
    alDeleteSources(1, &uiSource);
}

void ALUNIXAcoustic::finish_play(void)
{
}

void ALUNIXAcoustic::release(void)
{
}

void ALUNIXAcoustic::test(void)
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
        sleep(100);
        // Get Source State
        alGetSourcei( uiSource, AL_SOURCE_STATE, &iState);
    }
    while (iState == AL_PLAYING);

    // Clean up by deleting Source(s) and Buffer(s)
    alSourceStop(uiSource);
    alDeleteSources(1, &uiSource);
    alDeleteBuffers(1, &uiBuffer);

    ALFWShutdownOpenAL();
}

bool ALUNIXAcoustic::ALFWShutdownOpenAL(void)
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

bool ALUNIXAcoustic::ALFWLoadWaveToBuffer(const char* szWaveFile,
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

    if (WR_OK != sndloader.LoadWaveFile(szWaveFile, &WaveID))
    {
        std::cerr << "LoadWaveFile is failed" << std::endl;
        return bReturn;
    }

    if (WR_OK == sndloader.GetWaveSize(WaveID, (uInt64*)&iDataSize)
        && WR_OK == sndloader.GetWaveData(WaveID, (void**)&pData)
        && WR_OK == sndloader.GetWaveFrequency(WaveID,
                                               (uInt64*)&iFrequency)
        && WR_OK == sndloader.GetWaveALBufferFormat(WaveID, &alGetEnumValue,
                                                    (uInt64*)&eBufferFormat))
    {
        // Set XRAM Mode (if application)
#ifdef UNDEF
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
#endif
    }

    return bReturn;
}

Char fullPath[PATH_MAX];
Char* ALUNIXAcoustic::ALFWaddMediaPath(const ALchar* filename)
{
    sprintf(fullPath, "%s%s", "..\\..\\Media\\", filename);
    return fullPath;
}

// Extension Queries
#ifdef UNDEF
ALboolean ALUNIXAcoustic::ALFWIsXRAMSupported(void)
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

ALboolean ALUNIXAcoustic::ALFWIsEFXSupported(void)
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
void ALUNIXAcoustic::ALDeviceListInitializer(void)
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

    devices = (char*)alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    defaultDeviceName = (char*)alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    index = 0;
    // go through device list (each device terminated with a single NULL,
    // list terminated with double NULL)
    while (*devices != NULL)
    {
        if (strcmp(defaultDeviceName, devices) == 0)
        { defaultDeviceIndex = index; }
        ALCdevice* device = alcOpenDevice(devices);
        if (device)
        {
            ALCcontext* context = alcCreateContext(device, NULL);
            if (context)
            {
                alcMakeContextCurrent(context);
                // if new actual device name isn't already in the list, then add it...
                actualDeviceName = alcGetString(device,
                                                       ALC_DEVICE_SPECIFIER);
                bool bNewName = true;
                for (Size32 i = 0; i < GetNumDevices(); i++)
                {
                    if (strcmp(GetDeviceName(i), actualDeviceName) == 0)
                    { bNewName = false; }
                }
                { std::cerr << "after strdevicename XXXx" << std::endl; return; }
                if ((bNewName) && (actualDeviceName != NULL)
                    && (strlen(actualDeviceName) > 0))
                {
                    memset(&ALDeviceInfo, 0, sizeof(ALDEVICEINFO));
                    ALDeviceInfo.bSelected = true;
                    ALDeviceInfo.strDeviceName = actualDeviceName;
                    alcGetIntegerv(device, ALC_MAJOR_VERSION, sizeof(Int32),
                                   &ALDeviceInfo.iMajorVersion);
                    alcGetIntegerv(device, ALC_MINOR_VERSION, sizeof(Int32),
                                   &ALDeviceInfo.iMinorVersion);

                    ALDeviceInfo.pvstrExtensions = new vector<string>;
                    // Check for ALC Extensions
                    if (alcIsExtensionPresent(device, "ALC_EXT_CAPTURE") == AL_TRUE)
                    { ALDeviceInfo.pvstrExtensions->push_back("ALC_EXT_CAPTURE"); }
                    if (alcIsExtensionPresent(device, "ALC_EXT_EFX") == AL_TRUE)
                    { ALDeviceInfo.pvstrExtensions->push_back("ALC_EXT_EFX"); }
                    // Check for AL Extensions
                    if (alIsExtensionPresent("AL_EXT_OFFSET") == AL_TRUE)
                    { ALDeviceInfo.pvstrExtensions->push_back("AL_EXT_OFFSET"); }

                    if (alIsExtensionPresent("AL_EXT_LINEAR_DISTANCE") == AL_TRUE)
                    { ALDeviceInfo.pvstrExtensions->push_back("AL_EXT_LINEAR_DISTANCE"); }
                    if (alIsExtensionPresent("AL_EXT_EXPONENT_DISTANCE") == AL_TRUE)
                    { ALDeviceInfo.pvstrExtensions->push_back("AL_EXT_EXPONENT_DISTANCE"); }

                    if (alIsExtensionPresent("EAX2.0") == AL_TRUE)
                        ALDeviceInfo.pvstrExtensions->push_back("EAX2.0");
                    if (alIsExtensionPresent("EAX3.0") == AL_TRUE)
                        ALDeviceInfo.pvstrExtensions->push_back("EAX3.0");
                    if (alIsExtensionPresent("EAX4.0") == AL_TRUE)
                        ALDeviceInfo.pvstrExtensions->push_back("EAX4.0");
                    if (alIsExtensionPresent("EAX5.0") == AL_TRUE)
                        ALDeviceInfo.pvstrExtensions->push_back("EAX5.0");
                    if (alIsExtensionPresent("EAX-RAM") == AL_TRUE)
                        ALDeviceInfo.pvstrExtensions->push_back("EAX-RAM");
                    // Get Source Count
                    ALDeviceInfo.uiSourceCount = GetMaxNumSources();
                    vDeviceInfo.push_back(ALDeviceInfo);
                }
                alcMakeContextCurrent(NULL);
                alcDestroyContext(context);
            }
            alcCloseDevice(device);
        }
        devices += strlen(devices) + 1;
        index += 1;
    }

    ResetFilters();
}

/*
 *  Exit call
 */
void ALUNIXAcoustic::ALDeviceListDestroy(void)
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
}

/*
 *  Returns the number of devices in the complete device list
 */
Size32 ALUNIXAcoustic::GetNumDevices(void)
{
    return vDeviceInfo.size();
}

/*
 *  Returns the device name at an index in the complete device list
 */
char*  ALUNIXAcoustic::GetDeviceName(Size32 idx_)
{
    if (idx_ < GetNumDevices())
    { return (char* )vDeviceInfo[idx_].strDeviceName.c_str(); }
    return NULL;
}

/*
 *  Returns the major and minor version numbers for a device at a specified index in the complete list
 */
void ALUNIXAcoustic::GetDeviceVersion(Size32 idx_, int* major, int* minor)
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
unsigned int ALUNIXAcoustic::GetMaxNumSources(Size32 idx_)
{
    if (idx_ < GetNumDevices()) { return vDeviceInfo[idx_].uiSourceCount; }
    return 0;
}

/*
 *  Checks if the extension is supported on the given device
 */
bool ALUNIXAcoustic::IsExtensionSupported(Size32 idx_, char* szExtName)
{
    bool bReturn = false;

    if (idx_ < GetNumDevices())
    {
        for (unsigned int i = 0; i < vDeviceInfo[idx_].pvstrExtensions->size();
             i++)
        {
            if (!strncmp(vDeviceInfo[idx_].pvstrExtensions->at(i).c_str(),
                         szExtName, 3)) // , 3 is XXX
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
int ALUNIXAcoustic::GetDefaultDevice(void)
{
    return defaultDeviceIndex;
}

/*
 *  Deselects devices which don't have the specified minimum version
 */
void ALUNIXAcoustic::FilterDevicesMinVer(int major, int minor)
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
void ALUNIXAcoustic::FilterDevicesMaxVer(int major, int minor)
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
void ALUNIXAcoustic::FilterDevicesExtension(char* szExtName)
{
    bool bFound;

    for (unsigned int i = 0; i < vDeviceInfo.size(); i++)
    {
        bFound = false;
        for (unsigned int j = 0; j < vDeviceInfo[i].pvstrExtensions->size();
             j++)
        {
            if (!strncmp(vDeviceInfo[i].pvstrExtensions->at(j).c_str(),
                         szExtName, 3)) // , 3 is XXX
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
void ALUNIXAcoustic::ResetFilters(void)
{
    for (Size32 i = 0; i < GetNumDevices(); i++)
    { vDeviceInfo[i].bSelected = true; }
    filterIndex = 0;
}

/*
 *  Gets index of first filtered device
 */
Size32 ALUNIXAcoustic::GetFirstFilteredDevice(void)
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
Size32 ALUNIXAcoustic::GetNextFilteredDevice(void)
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
unsigned int ALUNIXAcoustic::GetMaxNumSources(void)
{
    ALuint uiSources[256];
    unsigned int iSourceCount = 0;

    // Clear AL Error Code
    alGetError();

    // Generate up to 256 Sources, checking for any errors
    for (iSourceCount = 0; iSourceCount < 256; iSourceCount++)
    {
        alGenSources(1, &uiSources[iSourceCount]);
        if (alGetError() != AL_NO_ERROR) { break; }
    }

    // Release the Sources
    alDeleteSources(iSourceCount, uiSources);
    if (alGetError() != AL_NO_ERROR)
    {
        for (unsigned int i = 0; i < 256; i++)
        { alDeleteSources(1, &uiSources[i]); }
    }

    return iSourceCount;
}
