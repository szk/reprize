#include "Common.hpp"

#include "resource/loader/Loader.hpp"
#include "resource/loader/VarLoader.hpp"
#include "resource/Entity.hpp"
#include "Win32FSReader.hpp"

using namespace reprize;
using namespace res;
using namespace std;

// XXX We can't handle /D "_UNICODE" /D "UNICODE" switch

Win32FSReader::Win32FSReader(void)
{
}

Win32FSReader::~Win32FSReader(void)
{
}

const bool Win32FSReader::check(VFS* vfs_, const Str& dir_)
{
    vfs = vfs_;

    fs_root = vfs->find("/Resource");

    const TCHAR* base = (const TCHAR*)dir_.c_str();
    _stprintf_s(fullpath, _T("%s"), base); // XXX
    // check resources
    crawl(fs_root);

    return true;
}

void Win32FSReader::crawl(Node* cwn_)
{
    WIN32_FIND_DATA findData;
    HANDLE h_found = FindFirstFile(fullpath, &findData);
    BOOL stat = TRUE;

    if (h_found == INVALID_HANDLE_VALUE) { return; }

    if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        != FILE_ATTRIBUTE_DIRECTORY)
    { return; } // fullpath is not directory

    TCHAR* ptr = fullpath + strlen(fullpath);
    *ptr++ = '\\';
    *ptr = 0;

    TCHAR cwd[MAX_PATH];
    _stprintf_s(cwd, _T("%s*"), fullpath); // FIXME

    // if directory was found
    for (h_found = FindFirstFile(cwd, &findData), stat = TRUE;
         h_found != INVALID_HANDLE_VALUE && stat;
         stat = FindNextFile(h_found, &findData))
    {
        if (!_tcscmp(findData.cFileName, _T("."))
            || !_tcscmp(findData.cFileName, _T("..")))
        { continue; }

        // check wcstombs, std::codecvt, USES_CONVERSION, T2A
	TCHAR cfn[MAX_PATH];
        _stprintf_s(cfn, MAX_PATH, _T("%s\\%s"), fullpath, findData.cFileName);

        strncpy_s(ptr, MAX_PATH - strlen(fullpath), findData.cFileName,
                  MAX_PATH - strlen(findData.cFileName)); // XXX
        TCHAR cfn_mbs[MAX_PATH], filename_mbs[MAX_PATH];
#ifdef UNICODE
        wcstombs_s(cfn_mbs, cfn, MAX_PATH);
        wcstombs_s(filename_mbs, findData.cFileName, MAX_PATH);
#else
        strncpy_s(cfn_mbs, cfn, MAX_PATH);
        strncpy_s(filename_mbs, findData.cFileName, MAX_PATH);
#endif

        if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            == FILE_ATTRIBUTE_DIRECTORY)
        {
            Node* vfs_dir = new Node(filename_mbs, NULL);
            cwn_->Add(vfs_dir);
            // TODO: limit recursion
            crawl(vfs_dir);
        }
        else { load(filename_mbs, fullpath, cwn_); }
    }

    ptr[-1] = 0;
    FindClose(h_found);

    return;
}

Node* Win32FSReader::load(const Str filename_, const Str fullpath_, Node* cwn_)
{
    for (list<Loader*>::iterator ldr_itr = loader_l.begin();
         ldr_itr != loader_l.end(); ++ldr_itr)
    {
        if ((*ldr_itr)->load(filename_, fullpath_))
        { cwn_->Add((*ldr_itr)->get()); }
    }

    return NULL;
}

const Str Win32FSReader::fix_path(const Str& src_)
{
    Str path(src_);
    for (Size32 pos = path.find('\\', 0); pos != Str::npos;
         pos = path.find('\\', pos))
    {
        path.insert(pos, "\\");
        pos += 1;
    }

    return path;
}

