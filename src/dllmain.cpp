#include <StateControllerExtension.h>
#include "valcommon.cpp"
#include "valadd.cpp"
#include "valset.cpp"
#include "stringvalset.cpp"
#include "valsetpointer.cpp"


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        addState(STX{
            "valadd",
            ValCommon::Register,
            ValAdd::Process,
            ValCommon::Free
        });
        addState(STX{
            "valset",
            ValCommon::Register,
            ValSet::Process,
            ValCommon::Free
        });
        addState(STX{
            "valsetpointer",
            ValSetPointer::Register,
            ValSetPointer::Process,
            ValSetPointer::Free
        });
        addState(STX{
            "stringvalset",
            StringValSet::Register,
            StringValSet::Process,
            StringValSet::Free
        });
    }
    return TRUE;
}
