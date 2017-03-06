//
//  splitter.cpp
//  
//
//  Created by 余冰洁 on 15/11/19.
//
//

#include "splitter.h"

#include <string>
using namespace std;

/**
 * This method is used to split the strings by the splitter.
 * @param string_to_split The string before splitting
 * @param punc_to_split punctuatuon splitter
 * @return return a vector contain the substrings after splitting
 */
vector<string> Splitter::split(string string_to_split, string punc_to_split){
    
    vector<string> sub_str;
    string punc = punc_to_split;
    std::string::size_type pos1, pos2;
    pos2 = string_to_split.find(punc);
    pos1 = 0;
    while(string::npos != pos2)
    {
        sub_str.push_back(string_to_split.substr(pos1, pos2-pos1));
        
        pos1 = pos2 + punc.size();
        pos2 = string_to_split.find(punc, pos1);
    }
    if(pos1 != string_to_split.length())
        sub_str.push_back(string_to_split.substr(pos1));
    
    return sub_str;
}
