//
// Created by LuoHuaqing on 06/12/2016.
//

#ifndef CANNON_ENDIANCONV_H_H
#define CANNON_ENDIANCONV_H_H

inline void memrev64(void *p) {
    unsigned char *x = (unsigned char *)p, t;

    t = x[0];
    x[0] = x[7];
    x[7] = t;
    t = x[1];
    x[1] = x[6];
    x[6] = t;
    t = x[2];
    x[2] = x[5];
    x[5] = t;
    t = x[3];
    x[3] = x[4];
    x[4] = t;
}

#if (BYTE_ORDER == BIG_ENDIAN)
#define memrev64ifle(p)
#else
#define memrev64ifle(p) memrev64(p)
#endif

#endif //CANNON_ENDIANCONV_H_H
