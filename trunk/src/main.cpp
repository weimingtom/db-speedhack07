#include <allegro.h>
#include <string>
#include <iostream>
#include <exception>


//#include "game.hpp"
//#include "guichan.hpp"

int main(int argc, char **argv)
{
	try
	{
//		Game game;
//		game.run();
		allegro_message("Hello Allegro!");
	}
	/*
	catch (gcn::Exception e)
    {
        std::cerr << e.getMessage();
        allegro_message(e.getMessage().c_str());
        return 1;   
    }
	*/
	catch (std::string e)
	{
		std::cerr << e;
        allegro_message(e.c_str());
		return 1;
	}

	return 0;
}
END_OF_MAIN()
