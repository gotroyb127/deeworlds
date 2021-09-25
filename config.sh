cc=${CC:-cc}

includes='-Isrc/glad/include'
defines=
cppflags="$includes $defines"

cflags="-std=c99 -Wall -pedantic -Werror $cppflags"

ldflags='-lglfw -ldl -lm -O0'
