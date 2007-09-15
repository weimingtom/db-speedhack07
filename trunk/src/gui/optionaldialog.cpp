#include "gui/optionaldialog.hpp"

OptionalDialog::OptionalDialog()
:mState(NONE)
{
    setBackgroundColor(gcn::Color(0x000000));
    setSize(190, 50);

    mQueryLabel = new gcn::Label("What do you want to do?");
    mQueryLabel->adjustSize();
    add(mQueryLabel);

    mBonusLevelButton = new DBSH07Button("Bonus level");
    mBonusLevelButton->addActionListener(this);
    add(mBonusLevelButton, 20, mQueryLabel->getHeight() + 5);

    mShopButton = new DBSH07Button("Shop");
    mShopButton->addActionListener(this);
    add(mShopButton, 20, mQueryLabel->getHeight() + mBonusLevelButton->getHeight());
}

OptionalDialog::~OptionalDialog()
{
    delete mBonusLevelButton;
    delete mShopButton;
    delete mQueryLabel;
}

void OptionalDialog::setVisible(bool visible)
{
    DecoratedContainer::setVisible(visible);

    if (visible)
    {
        mState = NONE;
    }
}

void OptionalDialog::action(const gcn::ActionEvent& actionEvent)
{
    if (actionEvent.getSource() == mBonusLevelButton)
    {
        mState = BONUS_LEVEL;
    }
    else if (actionEvent.getSource() == mShopButton)
    {
        mState = SHOP;
    }

    generateAction();
}

unsigned int OptionalDialog::getState()
{
    return mState;
}