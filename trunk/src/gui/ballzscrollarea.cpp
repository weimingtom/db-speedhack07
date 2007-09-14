#include <guichan.hpp>

#include "gui/ballzscrollarea.hpp"

BallzScrollArea::BallzScrollArea()
{
	setScrollPolicy(SHOW_NEVER,	SHOW_ALWAYS);
	addKeyListener(this);
	setFocusable(false);
	setBorderSize(0);
}

void BallzScrollArea::draw(gcn::Graphics *graphics)
{
    drawChildren(graphics);

	if (getVerticalMaxScroll() != 0)
	{
		int y = ((getHeight() - 32) * getVerticalScrollAmount()) /
			getVerticalMaxScroll();

		graphics->setColor(0x000000);
		graphics->drawRectangle(gcn::Rectangle(getWidth()-11, y, 8, 32));
		graphics->drawRectangle(gcn::Rectangle(getWidth()-10, y+1, 8, 32));

		graphics->setColor(0xffffff);

		graphics->fillRectangle(gcn::Rectangle(getWidth()-10, y+1, 6, 30));
	}
}

void BallzScrollArea::keyPressed(gcn::KeyEvent &keyEvent)
{
	if (keyEvent.getKey().getValue() == gcn::Key::DOWN)
	{
		setVerticalScrollAmount(getVerticalScrollAmount() + 16);
	}
	else if (keyEvent.getKey().getValue() == gcn::Key::UP)
	{
		setVerticalScrollAmount(getVerticalScrollAmount() - 16);
	}
}
