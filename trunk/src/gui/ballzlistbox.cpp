#include <guichan.hpp>

#include "gui/ballzlistbox.hpp"
#include "resourcehandler.hpp"
#include "guichan/allegro.hpp"

BallzListBox::BallzListBox()
{
	setBorderSize(0);
    mSelectedImage = ResourceHandler::getInstance()->getBitmap("hand.bmp");
}

BallzListBox::~BallzListBox()
{

}

void BallzListBox::draw(gcn::Graphics* graphics)
{
    gcn::AllegroGraphics* allegroGraphics = static_cast<gcn::AllegroGraphics*>(graphics);

	if (mListModel == NULL)
	{
		return;
	}

	allegroGraphics->setColor(getForegroundColor());
	allegroGraphics->setFont(getFont());

	int i, fontHeight;
	int y = 0;

	fontHeight = getFont()->getHeight();

	for (i = 0; i < mListModel->getNumberOfElements(); ++i)
	{
		allegroGraphics->drawText(mListModel->getElementAt(i), 10, y);

		if (i == mSelected)
		{
			if (isFocused())
			{
				allegroGraphics->drawBitmap(mSelectedImage, 0, y + 1);
			}
		}

		y += fontHeight;
	}
}

void BallzListBox::setSelected(int i)
{
    /*
	if (i >= 0 &&
			i < getListModel()->getNumberOfElements() &&
			getListModel()->getElementAt(i) == "")
	{
		if (i < getSelected())
		{
			i--;
		}
		else
		{
			i++;
		}
	}
    */
	
    gcn::ListBox::setSelected(i);
}
