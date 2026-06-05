#ifndef LOGIN_SCREEN_HPP
#define LOGIN_SCREEN_HPP

#include "Screen.hpp"

class LoginScreen : public Screen {
private:
    char username[16];
    char password[16];
    int usernameCount;
    int passwordCount;
    int activeField;
    int framesCounter;
    bool loginError;
    ScreenID nextScreen;
public:
    LoginScreen();
    void Update() override;
    void Draw() override;
    ScreenID CheckScreenSwitch() override;
};

#endif