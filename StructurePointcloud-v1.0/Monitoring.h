////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	      //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////

#ifndef __MONITORING_H__
#define __MONITORING_H__

#include <map>
#include <string>

using namespace std;

///Keeps track of which functions are used and how many times
static std::map<std::string, int> functions_used;

static void printFunctionsUsed()    {
    std::map<std::string, int>::iterator it = functions_used.begin();

    for (;it != functions_used.end();it++)   {
        std::cout << it->first << " => " << it->second << std::endl;
    }

    return;
}

#endif // __MONITORING_H__
