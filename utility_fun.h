#ifndef UTILITY_FUN_H
#define UTILITY_FUN_H

#include<QtGlobal>

#define PI 3.1415926

class UtilityFunction
{
public:
    /*********************************************************************************
    参数：
    data：待搜索的双精度浮点数数组；
    key：搜索键值；
    left_index：搜索的起始下标；
    right_index：搜索的终止下标；
    功能：遗传算法二分搜索法，如果数组data中存在key，则返回key所在的下标，如果出错，
    则返回-1，否则返回终止下标。
    *********************************************************************************/
    static qint16 GA_binary_search(qreal* data, qreal key, qint16 left_index, qint16 right_index);

    /********************************************************************************
    参数：
    from->随机数的下限, to->随机数的上限, digit_num->随机数保留的小数点位数;
    功能：生成随机数；
    注意：
    1). digit_num的大小不小于from和to的最大小数点位数，比如from=0.005, to=0.01时,
    digit_num>=3。
    ***********************************************************************************/
    static qreal random(qreal from, qreal to, qint8 digit_num);

    /**********************************************************************************
    参数：
    from->随机数的下限, to->随机数的上限;
    功能：生成随机数，其小数点位数是from和to中最大的位数；
    比如：
    double rand = 0;
    double from = 2.33;  //两位小数；
    double to = 3.898878  //六位小数；
    rand = random(from, to);  //rand有六位小数；
    注意：此函数主要用于小数点位数未知的数求随机数。
    ***********************************************************************************/
    static qreal random(qreal from, qreal to);

    static qreal f(qreal x);

};

#endif // UTILITY_FUN_H










