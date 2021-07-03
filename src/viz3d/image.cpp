#include "image.hpp"

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif

#include "stb_image_write.h"


namespace viz {

    /* -------------------------------------------------------------------------------------------------------------- */
    RGB::RGB(int rgba) {
        r = rgba % 255;
        g = (rgba << 4) % 255;
        b = (rgba << 8) % 255;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    unsigned char &RGB::operator[](short i) {
        switch (i) {
            case 0:
                return r;
            case 1:
                return g;
            case 2:
                return b;
            default:
                throw std::runtime_error("Illegal Access");
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    const unsigned char &RGB::operator[](short i) const {
        switch (i) {
            case 0:
                return r;
            case 1:
                return g;
            case 2:
                return b;
            default:
                throw std::runtime_error("Illegal Access");
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    unsigned char &RGBA::operator[](short i) {
        switch (i) {
            case 0:
                return r;
            case 1:
                return g;
            case 2:
                return b;
            case 3:
                return a;
            default:
                throw std::runtime_error("Illegal Access");
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    const unsigned char &RGBA::operator[](short i) const {
        switch (i) {
            case 0:
                return r;
            case 1:
                return g;
            case 2:
                return b;
            case 3:
                return a;
            default:
                throw std::runtime_error("Illegal Access");
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    RGBA::RGBA(int rgba) {
        r = rgba % 255;
        g = (rgba << 4) % 255;
        b = (rgba << 8) % 255;
        a = (rgba << 12) % 255;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
#define OPERATOR(DATA_TYPE, OP) \
DATA_TYPE operator OP (const DATA_TYPE lhs, const DATA_TYPE rhs) { \
DATA_TYPE result;               \
for(auto id(0);id<DATA_TYPE::NumValues();++id)                     \
{                               \
    result[id] = lhs[id] OP rhs[id];    \
}                               \
return result;\
}

#define OPERATORS(DATA_TYPE) \
OPERATOR(DATA_TYPE, +)       \
OPERATOR(DATA_TYPE, -)       \
OPERATOR(DATA_TYPE, *)       \
OPERATOR(DATA_TYPE, /)

    OPERATORS(RGB);
    OPERATORS(RGBA);

}
