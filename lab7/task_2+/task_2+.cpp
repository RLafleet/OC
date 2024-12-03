#ifdef __linux__
#include <iostream>
#include <unistd.h>
#include <sys/inotify.h>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <string>
#include <unordered_map>
#include <filesystem>

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

void handle_event(const struct inotify_event* event, const std::unordered_map<int, std::string>& watch_to_path) {
    std::string path = watch_to_path.at(event->wd);
    std::cout << "File: " << path << "/" << event->name << " | Event: ";
    if (event->mask & IN_CREATE) std::cout << "Created ";
    if (event->mask & IN_DELETE) std::cout << "Deleted ";
    if (event->mask & IN_MODIFY) std::cout << "Modified ";
    if (event->mask & IN_ATTRIB) std::cout << "Attributes changed ";

    std::cout << '\n';
}

void add_watch_recursively(int inotify_fd, const std::string& path, std::unordered_map<int, std::string>& watch_to_path) {
    int wd = inotify_add_watch(inotify_fd, path.c_str(), IN_CREATE | IN_DELETE | IN_MODIFY | IN_ATTRIB);
    if (wd == -1) {
        throw std::runtime_error(std::string("Failed to add inotify watch: ") + std::strerror(errno));
    }
    watch_to_path[wd] = path;

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_directory()) {
            add_watch_recursively(inotify_fd, entry.path().string(), watch_to_path);
        }
    }
}

void monitor_directory(const char* path) {
    try {
        InotifyHandle inotify;
        std::unordered_map<int, std::string> watch_to_path;

        add_watch_recursively(inotify.get(), path, watch_to_path);
        std::vector<char> buffer(1024 * (sizeof(struct inotify_event) + 16));

        std::cout << "Monitoring directory: " << path << " and its subdirectories.\n";

        while (true) {
            ssize_t length = read(inotify.get(), buffer.data(), buffer.size());
            if (length == -1 && errno != EAGAIN) {
                throw std::runtime_error(std::string("Read error: ") + std::strerror(errno));
            }

            if (length > 0) {
                for (char* ptr = buffer.data(); ptr < buffer.data() + length; ) {
                    struct inotify_event* event = reinterpret_cast<struct inotify_event*>(ptr);
                    handle_event(event, watch_to_path);

                    // Если создан новый каталог, добавить его в наблюдение
                    if (event->mask & IN_CREATE && (event->mask & IN_ISDIR)) {
                        std::string new_dir = watch_to_path[event->wd] + "/" + event->name;
                        add_watch_recursively(inotify.get(), new_dir, watch_to_path);
                    }

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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: fsmon PATH\n";
        return 1;
    }

    monitor_directory(argv[1]);
    return 0;
}


#endif // __linux__     