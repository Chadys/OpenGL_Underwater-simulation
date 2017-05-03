//
// Created by julie on 03/05/17.
//

#ifndef UNDER_THE_SEA_FOGPARAMS_H
#define UNDER_THE_SEA_FOGPARAMS_H

#define FOG_EQUATION_LINEAR 0
#define FOG_EQUATION_EXP 1
#define FOG_EQUATION_EXP2 2

namespace FogParameters {
    float fDensity = 0.005f;
    float fStart = 10.0f;
    float fEnd = 75.0f;
    glm::vec4 vFogColor = glm::vec4(0.037f, 0.17f, 0.33f, 1.0f);
    int iFogEquation = FOG_EQUATION_EXP2; // 0 = linear, 1 = exp, 2 = exp2
};

#endif //UNDER_THE_SEA_FOGPARAMS_H
