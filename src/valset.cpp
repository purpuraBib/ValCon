#include <StateControllerExtension.h>
#include "valcommon.cpp"

#pragma once
namespace ValSet {
    inline void Process(PLAYER* p, STATE_INFO* sinfo) {
        ValCommon::PARAMETERS* parameter = (ValCommon::PARAMETERS*)sinfo->params;
        DWORD32 addr = (DWORD32)p + EvalExpression(p, &parameter->disp, 0);
        int i;
        float j;
        switch (EvalExpression(p, &parameter->value, &i, &j)) {
            case EVAL_INT: {
                *(int*)addr = EvalExpression(p, &parameter->value, 0);
                break;
            }
            case EVAL_FLOAT: {
                *(float*)addr = EvalExpression(p, &parameter->value);
                break;
            }
        }
        return;
    }
};