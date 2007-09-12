/*
 * Copyright (c) 2007, Olof Naessen and Per Larsson
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 *    * Redistributions of source code must retain the above copyright notice, 
 *      this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright notice, 
 *      this list of conditions and the following disclaimer in the documentation 
 *      and/or other materials provided with the distribution.
 *    * Neither the name of the Darkbits nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without 
 *      specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "resourcehandler.hpp"
#include "os.hpp"

ResourceHandler* ResourceHandler::mInstance = 0;

ResourceHandler::ResourceHandler()
{

}

ResourceHandler* ResourceHandler::getInstance()
{
    if (mInstance == 0)
    {
        mInstance = new ResourceHandler();
    }

    return mInstance;
}

void ResourceHandler::destroy()
{
    std::map<std::string, BITMAP*>::iterator it;

    for (it = mBitmaps.begin(); it != mBitmaps.end(); it++)
    {
        destroy_bitmap((*it).second);
    }

	std::map<std::string, SAMPLE*>::iterator it2;

    for (it2 = mSamples.begin(); it2 != mSamples.end(); it2++)
    {
        destroy_sample((*it2).second);
    }
}

BITMAP* ResourceHandler::getBitmap(const std::string& filename)
{
    if (mBitmaps.find(filename) == mBitmaps.end())
    {
	    BITMAP* bitmap = load_bitmap(getRealFilename(filename).c_str(), NULL);
	    
        if (bitmap == NULL) 
        {
            throw "Unable to load: " + getRealFilename(filename);
	    }

        mBitmaps[filename] = bitmap;
    }

    return mBitmaps[filename];
}


SAMPLE* ResourceHandler::getSample(const std::string& filename)
{
    if (mSamples.find(filename) == mSamples.end())
    {

	    SAMPLE* sample = load_sample(getRealFilename(filename).c_str());
	    
        if (sample == NULL) 
        {
            throw "Unable to load: " + getRealFilename(filename);
	    }

        mSamples[filename] = sample;
    }

    return mSamples[filename];
}

std::string ResourceHandler::getRealFilename(const std::string& filename)
{
    return std::string(BALLZ_DATA_PREFIX) + "/" + filename;
}
