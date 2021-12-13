//
// Created by 1124a on 2021/10/27.
//
#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DENSITYGRID_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DENSITYGRID_HPP_
#include <cassert>
#include <iostream>
#include <vector>
#include <Algorithm/DataStructure/Point.hpp>
#include <algorithm>
#include <memory>


namespace SESAME{

class DensityGrid;
typedef std::shared_ptr<DensityGrid> DensityGridPtr;

class DensityGrid{
 public:
  /**
	 * For each dimension, its space Si, i =1, ··· ,d is divided into pi partitions as
	 * Si = Si,1 U Si,2 U ··· U Si,pi
	 * A density grid g that is composed of S1,j1 ×S2,j2 ···×Sd,jd , ji =1, ...,pi,
	 * has coordinates (j1,j2, ··· ,jd).
	 */
  std::vector<int> coordinates;
  /**
	 * The value of 'd' for the d-dimensional space S considered by D-Stream.
	 */
  int dimensions;

  /**
   * Flag denoting whether this density grid has been inspected during the adjustClustering()
   * step of D-Stream.
   */
   bool isVisited;
   /**
	 * A constructor method for a density grid
	 *
	 * @param c the coordinates of the density grid
	 */
   DensityGrid();
   DensityGrid(std::vector<int> coordin);

   /**
    * A constructor method for a density grid
    *
    * @param dg the density grid to copy
    */
   DensityGrid(DensityGrid const &grid);
   /**
	 * Generates a vector of neighbours for this density grid by varying each coordinate
	 * by one in either direction. Does not test whether the generated neighbours are valid as
	 * DensityGrid is not aware of the number of partitions in each dimension.
	 *
	 * @return a vector of neighbours for this density grid
	 */
    std::vector<DensityGrid> getNeighbours();

   /**
	 * Provides the probability of the argument instance belonging to the density grid in question.
	 *
	 * @return 1.0 if the instance equals the density grid's coordinates; 0.0 otherwise.
	 */

    double getInclusionProbability(Point point);

    bool operator==( DensityGrid& gridOther)const;




};
struct GridKeyHash{
  std::size_t operator()(const DensityGrid &densityGrid) const
  {
    //int[] primes = {31, 37, 41, 43, 47, 53, 59};
    int hc = 1;
    for (int i = 0 ; i < densityGrid.dimensions ; i++)
    {
      hc = (hc * 31) + densityGrid.coordinates[i];
    }

    return hc;
  }
};

struct EqualGrid
    {
  bool operator() (const DensityGrid &densityGrid1, const DensityGrid &densityGrid2) const
  {
    if(densityGrid1.dimensions != densityGrid2.dimensions)
      return false;
    for(int i = 0 ; i < densityGrid1.dimensions ; i++)
    {
      if(densityGrid1.coordinates[i] != densityGrid2.coordinates[i])
        return false;
    }
    return true;
  }
    };

}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DENSITYGRID_HPP_
