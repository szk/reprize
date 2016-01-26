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
    memset(&m_WaveIDs, 0, sizeof(m_WaveIDs));
}

ALSndLoader::~ALSndLoader(void)
{
    long lLoop;

    for (lLoop = 0; lLoop < MAX_NUM_WAVEID; lLoop++)
    {
        if (!m_WaveIDs[lLoop]) { continue; }

        if (m_WaveIDs[lLoop]->pData) { delete m_WaveIDs[lLoop]->pData; }
        if (m_WaveIDs[lLoop]->pFile) { fclose(m_WaveIDs[lLoop]->pFile); }

        delete m_WaveIDs[lLoop];
        m_WaveIDs[lLoop] = 0;
    }
}

const bool ALSndLoader::load(const Str& filename_, const Str& path_)
{
    loaded_node = NULL;

    if (compare_extension(filename_, ".wav")) { load_wav(path_, filename_); }
    else if (compare_extension(filename_, ".mp3")) { return false; }
    else if (compare_extension(filename_, ".ogg")) { return false; }

    if (loaded_node == NULL) { return false; }

//     g_log->printf("texture: %s, %s : %dx%d",
//                   path_.c_str(), filename_.c_str(),
//                   dynamic_cast<Sound*>(loaded_node)->get_width(),
//                   dynamic_cast<Sound*>(loaded_node)->get_height());
    return true;
}

res::Sound* ALSndLoader::build(const Str& name_, const uInt32* array_)
{
    return NULL;
}

void ALSndLoader::load_wav(const Str& path_, const Str& filename_)
{
    ALuint uiBuffer = 0;

    alGenBuffers(1, &uiBuffer );
    if (!LoadWaveToBuffer(path_.c_str(), uiBuffer))
    { std::cerr << "Failed to load: " << path_ << std::endl; }

    loaded_node = new res::Sound(filename_, uiBuffer);
    cerr << "wav: " << filename_ << endl;

    return;
}

typedef struct
{
    char szRIFF[4];
    unsigned long ulRIFFSize;
    char szWAVE[4];
} WAVEFILEHEADER;

typedef struct
{
    char szChunkName[4];
    unsigned long ulChunkSize;
} RIFFCHUNK;

typedef struct
{
    unsigned short usFormatTag;
    unsigned short usChannels;
    unsigned long ulSamplesPerSec;
    unsigned long ulAvgBytesPerSec;
    unsigned short usBlockAlign;
    unsigned short usBitsPerSample;
    unsigned short usSize;
    unsigned short usReserved;
    unsigned long ulChannelMask;
//     GUID guidSubFormat; XXX need for erase
} WAVEFMT;

WAVERESULT ALSndLoader::LoadWaveFile(const char *szFilename, WAVEID *pWaveID)
{
    cerr << "loadwavefile1" << endl;
    WAVERESULT wr = WR_OUTOFMEMORY;
    LPWAVEFILEINFO pWaveInfo;

    pWaveInfo = new WAVEFILEINFO;
    if (!pWaveInfo)
    {
        cerr << "wr_outofmemory?" << endl;
        return WR_OUTOFMEMORY;
    }

    cerr << "loadwavefile2" << endl;
    wr = ParseFile(szFilename, pWaveInfo);

    if (wr == WR_OK)
    {
        cerr << "parse wavefile" << endl;
        // Allocate memory for sample data
        pWaveInfo->pData = new char[pWaveInfo->ulDataSize];
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
                    if (!m_WaveIDs[lLoop])
                    {
                        m_WaveIDs[lLoop] = pWaveInfo;
                        *pWaveID = lLoop;
                        wr = WR_OK;
                        cerr << "success" << endl;
                        break;
                    }
                }

                if (lLoop == MAX_NUM_WAVEID)
                {
                    delete pWaveInfo->pData;
                    wr = WR_OUTOFMEMORY;
                    cerr << "outofmemory" << endl;
                }
                cerr << "sample data read" << endl;
            }
            else
            {
                delete pWaveInfo->pData;
                wr = WR_BADWAVEFILE;
                cerr << "badwavefile" << endl;
            }
        }
        fclose(pWaveInfo->pFile);
        pWaveInfo->pFile = 0;
    }

    cerr << "loadwavefile3" << endl;
    if (wr != WR_OK)
    {
        cerr << "wr is not ok" << endl;
        delete pWaveInfo;
    }

    return wr;
}

bool ALSndLoader::LoadWaveToBuffer(const char* szWaveFile,
                                   ALuint uiBufferID, ALenum eXRAMBufferMode)
{
    WAVEID WaveID;
    Int32 iDataSize, iFrequency;
    ALenum eBufferFormat;
    Char* pData;
    bool bReturn;

    bReturn = false;

    if (!SUCCEEDED(LoadWaveFile(szWaveFile, &WaveID)))
    {
        std::cerr << "LoadWaveFile is failed" << std::endl;
        return bReturn;
    }

    if ((SUCCEEDED(GetWaveSize(WaveID, (unsigned long*)&iDataSize)))
        && (SUCCEEDED(GetWaveData(WaveID, (void**)&pData)))
        && (SUCCEEDED(GetWaveFrequency(WaveID, (unsigned long*)&iFrequency)))
        && (SUCCEEDED(GetWaveALBufferFormat(WaveID, &alGetEnumValue,
                                            (unsigned long*)&eBufferFormat))))
    {
        // Set XRAM Mode (if application)
//         if (eaxSetBufferMode && eXRAMBufferMode)
//         { eaxSetBufferMode(1, &uiBufferID, eXRAMBufferMode); }

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
        DeleteWaveFile(WaveID);
    }

    return bReturn;
}


WAVERESULT ALSndLoader::ReadWaveData(WAVEID WaveID, void *pData,
                                     unsigned long ulDataSize,
                                     unsigned long *pulBytesWritten)
{
    LPWAVEFILEINFO pWaveInfo;

    if (!pData || !pulBytesWritten || ulDataSize == 0)
    { return WR_INVALIDPARAM; }

    if (!IsWaveID(WaveID)) { return WR_INVALIDWAVEID; }

    pWaveInfo = m_WaveIDs[WaveID];
    if (!pWaveInfo->pFile) { return WR_BADWAVEFILE; }

    unsigned long ulOffset = ftell(pWaveInfo->pFile);

    if ((ulOffset - pWaveInfo->ulDataOffset + ulDataSize)
        > pWaveInfo->ulDataSize)
    {
        ulDataSize = pWaveInfo->ulDataSize
            - (ulOffset - pWaveInfo->ulDataOffset);
    }

    *pulBytesWritten = (unsigned long)fread(pData, 1, ulDataSize,
                                            pWaveInfo->pFile);

    return WR_OK;
}

WAVERESULT ALSndLoader::SetWaveDataOffset(WAVEID WaveID, unsigned long ulOffset)
{
    LPWAVEFILEINFO pWaveInfo;

    if (!IsWaveID(WaveID)) { return WR_INVALIDWAVEID; }

    pWaveInfo = m_WaveIDs[WaveID];
    if (pWaveInfo->pFile)
    {
        fseek(pWaveInfo->pFile, pWaveInfo->ulDataOffset + ulOffset, SEEK_SET);
        return WR_OK;
    }

    return WR_INVALIDPARAM;
}

WAVERESULT ALSndLoader::GetWaveDataOffset(WAVEID WaveID,
                                          unsigned long *pulOffset)
{
    LPWAVEFILEINFO pWaveInfo;

    if (!IsWaveID(WaveID)) { return WR_INVALIDWAVEID; }

    pWaveInfo = m_WaveIDs[WaveID];
    if ((pWaveInfo->pFile) && (pulOffset))
    {
        // Get current position
        *pulOffset = ftell(pWaveInfo->pFile);
        *pulOffset -= pWaveInfo->ulDataOffset;
        return WR_OK;
    }
    return WR_INVALIDPARAM;
}

WAVERESULT ALSndLoader::ParseFile(const char *szFilename,
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
            cerr << "s";
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
            cerr << "wr_ok reached" << endl;
        }
        else
        {
            cerr << "closeed" << endl;
            fclose(pWaveInfo->pFile);
        }
    }

    return wr;
}


WAVERESULT ALSndLoader::DeleteWaveFile(WAVEID WaveID)
{
    if (IsWaveID(WaveID))
    {
        if (m_WaveIDs[WaveID]->pData) { delete m_WaveIDs[WaveID]->pData; }
        if (m_WaveIDs[WaveID]->pFile) { fclose(m_WaveIDs[WaveID]->pFile); }
        delete m_WaveIDs[WaveID];
        m_WaveIDs[WaveID] = 0;
        return WR_OK;
    }

    return WR_INVALIDWAVEID;
}


WAVERESULT ALSndLoader::GetWaveType(WAVEID WaveID, WAVEFILETYPE *wfType)
{
    if (!IsWaveID(WaveID)) { return WR_INVALIDWAVEID; }
    if (!wfType) { return WR_INVALIDPARAM; }

    *wfType = m_WaveIDs[WaveID]->wfType;

    return WR_OK;
}


WAVERESULT ALSndLoader::GetWaveFormatExHeader(WAVEID WaveID, WAVEFORMATEX *wfex)
{
    if (!IsWaveID(WaveID)) { return WR_INVALIDWAVEID; }
    if (!wfex) { return WR_INVALIDPARAM; }
    memcpy(wfex, &(m_WaveIDs[WaveID]->wfEXT.Format), sizeof(WAVEFORMATEX));

    return WR_OK;
}


WAVERESULT ALSndLoader::GetWaveFormatExtensibleHeader(WAVEID WaveID,
                                                      WAVEFORMATEXTENSIBLE *wfext)
{
    if (!IsWaveID(WaveID)) { return WR_INVALIDWAVEID; }
    if (m_WaveIDs[WaveID]->wfType != WF_EXT)
    { return WR_NOTWAVEFORMATEXTENSIBLEFORMAT; }
    if (!wfext) { return WR_INVALIDPARAM; }

    memcpy(wfext, &(m_WaveIDs[WaveID]->wfEXT), sizeof(WAVEFORMATEXTENSIBLE));

    return WR_OK;
}

WAVERESULT ALSndLoader::GetWaveData(WAVEID WaveID, void **lplpAudioData)
{
    if (!IsWaveID(WaveID)) { return WR_INVALIDWAVEID; }
    if (!lplpAudioData) { return WR_INVALIDPARAM; }
    *lplpAudioData = m_WaveIDs[WaveID]->pData;

    return WR_OK;
}

WAVERESULT ALSndLoader::GetWaveSize(WAVEID WaveID, unsigned long *size)
{
    if (!IsWaveID(WaveID)) { return WR_INVALIDWAVEID; }
    if (!size) { return WR_INVALIDPARAM; }
    *size = m_WaveIDs[WaveID]->ulDataSize;

    return WR_OK;
}


WAVERESULT ALSndLoader::GetWaveFrequency(WAVEID WaveID,
                                         unsigned long *pulFrequency)
{
    if (IsWaveID(WaveID))
    {
        if (pulFrequency)
        { *pulFrequency = m_WaveIDs[WaveID]->wfEXT.Format.nSamplesPerSec; }
        else { return WR_INVALIDPARAM; }
    }
    else { return WR_INVALIDWAVEID; }

    return WR_OK;
}

WAVERESULT ALSndLoader::GetWaveALBufferFormat(WAVEID WaveID,
                                              PFNALGETENUMVALUE pfnGetEnumValue,
                                              unsigned long *pulFormat)
{
    WAVERESULT wr = WR_OK;

    if (!IsWaveID(WaveID)) { return WR_INVALIDWAVEID; }
    if (!(pfnGetEnumValue && pulFormat)) { return WR_INVALIDPARAM; }

    *pulFormat = 0;

    if (m_WaveIDs[WaveID]->wfType == WF_EX)
    {
        if (m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 1)
        {
            if (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample == 16)
            { *pulFormat = pfnGetEnumValue("AL_FORMAT_MONO16"); }
            else { *pulFormat = pfnGetEnumValue("AL_FORMAT_MONO8"); }
        }
        else if (m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 2)
        {
            if (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample == 16)
            { *pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO16"); }
            else { *pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO8"); }
        }
        else if ((m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 4)
                 && (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample == 16))
        { *pulFormat = pfnGetEnumValue("AL_FORMAT_QUAD16"); }
    }
    else if (m_WaveIDs[WaveID]->wfType == WF_EXT)
    {
        if ((m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 1)
            && (m_WaveIDs[WaveID]->wfEXT.dwChannelMask == SPEAKER_FRONT_CENTER))
        {
            if (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample == 16)
            { *pulFormat = pfnGetEnumValue("AL_FORMAT_MONO16"); }
            else { *pulFormat = pfnGetEnumValue("AL_FORMAT_MONO8"); }
        }
        else if ((m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 2)
                 && (m_WaveIDs[WaveID]->wfEXT.dwChannelMask
                     == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT)))
        {
            if (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample == 16)
            { *pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO16"); }
            else { *pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO8"); }
        }
        else if ((m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 2)
                 && (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample == 16)
                 && (m_WaveIDs[WaveID]->wfEXT.dwChannelMask
                     == (SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
        { *pulFormat = pfnGetEnumValue("AL_FORMAT_REAR16"); }
        else if ((m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 4)
                 && (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample == 16)
                 && (m_WaveIDs[WaveID]->wfEXT.dwChannelMask
                     == (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT
                         | SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
        { *pulFormat = pfnGetEnumValue("AL_FORMAT_QUAD16"); }
        else if ((m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 6)
                 && (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample == 16)
                 && (m_WaveIDs[WaveID]->wfEXT.dwChannelMask
                     == (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT
                         | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY
                         | SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
        { *pulFormat = pfnGetEnumValue("AL_FORMAT_51CHN16"); }
        else if ((m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 7)
                 && (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample == 16)
                 && (m_WaveIDs[WaveID]->wfEXT.dwChannelMask
                     == (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT
                         | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY
                         | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT
                         | SPEAKER_BACK_CENTER)))
        { *pulFormat = pfnGetEnumValue("AL_FORMAT_61CHN16"); }
        else if ((m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 8)
                 && (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample == 16)
                 && (m_WaveIDs[WaveID]->wfEXT.dwChannelMask
                     == (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT
                         | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY
                         | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT
                         | SPEAKER_SIDE_LEFT|SPEAKER_SIDE_RIGHT)))
        { *pulFormat = pfnGetEnumValue("AL_FORMAT_71CHN16"); }
    }

    if (*pulFormat == 0) { wr = WR_INVALIDWAVEFILETYPE; }

    return wr;
}

bool ALSndLoader::IsWaveID(WAVEID WaveID)
{
    if ((WaveID >= 0) && (WaveID < MAX_NUM_WAVEID))
    {
        if (m_WaveIDs[WaveID]) { return true; }
    }

    return false;
}

char *ALSndLoader::GetErrorString(WAVERESULT wr, char *szErrorString,
                                  unsigned long nSizeOfErrorString)
{
    switch (wr)
    {
    case WR_OK:
        strncpy(szErrorString, "Success\n", nSizeOfErrorString-1);
        break;

    case WR_INVALIDFILENAME:
        strncpy(szErrorString, "Invalid file name or file does not exist\n",
                nSizeOfErrorString-1);
        break;

    case WR_BADWAVEFILE:
        strncpy(szErrorString, "Invalid Wave file\n", nSizeOfErrorString-1);
        break;

    case WR_INVALIDPARAM:
        strncpy(szErrorString, "Invalid parameter passed to function\n",
                nSizeOfErrorString-1);
        break;

    case WR_FILEERROR:
        strncpy(szErrorString, "File I/O error\n", nSizeOfErrorString-1);
        break;

    case WR_INVALIDWAVEID:
        strncpy(szErrorString, "Invalid WAVEID\n", nSizeOfErrorString-1);
        break;

    case WR_NOTSUPPORTEDYET:
        strncpy(szErrorString, "Function not supported yet\n",
                nSizeOfErrorString-1);
        break;

    case WR_WAVEMUSTBEMONO:
        strncpy(szErrorString, "Input wave files must be mono\n",
                nSizeOfErrorString-1);
        break;

    case WR_WAVEMUSTBEWAVEFORMATPCM:
        strncpy(szErrorString, "Input wave files must be in Wave Format PCM\n",
                nSizeOfErrorString-1);
        break;

    case WR_WAVESMUSTHAVESAMEBITRESOLUTION:
        strncpy(szErrorString,
                "Input wave files must have the same Bit Resolution\n",
                nSizeOfErrorString-1);
        break;

    case WR_WAVESMUSTHAVESAMEFREQUENCY:
        strncpy(szErrorString,
                "Input wave files must have the same Frequency\n",
                nSizeOfErrorString-1);
        break;

    case WR_WAVESMUSTHAVESAMEBITRATE:
        strncpy(szErrorString,
                "Input wave files must have the same Bit Rate\n",
                nSizeOfErrorString-1);
        break;

    case WR_WAVESMUSTHAVESAMEBLOCKALIGNMENT:
        strncpy(szErrorString,
                "Input wave files must have the same Block Alignment\n",
                nSizeOfErrorString-1);
        break;

    case WR_OFFSETOUTOFDATARANGE:
        strncpy(szErrorString,
                "Wave files Offset is not within audio data\n",
                nSizeOfErrorString-1);
        break;

    case WR_INVALIDSPEAKERPOS:
        strncpy(szErrorString,
                "Invalid Speaker Destinations\n", nSizeOfErrorString-1);
        break;

    case WR_OUTOFMEMORY:
        strncpy(szErrorString, "Out of memory\n", nSizeOfErrorString-1);
        break;

    case WR_INVALIDWAVEFILETYPE:
        strncpy(szErrorString, "Invalid Wave File Type\n",
                nSizeOfErrorString-1);
        break;

    case WR_NOTWAVEFORMATEXTENSIBLEFORMAT:
        strncpy(szErrorString,
                "Wave file is not in WAVEFORMATEXTENSIBLE format\n",
                nSizeOfErrorString-1);
        break;

    default:
        strncpy(szErrorString, "Undefined error\n", nSizeOfErrorString-1);
    }
    szErrorString[nSizeOfErrorString-1] = '\0';
    return szErrorString;
}
