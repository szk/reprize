#include "Common.hpp"

#ifdef WIN32
#include "audio/ALWin32Acoustic.hpp"
#else
#include "audio/ALUNIXAcoustic.hpp"
#endif // WIN32

#include "ALSndLoader.hpp"

using namespace reprize;
using namespace res;
using namespace aud;
using namespace std;

ALSndLoader::ALSndLoader(EnvDepInfo* depinfo_)
    : SndLoader(depinfo_)
{
    memset(&wavefiles, 0, sizeof(wavefiles));
}

ALSndLoader::~ALSndLoader(void)
{
    long lLoop;
    for (lLoop = 0; lLoop < MAX_NUM_WAVEID; lLoop++)
    {
        if (!wavefiles[lLoop]) { continue; }

        if (wavefiles[lLoop]->pData) { delete wavefiles[lLoop]->pData; }
        if (wavefiles[lLoop]->pFile) { fclose(wavefiles[lLoop]->pFile); }

        delete wavefiles[lLoop];
        wavefiles[lLoop] = 0;
    }
}

const bool ALSndLoader::load(const Str& filename_, const Str& path_)
{
    loaded_node = NULL;

    if (compare_extension(filename_, ".wav")) { load_wav(path_, filename_); }
    else if (compare_extension(filename_, ".mp3")) { return false; }
    else if (compare_extension(filename_, ".ogg")) { return false; }
    if (loaded_node == NULL) { return false; }

//     RE_DBG("texture: %s, %s : %dx%d",
//                   path_.c_str(), filename_.c_str(),
//                   dynamic_cast<Sound*>(loaded_node)->get_width(),
//                   dynamic_cast<Sound*>(loaded_node)->get_height());
    return true;
}

void ALSndLoader::load_wav(const Str& path_, const Str& filename_)
{
    ALuint uiBuffer = 0;

    alGenBuffers(1, &uiBuffer );
    if (!LoadWaveToBuffer(path_.c_str(), uiBuffer))
    { RE_DBG("Failed to load: %s", path_.c_str()); }

    loaded_node = new res::Sound(filename_, uiBuffer);
    RE_DBG("wav: %s", filename_.c_str());

    return;
}

typedef struct
{
    Char szRIFF[4];
    uInt64 ulRIFFSize;
    Char szWAVE[4];
} WAVEFILEHEADER;

typedef struct
{
    Char szChunkName[4];
    uInt64 ulChunkSize;
} RIFFCHUNK;

typedef struct
{
    unsigned short usFormatTag;
    unsigned short usChannels;
    uInt64 ulSamplesPerSec;
    uInt64 ulAvgBytesPerSec;
    unsigned short usBlockAlign;
    unsigned short usBitsPerSample;
    unsigned short usSize;
    unsigned short usReserved;
    uInt64 ulChannelMask;
//     GUID guidSubFormat; XXX need for erase
} WAVEFMT;

WAVERESULT ALSndLoader::LoadWaveFile(const Char *szFilename, WAVEID *pwave_id_)
{
    RE_DBG("loadwavefile1");
    WAVERESULT wr = WR_OUTOFMEMORY;
    LPWAVEFILEINFO pWaveInfo;

    pWaveInfo = new WAVEFILEINFO;
    if (!pWaveInfo)
    {
        RE_DBG("wr_outofmemory?");
        return WR_OUTOFMEMORY;
    }

    RE_DBG("loadwavefile2");
    wr = ParseFile(szFilename, pWaveInfo);

    if (wr == WR_OK)
    {
        RE_DBG("parse wavefile");
        // Allocate memory for sample data
        pWaveInfo->pData = new Char[pWaveInfo->ulDataSize];
        if (!pWaveInfo->pData)
        { wr = WR_OUTOFMEMORY; }
        else
        {
            // Seek to start of audio data
            fseek(pWaveInfo->pFile, pWaveInfo->ulDataOffset, SEEK_SET);

            // Read Sample Data
            if (fread(pWaveInfo->pData, 1, pWaveInfo->ulDataSize,
                      pWaveInfo->pFile) == pWaveInfo->ulDataSize)
            {
                long lLoop = 0;
                for (lLoop = 0; lLoop < MAX_NUM_WAVEID; lLoop++)
                {
                    if (!wavefiles[lLoop])
                    {
                        wavefiles[lLoop] = pWaveInfo;
                        *pwave_id_ = lLoop;
                        wr = WR_OK;
                        RE_DBG("success");
                        break;
                    }
                }

                if (lLoop == MAX_NUM_WAVEID)
                {
                    delete pWaveInfo->pData;
                    wr = WR_OUTOFMEMORY;
                    RE_DBG("outofmemory");
                }
                RE_DBG("sample data read");
            }
            else
            {
                delete pWaveInfo->pData;
                wr = WR_BADWAVEFILE;
                RE_DBG("badwavefile");
            }
        }
        fclose(pWaveInfo->pFile);
        pWaveInfo->pFile = 0;
    }

    RE_DBG("loadwavefile3");
    if (wr != WR_OK)
    {
        RE_DBG("wr is not ok");
        delete pWaveInfo;
    }

    return wr;
}

bool ALSndLoader::LoadWaveToBuffer(const Char* szWaveFile,
                                   ALuint uiBufferID, ALenum eXRAMBufferMode)
{
    WAVEID wave_id;
    Int32 iDataSize, iFrequency;
    ALenum eBufferFormat;
    Char* pData;
    bool bReturn;

    bReturn = false;

    if (!SUCCEEDED(LoadWaveFile(szWaveFile, &wave_id)))
    {
        RE_DBG("LoadWaveFile is failed");
        return bReturn;
    }

    if ((SUCCEEDED(GetWaveSize(wave_id, (uInt64*)&iDataSize)))
        && (SUCCEEDED(GetWaveData(wave_id, (void**)&pData)))
        && (SUCCEEDED(GetWaveFrequency(wave_id, (uInt64*)&iFrequency)))
        && (SUCCEEDED(GetWaveALBufferFormat(wave_id, &alGetEnumValue,
                                            (uInt64*)&eBufferFormat))))
    {
        // Set XRAM Mode (if application)
//         if (eaxSetBufferMode && eXRAMBufferMode)
//         { eaxSetBufferMode(1, &uiBufferID, eXRAMBufferMode); }

        if (alGetError() == AL_NO_ERROR)
        { RE_DBG("no_error before buffer data"); }

        alBufferData(uiBufferID, eBufferFormat, pData, iDataSize, iFrequency);
        ALenum e = alGetError();

        if (e == AL_NO_ERROR)
        {
            RE_DBG("al_no_error");
            bReturn = true;
        }

        RE_DBG("al_an_error: %d", e);
        DeleteWaveFile(wave_id);
    }

    return bReturn;
}


WAVERESULT ALSndLoader::ReadWaveData(WAVEID wave_id, void *pData,
                                     uInt64 ulDataSize,
                                     uInt64 *pulBytesWritten)
{
    LPWAVEFILEINFO pWaveInfo;

    if (!pData || !pulBytesWritten || ulDataSize == 0)
    { return WR_INVALIDPARAM; }

    if (!is_correct_file(wave_id)) { return WR_INVALIDWAVEID; }

    pWaveInfo = wavefiles[wave_id];
    if (!pWaveInfo->pFile) { return WR_BADWAVEFILE; }

    uInt64 ulOffset = ftell(pWaveInfo->pFile);

    if ((ulOffset - pWaveInfo->ulDataOffset + ulDataSize)
        > pWaveInfo->ulDataSize)
    {
        ulDataSize = pWaveInfo->ulDataSize
            - (ulOffset - pWaveInfo->ulDataOffset);
    }

    *pulBytesWritten = (uInt64)fread(pData, 1, ulDataSize,
                                            pWaveInfo->pFile);

    return WR_OK;
}

WAVERESULT ALSndLoader::SetWaveDataOffset(WAVEID wave_id_,
                                          uInt64 ulOffset)
{
    LPWAVEFILEINFO pWaveInfo;

    if (!is_correct_file(wave_id_)) { return WR_INVALIDWAVEID; }

    pWaveInfo = wavefiles[wave_id_];
    if (pWaveInfo->pFile)
    {
        fseek(pWaveInfo->pFile, pWaveInfo->ulDataOffset + ulOffset, SEEK_SET);
        return WR_OK;
    }

    return WR_INVALIDPARAM;
}

WAVERESULT ALSndLoader::GetWaveDataOffset(WAVEID wave_id_,
                                          uInt64 *pulOffset)
{
    LPWAVEFILEINFO pWaveInfo;

    if (!is_correct_file(wave_id_)) { return WR_INVALIDWAVEID; }

    pWaveInfo = wavefiles[wave_id_];
    if ((pWaveInfo->pFile) && (pulOffset))
    {
        // Get current position
        *pulOffset = ftell(pWaveInfo->pFile);
        *pulOffset -= pWaveInfo->ulDataOffset;
        return WR_OK;
    }
    return WR_INVALIDPARAM;
}

WAVERESULT ALSndLoader::ParseFile(const Char *szFilename,
                                  LPWAVEFILEINFO pWaveInfo)
{
    WAVEFILEHEADER waveFileHeader;
    RIFFCHUNK riffChunk;
    WAVEFMT waveFmt;
    WAVERESULT wr = WR_BADWAVEFILE;

    if (!szFilename || !pWaveInfo) { return WR_INVALIDPARAM; }

    memset(pWaveInfo, 0, sizeof(WAVEFILEINFO));

    // Open the wave file for reading
    pWaveInfo->pFile = fopen(szFilename, "rb");
    if (!pWaveInfo->pFile) { return WR_INVALIDFILENAME; }
    // Read Wave file header
    fread(&waveFileHeader, 1, sizeof(WAVEFILEHEADER), pWaveInfo->pFile);
    if (!strncmp(waveFileHeader.szRIFF, "RIFF", 4)
        && !strncmp(waveFileHeader.szWAVE, "WAVE", 4))
    {
        while (fread(&riffChunk, 1, sizeof(RIFFCHUNK),
                     pWaveInfo->pFile) == sizeof(RIFFCHUNK))
        {
            if (!strncmp(riffChunk.szChunkName, "fmt ", 4))
            {
                if (riffChunk.ulChunkSize <= sizeof(WAVEFMT))
                {
                    fread(&waveFmt, 1, riffChunk.ulChunkSize,
                          pWaveInfo->pFile);

                    // Determine if this is a WAVEFORMATEX
                    // or WAVEFORMATEXTENSIBLE wave file
                    if (waveFmt.usFormatTag == WAVE_FORMAT_PCM)
                    {
                        pWaveInfo->wfType = WF_EX;
                        memcpy(&pWaveInfo->wfEXT.Format, &waveFmt,
                               sizeof(PCMWAVEFORMAT));
                    }
                    else if (waveFmt.usFormatTag == WAVE_FORMAT_EXTENSIBLE)
                    {
                        pWaveInfo->wfType = WF_EXT;
                        memcpy(&pWaveInfo->wfEXT, &waveFmt,
                               sizeof(WAVEFORMATEXTENSIBLE));
                    }
                }
                else
                { fseek(pWaveInfo->pFile, riffChunk.ulChunkSize, SEEK_CUR); }
            }
            else if (!strncmp(riffChunk.szChunkName, "data", 4))
            {
                pWaveInfo->ulDataSize = riffChunk.ulChunkSize;
                pWaveInfo->ulDataOffset = ftell(pWaveInfo->pFile);
                fseek(pWaveInfo->pFile, riffChunk.ulChunkSize, SEEK_CUR);
            }
            else
            { fseek(pWaveInfo->pFile, riffChunk.ulChunkSize, SEEK_CUR); }

            // Ensure that we are correctly aligned for next chunk
            if (riffChunk.ulChunkSize & 1)
            {
                fseek(pWaveInfo->pFile, 1, SEEK_CUR);
            }
        }

        if (pWaveInfo->ulDataSize && pWaveInfo->ulDataOffset
            && ((pWaveInfo->wfType == WF_EX) || (pWaveInfo->wfType == WF_EXT)))
        {
            wr = WR_OK;
            RE_DBG("wr_ok reached");
        }
        else
        {
            RE_DBG("closeed");
            fclose(pWaveInfo->pFile);
        }
    }

    return wr;
}


WAVERESULT ALSndLoader::DeleteWaveFile(WAVEID wave_id_)
{
    if (is_correct_file(wave_id_))
    {
        if (wavefiles[wave_id_]->pData) { delete wavefiles[wave_id_]->pData; }
        if (wavefiles[wave_id_]->pFile) { fclose(wavefiles[wave_id_]->pFile); }
        delete wavefiles[wave_id_];
        wavefiles[wave_id_] = 0;
        return WR_OK;
    }

    return WR_INVALIDWAVEID;
}


WAVERESULT ALSndLoader::GetWaveType(WAVEID wave_id_, WAVEFILETYPE *wfType)
{
    if (!is_correct_file(wave_id_)) { return WR_INVALIDWAVEID; }
    if (!wfType) { return WR_INVALIDPARAM; }

    *wfType = wavefiles[wave_id_]->wfType;

    return WR_OK;
}


WAVERESULT ALSndLoader::GetWaveFormatExHeader(WAVEID wave_id_, WAVEFORMATEX *wfex)
{
    if (!is_correct_file(wave_id_)) { return WR_INVALIDWAVEID; }
    if (!wfex) { return WR_INVALIDPARAM; }
    memcpy(wfex, &(wavefiles[wave_id_]->wfEXT.Format), sizeof(WAVEFORMATEX));

    return WR_OK;
}


WAVERESULT ALSndLoader::GetWaveFormatExtensibleHeader(WAVEID wave_id_,
                                                      WAVEFORMATEXTENSIBLE *wfext)
{
    if (!is_correct_file(wave_id_)) { return WR_INVALIDWAVEID; }
    if (wavefiles[wave_id_]->wfType != WF_EXT)
    { return WR_NOTWAVEFORMATEXTENSIBLEFORMAT; }
    if (!wfext) { return WR_INVALIDPARAM; }

    memcpy(wfext, &(wavefiles[wave_id_]->wfEXT), sizeof(WAVEFORMATEXTENSIBLE));

    return WR_OK;
}

WAVERESULT ALSndLoader::GetWaveData(WAVEID wave_id_, void **lplpAudioData)
{
    if (!is_correct_file(wave_id_)) { return WR_INVALIDWAVEID; }
    if (!lplpAudioData) { return WR_INVALIDPARAM; }
    *lplpAudioData = wavefiles[wave_id_]->pData;

    return WR_OK;
}

WAVERESULT ALSndLoader::GetWaveSize(WAVEID wave_id_, uInt64 *size)
{
    if (!is_correct_file(wave_id_)) { return WR_INVALIDWAVEID; }
    if (!size) { return WR_INVALIDPARAM; }
    *size = wavefiles[wave_id_]->ulDataSize;

    return WR_OK;
}


WAVERESULT ALSndLoader::GetWaveFrequency(WAVEID wave_id_,
                                         uInt64 *pulFrequency)
{
    if (is_correct_file(wave_id_))
    {
        if (pulFrequency)
        { *pulFrequency = wavefiles[wave_id_]->wfEXT.Format.nSamplesPerSec; }
        else { return WR_INVALIDPARAM; }
    }
    else { return WR_INVALIDWAVEID; }

    return WR_OK;
}

WAVERESULT ALSndLoader::GetWaveALBufferFormat(WAVEID wave_id_,
                                              PFNALGETENUMVALUE pfnGetEnumValue,
                                              uInt64 *pulFormat)
{
    WAVERESULT wr = WR_OK;

    if (!is_correct_file(wave_id_)) { return WR_INVALIDWAVEID; }
    if (!(pfnGetEnumValue && pulFormat)) { return WR_INVALIDPARAM; }

    *pulFormat = 0;

    if (wavefiles[wave_id_]->wfType == WF_EX)
    {
        if (wavefiles[wave_id_]->wfEXT.Format.nChannels == 1)
        {
            if (wavefiles[wave_id_]->wfEXT.Format.wBitsPerSample == 16)
            { *pulFormat = pfnGetEnumValue("AL_FORMAT_MONO16"); }
            else { *pulFormat = pfnGetEnumValue("AL_FORMAT_MONO8"); }
        }
        else if (wavefiles[wave_id_]->wfEXT.Format.nChannels == 2)
        {
            if (wavefiles[wave_id_]->wfEXT.Format.wBitsPerSample == 16)
            { *pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO16"); }
            else { *pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO8"); }
        }
        else if ((wavefiles[wave_id_]->wfEXT.Format.nChannels == 4)
                 && (wavefiles[wave_id_]->wfEXT.Format.wBitsPerSample == 16))
        { *pulFormat = pfnGetEnumValue("AL_FORMAT_QUAD16"); }
    }
    else if (wavefiles[wave_id_]->wfType == WF_EXT)
    {
        if ((wavefiles[wave_id_]->wfEXT.Format.nChannels == 1)
            && (wavefiles[wave_id_]->wfEXT.dwChannelMask
                == SPEAKER_FRONT_CENTER))
        {
            if (wavefiles[wave_id_]->wfEXT.Format.wBitsPerSample == 16)
            { *pulFormat = pfnGetEnumValue("AL_FORMAT_MONO16"); }
            else { *pulFormat = pfnGetEnumValue("AL_FORMAT_MONO8"); }
        }
        else if ((wavefiles[wave_id_]->wfEXT.Format.nChannels == 2)
                 && (wavefiles[wave_id_]->wfEXT.dwChannelMask
                     == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT)))
        {
            if (wavefiles[wave_id_]->wfEXT.Format.wBitsPerSample == 16)
            { *pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO16"); }
            else { *pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO8"); }
        }
        else if ((wavefiles[wave_id_]->wfEXT.Format.nChannels == 2)
                 && (wavefiles[wave_id_]->wfEXT.Format.wBitsPerSample == 16)
                 && (wavefiles[wave_id_]->wfEXT.dwChannelMask
                     == (SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
        { *pulFormat = pfnGetEnumValue("AL_FORMAT_REAR16"); }
        else if ((wavefiles[wave_id_]->wfEXT.Format.nChannels == 4)
                 && (wavefiles[wave_id_]->wfEXT.Format.wBitsPerSample == 16)
                 && (wavefiles[wave_id_]->wfEXT.dwChannelMask
                     == (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT
                         | SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
        { *pulFormat = pfnGetEnumValue("AL_FORMAT_QUAD16"); }
        else if ((wavefiles[wave_id_]->wfEXT.Format.nChannels == 6)
                 && (wavefiles[wave_id_]->wfEXT.Format.wBitsPerSample == 16)
                 && (wavefiles[wave_id_]->wfEXT.dwChannelMask
                     == (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT
                         | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY
                         | SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
        { *pulFormat = pfnGetEnumValue("AL_FORMAT_51CHN16"); }
        else if ((wavefiles[wave_id_]->wfEXT.Format.nChannels == 7)
                 && (wavefiles[wave_id_]->wfEXT.Format.wBitsPerSample == 16)
                 && (wavefiles[wave_id_]->wfEXT.dwChannelMask
                     == (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT
                         | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY
                         | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT
                         | SPEAKER_BACK_CENTER)))
        { *pulFormat = pfnGetEnumValue("AL_FORMAT_61CHN16"); }
        else if ((wavefiles[wave_id_]->wfEXT.Format.nChannels == 8)
                 && (wavefiles[wave_id_]->wfEXT.Format.wBitsPerSample == 16)
                 && (wavefiles[wave_id_]->wfEXT.dwChannelMask
                     == (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT
                         | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY
                         | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT
                         | SPEAKER_SIDE_LEFT|SPEAKER_SIDE_RIGHT)))
        { *pulFormat = pfnGetEnumValue("AL_FORMAT_71CHN16"); }
    }

    if (*pulFormat == 0) { wr = WR_INVALIDWAVEFILETYPE; }

    return wr;
}

bool ALSndLoader::is_correct_file(WAVEID wave_id_)
{
    if ((wave_id_ >= 0) && (wave_id_ < MAX_NUM_WAVEID))
    {
        if (wavefiles[wave_id_]) { return true; }
    }

    return false;
}

void ALSndLoader::GetErrorString(WAVERESULT wr_)
{
    switch (wr_)
    {
    case WR_OK: RE_DBG("Success");
        break;
    case WR_INVALIDFILENAME:
        RE_DBG("Invalid file name or file does not exist");
        break;
    case WR_BADWAVEFILE: RE_DBG("Invalid Wave file");
        break;
    case WR_INVALIDPARAM:
        RE_DBG("Invalid parameter passed to function");
        break;
    case WR_FILEERROR: RE_DBG("File I/O error");
        break;
    case WR_INVALIDWAVEID: RE_DBG("Invalid WAVEID");
        break;
    case WR_NOTSUPPORTEDYET: RE_DBG("Function not supported yet");
        break;
    case WR_WAVEMUSTBEMONO: RE_DBG("Input wave files must be mono");
        break;
    case WR_WAVEMUSTBEWAVEFORMATPCM:
        RE_DBG("Input wave files must be in Wave Format PCM");
        break;
    case WR_WAVESMUSTHAVESAMEBITRESOLUTION:
        RE_DBG("Input wave files must have the same Bit Resolution");
        break;
    case WR_WAVESMUSTHAVESAMEFREQUENCY:
        RE_DBG("Input wave files must have the same Frequency");
        break;
    case WR_WAVESMUSTHAVESAMEBITRATE:
        RE_DBG("Input wave files must have the same Bit Rate");
        break;
    case WR_WAVESMUSTHAVESAMEBLOCKALIGNMENT:
        RE_DBG("Input wave files must have the same Block Alignment");
        break;
    case WR_OFFSETOUTOFDATARANGE:
        RE_DBG("Wave files Offset is not within audio data");
        break;
    case WR_INVALIDSPEAKERPOS:
        RE_DBG("Invalid Speaker Destinations");
        break;
    case WR_OUTOFMEMORY: RE_DBG("Out of memory");
        break;
    case WR_INVALIDWAVEFILETYPE: RE_DBG("Invalid Wave File Type");
        break;
    case WR_NOTWAVEFORMATEXTENSIBLEFORMAT:
        RE_DBG("Wave file is not in WAVEFORMATEXTENSIBLE format");
        break;
    default: RE_DBG("Undefined error");
    }
}

res::Sound* ALSndLoader::build(const Str& name_, const uInt64* array_)
{
    // Generate using math
    // We want a note of 440 Hz. 32 samples for each oscillation
    // for 1 seconds, means 440*32 bytes
    uChar *sineWave;
    int samples = 16, frequency = 442;
    int freq = samples * frequency;
    int size = freq; // We don't need an higher frequency than the signal
    if (!(sineWave = (uChar*)malloc(size))) { return NULL; }

    for (int i = 0; i < size; ++i)
    {
        float x = i * 360.0 / (float)samples;
        sineWave[i] = sin(RADIAN(x)) * 128 + 128; // Clamp to [0,255]
    }
    ALvoid* data = sineWave;
    ALuint buffer[1];
    alGenBuffers(1, buffer);
    ALenum format = AL_FORMAT_MONO8;

    alBufferData(buffer[0], format, data, size, freq);

    Sound* snd = new Sound(name_, buffer[0]);

    // Setup sources
    ALuint source[1];
    alGenSources(1, source);
    if (alGetError() != AL_NO_ERROR) { return NULL; }

    float srcPos[] = {3.0, 0.0, -3.0};

    alSourcei(source[0], AL_BUFFER, buffer[0]);
    alSourcefv(source[0], AL_POSITION, srcPos);

    float lstPos[] = {0.0, 0.0, 0.0};
    alListenerfv(AL_POSITION, lstPos);

    alSourcePlay(source[0]);

    // Wait until termination of the sound
//     Sleep(2000);

    // Clear the things
    alDeleteSources(1, source);
    alDeleteBuffers(1, buffer);
    return snd;
}
