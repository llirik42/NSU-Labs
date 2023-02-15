#pragma once

#include "../utils.h"
#include "../containers/files_container.h"
#include <stdbool.h>

Status Huffman(bool toCompress, const struct FilesContainer* filesContainer);
