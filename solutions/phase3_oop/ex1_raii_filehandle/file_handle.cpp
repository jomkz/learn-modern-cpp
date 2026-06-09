// Phase 3, Exercise 1 — RAII FileHandle

#include "file_handle.h"
#include <stdexcept>
#include <string>

FileHandle::FileHandle(const std::string& path, const std::string& mode) {
    // Attempt to open the file. std::fopen returns nullptr on failure.
    file_ = std::fopen(path.c_str(), mode.c_str());
    if (file_ == nullptr) {
        // Throw immediately — never construct a broken object. The caller
        // can catch this and decide how to handle the error.
        throw std::runtime_error("FileHandle: cannot open '" + path +
                                 "' with mode '" + mode + "'");
    }
}

FileHandle::~FileHandle() noexcept {
    // std::fclose flushes the user-space buffer and closes the OS file
    // descriptor. We must close even if an exception is in flight — RAII
    // guarantees this because the destructor always runs.
    if (file_ != nullptr) {
        std::fclose(file_);
        // Do NOT set file_ = nullptr here — the object is being destroyed;
        // nobody will read file_ again.
    }
}

std::size_t FileHandle::write(const void* data, std::size_t size) {
    return std::fwrite(data, 1, size, file_);
}

std::size_t FileHandle::read(void* buf, std::size_t size) {
    return std::fread(buf, 1, size, file_);
}
