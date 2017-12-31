//
//  dflas.cpp
//  DFLAS
//
//  Created by Yang Yunfei on 2017/12/23.
//  Copyright © 2017年 Yang Yunfei. All rights reserved.
//

#include "dflas.hpp"
//
//  dflas.cpp
//  DFLAS
//
//  Created by Yang Yunfei on 2017/12/13.
//  Copyright © 2017年 Yang Yunfei. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <map>
#include "charUtils.hpp"
#include "wordSegment.hpp"
#include "POSTagging.hpp"

using namespace std;
using namespace dflas;
int main(int argc, const char * argv[]) {
    string sent = "我爱北京天安门。";
    std::vector<string> result;
    std::vector<PAIR> pos_res;
    char * filename = (char *)"../../../dict/dict_small.txt";
    WordSegment::initDict(filename);
    WordSegment::wordSegment(sent, result);
    HMMPOSTag::initTrie("../../../dict/tag_dic.txt");
    HMMPOSTag::postag(result, pos_res);
    for(size_t i = 0; i < pos_res.size(); ++i)
    {
        cout << pos_res[i].first << "_" << pos_res[i].second << " ";
    }
    cout<<endl;
    
    return 0;
}
