#ifndef DBSH07_HIGHSCORE_HPP
#define DBSH07_HIGHSCORE_HPP

#include "guichan.hpp"

class HighScore: public gcn::Container
{
public:
    HighScore();
    ~HighScore();
    void setVisible(bool visible);
	void load(const std::string& filename);

protected:

	gcn::Label* mHighScoreLabel;
	gcn::TextBox* mScoreTable;

	struct HighScorePair
	{
		std::string name;
		int points;
	};
	std::vector<HighScorePair> mHighScore;

private:
	void updateTextBox();
};

#endif