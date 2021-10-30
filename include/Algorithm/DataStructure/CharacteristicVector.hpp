//
// Created by 1124a on 2021/10/27.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CHARACTERISTICVECTOR_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CHARACTERISTICVECTOR_HPP_
#include <ctime>
#include <cmath>
namespace SESAME{
enum Status{
  SPARSE,TRANSITIONAL ,DENSE
};
class CharacteristicVector {
 public:
  /**
    * t_g: The last time when g is updated
    */
   clock_t updateTime;

  /**
   * tm : last time when g is removed from grid_list as a sporadic grid (if ever).
   */
  clock_t removeTime;

  /**
   * D: the grid density at the last update
   */
  double gridDensity;

  /**
   * label: the cluster label of the grid
   */
   int label;

  /**
   * status: status = {SPORADIC, NORMAL}
   */
  bool isSporadic;

  /**
   * attribute: attribute = {SPARSE, TRANSITIONAL, DENSE}
   */
   int attribute;

  /**
   * time stamp at which the grid's density was last updated (including initial and adjust clustering)
   */
  clock_t densityUpdateTime;

  /**
   * Flag marking whether there was a change in the attribute field
   * the last time the grid density was updated.
   */
  bool attChange;


  CharacteristicVector(clock_t updateTime, clock_t removeTime, double Density, int label, bool status, double dl, double dm);
  double getCurrGridDensity(clock_t NowTime, double lambda);

  bool isSparse(double dl);
  bool isDense(double dm);
  /**
	 * Implements the density update function given in
	 * eq 5 (Proposition 3.1) of Chen and Tu 2007.
	 *
	 * @param currTime the data stream's current internal time
	 * @param decayFactor the value of lambda
   */
  void densityWithNew(clock_t NowTime, double decayFactor);
  /**
	 * Implements the update the density of all grids step given at line 2 of
	 * both Fig 3 and Fig 4 of Chen and Tu 2007.
	 *
	 * @param currTime the data stream's current internal time
	 * @param decayFactor the value of lambda
	 * @param dl the threshold for sparse grids
	 * @param dm the threshold for dense grids
	 */
  void UpdateAllDensity(clock_t NowTime, double decayFactor, double dl, double dm);
  void ChangeAttribute(double dl, double dm);
};
}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CHARACTERISTICVECTOR_HPP_
