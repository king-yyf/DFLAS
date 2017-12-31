//
//  dflas.hpp
//  DFLAS
//
//  Created by Yang Yunfei on 2017/12/23.
//  Copyright © 2017年 Yang Yunfei. All rights reserved.
//

#ifndef dflas_hpp
#define dflas_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <cstring>
#include "wordSegment.hpp"
#include "POSTagging.hpp"

namespace dflas {
    
    using std::string;
    using std::vector;
    class DFLAS
    {
    public:
        
        enum key_type
        {
            Organization = 1,   //组织机构
            Datetime = 2,       //时间，日期
            Location = 3,       //地名,包括城市，国家等
            University = 4,     //大学名
            Person = 5,         //人名
            Number = 6,         //数字
            Basic = 7           //其他实体，默认初始为basic
        };
        
        struct Word{
            string word;
            string pos;
            int begin_pos;
            key_type key;
            Word(string _word, string _pos, int _begin_pos, key_type _key)
            :word(_word), pos(_pos), begin_pos(_begin_pos), key(_key)
            {}
            Word(string _word)
            :word(_word)
            {
                key = Basic;
            }
            Word(string _word, string _pos)
            :word(_word), pos(_pos)
            {
                key = Basic;
            }
            Word(PAIR p)
            {
                word = p.first;
                pos = p.second;
                key = Basic;
            }
        };
        
        DFLAS(){}
        ~DFLAS(){}
        
        void loadfile(const char * dicfile, const char * tagfile)
        {
            WordSegment::initDict(dicfile);
            HMMPOSTag::initTrie(tagfile);
        }
        
        void regexMatch(vector<PAIR> posvec, vector<Word> & result)
        {
            result.clear();
            result.reserve(posvec.size());
            
        }
        
        void getResult(const string & input, vector<Word> & result)
        {
            vector<string> cutResult;
            vector<PAIR> posResult;
            WordSegment::wordSegment(input, cutResult);
            HMMPOSTag::postag(cutResult, posResult);
            
        }
        
    private:
        Trie country;
       // HMMPOSTag postag;
    };
    
}  //namespace dflas
#endif /* dflas_hpp */
