#include <StateControllerExtension.h>

constexpr auto ULONG_MAX_STRING_LENGTH = 10; //strlen("4294967295")
#pragma once
#pragma warning(disable : 6054)
namespace ValSetPointer {
    struct PARAMETERS {
        char pointer[256] = ""; //pointer
        EVAL_EXP params[5]{}; //params
        EVAL_EXP value; //value
    };

    inline BOOL ishex(std::string* c) {
        return std::string(*c).find("0x", 0) + 1;
    }

    //hex to dword
    //"0x4b4000" -> (DWORD32)4931584
    inline DWORD32 xtod(std::string* c) {
        return (DWORD32)stoi(*c, nullptr, 16);
    }

    //reference to dword
    //"[0x4b4000]" -> *(DWORD32*)0x4b4000
    inline DWORD32 rtod(std::string* c) {
        std::erase_if(*c, [](char p) { return p == '[' || p == ']'; });
        return *(DWORD32*)(ishex(c) ? xtod(c) : (DWORD32)stoi((*c).c_str()));
    }

    inline void ReplaceString(std::string* to, const char* from, size_t start, size_t end, size_t buffersize) {
        (*to).erase(start, end - start);
        if ((*to).length() + strlen(from) < buffersize) {
            (*to).insert(start, from);
        }
        else {
            throw new(out_of_range::exception);
        }
    }

    inline void ParseHexToDword(std::string* c) {
        char buf[ULONG_MAX_STRING_LENGTH];
        DWORD32 prf = 0;
        //DebugBreak();
        while (true) {
            prf = (*c).find("0x", prf);
            if (prf == 0xFFFFFFFF) break;
            DWORD32 i = prf + 2;
            while (true) {
                char j = (*c)[i];
                if ((j >= '0' && j <= '9') ||
                    (j >= 'A' && j <= 'F') ||
                    (j >= 'a' && j <= 'f')) {
                    i++;
                    if (i > (*c).length()) goto jexit;
                }
                else {
                    //DebugBreak();
                    std::string ss = (*c).substr(prf, i - prf);
                    _ultoa_s(xtod(&ss), buf, 10);
                    ReplaceString(c, buf, prf, i, 4096u);
                    break;
                }
            }
        }
    jexit:
        return;
    }

    inline DWORD32 ParseReferenceExpression(std::string* c, int offset, PLAYER* p) {
        DWORD TEMP;

        DWORD32 start = (*c).find('[', offset);
        DWORD32 end = (*c).find(']', offset);

        if(start != 0xFFFFFFFF){
            //DebugBreak();
            while (start < end) {
                DWORD32 v = ParseReferenceExpression(c, start + 1, p);
                if (v == 0xFFFFFFFF)return 0xFFFFFFFF;

                end = (*c).find(']', start + 1);
                char buf[ULONG_MAX_STRING_LENGTH];
                _ultoa_s(v, buf, 10);
                //DebugBreak();
                ReplaceString(c, buf, start, end + 1, 4096u);
                start = (*c).find('[', offset);
                end = (*c).find(']', offset);
            }
            start = (*c).find('[', offset - 1);
        }
        
        end = (*c).find(']', start + 1);
        if (start != 0xFFFFFFFF && end == 0xFFFFFFFF)return 0xFFFFFFFF;
        if (offset == 0 && end != 0xFFFFFFFF)return 0xFFFFFFFF;

        if (end != 0xFFFFFFFF) {
            EVAL_EXP x;

            SCtrlReadExpList((*c).substr(offset, end - offset).c_str(), "i", p->pcache, &TEMP, &x);
            DWORD32 y = (DWORD32)EvalExpression(p, &x, 0);

            char buf[ULONG_MAX_STRING_LENGTH];
            _ultoa_s(y, buf, 10);
            //DebugBreak();
            ReplaceString(c, buf, offset, end, 4096u);
            
            end = (*c).find(']', start);
            std::string ss = (*c).substr(offset - 1, end - offset + 2);
            DWORD32 re = rtod(&ss);
            return re;
        }

        EVAL_EXP x;
        SCtrlReadExpList((*c).c_str(), "i", p->pcache, &TEMP, &x);
        return (DWORD32)EvalExpression(p, &x, 0);
    }


    inline int Register(TPFILE* tpf, STATE_INFO* sinfo, PLAYER_CACHE* pcache) {
        PARAMETERS* parameter = new PARAMETERS;
        sinfo->params = parameter;
        DWORD TEMP;

        SCtrlReadExpList(TPGetValue(tpf, "params"), "nnnnn", pcache, &TEMP, &parameter->params[0], &parameter->params[1], &parameter->params[2], &parameter->params[3], &parameter->params[4]);
        GetQuotedString(tpf, "pointer", parameter->pointer, 256);

        const char* value = TPGetValue(tpf, "value");        
        if (value) {
            constExp(&parameter->value, 0);
            SCtrlReadExpList(value, "n", pcache, &TEMP, &parameter->value);
            return TRUE;
        }
        return FALSE;
    }

    inline void Process(PLAYER* p, STATE_INFO* sinfo) {
        PARAMETERS* parameter = (PARAMETERS*)sinfo->params;
        char t[4096] = "";
        EVAL_EXP e;
        DWORD32 par[5]{};
        for (int i = 0; i < 5; i++)
        {
            int j;
            float k;
            par[i] = EvalExpression(p, &parameter->params[i], &j, &k) == EVAL_INT ? j : 0;
        }
        sprintf_s(t, parameter->pointer, par[0], par[1], par[2], par[3], par[4]);

        std::string ts = std::string(t);

        ParseHexToDword(&ts);
        DWORD32 v;
        try {
            v = ParseReferenceExpression(&ts, 0, p);
        }
        catch (exception e) {
            Warn(p,"Buffer overflow expected");
            return;
        }
        
        //DebugBreak();
        if (v != 0xFFFFFFFF) {
            int i;
            float j;
            switch (EvalExpression(p, &parameter->value, &i, &j)) {
                case EVAL_INT: {
                    *(int*)v = EvalExpression(p, &parameter->value, 0);
                    break;
                }
                case EVAL_FLOAT: {
                    *(float*)v = EvalExpression(p, &parameter->value);
                    break;
                }
            }
        }
        else {
            Warn(p, "Illegal pointer format");
        }
        return;
    }

    inline void Free(STATE_INFO* sinfo) {
        PARAMETERS* parameter = (PARAMETERS*)sinfo->params;
        FreeExpression(&parameter->value);
        for (EVAL_EXP i : parameter->params) {
            FreeExpression(&i);
        }
        delete parameter;
        return;
    }
};