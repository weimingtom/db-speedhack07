#ifndef DBSH07_HIGHSCORE_HPP
#define DBSH07_HIGHSCORE_HPP

#include "guichan.hpp"

class HighScore: public gcn::Container
{
public:
	struct HighScorePair
	{
		//HighScorePair(const std::string& name, int points) : name(name), points(points); }
		std::string name;
		unsigned int points;
		static bool compareScore(const HighScorePair& a, const HighScorePair& b) { return a.points < b.points; };
		//HighScorePair operator-(const HighScorePair& a) { return HighScorePair(); };
	};

	HighScore();
    ~HighScore();
	void load(const std::string& filename);
	void save(const std::string& filename);
	void addScore(const std::string& name, int score);
	unsigned int getMinScore();

protected:

	gcn::Label* mHighScoreLabel;
	gcn::TextBox* mScoreTable;


	std::vector<HighScorePair> mHighScore;

private:
	void updateTextBox();
};

#endif