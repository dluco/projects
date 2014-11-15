/*
 *          File: manager0.cpp
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: April 8, 2000
 *         Topic: Introduction to Inheritance in C++
 * ----------------------------------------------------------------
 *
 * The poorer Manager method definitions.  It doesn't reuse Employee!
 */

#include "manager0.h"

Manager::Manager(string theName,
                 float thePayRate,
                 bool isSalaried)
{
  name = theName;
  payRate = thePayRate;
  salaried = isSalaried;
}

string Manager::getName() const
{
  return name;
}

float Manager::getPayRate() const
{
  return payRate;
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
  return hoursWorked * payRate;
}
