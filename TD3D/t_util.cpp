#include "TD3D.h"

#include <string>

void LogHRMessage(HRESULT hr)
{
    char* errorMsg = nullptr;
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&errorMsg, 0, nullptr);

    std::string message = errorMsg ? errorMsg : "Unknown error";
    LocalFree(errorMsg);

    TLogManager::GetLogManager()->logMessage("HRESULT LOG", message.c_str());
}

void LogShaderErrorMessage(const char* header, ID3DBlob* blob)
{
    if (blob)
    {
        /*****************************************************************************************************
        TLogManager::GetLogManager()->logMessage(header, static_cast<const char *>(blob->GetBufferPointer()));

        TODO: implement

        *****************************************************************************************************/
    }
}
