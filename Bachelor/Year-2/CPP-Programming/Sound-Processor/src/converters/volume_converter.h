#pragma once

#include "abstract_converter.h"

class RawVolumeConverter : public Converters::RawAbstractConverter{
public:
    void convert(std::string_view command,
                 Streams::OutputStream& output_stream,
                 Streams::InputStream& input_stream,
                 const Converters::ConverterParams& params) const override;

    ~RawVolumeConverter() override = default;
};
