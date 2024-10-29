#ifndef TD3DMACROS_H
#define TD3DMACROS_H

#include "TD3D.h"

#define RELEASE_COM(p) if (p) { (p)->Release(); (p) = nullptr; }
#define RETURN_HR(hr) if (FAILED(hr)) { LogHRMessage(hr); return (hr); }
#define LOG_HR(hr) if (FAILED(hr)) { LogHRMessage(hr); }
// TODO: implement #define LOG_SHADER_ERROR(hr, blob, header) if (hr != S_OK) { LogShaderErrorMessage(header, blob); }


#endif
