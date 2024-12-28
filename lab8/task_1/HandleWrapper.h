#ifndef HANDLE_WRAPPER_H
#define HANDLE_WRAPPER_H

#include <windows.h>

template <typename T>
class HandleWrapper 
{
public:
    explicit HandleWrapper(T handle = nullptr) : handle_(handle) {}

    ~HandleWrapper() 
    {
        if (handle_) 
        {
            DeleteObject(handle_);
        }
    }

    HandleWrapper(const HandleWrapper&) = delete;
    HandleWrapper& operator=(const HandleWrapper&) = delete;

    HandleWrapper(HandleWrapper&& other) noexcept : handle_(other.handle_) 
    {
        other.handle_ = nullptr;
    }

    HandleWrapper& operator=(HandleWrapper&& other) noexcept 
    {
        if (this != &other) 
        {
            if (handle_) 
            {
                DeleteObject(handle_);
            }
            handle_ = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

    T get() const 
    { 
        return handle_; 
    }
    T release() 
    {
        T temp = handle_;
        handle_ = nullptr;
        return temp;
    }

private:
    T handle_;
};

#endif // HANDLE_WRAPPER_H
