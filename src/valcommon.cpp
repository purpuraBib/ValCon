#include <StateControllerExtension.h>

#pragma once
namespace ValCommon {
    struct PARAMETERS {
        EVAL_EXP disp;
        EVAL_EXP value;
    };

    inline int Register(TPFILE* tpf, STATE_INFO* sinfo, PLAYER_CACHE* pcache) {
        PARAMETERS* parameter = new PARAMETERS;
        sinfo->params = parameter;
        DWORD TEMP;

        const char* value = TPGetValue(tpf, "value");
        if (!value) return FALSE;
        const char* disp = TPGetValue(tpf, "disp");
        if (disp) {
            constExp(&parameter->value, 0);
            SCtrlReadExpList(disp, "i", pcache, &TEMP, &parameter->disp);
            SCtrlReadExpList(value, "n", pcache, &TEMP, &parameter->value);
            return TRUE;
        }
        return FALSE;
    }

    inline void Free(STATE_INFO* sinfo) {
        PARAMETERS* parameter = (PARAMETERS*)sinfo->params;
        FreeExpression(&parameter->disp);
        FreeExpression(&parameter->value);
        delete parameter;
        return;
    }
}