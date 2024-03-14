#ifndef PyroConditions_hpp
#define PyroConditions_hpp

#include "Data.hpp"
#include "State_Machine.h"

#define MainChutesTargetAlt 1000

bool ApogeeChutes(Data* data) {
  if (data->state.value == State::descent) {
    return true;
  }

  return false;
}

bool MainChute(Data* data) {
  if (data->state.value == State::parachute_descent && data->filted_alt.value < MainChutesTargetAlt) {
    return true;
  }

  return false;
}

bool TestCondition(Data* data) {
  if (data->time.value > 5.0) {
    return true;
  }

  return false;
}


#endif