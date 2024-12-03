struct OverlappedWrapper {
    OVERLAPPED overlapped{};
    OverlappedWrapper() {
        ZeroMemory(&overlapped, sizeof(overlapped));
    }
};
