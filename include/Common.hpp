#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>

// string
#include <string>

// stream
#include <iostream>
#include <fstream>
#include <sstream>

// STL
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <algorithm>
#include <utility>

// Reprize
#include "reprize.hpp"

// misc
#include "misc/Vector3.hpp"
#include "misc/Quaternion.hpp"
#include "misc/Matrix44.hpp"

namespace reprize
{
typedef Vector3<Unit> Vec3;
typedef Matrix44<Unit> Mtx44;
typedef Quaternion<Unit> Quat;

extern mem::MemHunk memhunk;
}

// Private classes
#include "reprize_pvt.hpp"

#include "resource/IDRegistry.hpp"
#include "resource/Entry.hpp"
#include "resource/DirEntry.hpp"
#include "resource/Property.hpp"
#include "resource/Props.hpp"
#include "resource/Node.hpp"
#include "resource/PropStr.hpp"
#include "resource/PropNode.hpp"
#include "resource/PropProp.hpp"
#include "resource/Paragraph.hpp"
#include "resource/VFS.hpp"
