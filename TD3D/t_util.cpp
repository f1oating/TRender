#include "TD3D.h"

#include <string>

void TD3D::LogHRMessage(HRESULT hr)
{
    char* errorMsg = nullptr;
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&errorMsg, 0, nullptr);

    std::string message = errorMsg ? errorMsg : "Unknown error";
    LocalFree(errorMsg);

    // TODO: implement
}

void TD3D::LogShaderErrorMessage()
{
    if (m_pBlob)
    {
        // TODO: implement
    }
}
