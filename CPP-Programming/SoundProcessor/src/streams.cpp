#include "streams.h"

Streams::InputStream::InputStream(std::string_view file_path): _reader(WAVManagement::WAVReader(file_path)) {}

WAVFormatInfo::Sample Streams::InputStream::read_element(){
    return _reader.read_sample();
}

void Streams::InputStream::skip(size_t count){
    _reader.skip(count);
}

void Streams::InputStream::skip(size_t start_sample, size_t end_sample){
    _reader.skip(start_sample, end_sample);
}

float Streams::InputStream::get_duration_s() const{
    return _reader.get_duration_s();
}

size_t Streams::InputStream::get_size() const{
    return _reader.get_samples_count();
}

bool Streams::InputStream::available() const{
    return _reader.available();
}

Streams::OutputStream::OutputStream(std::string_view file_path): _writer(WAVManagement::WAVWriter(file_path)) {}

void Streams::OutputStream::write(WAVFormatInfo::Sample sample){
    _writer.write_sample(sample);
}
