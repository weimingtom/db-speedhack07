#include "shop.hpp"

#include "stringutil.hpp"
#include "gamestate.hpp"
#include "resourcehandler.hpp"

Shop::Shop()
{
    mShopLabel = new gcn::Label("THE LOVE UPGRADE SHOP");
    add(mShopLabel, 20, 10);
    mLeaveButton = new DBSH07Button("LEAVE SHOP");
    add(mLeaveButton, 20, 200);
    mLeaveButton->addActionListener(this);

    mEnergyOrbsLabel = new gcn::Label("YOU HAVE " + toString(GameState::getInstance()->getEnergyOrbs()) + "x}");
    add(mEnergyOrbsLabel, 20, 24);

    mItemLabel = new gcn::Label("ITEM\x83");
    add(mItemLabel, 24, 80);

    mCostLabel = new gcn::Label("COST\x83");
    add(mCostLabel, 170, 80);

    mYouHaveLabel = new gcn::Label("EQUIPPED\x83");
    add(mYouHaveLabel, 225, 80);

    mBuyLifeButton = new DBSH07Button("LIFE ~");
    add(mBuyLifeButton, 20, 100);
    mBuyLifeButton->addActionListener(this);
    mLifeCostLabel = new gcn::Label(toString(LIFE_COST)+"}");
    add(mLifeCostLabel, 170, 104);
    mLivesEquipedLabel = new gcn::Label(toString(GameState::getInstance()->getLives()) + "x~");
    add(mLivesEquipedLabel, 225, 104);

    mBuyPodButton = new DBSH07Button("FIREING POD {");
    add(mBuyPodButton, 20, 100 + mBuyLifeButton->getHeight());
    mBuyPodButton->addActionListener(this);
    mPodCostLabel = new gcn::Label(toString(POD_COST)+"}");
    add(mPodCostLabel, 170, 104 + mBuyLifeButton->getHeight());
    mPodsEquipedLabel = new gcn::Label(toString(GameState::getInstance()->getPods()) + "x{");
    add(mPodsEquipedLabel, 225, 103 + mBuyLifeButton->getHeight());

    mBuyCannonUpgradeButton = new DBSH07Button("CANNON UPGRADE");
    add(mBuyCannonUpgradeButton, 20, 100 + mBuyLifeButton->getHeight() * 2);
    mBuyCannonUpgradeButton->addActionListener(this);
    mCannonUpgradeCostLabel = new gcn::Label(toString(CANNON_UPGRADE_COST)+"}");
    add(mCannonUpgradeCostLabel, 170, 104 + mBuyLifeButton->getHeight() * 2);
    mCannonUpgradeEquipedLabel = new gcn::Label("LEVEL " + toString(GameState::getInstance()->getCannonLevel()));
    add(mCannonUpgradeEquipedLabel, 225, 103 + mBuyLifeButton->getHeight() * 2);

    mDialog = new Dialog();
    mDialog->setSize(320, 75);
    add(mDialog, 0, 240 - 75);

    mBuySample = ResourceHandler::getInstance()->getSample("shopbuy.wav");
}

Shop::~Shop()
{
    delete mCannonUpgradeCostLabel;
    delete mPodCostLabel;
    delete mLifeCostLabel;
    delete mCostLabel;
    delete mYouHaveLabel;
    delete mItemLabel;
    delete mBuyCannonUpgradeButton;
    delete mCannonUpgradeEquipedLabel;
    delete mPodsEquipedLabel;
    delete mEnergyOrbsLabel;
    delete mLivesEquipedLabel;
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
            mDialog->setText("C I'm sorry but you don't have enough } for buying a life.");
            mDialog->setVisible(true);
            return;
        }
        
        GameState::getInstance()->setEnergyOrbs(GameState::getInstance()->getEnergyOrbs() - LIFE_COST);
        GameState::getInstance()->setLives(GameState::getInstance()->getLives() + 1);
        mLivesEquipedLabel->setCaption(toString(GameState::getInstance()->getLives()) + "x~");
        mLivesEquipedLabel->adjustSize();
        play_sample(mBuySample, 255, 128, 1000, 0);
    }
    else if (actionEvent.getSource() == mBuyPodButton)
    {
        if (GameState::getInstance()->getEnergyOrbs() < POD_COST)
        {
            mDialog->setText("C I'm sorry but you don't have enough } for buying a pod.");
            mDialog->setVisible(true);
            return;
        }
        
        GameState::getInstance()->setEnergyOrbs(GameState::getInstance()->getEnergyOrbs() - POD_COST);
        GameState::getInstance()->setPods(GameState::getInstance()->getPods() + 1);
        mPodsEquipedLabel->setCaption(toString(GameState::getInstance()->getPods()) + "x{");
        mPodsEquipedLabel->adjustSize();
        play_sample(mBuySample, 255, 128, 1000, 0);
    }
    else if (actionEvent.getSource() == mBuyCannonUpgradeButton)
    {
        if (GameState::getInstance()->getEnergyOrbs() < CANNON_UPGRADE_COST)
        {
            mDialog->setText("C I'm sorry but you don't have enough } for a cannon upgrade.");
            mDialog->setVisible(true);
            return;
        }
        
        GameState::getInstance()->setEnergyOrbs(GameState::getInstance()->getEnergyOrbs() - CANNON_UPGRADE_COST);
        GameState::getInstance()->setCannonLevel(GameState::getInstance()->getCannonLevel() + 1);
        mCannonUpgradeEquipedLabel->setCaption("LEVEL " + toString(GameState::getInstance()->getCannonLevel()));
        mCannonUpgradeEquipedLabel->adjustSize();
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
        mDialog->setText("C Welcome to the shop! To buy items you need energy orbs. Have fun!");
        mDialog->setVisible(true);
    }
}
