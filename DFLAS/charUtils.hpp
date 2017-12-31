//
//  charUtils.hpp
//  DFLAS
//
//  Created by Yang Yunfei on 2017/12/13.
//  Copyright © 2017年 Yang Yunfei. All rights reserved.
//

#ifndef charUtils_hpp
#define charUtils_hpp

#include <stdio.h>
#include <string>
#include <map>

namespace dflas
{
    
    using std:: map;
    using std:: string;
    
    class CharUtils
    {
    public:
        //key为全角字符，value为半角字符
        static const map<string, char> sbcMap;
        
        //key为半角字符，value为全角字符
        static const map<char, string> dbcMap;
        
        static bool sbc2dbc(const string & sbcStr, char & dbcChar)
        {
            if(sbcMap.count(sbcStr) > 0)
            {
                dbcChar = sbcMap.at(sbcStr);
                return true;
            }
            return false;
        }
        
        static bool dbc2sbc(const char & dbcChar, string & sbcStr)
        {
            if(dbcMap.count(dbcChar) > 0)
            {
                sbcStr = dbcMap.at(dbcChar);
                return true;
            }
            return false;
        }
        
        //全半角转换，只支持utf-8编码方式
        static void sbc2dbc(const string & input, string & output)
        {
            output.clear();
            size_t i = 0, len = input.length();
            size_t cnt = 1;
            string word = "";
            char dbcChar;
            while (i < len)
            {
                if(input[i] & 0x80)
                {
                    char ch = input[i];
                    ch <<= 1;
                    do{
                        ch <<= 1;
                        ++cnt;
                    }while (ch & 0x80);
                    word = input.substr(i, cnt);
                    bool ok = sbc2dbc(word, dbcChar);
                    if(ok)
                    {
                        output.append(1, dbcChar);
                    }
                    else
                    {
                        output += word;
                    }
                    i += cnt;
                    cnt = 1;
                }
                else{
                    output.append(1, input[i]);
                    i++;
                }
            }
        }
        
    };
    
}  // namespace dflas


#endif /* charUtils_hpp */
