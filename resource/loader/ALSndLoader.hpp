#ifndef AL_SND_LOADER_HPP_
#define AL_SND_LOADER_HPP_

#include "SndLoader.hpp"

// AL 1.0 did not define the ALchar and ALCchar types, so define them here
// if they don't exist

#ifndef ALchar
#define ALchar char
#endif

namespace reprize
{
namespace res
{
#define MAX_NUM_WAVEID 1024

enum WAVEFILETYPE
{
    WF_EX  = 1,
    WF_EXT = 2,
};

enum WAVERESULT
{
    WR_OK = 0,
    WR_INVALIDFILENAME = -1,
    WR_BADWAVEFILE = -2,
    WR_INVALIDPARAM = -3,
    WR_INVALIDWAVEID = -4,
    WR_NOTSUPPORTEDYET = -5,
    WR_WAVEMUSTBEMONO = -6,
    WR_WAVEMUSTBEWAVEFORMATPCM = -7,
    WR_WAVESMUSTHAVESAMEBITRESOLUTION = -8,
    WR_WAVESMUSTHAVESAMEFREQUENCY = -9,
    WR_WAVESMUSTHAVESAMEBITRATE = -10,
    WR_WAVESMUSTHAVESAMEBLOCKALIGNMENT = -11,
    WR_OFFSETOUTOFDATARANGE = -12,
    WR_FILEERROR = -13,
    WR_OUTOFMEMORY = -14,
    WR_INVALIDSPEAKERPOS = -15,
    WR_INVALIDWAVEFILETYPE = -16,
    WR_NOTWAVEFORMATEXTENSIBLEFORMAT = -17,
};

#ifndef _WAVEFORMAT_
#define _WAVEFORMAT_
typedef struct
{
    uInt16 wFormatTag;
    uInt16 nChannels;
    uInt32 nSamplesPerSec;
    uInt32 nAvgBytesPerSec;
    uInt16 nBlockAlign;
    uInt16 wBitsPerSample;
} WAVEFORMAT;
#endif

#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_
typedef struct
{
    uInt16 wFormatTag;
    uInt16 nChannels;
    uInt32 nSamplesPerSec;
    uInt32 nAvgBytesPerSec;
    uInt16 nBlockAlign;
    uInt16 wBitsPerSample;
    uInt16 cbSize;
} WAVEFORMATEX;
#endif

#ifndef PCMWAVEFORMAT
typedef struct
{
    WAVEFORMAT wf;
    uInt16 wBitsPerSample;
} PCMWAVEFORMAT;
#endif

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_
typedef struct
{
    WAVEFORMATEX Format;
    union
    {
        uInt16 wValidBitsPerSample; /* bits of precision  */
        uInt16 wSamplesPerBlock;    /* valid if wBitsPerSample==0 */
        uInt16 wReserved;           /* If neither applies, set to zero. */
    } Samples;
    uInt32 dwChannelMask;      /* which channels are present in stream  */
//     GUID SubFormat; XXX need for erase
} WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;
#endif

typedef struct
{
    WAVEFILETYPE wfType;
    WAVEFORMATEXTENSIBLE wfEXT; // For non-WAVEFORMATEXTENSIBLE wavefiles, the header is stored in the Format member of wfEXT
    char* pData;
    unsigned long ulDataSize;
    FILE* pFile;
    unsigned long ulDataOffset;
} WAVEFILEINFO, *LPWAVEFILEINFO;

#ifdef WIN32
typedef int (__cdecl *PFNALGETENUMVALUE)(const char *szEnumName);
#else
typedef ALenum (*PFNALGETENUMVALUE)(const ALchar*);
#define SUCCEEDED(n) !(n)

#endif
typedef int WAVEID;

class ALSndLoader : public SndLoader
{
public:
    ALSndLoader(EnvDepInfo* depinfo_);
    virtual ~ALSndLoader(void);

    const bool load(const Str& filename_, const Str& path_);
    void load_wav(const Str& path_, const Str& filename_);

    WAVERESULT LoadWaveFile(const Char *szFilename, WAVEID *WaveID);
    bool LoadWaveToBuffer(const Char* szWaveFile,
                          ALuint uiBufferID, ALenum eXRAMBufferMode = 0);
    WAVERESULT ReadWaveData(WAVEID wave_id_, void *pData,
                            uInt64 ulDataSize,
                            uInt64 *pulBytesWritten);
    WAVERESULT SetWaveDataOffset(WAVEID wave_id_, uInt64 ulOffset);
    WAVERESULT GetWaveDataOffset(WAVEID wave_id_, uInt64 *pulOffset);
    WAVERESULT GetWaveType(WAVEID wave_id_, WAVEFILETYPE *pwfType);
    WAVERESULT GetWaveFormatExHeader(WAVEID wave_id_, WAVEFORMATEX *pWFEX);
    WAVERESULT GetWaveFormatExtensibleHeader(WAVEID wave_id_,
                                             WAVEFORMATEXTENSIBLE *pWFEXT);
    WAVERESULT GetWaveData(WAVEID wave_id_, void **ppAudioData);
    WAVERESULT GetWaveSize(WAVEID wave_id_, uInt64 *pulDataSize);
    WAVERESULT GetWaveFrequency(WAVEID wave_id_, uInt64 *pulFrequency);
    WAVERESULT GetWaveALBufferFormat(WAVEID wave_id_,
                                     PFNALGETENUMVALUE pfnGetEnumValue,
                                     uInt64 *pulFormat);

    WAVERESULT DeleteWaveFile(WAVEID wave_id_);
    bool is_correct_file(WAVEID wave_id_);

    WAVERESULT ParseFile(const Char *szFilename, LPWAVEFILEINFO pWaveInfo);
    WAVEID InsertWaveID(LPWAVEFILEINFO pWaveFileInfo);
protected:
    void GetErrorString(WAVERESULT wr_);
    Sound* build(const Str& name_, const uInt64* array_);
    LPWAVEFILEINFO wavefiles[MAX_NUM_WAVEID];
};
}
}
#endif
