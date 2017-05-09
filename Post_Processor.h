//
// Created by julie on 09/05/17.
//

#ifndef UNDER_THE_SEA_POST_PROCESSING_H
#define UNDER_THE_SEA_POST_PROCESSING_H


#include <GL/glew.h>
#include <glm/vec2.hpp>

namespace PostProd{
    enum POSTPROD_EFFECT{
        NO_POSTPROD,
        //Blur outline and sharpen
        AVERAGER,
        SHARPENER,
        GAUSSIAN,
        SOBEL,
        LAPLACIAN,

        //Colors modifier
        GREYSCALE,
        INVERT,
        BRIGHT,
        DARK,
        THRESHOLD,
        RED_CANAL,
        GREEN_CANAL,
        BLUE_CANAL,
        BIT_REDUCE,

        //Color transform
        MOSAIC,
        PAINTING,
        HALFTONING,

        //Physical transform
        SWIRL,
        FISHEYE,
        TIMEWARP,
        NO_POSTPROD_LAST
    };

    extern const GLfloat kernel[18];
    extern const GLfloat average_kernel[9];
    extern const GLfloat sharpen_kernel[9];
    extern const GLfloat gauss_kernel[9];
    extern const GLfloat sobel_kernel[9];
    extern const GLfloat laplacian_kernel[9];
    extern const GLfloat offset;
}




#endif //UNDER_THE_SEA_POST_PROCESSING_H
