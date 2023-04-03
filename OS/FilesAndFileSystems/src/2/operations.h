#pragma once

int make_dir(int argc, char** argv);

int print_directory_content(int argc, char** argv);

int remove_directory(int argc, char** argv);

int create_file(int argc, char** argv);

int print_file_content(int argc, char** argv);

int remove_file_raw(const char* path);
int remove_file(int argc, char** argv);

int create_symlink(int argc, char** argv);

int print_symlink_content(int argc, char** argv);

int print_symlink_source_content(int argc, char** argv);

int remove_symlink(int argc, char** argv);

int create_hard_link_raw(const char* old_path, const char* new_path);
int create_hard_link(int argc, char** argv);

int remove_hard_link(int argc, char** argv) ;

int print_permission_and_hard_links(int argc, char** argv);

int change_mode(int argc, char** argv);
