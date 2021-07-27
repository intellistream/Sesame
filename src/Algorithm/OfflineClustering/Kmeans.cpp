//
// Created by Shuhao Zhang on 20/07/2021.
//

#include <Algorithm/OfflineClustering/KMeans.hpp>
#include <Utils/UtilityFunctions.hpp>
#include <Utils/Logger.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>

/**
 * kMeans++ algorithm for n points of dimension d with k centres
 * @param k
 * @param n
 * @param d
 * @param points
 * @param resultCost
 * @return
 */
std::vector<SESAME::PointPtr> SESAME::KMeans::lloydPlusPlus(int k,
                                                            int n,
                                                            int d,
                                                            std::vector<PointPtr> &points,
                                                            double *resultCost) {
  SESAME_INFO("starting kMeans++");
  //choose random centres
  std::vector<SESAME::PointPtr> centres = chooseRandomCentres(k, n, points);
  double cost = targetFunctionValue(k, n, centres, points);
  double newCost = cost;
  std::vector<PointPtr> massCentres; //= new Point[k];
  double numberOfPoints[k];

  do {
    cost = newCost;
    //reset centres of mass
    int i = 0;
    for (i = 0; i < k; i++) {
      massCentres.push_back(DataStructureFactory::createPoint(0, 1.0, d, 0.0));
      numberOfPoints[i] = 0.0;
    }
    //compute centres of mass
    for (i = 0; i < n; i++) {
      int centre = determineClusterCentreKMeans(k, points[i], centres);
      int l = 0;
      for (l = 0; l < points[i]->getDimension(); l++) {
        if (points[i]->getWeight() != 0.0)
          massCentres[centre]->setFeatureItem(points[i]->getFeatureItem(l), l);
      }
      numberOfPoints[centre] += points[i]->getWeight();

    }

    //move centres
    for (i = 0; i < k; i++) {
      int l = 0;
      for (l = 0; l < centres[i]->getDimension(); l++) {
        centres[i]->setFeatureItem(massCentres[i]->getFeatureItem(l), l);
        centres[i]->setWeight(numberOfPoints[i]);
      }
    }

    //calculate costs
    newCost = targetFunctionValue(k, n, centres, points);
    SESAME_INFO("old cost:" << cost << ", new cost:" << newCost << " \n");
  } while (newCost < THRESHOLD * cost);

  SESAME_INFO("Centres: \n");
  int i = 0;
  for (i = 0; i < k; i++) {
    printf("(");
    int l = 0;
    for (l = 0; l < centres[i]->getDimension(); l++) {
      printf("%f,", centres[i]->getFeatureItem(l) / centres[i]->getWeight());
    }
    printf(")\n");
  }
  *resultCost = newCost;
  SESAME_INFO("kMeans++ finished\n");
  return centres;
}

/**
 * Randomly chooses k centres with kMeans++ distribution
 * @param k
 * @param n
 * @param d
 * @param points
 * @return
 */
std::vector<SESAME::PointPtr> SESAME::KMeans::chooseRandomCentres(int k, int n, std::vector<PointPtr> &points) {
  //array to store the choosen centres
  std::vector<SESAME::PointPtr> centres; //= new Point[k];

  //choose the first centre (each point has the same probability of being choosen)
  int i = 0;

  int next = 0;
  int j = 0;
  do {//only choose from the n-i points not already choosen
    next = UtilityFunctions::genrand_int31() % (n - 1);

    //check if the choosen point is not a dummy
  } while (points[next]->getWeight() < 1);

  //set j to next unchoosen point
  j = next;
  //copy the choosen point to the array
//  centres[i]=Initialization(0, 1, d, 0);
  centres.push_back(points[j]->copy());

  //TODO: set the current centre for all points to the choosen centre??
  for (i = 0; i < n; i++) {
    points[i]->setClusteringCenter(0);
    points[i]->setCost(costOfPointToCenter(points[i], centres[0]));
  }
  //choose centre 1 to k-1 with the kMeans++ distribution
  for (i = 1; i < k; i++) {
    double cost = 0.0;
    for (j = 0; j < n; j++) {
      cost += points[j]->getCost();
    }

    double random = 0;
    double sum = 0.0;
    int pos = -1;

    do {
      random = UtilityFunctions::genrand_real3();
      sum = 0.0;
      pos = -1;
      for (j = 0; j < n; j++) {
        sum = sum + points[j]->getCost();
        if (random <= sum / cost) {
          pos = j;
          break;
        }
      }

    } while (points[pos]->getWeight() < 1);
    //copy the choosen centre
//    centres[i]->Initialization(0, 1, d, 0);
    centres.push_back(points[pos]->copy());
    //check which points are closest to the new centre
    for (j = 0; j < n; j++) {
      double newCost = costOfPointToCenter(points[j], centres[i]);
      if (points[j]->getCost() > newCost) {
        points[j]->setCost(newCost);
        points[j]->setClusteringCenter(i);
      }
    }

  }

  SESAME_INFO("random centres:");
  for (i = 0; i < k; i++) {
    SESAME_INFO(to_string(i) + "(");
    int l = 0;
    for (l = 0; l < centres[i]->getDimension(); l++) {
      SESAME_INFO(centres[i]->getFeatureItem(l) / centres[i]->getWeight());
    }
    SESAME_INFO(")");
  }

  return centres;
}

/**
 * Computes the index of the centre nearest to p with the given array of centres centres[] (of size k)
 * @param k
 * @param p
 * @param centres
 * @return
 */
int SESAME::KMeans::determineClusterCentreKMeans(int k, PointPtr p, std::vector<PointPtr> &centres) {
  int index = 0;
  double nearestCost = -1.0;
  int j = 0;
  for (j = 0; j < k; j++) {
    double distance = 0.0;
    int l = 0;
    for (l = 0; l < p->getDimension(); l++) {
      //Centroid coordinate of the point
      double centroidCoordinatePoint;
      if (p->getWeight() != 0.0) {
        centroidCoordinatePoint = p->getFeatureItem(l) / p->getWeight();
      } else {
        centroidCoordinatePoint = p->getFeatureItem(l);
      }
      //Centroid coordinate of the centre
      double centroidCoordinateCentre;
      if (centres[j]->getFeatureItem(l) != 0.0) {
        centroidCoordinateCentre = centres[j]->getFeatureItem(l) / centres[j]->getWeight();
      } else {
        centroidCoordinateCentre = centres[j]->getFeatureItem(l);
      }
      distance += (centroidCoordinatePoint - centroidCoordinateCentre) *
          (centroidCoordinatePoint - centroidCoordinateCentre);
    }

    if (nearestCost < 0 || distance < nearestCost) {
      nearestCost = distance;
      index = j;
    }
  }
  return index;
}

/**
 * computes the target function for the given pointarray points[] (of size n) with the given array of centres centres[] (of size k)
 * @param k
 * @param n
 * @param centres
 * @param points
 * @return
 */
double SESAME::KMeans::targetFunctionValue(int k,
                                           int n,
                                           std::vector<PointPtr> &centres,
                                           std::vector<PointPtr> &points) {
  int i = 0;
  double sum = 0.0;
  for (i = 0; i < n; i++) {
    double nearestCost = -1.0;
    int j = 0;
    for (j = 0; j < k; j++) {
      double distance = 0.0;
      int l = 0;
      for (l = 0; l < points[i]->getDimension(); l++) {
        //Centroid coordinate of the point
        double centroidCoordinatePoint;
        if (points[i]->getWeight() != 0.0) {
          centroidCoordinatePoint = points[i]->getFeatureItem(l) / points[i]->getWeight();
        } else {
          centroidCoordinatePoint = points[i]->getFeatureItem(l);
        }
        //Centroid coordinate of the centre
        double centroidCoordinateCentre;
        if (centres[j]->getWeight() != 0.0) {
          centroidCoordinateCentre = centres[j]->getFeatureItem(l) / centres[j]->getWeight();
        } else {
          centroidCoordinateCentre = centres[j]->getFeatureItem(l);
        }
        distance += (centroidCoordinatePoint - centroidCoordinateCentre) *
            (centroidCoordinatePoint - centroidCoordinateCentre);

      }
      if (nearestCost < 0 || distance < nearestCost) {
        nearestCost = distance;
      }
    }
    sum += nearestCost * points[i]->getWeight();
  }
  return sum;
}
/**
Computes the cost of point p with centre centre
**/
double SESAME::KMeans::costOfPointToCenter(PointPtr p, PointPtr centre) {
  if (p->getWeight() == 0.0) {
    return 0.0;
  }

  //stores the distance between p and centre
  double distance = 0.0;

  //loop counter
  int l;

  for (l = 0; l < p->getDimension(); l++) {
    //Centroid coordinate of the point
    double centroidCoordinatePoint;
    if (p->getWeight() != 0.0) {
      centroidCoordinatePoint = p->getFeatureItem(l) / p->getWeight();
    } else {
      centroidCoordinatePoint = p->getFeatureItem(l);
    }
    //Centroid coordinate of the centre
    double centroidCoordinateCentre;
    if (centre->getWeight() != 0.0) {
      centroidCoordinateCentre = centre->getFeatureItem(l) / centre->getWeight();
    } else {
      centroidCoordinateCentre = centre->getFeatureItem(l);
    }
    distance += (centroidCoordinatePoint - centroidCoordinateCentre) *
        (centroidCoordinatePoint - centroidCoordinateCentre);

  }
  return distance * p->getWeight();
}

