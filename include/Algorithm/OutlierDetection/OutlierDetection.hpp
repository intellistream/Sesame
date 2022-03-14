//
// Created by 1124a on 2021/11/2.
//

#ifndef SESAME_INCLUDE_ALGORITHM_OUTLIERDETECTION_OUTLIERDETECTION_HPP_
#define SESAME_INCLUDE_ALGORITHM_OUTLIERDETECTION_OUTLIERDETECTION_HPP_

#include "Algorithm/DesignAspect/Param.hpp"

#include <memory>

namespace SESAME {
class OutlierDetection;
typedef std::shared_ptr<OutlierDetection> OutlierDetectionPtr;

class OutlierDetection {};

class Distance : public OutlierDetection {
public:
  Distance(const StreamClusteringParam &param) {}
  // TODO
};

} // namespace SESAME
#endif // SESAME_INCLUDE_ALGORITHM_OUTLIERDETECTION_OUTLIERDETECTION_HPP_
