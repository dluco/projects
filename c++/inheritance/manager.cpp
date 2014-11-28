/*
 *          File: manager.cpp
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: April 8, 2000
 *         Topic: Introduction to Inheritance in C++
 * ----------------------------------------------------------------
 *
 * Manager method definitions.
 */

#include "manager.h"

Manager::Manager(string theName,
                 float thePayRate,
                 bool isSalaried)
  : Employee(theName, thePayRate)
{
  salaried = isSalaried;
}

bool Manager::getSalaried() const
{
  return salaried;
}

float Manager::pay(float hoursWorked) const
{
  if (salaried)
    return payRate;
  /* else */
  return Employee::pay(hoursWorked);
}
