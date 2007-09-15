#include "shop.hpp"

#include "stringutil.hpp"
#include "gamestate.hpp"
#include "resourcehandler.hpp"

Shop::Shop()
{
    mShopLabel = new gcn::Label("THE MEGA UPGRADE SHOP");
    add(mShopLabel, 20, 10);
    mLeaveButton = new DBSH07Button("LEAVE SHOP");
    add(mLeaveButton, 20, 200);
    mLeaveButton->addActionListener(this);

    mEnergyOrbsLabel = new gcn::Label("YOU HAVE " + toString(GameState::getInstance()->getEnergyOrbs()) + "x}");
    add(mEnergyOrbsLabel, 20, 24);

    mBuyLifeButton = new DBSH07Button("BUY LIFE ~ COST "+toString(LIFE_COST)+"}");
    add(mBuyLifeButton, 20, 100);
    mBuyLifeButton->addActionListener(this);
    mLivesLabel = new gcn::Label("YOU HAVE " + toString(GameState::getInstance()->getLives()) + "x~");
    add(mLivesLabel, 210, 104);

    mBuyPodButton = new DBSH07Button("BUY FIREING POD { COST "+toString(POD_COST)+"}");
    add(mBuyPodButton, 20, 100 + mBuyLifeButton->getHeight());
    mBuyPodButton->addActionListener(this);
    mPodsLabel = new gcn::Label("YOU HAVE " + toString(GameState::getInstance()->getPods()) + "x{");
    add(mPodsLabel, 210, 103 + mBuyLifeButton->getHeight());

    mDialog = new Dialog();
    mDialog->setSize(320, 75);
    add(mDialog, 0, 240 - 75);

    mBuySample = ResourceHandler::getInstance()->getSample("shopbuy.wav");
}

Shop::~Shop()
{
    delete mPodsLabel;
    delete mEnergyOrbsLabel;
    delete mLivesLabel;
    delete mDialog;
    delete mShopLabel;
    delete mLeaveButton;
    delete mBuyLifeButton;
    delete mBuyPodButton;
}

void Shop::action(const gcn::ActionEvent& actionEvent)
{
    if (actionEvent.getSource() == mLeaveButton)
    {
        generateAction();
    }
    else if (actionEvent.getSource() == mBuyLifeButton)
    {
        if (GameState::getInstance()->getEnergyOrbs() < LIFE_COST)
        {
            mDialog->setText("I'm sorry but you don't have enough }");
            mDialog->setVisible(true);
            return;
        }
        
        GameState::getInstance()->setEnergyOrbs(GameState::getInstance()->getEnergyOrbs() - LIFE_COST);
        GameState::getInstance()->setLives(GameState::getInstance()->getLives() + 1);
        mLivesLabel->setCaption("YOU HAVE " + toString(GameState::getInstance()->getLives()) + "x~");
        mLivesLabel->adjustSize();
        play_sample(mBuySample, 255, 128, 1000, 0);
    }
    else if (actionEvent.getSource() == mBuyPodButton)
    {
        if (GameState::getInstance()->getEnergyOrbs() < POD_COST)
        {
            mDialog->setText("I'm sorry but you don't have enough }");
            mDialog->setVisible(true);
            return;
        }
        
        GameState::getInstance()->setEnergyOrbs(GameState::getInstance()->getEnergyOrbs() - POD_COST);
        GameState::getInstance()->setPods(GameState::getInstance()->getPods() + 1);
        mPodsLabel->setCaption("YOU HAVE " + toString(GameState::getInstance()->getPods()) + "x{");
        mPodsLabel->adjustSize();
        play_sample(mBuySample, 255, 128, 1000, 0);
    }

    mEnergyOrbsLabel->setCaption("YOU HAVE " + toString(GameState::getInstance()->getEnergyOrbs()) + "x}");
    mEnergyOrbsLabel->adjustSize();
}

void Shop::setVisible(bool visible)
{
    Container::setVisible(visible);
    
    if (visible)
    {
        mDialog->setText("Welcome to the shop!");
        mDialog->setVisible(true);
    }
}
