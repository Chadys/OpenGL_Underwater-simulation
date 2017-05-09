//
// Created by julie on 09/05/17.
//

#include "Post_Processor.h"

namespace PostProd{
    const GLfloat offset = 1.0f/300;

    const GLfloat kernel[18] = {
            -offset, offset,  // top-left
            0.0f, offset,     // top-center
            offset, offset,   // top-right
            -offset, 0.0f,    // center-left
            0.0f, 0.0f,       // center-center
            offset, 0.0f,     // center-right
            -offset, -offset, // bottom-left
            0.0f, -offset,    // bottom-center
            offset, -offset   // bottom-right
    };

    const GLfloat average_kernel[9] = {
            1.0f/9.0f,  1.0f/9.0f, 1.0f/9.0f,
            1.0f/9.0f,  1.0f/9.0f, 1.0f/9.0f,
            1.0f/9.0f,  1.0f/9.0f, 1.0f/9.0f
    };

    const GLfloat sharpen_kernel[9] = {
            -1.0f/9.0f,  -1.0f/9.0f,     -1.0f/9.0f,
            -1.0f/9.0f,  2.0f-1.0f/9.0f, -1.0f/9.0f,
            -1.0f/9.0f,  -1.0f/9.0f,     -1.0f/9.0f
    };

    const GLfloat gauss_kernel[9] = {
            1.0f/16.0f, 1.0f/8, 1.0f/16.0f,
            1.0f/8.0f,  1.0f/4, 1.0f/8.0f,
            1.0f/16.0f, 1.0f/8, 1.0f/16.0f
    };

    const GLfloat sobel_kernel[9] = {
             1.0f,  2.0f,  1.0f,
             0.0f,  0.0f,  0.0f,
            -1.0f, -2.0f, -1.0f
    };

    const GLfloat laplacian_kernel[9] = {
            -1.0f, -1.0f, -1.0f,
            -1.0f,  8.0f, -1.0f,
            -1.0f, -1.0f, -1.0f
    };
}