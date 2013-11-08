#include"genetic_algorithm.h"

Population::Population()
{
    this->setIndividualNum(100);
    this->setCrossoverRate(0.5);
    this->setMutationRate(0.01);
    this->setRangeMinAndMax(-1, 1);
    this->setSelectMode(SELECT_MAX);
    this->init_population();
}

Population::Population(qint16 individual_num, qreal crossover_rate, qreal mutation_rate,
                       qreal range_min, qreal range_max, qint8 select_mode)
{
    this->setIndividualNum( individual_num );
    this->setCrossoverRate( crossover_rate );
    this->setMutationRate( mutation_rate );
    this->setRangeMinAndMax( range_min, range_max );
    this->setSelectMode( select_mode );
    this->init_population();
}

Population::Population(const Population &p)
{
    this->setIndividualNum( p.getIndividualNum() );
    this->setCrossoverRate( p.getCrossoverRate() );
    this->setMutationRate( p.getMutationRate() );
    this->setRangeMinAndMax( p.getRangeMin(), p.getRangeMax());
    this->setSelectMode( p.getSelectMode() );

    this->population.clear();

    qint16 size = p.population.size();

    for(qint16 i = 0; i < size; i++)
    {
        Individual indi;
        indi.setValue( p.population.at( i ).getValue() );
        indi.setFitness( p.population.at( i ).getFitness() );
        this->population.append( indi );
    }

}

Population & Population::operator =(const Population & p)
{
    if( this != &p)//avoid self assignment
    {
        this->setIndividualNum( p.getIndividualNum() );
        this->setCrossoverRate( p.getCrossoverRate() );
        this->setMutationRate( p.getMutationRate() );
        this->setRangeMinAndMax( p.getRangeMin(), p.getRangeMax() );
        this->setSelectMode( p.getSelectMode() );

        this->population.clear();

        qint16 size = p.population.size();

        for(qint16 i = 0; i < size; i++)
        {
            Individual indi;
            indi.setValue( p.population.at( i ).getValue() );
            indi.setFitness( p.population.at( i ).getFitness() );
            this->population.append( indi );
        }
    }
        return *this;
}

void Population::init_population()
{
    qreal range = range_max - range_min;
    qreal interval = range / ( individual_num - 1 );

    for(qint16 i = 0 ; i < individual_num ; i ++ )
    {
        Individual indi;
        indi.setValue( range_min + i * interval );
        indi.setFitness( 1.0 );

        population.append(indi);
    }
}

void Population::select_operator_roulette()
{
    qreal indi_sum = 0.0, sum = 0.0;
    qint16 size = population.size();
    qreal * accu_sum = new qreal [ size ];
    QList<Individual> temp_population;

    for( qint16 i = 0; i < size; i ++ )
    {
        indi_sum += population.at( i ).getFitness() ;
    }

    for( qint16 i = 0; i < size; i ++ )
    {
        sum += population.at( i ).getFitness() / indi_sum ;
        accu_sum[ i ] = sum ;
    }

    srand( time( 0 ) );

    for(qint16 i = 0; i < size; i ++)
    {
        qreal rand_num = UtilityFunction::random(0.0, 1.0, 4);
        qint16 index = UtilityFunction::GA_binary_search( accu_sum, rand_num, 0, size - 1);

        Individual indi;
        indi.setValue( population.at( index ).getValue() );
        indi.setFitness( population.at( index ).getFitness() );
        temp_population.append( indi );
    }

    for(qint16 i = 0; i < size; i ++)
    {
        population[ i ].setValue( temp_population.at( i ).getValue() );
        population[ i ].setFitness( temp_population.at( i ).getFitness() );
    }

    delete [] accu_sum;
}

void Population::select_operator_elitist_strategy()
{
    qreal max_fitness, min_fitness;
    min_fitness = max_fitness = this->population.at( 0 ).getFitness();
    qreal temp_fitness = 1.0;
    qint16 min_fitness_index = 0, max_fitness_index = 0;

    qint16 size = this->population.size();

    for( qint16 i = 1; i < size; i ++)
    {
        temp_fitness = this->population.at( i ).getFitness();
        if( temp_fitness > max_fitness )
        {
            max_fitness = temp_fitness;
            max_fitness_index = i;
        }
        if( temp_fitness < min_fitness )
        {
            min_fitness = temp_fitness;
            min_fitness_index = i;
        }
    }

    Individual max_fitness_indi;
    max_fitness_indi.setFitness( max_fitness );
    max_fitness_indi.setValue( this->population.at( max_fitness_index ).getValue() );

    //用最大适应度值的个体代替最小适应度值得个体
    this->population.replace(min_fitness_index, max_fitness_indi);
}

void Population::calculate_fitness()
{
    qint16 size = this->population.size();
    qreal min_value , max_value;
    min_value = max_value = UtilityFunction::f( this->population.at( 0 ).getValue() );

    switch( this->getSelectMode() )
    {
        case SELECT_MAX://select max

            for( qint16 i = 1; i < size; i ++)
            {
                qreal temp = UtilityFunction::f( this->population.at( i ).getValue() );
                if( temp < min_value )
                {
                    min_value = temp;
                }
            }

            for( qint16 i = 0; i < size; i ++)
            {
                this->population[ i ].setFitness( UtilityFunction::f( this->population.at( i ).getValue() ) - min_value + 1 );
            }

            break;

        case SELECT_MIN://select min

            for(qint16 i = 1; i < size; i++)
            {
                qreal temp = UtilityFunction::f( this->population.at( i ).getValue() );
                if( temp >  max_value )
                {
                    max_value = temp;
                }
            }

            for( qint16 i = 0; i < size; i ++)
            {
                this->population[ i ] .setFitness( max_value - UtilityFunction::f( this->population.at( i ).getValue() ) + 1 );
            }

            break;

        default:

            for( qint16 i = 0; i < size; i ++)
            {
                this->population[ i ].setFitness( 1.0 );
            }

            break;
    }
}

void Population::crossover_operator()
{
    qint16 crossover_times = (qint16)( getIndividualNum() * getCrossoverRate() );//交叉次数
    qint16 best_indi_index = find_best_individual_index();

    srand( time( 0 ) );//设置随机数种子

    for( qint16 i = 0; i < crossover_times; i ++)
    {
        qint16 rand_indi_1_index = ( qint16 ) UtilityFunction::random( 0, getIndividualNum() );//随机产生用于交叉的个体
        qint16 rand_indi_2_index = ( qint16 ) UtilityFunction::random( 0, getIndividualNum() );//随机产生用于交叉的个体

        //保存第一个出现的适应度值最大的个体，这个个体不做交叉算子和变异算子
        if( rand_indi_1_index == best_indi_index )
        {
            //避免越界
            ( rand_indi_1_index + 1 == getIndividualNum() ) ? ( rand_indi_1_index -- ) : (rand_indi_1_index ++ );
        }
        if( rand_indi_2_index == best_indi_index )
        {
            //避免越界
            ( rand_indi_2_index + 1 == getIndividualNum() ) ? ( rand_indi_2_index -- ) : ( rand_indi_2_index ++ );
        }

        qreal crossover_factor = UtilityFunction::random(0.0, 1.0, 3);//交叉系数
        qreal rand_indi_1_value = this->population.at( rand_indi_1_index ).getValue();
        qreal rand_indi_2_value = this->population.at( rand_indi_2_index ).getValue();

        //交叉之后新个体的值在两个父个体值之间
        this->population[ rand_indi_1_index ].setValue( crossover_factor * rand_indi_1_value + ( 1.0 - crossover_factor ) *rand_indi_2_value );
        this->population[ rand_indi_2_index ].setValue( crossover_factor * rand_indi_2_value + ( 1.0 - crossover_factor ) * rand_indi_1_value);
    }

}

void Population::mutate_operator()
{
    qint16 mutation_times = (qint16) ( getIndividualNum() * getMutationRate() );
    qint16 best_indi_index = find_best_individual_index();

    srand( time( 0 ) );

    for(qint16 i = 0; i < mutation_times; i ++)
    {
        qreal rand_mutation_factor = UtilityFunction::random( 0.0, 1.0, 3);
        qint16 rand_indi_index = ( qint16 ) UtilityFunction::random(0, getIndividualNum() - 1 );

        //保存第一个出现的适应度值最大的个体，这个个体不做交叉算子和变异算子
        if( rand_indi_index == best_indi_index)
        {
            ( rand_indi_index + 1 == getIndividualNum() ) ? ( rand_indi_index -- ) : ( rand_indi_index ++ );
        }

        //交叉公式为：x' = x + (a - 0.5)Xmax;
        this->population[ rand_indi_index ].setValue( this->population.at( rand_indi_index ).getValue()
                                                            + ( rand_mutation_factor - 0.5 ) * ( getRangeMax() - getRangeMin() ) );

    }

}

qint16 Population::find_best_individual_index()
{
    qint16 size = this->population.size();
    qreal biggest_fitness = this->population.at( 0 ).getFitness();
    qint16 biggest_fitness_index = 0;

    for( qint16 i = 1; i < size; i ++)
    {
        qreal temp_fitness = this->population.at( i ).getFitness();

        if( temp_fitness > biggest_fitness )
        {
            biggest_fitness = temp_fitness;
            biggest_fitness_index = i ;
        }
    }

    return biggest_fitness_index;

}

void Population::setIndividualNum(qint16 individual_num)
{
    if( individual_num < 0 )
    {
        this ->individual_num = 100;
    }
    else
    {
        this ->individual_num = individual_num;
    }
}

void Population::setRangeMinAndMax(qreal min, qreal max)
{
    if( min > max )
    {
        this ->range_min = max;
        this ->range_max = min;
    }
    else
    {
        this ->range_min = min;
        this ->range_max = max;
    }
}

void Population::setCrossoverRate(qreal crossover_rate)
{
    if( crossover_rate < 0 || crossover_rate > 1 )
    {
        this ->crossover_rate = 0.6;
    }
    else
    {
        this->crossover_rate = crossover_rate;
    }
}

void Population::setMutationRate(qreal mutation_rate)
{
    if( mutation_rate < 0 || mutation_rate > 1 )
    {
        this->mutation_rate = 0.01;
    }
    else
    {
        this ->mutation_rate = mutation_rate;
    }
}

void Population::setSelectMode(qint8 select_mode)
{
    switch (select_mode)
    {
        case SELECT_MIN:
            this->select_mode = SELECT_MIN;
            break;
        case SELECT_MAX:
            this->select_mode = SELECT_MAX;
            break;
        default:
            this->select_mode = SELECT_MAX;
            break;
    }
}

qint16 Population::getIndividualNum() const
{
    return this->individual_num;
}

qreal Population::getCrossoverRate() const
{
    return this->crossover_rate;
}

qreal Population::getMutationRate() const
{
    return this->mutation_rate;
}

qreal Population::getRangeMin() const
{
    return this->range_min;
}

qreal Population::getRangeMax() const
{
    return this->range_max;
}

qint8 Population::getSelectMode() const
{
    return this->select_mode;
}

qreal Population::getFitnessAt(qint16 index) const
{
    return this->population.at( index ).getFitness();
}

qreal Population::getValueAt(qint16 index) const
{
    return this->population.at( index ).getValue();
}









