#pragma once

#include "status_code.h"
#include "simple_string.h"

struct StatusCode calculateStringValue(struct String string, bool supportDouble,
                                       struct StatusCode prevCode);
