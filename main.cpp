#include <iostream>
#include <vector>
#include <time.h>
#include <set>
#include <algorithm>
using namespace std;

unsigned int _getRand(int max)
{
    static const unsigned long x[55] = {
        14106545636UL, 3012752342UL, 3491775623UL, 28921450626UL, 5711949714UL,
        325308284UL, 34898018UL, 3879498491UL, 25973737UL, 1981903553UL,
        3160251843UL, 129444464UL, 1851443344UL, 4156445905UL, 22604922UL,
        155067070UL, 393493484UL, 1460937157UL, 2528362617UL, 317430674UL,
        3229354360UL, 11749133UL, 832845075UL, 196160017UL, 132157429UL,
        747750121UL, 545747446UL, 810476036UL, 503334515UL, 4088144633UL,
        2824216555UL, 3738252341UL, 3493754131UL, 3672533954UL, 29494241UL,
        118928407UL, 4213624418UL, 3362851UL, 321315737UL, 114523552UL,
        295798497UL, 407866503UL, 22626102UL, 65478801UL, 252208841UL,
        19602036UL, 3156581UL, 11960864UL, 8046154UL, 142733266UL,
        201715031UL, 3885900UL, 81075053UL, 38466609UL, 239386197UL };

    unsigned int seed = (unsigned int)(time(NULL));
    srand(seed % 2 == 1 ? seed+rand(): seed-rand());
    srand(x[rand()%55]);
    return rand() % max;
}


/*
principles:
    1 every one has the same probability
    2 every one can get at least 0.01

*/
bool dispatchLuckyMoney(double total, unsigned int num, std::vector<double> &result)
{
    // check if every one should get at least 0.01
    if (num < 1 || total / num < 0.01) return false;

    //transform total from double to int, for the format of total must be xxx.xx
    unsigned int iTotal = (unsigned int)(total * 100);

    //for each person, get a random num
    /* calculate random maximum number*/
    const unsigned int randMax = iTotal-(num -1);
    std::vector<unsigned int> randInts;
    randInts.reserve(num);
    for (unsigned int i = 0; i < num; i++)
        //randInt+1 means we don't expect 0, preventing randTotal below is 0
        randInts.push_back(_getRand(randMax)+1);

    //calculate each person should get, floor what they get
    /* firstly, calculate the total num , let's assume total num won't exceed maximum of unsigned int */
    unsigned int randTotal = 0;
    for (auto iter = randInts.begin(); iter != randInts.end(); iter++)
        randTotal += *iter;
    /* secondly, calculate each person should get, record those index bigger than 0.01 and smaller than 0.01 */
    std::set<unsigned int> bigger;
    std::set<unsigned int> smaller;
    std::vector<unsigned int> tmpResult;
    tmpResult.reserve(num+1);
    unsigned int eachResult = 0, dispatchTotal = 0;
    for (unsigned int i = 0; i < num; i++)
    {
        eachResult= iTotal * randInts[i] / randTotal;
        tmpResult.push_back(eachResult);
        dispatchTotal += eachResult;
        if (eachResult > 1)
            bigger.insert(i);
        else if (eachResult < 1)
            smaller.insert(i);
    }
    /*dispatch the money left to random index*/
    unsigned int randIndex = 0;
    for (unsigned int i = 0; i < iTotal - dispatchTotal; i++)
    {
         randIndex = _getRand(num);
        if (++tmpResult[randIndex] == 1)
            smaller.erase(randIndex);
        else 
            bigger.insert(randIndex);
    }

    //check if every one get at least 1, if not, separate 1 from those more than 2
    unsigned int biggerSize = 0;
    for (auto iter = smaller.begin(); iter != smaller.end();)
    {
        biggerSize = bigger.size();
        randIndex = _getRand(biggerSize);
        std::set<unsigned int>::const_iterator it_target = bigger.begin();
        std::advance(it_target, randIndex); 
        ++tmpResult[*iter];
        smaller.erase(iter++);
        if (--tmpResult[*it_target] < 2)
            bigger.erase(it_target);
    }

    //transform int to double
    result.reserve(num);
    for (auto iter = tmpResult.begin(); iter != tmpResult.end(); ++iter)
        result.push_back(((double)*iter) / 100);

    return true;
}



int main()
{
    double total = 5;
    unsigned int num = 6;
    std::vector<double> result;
    
    if (dispatchLuckyMoney(total, num, result))
    {
        for (auto i = 0; i < num; i++)
            cout << result[i] << "\t";
    }
    else
        cout << "error input";

    return 0;
}