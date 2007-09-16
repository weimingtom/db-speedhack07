#ifndef DBSH07_TEXTFIELD_HPP
#define DBSH07_TEXTFIELD_HPP

#include "guichan.hpp"

class DBSH07TextField: public gcn::TextField
{
public:
    DBSH07TextField(const std::string& text);
    void draw(gcn::Graphics* graphics);
    void drawCaret(gcn::Graphics* graphics, int x);
    void logic();
protected:
    int mFrameCounter;
};

#endif