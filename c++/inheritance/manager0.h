/*
 *          File: manager0.h
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: April 8, 2000
 *         Topic: Introduction to Inheritance in C++
 * ----------------------------------------------------------------
 *
 * The poorer Manager class definition.  It doesn't reuse Employee!
 */

#ifndef _MANAGER_H
#define _MANAGER_H

#include <string>

class Manager {
public:
  Manager(string theName,
          float thePayRate,
          bool isSalaried);

  string getName() const;
  float getPayRate() const;
  bool getSalaried() const;

  float pay(float hoursWorked) const;

protected:
  string name;
  float payRate;
  bool salaried;
};

#endif /* not defined _MANAGER_H */
