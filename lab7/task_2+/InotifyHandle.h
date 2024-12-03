#include <iostream>
#include <unistd.h>
#include <sys/inotify.h>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <fcntl.h>

class InotifyHandle {
    int fd;
public:
    InotifyHandle() {
        fd = inotify_init1(IN_NONBLOCK);
        if (fd == -1) {
            throw std::runtime_error(std::string("Failed to initialize inotify: ") + std::strerror(errno));
        }
    }

    ~InotifyHandle() {
        if (fd != -1) {
            close(fd);
        }
    }

    int get() const { return fd; }
};

void handle_event(const struct inotify_event* event) {
    std::cout << "File: " << event->name << " | Event: ";
    if (event->mask & IN_CREATE) std::cout << "Created ";
    if (event->mask & IN_DELETE) std::cout << "Deleted ";
    if (event->mask & IN_MODIFY) std::cout << "Modified ";
    if (event->mask & IN_ATTRIB) std::cout << "Attributes changed ";
    std::cout << '\n';
}

void monitor_directory(const char* path) {
    try {
        InotifyHandle inotify;
        int wd = inotify_add_watch(inotify.get(), path, IN_CREATE | IN_DELETE | IN_MODIFY | IN_ATTRIB);
        if (wd == -1) {
            throw std::runtime_error(std::string("Failed to add inotify watch: ") + std::strerror(errno));
        }

        const size_t buf_size = 1024 * (sizeof(struct inotify_event) + 16);
        std::vector<char> buffer(buf_size);

        std::cout << "Monitoring directory: " << path << '\n';

        while (true) {
            ssize_t length = read(inotify.get(), buffer.data(), buf_size);
            if (length == -1 && errno != EAGAIN) {
                throw std::runtime_error(std::string("Read error: ") + std::strerror(errno));
            }

            if (length > 0) {
                for (char* ptr = buffer.data(); ptr < buffer.data() + length; ) {
                    struct inotify_event* event = reinterpret_cast<struct inotify_event*>(ptr);
                    handle_event(event);
                    ptr += sizeof(struct inotify_event) + event->len;
                }
            }

            usleep(500 * 1000); 
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}