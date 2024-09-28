#ifdef __linux__
#ifndef CDIRHANDLE_H
#define CDIRHANDLE_H

#include <dirent.h>

class CDirHandle {
public:
    explicit CDirHandle(DIR* dir);
    ~CDirHandle();

    DIR* get() const;
    operator DIR* () const; 

private:
    DIR* dirHandle;
};

#endif // CDIRHANDLE_H
#endif //__linux__