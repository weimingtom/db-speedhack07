#include "Precompiled.hpp"
#include "SplashScreen2.hpp"
#include "Music.hpp"
#include "Resource.hpp"

SplashScreen2::SplashScreen2()
    : Screen()
	, myFrameCounter(0)
{
    myDarkbitsLogo = Resource::getBitmap("data/images/darkbitslogo.bmp");
	myDarkbitsLogoBlackAndWhite = Resource::getBitmap("data/images/darkbitslogo_black_and_white.bmp");
	myDarkbitsLogoGlow = Resource::getBitmap("data/images/darkbitslogo_glow.bmp");
	mySoftware = Resource::getBitmap("data/images/software.bmp");
	myDarkbitsLogoBlink = Resource::getBitmap("data/images/darkbitslogo_blink.bmp");
}

void SplashScreen2::onLogic()
{	
	 myFrameCounter++;

	if (myFrameCounter < 60)
		return;

	if (myFrameCounter == 61)
	{
		 Music::playSong("data/music/splash2.xm");
	}

	if (myFrameCounter == 790)
	{
		Music::stop();
	}

	if (myFrameCounter > 840)
	{
		exit();
		return;
	}
}

void SplashScreen2::onDraw(BITMAP* aBuffer)
{    
	int flashStart = 120;
	int flashEnd = 144;

	if (myFrameCounter < flashStart 
		|| myFrameCounter > 780)
	{
		clear_to_color(aBuffer, 0);
		return;
	}
	if ((myFrameCounter < flashEnd && myFrameCounter % 5 < 2))
	{
		masked_blit(myDarkbitsLogoBlackAndWhite, aBuffer, 
					0, 
					0, 
					0, 
					0, 
					myDarkbitsLogo->w, 
					myDarkbitsLogo->h);
	}
	else if (myFrameCounter < flashEnd && myFrameCounter % 5 >= 2
			|| myFrameCounter >= flashEnd)
	{
		if (myFrameCounter > 168 && myFrameCounter < 240)
		{
			static int x = 0;
			masked_blit(myDarkbitsLogoGlow, 
						aBuffer, 
						0, 
						0, 
						(x * 6) - 120, 
						0, 
						myDarkbitsLogo->w, 
						myDarkbitsLogo->h);
			x++;
		}
		else
		{
			clear_to_color(aBuffer, makecol(166, 166, 166));
		}

		masked_blit(myDarkbitsLogo, 
					aBuffer, 
					0, 
					0, 
					0, 
					0, 
					myDarkbitsLogo->w, 
					myDarkbitsLogo->h);
	}
	else
	{
		clear_to_color(aBuffer, 0);
	}

	if ((myFrameCounter > 348 && myFrameCounter < 396 && rand() % 3 == 0)
			|| myFrameCounter >= 396)
		masked_blit(mySoftware,
					aBuffer, 
					0, 
					0, 
					320 / 2 - mySoftware->w / 2, 
					180, 
					mySoftware->w, 
					mySoftware->h);

	if (myFrameCounter > 228 && myFrameCounter < 240)
		masked_blit(myDarkbitsLogoBlink, aBuffer, 0, 0, 70, 67, myDarkbitsLogoBlink->w, myDarkbitsLogoBlink->h);
	if (myFrameCounter > 252 && myFrameCounter < 264)
		masked_blit(myDarkbitsLogoBlink, aBuffer, 0, 0, 263, 114, myDarkbitsLogoBlink->w, myDarkbitsLogoBlink->h);
}