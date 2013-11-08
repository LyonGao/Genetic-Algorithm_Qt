#include"individual.h"

Individual::Individual()
{
    this->setValue(1.0);
    this->setFitness(1.0);
}

Individual::Individual(qreal value, qreal fitness)
{
    this->setValue(value);
    this->setFitness(fitness);
}

Individual::Individual(const Individual &indi)
{
    this->setValue( indi.getValue() );
    this->setFitness( indi.getFitness() );
}

Individual &Individual::operator=(const Individual &indi)
{  
    if( &indi != this)
    {
        this->setValue( indi.getValue() );
        this->setFitness( indi.getFitness() );
    }
    return *this;
}

void Individual::setValue(qreal value)
{
    this->value = value;
}

void Individual::setFitness(qreal fitness)
{
    this->fitness = fitness;
}

qreal Individual::getValue() const
{
    return value;
}

qreal Individual::getFitness() const
{
    return fitness;
}
