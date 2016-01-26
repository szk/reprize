#include "Common.hpp"

using namespace reprize;
using namespace mem;
using namespace std;

MemHunk::MemHunk(void)
{
    pvt = new mem_pvt;
}

MemHunk::~MemHunk(void)
{
}

const bool MemHunk::calloc(uInt32 size_mb_)
{
//     free(pvt->hunk);

//     if ((pvt->hunk = (void*)calloc(1024 * 1024 * size_mb_)))
//     {
//         if ((pvt->hunk = (void*)calloc(1024 * 1024 * 32)))
//         {
//             RE_DBG("Work on 32MB memory.");
//         }
//         else
//         {
//             RE_DBG("Could not allocate memory.");
//             g_finish = true;
//         }
//     }
//     else
//     {
//         RE_DBG("Allocate %dMB memory.", size_mb_);
//     }

//    pvt->avail_mem.insert(std::pair<Size32, void*> (sizeof(pvt->hunk), pvt->hunk));

    return true;
}

const bool MemHunk::clear(void)
{
    return true;
}

void* MemHunk::get_umem(Size32 size_)
{
    return malloc(size_);
}

void* MemHunk::get_lmem(Size32 size_)
{
    return NULL;
}
