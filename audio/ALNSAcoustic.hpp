#ifndef AL_UNIX_ACOUSTIC_HPP_
#define AL_UNIX_ACOUSTIC_HPP_

#include "Acoustic.hpp"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace reprize
{
namespace aud
{

typedef struct
{
    std::string strDeviceName;
    int iMajorVersion;
    int iMinorVersion;
    unsigned int uiSourceCount;
    std::vector<std::string>* pvstrExtensions;
    bool bSelected;
} ALDEVICEINFO, *LPALDEVICEINFO;

// typedef PFNALGETENUMVALUE (ALenum (*)(const ALchar*));

class ALUNIXAcoustic : public Acoustic
{
public:
    ALUNIXAcoustic(EnvDepInfo* depinfo_);
    virtual ~ALUNIXAcoustic(void);

    virtual const bool init(void);

    virtual void begin_play(void);
    virtual void finish_play(void);

    virtual void test(void);
    virtual void release(void);
private:
    bool ALFWShutdownOpenAL(void);

    bool ALFWLoadWaveToBuffer(const char *szWaveFile,
                              uInt32 uiBufferID, ALenum eXRAMBufferMode = 0);
    Char* ALFWaddMediaPath(const Char *filename);

    void ALDeviceListInitializer(void);
    void ALDeviceListDestroy(void);
    Size32 GetNumDevices(void);
    char *GetDeviceName(Size32 idx_);
    void GetDeviceVersion(Size32 idx_, int* major, int* minor);
    unsigned int GetMaxNumSources(Size32 idx_);
    bool IsExtensionSupported(Size32 idx_, char* szExtName);
    int GetDefaultDevice(void);
    void FilterDevicesMinVer(int major, int minor);
    void FilterDevicesMaxVer(int major, int minor);
    void FilterDevicesExtension(char *szExtName);
    void ResetFilters(void);
    Size32 GetFirstFilteredDevice(void);
    Size32 GetNextFilteredDevice(void);

    unsigned int GetMaxNumSources(void);

    std::vector<ALDEVICEINFO> vDeviceInfo;
    int defaultDeviceIndex;
    Size32 filterIndex;
};
}
}
#endif
