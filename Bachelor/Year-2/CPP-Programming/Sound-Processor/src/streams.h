#pragma once

#include <string>
#include "wav.h"

namespace Streams{
    class InputStream{
    public:
        explicit InputStream(std::string_view file_path);

        WAVFormatInfo::Sample read_element();

        void skip(size_t count);

        void skip(size_t start_sample, size_t end_sample);

        [[nodiscard]] float get_duration_s() const;

        [[nodiscard]] size_t get_size() const;

        [[nodiscard]] bool available() const;
    private:
        WAVManagement::WAVReader _reader;
    };

    class OutputStream{
    public:
        explicit OutputStream(std::string_view file_path);

        void write(WAVFormatInfo::Sample sample);
    private:
        WAVManagement::WAVWriter _writer;
    };
}
