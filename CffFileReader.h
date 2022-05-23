#ifndef CFFFILEREADER_H
#define CFFFILEREADER_H

#include <string>

#include "CEvent.h"


class CffFileReader
{
    private:
    static long nowTime();
public:
	CffFileReader();
	~CffFileReader() = default;
    static void work(std::string fileid, std::string fileurl, CEvent *pEvent, bool islooped );
};


#endif // CFFFILEREADER_H