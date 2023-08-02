#include <StateControllerExtension.h>

#pragma once
namespace StringValSet {
	struct PARAMETERS {
		EVAL_EXP disp;
		char text[256];
		EVAL_EXP terminate;
	};

	inline int Register(TPFILE* tpf, STATE_INFO* sinfo, PLAYER_CACHE* pcache) {
        PARAMETERS* parameter = new PARAMETERS;
        sinfo->params = parameter;
        DWORD TEMP;

        constExp(&parameter->terminate, 1);
        SCtrlReadExpList(TPGetValue(tpf, "terminate"), "i", pcache, &TEMP, &parameter->terminate);
        GetQuotedString(tpf, "text", parameter->text, 256);

        const char* disp = TPGetValue(tpf, "disp");
        if (disp) {
            SCtrlReadExpList(disp, "i", pcache, &TEMP, &parameter->disp);
            return TRUE;
        }
        return FALSE;
	}

    inline void Process(PLAYER* p, STATE_INFO* sinfo) {
        PARAMETERS* parameter = (PARAMETERS*)sinfo->params;
        DWORD32 Addr = (DWORD32)p + EvalExpression(p, &parameter->disp, 0);
        rsize_t Length = strlen(parameter->text) + !!EvalExpression(p, &parameter->terminate, 0);
        memcpy_s((void*)Addr, Length, parameter->text, Length);
    }

    inline void Free(STATE_INFO* sinfo) {
        PARAMETERS* parameter = (PARAMETERS*)sinfo->params;
        FreeExpression(&parameter->disp);
        FreeExpression(&parameter->terminate);
        delete parameter;
        return;
    }
}