/*
 * Copyright 2017 The Cartographer Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CARTOGRAPHER_MAPPING_POSE_EXTRAPOLATOR_H_
#define CARTOGRAPHER_MAPPING_POSE_EXTRAPOLATOR_H_

#include <deque>

#include "cartographer/common/time.h"
#include "cartographer/transform/rigid_transform.h"

namespace cartographer {
namespace mapping {

// Keeps poses for a certain duration to estimate linear and angular velocity,
// and uses the velocities to extrapolate motion.
//
// TODO(whess): Add IMU data and odometry and provide improved extrapolation
// models making use of all available data.
class PoseExtrapolator {
 public:
  explicit PoseExtrapolator(common::Duration pose_queue_duration);

  PoseExtrapolator(const PoseExtrapolator&) = delete;
  PoseExtrapolator& operator=(const PoseExtrapolator&) = delete;

  // Returns the time of the last added pose or Time::min() if no pose was added
  // yet.
  common::Time GetLastPoseTime() const;

  void AddPose(common::Time time, const transform::Rigid3d& pose);
  transform::Rigid3d ExtrapolatePose(common::Time time);

 private:
  const common::Duration pose_queue_duration_;
  struct TimedPose {
    common::Time time;
    transform::Rigid3d pose;
  };
  std::deque<TimedPose> timed_pose_queue_;
};

}  // namespace mapping
}  // namespace cartographer

#endif  // CARTOGRAPHER_MAPPING_POSE_EXTRAPOLATOR_H_
