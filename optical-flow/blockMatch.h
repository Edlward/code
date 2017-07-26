#ifndef _BLOCKMATCH_H_
#define _BLOCKMATCH_H_

struct Blockof
{
    int search_half_size;      // 搜索窗口大小
    int win_half_size;          // 匹配窗口大小
    Blockof()
    {
        search_half_size = 20;
        win_half_size = 5;
    }
};

#endif