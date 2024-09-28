#ifdef __linux__
#include "CDirHandle.h"
#include <cerrno>
#include <iostream>

CDirHandle::CDirHandle(DIR* dir) : dirHandle(dir) {}

CDirHandle::~CDirHandle() {
    if (dirHandle) {
        closedir(dirHandle);
    }
}

DIR* CDirHandle::get() const {
    return dirHandle;
}

CDirHandle::operator DIR* () const {
    return dirHandle;
}
#endif //  __linux__
