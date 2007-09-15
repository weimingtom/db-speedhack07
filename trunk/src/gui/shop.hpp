#ifndef DBSH07_SHOP_HPP
#define DBSH07_SHOP_HPP

#include "guichan.hpp"
#include "gui/dbsh07button.hpp"
#include "gui/dialog.hpp"

class Shop: public gcn::Container, public gcn::ActionListener
{
public:
    Shop();
    ~Shop();
    void action(const gcn::ActionEvent& actionEvent);
    void setVisible(bool visible);

protected:
    static const int POD_COST = 20;
    static const int LIFE_COST = 10;

    gcn::Label* mShopLabel;
    DBSH07Button* mLeaveButton;
    DBSH07Button* mBuyLifeButton;
    DBSH07Button* mBuyPodButton;
    gcn::Label* mLivesLabel;
    gcn::Label* mPodsLabel;
    gcn::Label* mEnergyOrbsLabel;
    Dialog* mDialog;
    SAMPLE* mBuySample;
};

#endif