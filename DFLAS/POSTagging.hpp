//
//  POSTagging.hpp
//  DFLAS
//
//  Created by Yang Yunfei on 2017/12/18.
//  Copyright © 2017年 Yang Yunfei. All rights reserved.
//

#ifndef POSTagging_hpp
#define POSTagging_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <cassert>
#include "trie_map.hpp"

namespace dflas
{
    using std::vector;
    using std::string;
    using std::map;
    
    typedef std::pair<string, string> PAIR;
    
    class HMMPOSTag
    {
    public:
        HMMPOSTag()
        {
            for(int i = 0; i < statusNum; ++i)
            {
                emitProb[i] = new double [wordNum];
            }
        }
        ~HMMPOSTag()
        {}
        
        //初始化HMM参数
        void initHMM(const char * fword2id, const char * hmm_para)
        {
            FILE * fp_word = fopen(fword2id, "r");
            FILE * fp_hmm = fopen(hmm_para, "r");
            if(fp_word == NULL || fp_hmm == NULL)
            {
                perror("open file error, in initHMM(), POSTagging.hpp\n");
                return;
            }
            char word[100];
            int id;
            while (!feof(fp_word))
            {
                fscanf(fp_word, "%s %d", word, &id);
                string temp = word;
                word2id.emplace(temp, id);
       //         id2word.emplace(id, temp);
            }
            
            while (!feof(fp_hmm))
            {
                for(int i = 0; i < statusNum; ++i)
                {
                    fscanf(fp_hmm, "%lf", &initProb[i]);
                }
                for(int i = 0; i < statusNum; ++i)
                {
                    for(int j = 0; j < statusNum; ++j)
                    {
                        fscanf(fp_hmm, "%lf", &transProb[i][j]);
                    }
                }
                for(int i = 0; i < statusNum; ++i)
                {
                    for(int j = 0; j < wordNum; ++j)
                    {
                        fscanf(fp_hmm, "%lf", &emitProb[i][j]);
                    }
                }
            }
        }
        
        //viterbi算法，根据观察序列求最佳词性标注序列
        void viterbi(const vector<string> & input, vector<string> & output)
        {
            output.clear();
            output.reserve(input.size());
            double ** sigma = new double *[statusNum];
            int ** path = new int *[statusNum];
            int len = (int)input.size();
            
            for(int i = 0; i < statusNum; ++i)
            {
                sigma[i] = new double [len];
                path[i] = new int [len];
            }
            double minProb = 0.0;
            if(word2id.count(input[0]) > 0)
            {
                int id = word2id.at(input[0]);
                double prob = minProb;
                for(int i = 0; i < statusNum; ++i)
                {
                    prob = emitProb[i][id];
                    sigma[i][0] = emitProb[i][id] * initProb[i];
                }
            }
            else
            {
                for(int i = 0; i < statusNum; ++i)
                {
                    if(i == 1 || i == 8 || i == 9 || i == 16 || i == 30)
                    {
                        sigma[i][0] = initProb[i] ;
                    }else
                    {
                        sigma[i][0] = initProb[i] * 0.4;
                    }
                    
                }
            }
           
            for(int i = 1; i < len; ++i)
            {
                string word = input[i];
                double prob = minProb;
                int id = -1;
                if(word2id.count(word) > 0)
                {
                    id = word2id[word];
                }
                for(int j = 0; j < statusNum; ++j)
                {
                    sigma[j][i] = minProb;
                    path[j][i] = 0;
                    for(int k = 0; k < statusNum; ++k)
                    {
                        prob = (id == -1) ? emitProb[j][id] : 0.0025;
                        double tmp = sigma[k][i - 1] * transProb[k][j] * prob;
                        if(tmp > sigma[j][i])
                        {
                            sigma[j][i] = tmp;
                            path[j][i] = k;
                            
                            std:: cout << tmp << "  " << k << std::endl;
                        }
                    }  //  emnd for k
                }  // end for j
            }  // end for i
            int pos = 0;
            int labels[len];
            for(int i = 1; i < statusNum; ++i)
            {
                if(sigma[i][len -1] > sigma[pos][len - 1])
                {
                    pos = i;
                }
            }
            
            labels[len - 1] = pos;
            for(int i = len - 1; i > 0; --i)
            {
                std::cout << pos << " ";
                assert(pos >= 0 && pos < statusNum);
                labels[i - 1] = path[pos][i];
                pos = path[pos][i];
            }
            
            for(int i = 0; i < len; ++i)
            {
                output.emplace_back(id2pos.at(labels[i]));
            }
            
            for(int i = 0; i < statusNum; ++i)
            {
                delete [] sigma[i];
                delete [] path[i];
            }
            delete [] sigma;
            delete [] path;
        }  // viterbi
        
        static void initTrie(const char * tagfile)
        {
            FILE * fp = fopen(tagfile, "r");
            char word[100], tag[16];
            while (!feof(fp))
            {
                fscanf(fp, "%s%s", word, tag);
                posTree.insert(word, tag);
            }
            fclose(fp);
        }
        
        static void postag(const vector<string> & input, vector<PAIR> & result)
        {
            result.clear();
            result.reserve(input.size());
            string value;
            for(size_t i = 0; i < input.size(); ++i)
            {
                if(posTree.exists(input[i]))
                {
                    posTree.value_at(input[i], value);
                    result.emplace_back(input[i], value);
                }else
                {
                    value = "x";
                    result.emplace_back(input[i], value);
                }
            }
        }
 //       static const map<string, int> pos2id;
        static const map<int, string> id2pos;
         static trie<string> posTree;
        
    private:
        
        static const int statusNum = 44;
        static const int wordNum = 55872;
        //初始概率
        double initProb[statusNum];
        //转移概率
        double transProb[statusNum][statusNum];
        //发射概率
         double * emitProb[statusNum];
        map<string, int> word2id;
  //      map<int, string> id2word;
               
        
    };
}  // namespace dflas

#endif /* POSTagging_hpp */
