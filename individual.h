#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include<QtGlobal>

class Individual
{
public:
    //缺省构造函数
    Individual();
    Individual( qreal value, qreal fitness);
    //拷贝构造函数，接受的参数必须是应用类型，不然会陷入死循环
    Individual(const Individual &indi);
    //赋值运算符
    Individual &operator=(const Individual &indi);

    qreal getValue() const ;
    qreal getFitness()const ;
    void setValue(qreal value);
    void setFitness(qreal fitness);

private:
    qreal value;     //个体的值
    qreal fitness;   //个体的适应度
};

#endif // INDIVIDUAL_H
