//
// Created by 1124a on 2021/10/27.
//

#include <Algorithm/DataStructure/CharacteristicVector.hpp>
SESAME::CharacteristicVector::CharacteristicVector(){

}
SESAME::CharacteristicVector::CharacteristicVector(clock_t updateTime, clock_t removeTime, double Density,
                                                   int label, bool status, double dl, double dm)
{
  this->updateTime=updateTime;
  this->removeTime=removeTime;
  this->gridDensity=Density;
  this->densityUpdateTime=updateTime;
  this->label=label;
  this->isSporadic=status;
  if (this->isSparse(dl))
    this->attribute = SPARSE;
  else if (this->isDense(dm))
    this->attribute = DENSE;
  else
    this->attribute = TRANSITIONAL;
  this->attChange=false;
}



bool SESAME::CharacteristicVector::isSparse(double dl)
{
  if(this->gridDensity<=dl)
    return true;
  else
    return false;
}

bool SESAME::CharacteristicVector::isDense(double dm){
  if(this->gridDensity>=dm)
    return true;
  else
    return false;
}
bool SESAME::CharacteristicVector::isTransitional(double dm, double dl){
  if(this->gridDensity>=dl && this->gridDensity<=dm)
    return true;
  else
    return false;
}
double SESAME::CharacteristicVector::getCurrGridDensity(clock_t NowTime, double lambda)
{
  return pow(lambda, (double)(NowTime-this->updateTime)/CLOCKS_PER_SEC) * this->gridDensity;
}
void SESAME::CharacteristicVector::densityWithNew(clock_t NowTime, double decayFactor)
{
  // Update the density grid's density
  this->gridDensity = getCurrGridDensity(NowTime,decayFactor)+1.0;
  //System.out.println(densityOfG);
  this->densityUpdateTime=NowTime;
}
void SESAME::CharacteristicVector::UpdateAllDensity(clock_t NowTime, double decayFactor, double dl, double dm)
{

  // record the last attribute
  int lastAtt = this->attribute;
  // Update the density grid's density
  this->gridDensity = getCurrGridDensity(NowTime,decayFactor);
  this->densityUpdateTime=NowTime;

  // Evaluate whether the density grid is now SPARSE, DENSE or TRANSITIONAL
  if (this->isSparse(dl))
    this->attribute = SPARSE;
  else if (this->isDense(dm))
    this->attribute = DENSE;
  else
    this->attribute = TRANSITIONAL;
  // Evaluate whether the density grid attribute has changed and set the attChange flag accordingly
  if (this->attribute == lastAtt)
    this->attChange = false;
  else
    this->attChange = true;
}
void SESAME::CharacteristicVector::ChangeAttribute(double dl, double dm){
  if (this->isSparse(dl))
    this->attribute = SPARSE;
  else if (this->isDense(dm))
    this->attribute = DENSE;
  else
    this->attribute = TRANSITIONAL;
}