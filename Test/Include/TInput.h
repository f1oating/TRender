#ifndef TInput_H
#define TInput_H

#include <windows.h>

class TInput
{
public:
    TInput(HWND window);
    ~TInput();

    void Update();

    bool GetKeyPress(char key, bool ignorePressStamp = false);
    bool GetButtonPress(int button, bool ignorePressStamp = false);
    bool IsKeyDown(char key);  // New method to check if a key is held down
    bool AreKeysPressed(const char* keys);  // New method to check multiple key presses

    long GetPosX();
    long GetPosY();
    long GetDeltaX();
    long GetDeltaY();
    long GetDeltaWheel();

private:
    HWND m_window;
    unsigned long m_pressStamp;

    BYTE m_keyState[256];
    unsigned long m_keyPressStamp[256];

    POINT m_position;
    POINT m_lastPosition;
    unsigned long m_buttonPressStamp[3];
    long m_deltaWheel;
};

#endif
