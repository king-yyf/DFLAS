//
//  wordSegment.hpp
//  DFLAS
//
//  Created by Yang Yunfei on 2017/12/14.
//  Copyright © 2017年 Yang Yunfei. All rights reserved.
//

#ifndef wordSegment_hpp
#define wordSegment_hpp

#include <stdio.h>
#include <vector>
#include <cctype>
#include "trie.hpp"

namespace dflas
{
    using std::string;
    
    class WordSegment
    {
    public:
        WordSegment(){}
        static Trie dict;
        static Trie suffix;
        
        static void initDict(const char * filename)
        {
            FILE * fp_in = fopen(filename, "r");
            if(fp_in == NULL)
            {
                perror("\n Open dict error, in initDict() wordSegment.hpp\n");
                return;
            }
            char * buff;
            size_t fileLen;
            fseek(fp_in, 0L, SEEK_END);
            fileLen = ftell(fp_in);
            buff = (char *)malloc(fileLen + 1);
            if(buff == NULL)
            {
                fclose(fp_in);
                perror("\nmalloc memory error, in initDict() wordSegment.hpp\n");
                return;
            }
            fseek(fp_in, 0L, SEEK_SET);
            fread(buff, fileLen, 1, fp_in);
            buff[fileLen] = 0;
            fclose(fp_in);
            char * pch;
            pch = strtok(buff, "\r");
           
           // printf("%s\n", pch);
            while (pch != NULL) {
                string word(pch);
                dict.insert(word);
                pch = strtok(NULL, "\r");
            }
            free(buff);
            suffix.insert("年");
            suffix.insert("月");
            suffix.insert("日");
            suffix.insert("号");
            suffix.insert("时");
            suffix.insert("分");
            suffix.insert("秒");
            suffix.insert("亿");
            suffix.insert("万");
            suffix.insert("千");
            suffix.insert("百");
            suffix.insert("十");
            suffix.insert("萬");
            suffix.insert("仟");
            suffix.insert("佰");
            suffix.insert("拾");
            suffix.insert("元");
            suffix.insert("元人民币");
            suffix.insert("人民币");
            suffix.insert("￥");
            suffix.insert("美金");
            suffix.insert("美元");
            suffix.insert("＄");
            suffix.insert("英镑");
            suffix.insert("日元");
            suffix.insert("角");
            suffix.insert("角钱");
            suffix.insert("毛");
            suffix.insert("毛钱");
            suffix.insert("分");
            suffix.insert("分钱");
            suffix.insert("美分");
            suffix.insert("%");
            suffix.insert("％");
        }
        
        static void wordSegment(string sentence, std::vector<string> & result)
        {
            if(sentence.empty())
            {
                perror("sentence to cut is empty, in wordSegment, wordSegment.hpp\n");
                return;
            }
            size_t i = 0, cnt = 1, size = sentence.length();
            string word = "", temp;
            bool isNum = false, used = true;
            while(i < size){
                if(used){
                    used = false;
                    cnt = 1;
                    if(sentence[i] & 0x80){
                        char ch = sentence[i];
                        ch <<= 1;
                        do{
                            ch <<= 1;
                            ++cnt;
                        }while (ch & 0x80);
                    }
                    else{
                        char ch = sentence[i];
                        if(isdigit(ch)){
                            std::stod(sentence.substr(i), &cnt);
                        }
                        else if(isalpha(ch)){
                            while(i + cnt < size && isalpha(sentence[i + cnt])){
                                cnt++;
                            }
                        }
                    }
                    temp = sentence.substr(i, cnt);
                }
                if(word == "" || dict.find(word + temp)){
                    if(isdigit(temp[0])){
                        isNum = true;
                    }
                    word.append(temp);
                    used = true;
                    i += cnt;
                }else{
                    if(isNum){
                        if(suffix.find(temp)){
                            word.append(temp);
                            i += cnt;
                            used = true;
                        }
                    }
                    result.emplace_back(word);
                    word.clear();
                    isNum = false;
                }
            }
            result.emplace_back(word);
        }
        
    };
}  // namespace dflas
#endif /* wordSegment_hpp */
