#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include<QtGlobal>
#include<QList>
#include<cstdlib>
#include<ctime>
#include"individual.h"
#include"utility_fun.h"

#define SELECT_MAX 1
#define SELECT_MIN 0

class Population
{
public:

    Population();//default constructor
    Population(qint16 individual_num, qreal crossover_rate, qreal mutation_rate, qreal range_min, qreal range_max, qint8 select_mode);
    Population(const Population & p); //copy constructor
    Population & operator=(const Population & p);//assignment operator

    //种群初始化函数
    void init_population();
    //轮盘赌选择算子，根据每个个体的适应度值大小来选择，适应值大的个体入选的概率大
    void select_operator_roulette();
    //精英策略选择算子，用最大适应度值的个体代替最小适应度值得个体
    void select_operator_elitist_strategy();
    //交叉算子
    void crossover_operator();
    //变异算子
    void mutate_operator();
    //计算种群的适应度
    void calculate_fitness();
    //寻找最优个体
    qint16 find_best_individual_index();

    void setIndividualNum(qint16 individual_num);
    void setCrossoverRate(qreal crossover_rate);
    void setMutationRate(qreal mutation_rate);
    void setRangeMinAndMax(qreal min, qreal max);
    void setSelectMode(qint8 select_mode);

    qint16 getIndividualNum() const;
    qreal getCrossoverRate() const;
    qreal getMutationRate() const;
    qreal getRangeMin() const;
    qreal getRangeMax() const;
    qint8 getSelectMode() const;
    //获得在index处个体的适应度值
    qreal getFitnessAt( qint16 index ) const ;
    //获得在index处个体的值
    qreal getValueAt( qint16 index ) const ;


private:
    qint16 individual_num;//种群的个体个数
    qreal crossover_rate;//交叉率
    qreal mutation_rate;//变异率
    qreal range_min;//个体取值下限
    qreal range_max;//个体取值上限
    qint8 select_mode; //选择最大值或者最小值     SELECT_MAX or SELECT_MIN
    QList<Individual> population;//种群
};

#endif // GENETIC_ALGORITHM_H






