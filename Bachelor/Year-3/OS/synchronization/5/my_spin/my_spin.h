#pragma once

typedef volatile int my_spinlock_t;

int my_spin_init(my_spinlock_t* lock);

int my_spin_destroy(my_spinlock_t* lock);

int my_spin_lock(my_spinlock_t* lock);

int my_spin_trylock(my_spinlock_t* lock);

int my_spin_unlock(my_spinlock_t* lock);
