#include "highscore.hpp"

#include "stringutil.hpp"
#include "gamestate.hpp"
#include "resourcehandler.hpp"
#include "fileutil.hpp"

HighScore::HighScore()
{
    mHighScoreLabel = new gcn::Label("HIGHSCORE");
	mHighScoreLabel->adjustSize();
	add(mHighScoreLabel);

	mScoreTable = new gcn::TextBox();
	mScoreTable->setEditable(false);

	mScoreTable->setBorderSize(0);
	mScoreTable->setOpaque(false);
	add(mScoreTable);

	setOpaque(false);
	setSize(100, 200);

}

void HighScore::load(const std::string& filename)
{
    std::vector<std::string> data = tokenize(loadFile(filename), "\n");
	
	mHighScore.clear();
	for (int row = 0; row < data.size(); row++)
    {
		std::vector<std::string> stringPair = tokenize(data[row], ",");

		HighScorePair p;
		p.name = stringPair[0];
		p.points = fromString<int>(stringPair[1]);
		mHighScore.push_back(p);
	}
	updateTextBox();
}

void HighScore::updateTextBox()
{
	std::string str = "";
	for (int row = 0; row < mHighScore.size(); row++)
	{
		str += mHighScore[row].name;
		str += " - ";
		str += toString(mHighScore[row].points);
		str += "\n";
	}
	mScoreTable->setText(str);
	mHighScoreLabel->setPosition(getWidth()/2.0f-mHighScoreLabel->getWidth()/2.0, 0);
	mScoreTable->setPosition(getWidth()/2.0f-mScoreTable->getWidth()/2.0f, 10);

	//mScoreTable->adjustSize();
}

HighScore::~HighScore()
{
	delete mHighScoreLabel;
	delete mScoreTable;
}

void HighScore::setVisible(bool visible)
{
}