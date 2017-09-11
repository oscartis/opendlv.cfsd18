/**
* Copyright (C) 2017 Chalmers Revere
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
* USA.
*/

#include <iostream>

#include <opendavinci/odcore/base/Lock.h>
#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>
#include <opendavinci/odcore/strings/StringToolbox.h>
#include <opendavinci/odcore/wrapper/Eigen.h>



#include "autobrake.hpp"

namespace opendlv {
namespace logic {
namespace cfsd18 {

AutoBrake::AutoBrake(int32_t const &a_argc, char **a_argv)
: DataTriggeredConferenceClientModule(a_argc, a_argv, "logic-cfsd18-autobrake")
, m_speedThreshold(80/3.6)
, m_groundSpeed()
, positiveAutobrakeAcceleration(2)
, negativeAutobrakeAcceleration(-2)
{
}

AutoBrake::~AutoBrake()
{
}

void AutoBrake::nextContainer(odcore::data::Container &a_container)
{
  int32_t dataType = a_container.getDataType();
  if (dataType == opendlv::proxy::GroundSpeedReading::ID()) {
          auto groundSpeedReading = a_container.getData<opendlv::proxy::GroundSpeedReading>(); 
          
          const double groundSpeedKph = static_cast<double>(groundSpeedReading.getGroundSpeed());
          m_groundSpeed = groundSpeedKph / 3.6;

          TriggerAutobrake(m_groundSpeed);
              /*
          Container actuationRequestContainer = Container():
          getConference().send(actuationRequestContainer);*/
          /*
          opendlv::proxy::GroundSpeedReading groundSpeedReading;
          groundSpeedReading.setGroundSpeed(groundSpeed);

          Container groundSpeedReadingContainer = Container(groundSpeedReading);
          getConference().send(groundSpeedReadingContainer);*/
  }

}

/*odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode AutoBrake::body()
{
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {

//      Eigen::AngleAxisd deltaRollAngle(deltaRoll, Eigen::Vector3d::UnitX());

  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
*/
void AutoBrake::setUp()
{
  std::string const name = getKeyValueConfiguration().getValue<std::string>(
        "logic-cfsd18-autobrake.name");

  if (isVerbose()) {
    std::cout << "Name: " << name << std::endl;
  }
}

void AutoBrake::tearDown()
{
}

void AutoBrake::TriggerAutobrake(double vehicleSpeed)
{
  
  if (vehicleSpeed >= m_speedThreshold) {
    float autobrakeAcceleration = negativeAutobrakeAcceleration;
    opendlv::proxy::GroundSpeedReading groundSpeedReading;
    groundSpeedReading.setGroundSpeed((double)autobrakeAcceleration);

    odcore::data::Container groundSpeedReadingContainer(groundSpeedReading);
    getConference().send(groundSpeedReadingContainer);

  }
  else
  {
    float autobrakeAcceleration = positiveAutobrakeAcceleration;
    opendlv::proxy::GroundSpeedReading groundSpeedReading;
    groundSpeedReading.setGroundSpeed((double)autobrakeAcceleration);

    odcore::data::Container groundSpeedReadingContainer(groundSpeedReading);
    getConference().send(groundSpeedReadingContainer);

  }
  
          
}

}
}
}
