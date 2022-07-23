//
// Created by 1124a on 2021/10/27.
//
#include <Algorithm/DataStructure/DensityGrid.hpp>
#include <Utils/Logger.hpp>

SESAME::DensityGrid::DensityGrid(){

}
SESAME::DensityGrid::DensityGrid(std::vector<double> coordin)
{
  this->dims = coordin.size();
  this->coordinates= std::vector<double> (this->dims,0) ;
  for (int i = 0 ; i < this->dims ; i++)
    this->coordinates[i] = coordin[i];
  this->isVisited = false;
}

SESAME::DensityGrid::DensityGrid(DensityGrid const &grid)
{
  std::vector<double> coord = grid.coordinates;
  this->dims = grid.dims;
  this->coordinates= std::vector<double> (this->dims,0) ;
  for (int i = 0 ; i < this->dims ; i++)
    this->coordinates[i] = coord[i];
  this->isVisited = false;
}

/**
  * Generates a vector of neighbours for this density grid by varying each coordinate
  * by one in either direction. Does not test whether the generated neighbours are valid as
  * DensityGrid is not aware of the number of partitions in each dim.
  *
  * @return a vector of neighbours for this density grid
  */

std::vector<SESAME::DensityGrid> SESAME::DensityGrid::getNeighbours()
{
//  SESAME_INFO("Obtain neighbours");
  std::vector<DensityGrid> neighbours;
  std::vector<double> hCoord = this->coordinates ;
  for (int i = 0 ; i < this->dims ; i++)
  {
    hCoord[i] = hCoord[i]-1;
    DensityGrid grid(hCoord);
    neighbours.push_back(grid);

    hCoord[i] = hCoord[i]+2;
    DensityGrid grid2(hCoord);
    neighbours.push_back(grid2);

    hCoord[i] = hCoord[i]-1;
  }

  return neighbours;
}


/**
  * Provides the probability of the argument instance belonging to the density grid in question.
  *
  * @return 1.0 if the instance equals the density grid's coordinates; 0.0 otherwise.
  */


double SESAME::DensityGrid::getInclusionProbability(Point point) {
  for (int i = 0 ; i < this->dims ; i++)
  {
    if ((int) point.getFeatureItem(i) != this->coordinates[i])
      return 0.0;
  }
  return 1.0;
}

bool SESAME::DensityGrid::operator==( DensityGrid& gridOther)const {
  bool equal=true;
  if (this == &gridOther) {
    equal=true;
  }
  if(this->dims !=  gridOther.dims)
    equal = false;
  for(int i = 0 ; i < this->dims ; i++)
  {
    if(this->coordinates[i] != gridOther.coordinates[i])
      equal= false;
  }
  return equal;
}