#include "Common.hpp"

#include <dirent.h>
#include <fcntl.h>

#include "resource/loader/Loader.hpp"
#include "resource/loader/VarLoader.hpp"
#include "resource/Entity.hpp"
#include "POSIXFSReader.hpp"

using namespace reprize;
using namespace res;
using namespace std;

POSIXFSReader::POSIXFSReader(void)
    : vfs(NULL)
{
    RE_ZERO(fullpath, PATH_MAX+1);
}

POSIXFSReader::~POSIXFSReader(void)
{
}

const bool POSIXFSReader::check(VFS* vfs_, const Str& dir_)
{
    vfs = vfs_;
    fs_root = vfs->find("/Resource");

    strcpy(fullpath, dir_.c_str());
    crawl(fs_root);

    return true;
}

//ref. APUE
void POSIXFSReader::crawl(Node* cwn_)
{
    struct stat	statbuf;
    if (lstat(fullpath, &statbuf) < 0)
    {
        g_log->printf("lstat error");
        return;
    }

    // fullpath is not directory
    if (S_ISDIR(statbuf.st_mode) == 0) { return; }

    // point to end of fullpath
    char* ptr = fullpath + strlen(fullpath);
    *ptr++ = '/';
    *ptr = 0;

    DIR* dp = NULL;
    if (!(dp = opendir(fullpath)))
    { g_log->printf("can't read directory: %s", fullpath); return; }

    // if directory was found
    struct dirent* dirp = 0;
    while ((dirp = readdir(dp)))
    {
        if (!strncmp(dirp->d_name, ".", 1) || !strncmp(dirp->d_name, "..", 2))
        { continue; }

        strcpy(ptr, dirp->d_name);
        lstat(fullpath, &statbuf);
        if (S_ISDIR(statbuf.st_mode))
        {
            // create dir as constant
            Node* vfs_dir = new Node(dirp->d_name, NULL);
            cwn_->Add(vfs_dir);
            // store cwd
            Node* next_wn = vfs_dir;

            // TODO: limit recursion
            crawl(next_wn);
        }
        else { load(dirp->d_name, fullpath, cwn_); }
    }

    ptr[-1] = 0; // erase everything from slash onwards

    if (closedir(dp) < 0)
    { g_log->printf("Couldn't close directory %s", fullpath); }

    return;
}

Node* POSIXFSReader::load(const Str filename_, const Str fullpath_, Node* cwn_)
{
    for (list<Loader*>::iterator ldr_itr = loader_l.begin();
         ldr_itr != loader_l.end(); ++ldr_itr)
    {
        if ((*ldr_itr)->load(filename_, fullpath_))
        { cwn_->Add((*ldr_itr)->get()); }
    }

    return NULL;
}
