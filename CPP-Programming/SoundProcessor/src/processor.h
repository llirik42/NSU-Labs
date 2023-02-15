#pragma once

#include <string>
#include <vector>
#include <cmath>
#include "converters_factory.h"

namespace Processing{
    const std::string_view LEFT_ARROW = "<-";
    const std::string_view RIGHT_ARROW = "->";

    const float LEFT_TIME_ARROW_ALIAS = NAN;
    const float RIGHT_TIME_ARROW_ALIAS = INFINITY;

    class Processor{
    public:
        Processor(const std::string_view& config,
                  const std::string_view& out,
                  const std::string_view& in,
                  const std::vector<std::string>& additional_files,
                  const Factory::ConvertersFactory& factory);

        void process() const;

        ~Processor();
    private:
        struct Impl;
        Impl* _pimpl;
    };

}
