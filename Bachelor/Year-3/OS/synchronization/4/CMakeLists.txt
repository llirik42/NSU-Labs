cmake_minimum_required(VERSION 3.20)
project(SuperQueue C)

set(CMAKE_C_STANDARD 23)

add_executable(SpinQueue src/main.c src/storage.c src/utils.c src/sync/spin/sync.c)
add_executable(MutexQueue src/main.c src/storage.c src/utils.c src/sync/mutex/sync.c)
add_executable(MySpinQueue src/main.c src/storage.c src/utils.c src/sync/my_spin/sync.c src/sync/my_spin/my_spin.c)
add_executable(MyMutexQueue src/main.c src/storage.c src/utils.c src/sync/my_mutex/sync.c src/sync/my_mutex/my_mutex.c)
add_executable(RWLockQueue src/main.c src/storage.c src/utils.c src/sync/rwlock/sync.c)
