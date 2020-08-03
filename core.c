extern void (*_spreinit_array[])(void) __attribute__((weak));

extern void (*_epreinit_array[])(void) __attribute__((weak));

extern void (*_sinit_array[])(void) __attribute__((weak));

extern void (*_einit_array[])(void) __attribute__((weak));

extern int main();

void init() {
    int cpp_count;
    int size = _epreinit_array - _spreinit_array;
    for (cpp_count = 0; cpp_count < size; ++cpp_count) {
        _spreinit_array[cpp_count]();
    }

    size = &(_einit_array[0]) - &(_sinit_array[0]);
    for (cpp_count = 0; cpp_count < size; ++cpp_count) {
        _sinit_array[cpp_count]();
    }

    main();
}