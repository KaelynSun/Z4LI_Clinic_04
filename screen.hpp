#ifndef SCREEN_HPP
#define SCREEN_HPP

enum ScreenID{
    SCREEN_NONE,
    SCREEN_LOGIN,
    SECREEN_GAME,
    SCREEN_SETTINGS
};

class Screen{
public:
    virtual ~Screen() {}

    virtual void Update() = 0;
    virtual void Draw() = 0;
    //Pure virtual functions every screen MUST implement

    virtual ScreenID CheckScreenSwitch() {return SCREEN_NONE; }
};

#endif