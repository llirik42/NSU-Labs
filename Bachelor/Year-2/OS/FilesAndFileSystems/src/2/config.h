#pragma once

#include "operations.h"
#include "utils.h"

static const struct Operation OPERATIONS[] = {
        {make_dir, "make_dir", "make directory"},
        {print_directory_content, "dir_content", "print directory content"},
        {remove_directory, "remove_dir", "remove directory"},
        {create_file, "create_file", "create file"},
        {print_file_content, "file_content", "print file content"},
        {remove_file, "remove_file", "remove file"},
        {create_symlink, "create_symlink", "create symlink (source, symlink)"},
        {print_symlink_content, "print_symlink_content", "print content of symlink"},
        {print_symlink_source_content, "print_symlink_source_content", "print content of symlink source"},
        {remove_symlink, "remove_symlink", "remove symlink"},
        {create_hard_link, "create_hard_link", "create hard link (source, hard_link)"},
        {remove_hard_link, "remove_hard_link", "remove hard link"},
        {print_permission_and_hard_links, "file_info", "print number of hard links and permissions"},
        {change_mode, "change_mode", "change file mode (file, permissions). Example of permissions: \"rwxrwxrwx\""},
};
static const unsigned int OPERATIONS_COUNT = sizeof(OPERATIONS) / sizeof(struct Operation);
