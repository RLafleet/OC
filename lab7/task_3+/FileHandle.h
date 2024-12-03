#include <windows.h>
#include <stdexcept>

class FileHandle {
    HANDLE m_handle;

public:
    explicit FileHandle(HANDLE handle) : m_handle(handle) {
        if (m_handle == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("Failed to open file.");
        }
    }

    ~FileHandle() {
        if (m_handle != INVALID_HANDLE_VALUE) {
            CloseHandle(m_handle);
        }
    }

    HANDLE get() const {
        return m_handle;
    }
};

class EventHandle {
    HANDLE m_event;

public:
    EventHandle() : m_event(CreateEvent(nullptr, TRUE, FALSE, nullptr)) {
        if (!m_event) {
            throw std::runtime_error("Failed to create event.");
        }
    }

    ~EventHandle() {
        CloseHandle(m_event);
    }

    HANDLE get() const {
        return m_event;
    }
};
