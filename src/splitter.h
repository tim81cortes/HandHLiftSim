//
//  splitter.hpp
//  
//
//  Created by 余冰洁 on 15/11/19.
//
//

#ifndef splitter_hpp
#define splitter_hpp

#include <stdio.h>
#include <vector>
#include <string>

using namespace std;


class Splitter{
public:
    vector<string> split(string string_to_split, string punc_to_split);// split the strings by the splitter
};

#endif /* splitter_hpp */
