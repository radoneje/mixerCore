#ifndef CFFFILEREADER_H
#define CFFFILEREADER_H

#include <string>

#include "CEvent.h"


class CffFileReader
{
public:
	CffFileReader();
	~CffFileReader() = default;
    static void work(std::string fileid, std::string fileurl, CEvent *pEvent );
};


#endif // CFFFILEREADER_H