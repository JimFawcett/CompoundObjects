///////////////////////////////////////////////////////////////////////
// CompObj.cpp - Compound Object Operations                          //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Midterm Spring 2016 //
///////////////////////////////////////////////////////////////////////
/*
 * The main objectives of this demonstrations are to:
 * - illustrate the importance of constructor initialization sequences
 * - expose all of the operations that compound objects are expected
 *   to provide.
 */
#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"

void putMsg(const std::string& msg)
{
  std::cout << "\n  " << msg.c_str();
}

struct Cosmetic {
  ~Cosmetic() { std::cout << "\n\n"; }
} GlobalCosm;

///////////////////////////////////////////////////////////////////////
// C class - instances are composed by the base class B

class C
{
public:
  C() { putMsg("C default construction"); }
  C(const C& c) : name_(c.name_)  // note initialization
  { 
    putMsg("C copy construction"); 
  }
  C& operator=(const C& c) 
  { 
    putMsg("C assignment");
    if (this == &c)
      return *this;
    name_ = c.name_;
    return *this;
  }
  C(C&& c) : name_(std::move(c.name_))
  { 
    putMsg("C move construction"); 
  }
  C& operator=(C&& c) 
  { 
    putMsg("C move assignment"); 
    if (this == &c)
      return *this;
    name_ = std::move(c.name_);
    return *this;
  }
  ~C() 
  { 
    putMsg("C destruction"); 
  }
private:
  std::string name_;
};
///////////////////////////////////////////////////////////////////////
// B class - serves as the base of inheritance for D
/*
*  B's constructors, as their first act, invoke a C constructor.  They
*  don't execute their bodies until C's construction completes.
*/
class B
{
public:
  B() 
  { 
    putMsg("B default construction"); 
  }
  B(const B& b) : c(b.c) 
  { 
    putMsg("B copy construction"); 
  }
  B& operator=(const B& b) 
  { 
    putMsg("B copy assignment"); 
    if (this == &b)
      return *this;
    c = b.c; 
    return *this;
  }
  B(B&& b) : c(std::move(b.c)) 
  { 
    putMsg("B move construction"); 
  }
  B& operator=(B&& b) 
  { 
    putMsg("B move assignment"); 
    if (this == &b)
      return *this;
    c = std::move(b.c); 
    return *this;
  }
  virtual void g()
  {
    putMsg("Calling B::g()");
  }
  virtual ~B() 
  { 
    putMsg("B destruction"); 
  }
private:
  C c;
};
///////////////////////////////////////////////////////////////////////
// U class - instances are used by D

class U
{
public:
  U() 
  { 
    putMsg("U default construction"); 
  }
  U(const U& u) 
  { 
    putMsg("U copy construction"); 
  }
  U& operator=(const U& u) 
  { 
    putMsg("U assignment"); 
    return *this;
  }
  U(U&& u) 
  { 
    putMsg("U move construction"); 
  }
  U& operator=(U&& u) 
  { 
    putMsg("U move assignment"); 
    return *this;
  }
  ~U() 
  { 
    putMsg("U destruction"); 
  }
};
///////////////////////////////////////////////////////////////////////
// D class 
// - D derives from B.
// - There are no virtual functions because our goal is to 
//   illustrate operations that occur when a compound object
//   is created, assigned, and destroyed.
/*
*  D's constructors, as their first act, invoke a B constructor.  They
*  don't execute their bodies until B's construction completes.
*/
class D : public B
{
public:
  D() 
  { 
    putMsg("D default construction"); 
  }
  D(const D& d) : B(d) 
  { 
    putMsg("D copy construction"); 
  }
  D& operator=(const D& d) 
  { 
    putMsg("D assignment"); 
    B::operator=(d); 
    return *this;
  }
  D(D&& d) : B(std::move(d)) 
  { 
    putMsg("D move construction"); 
  }
  D& operator=(D&& d) 
  { 
    putMsg("D move assignment");
    if (this == &d)
      return *this;
    B::operator=(std::move(d)); 
    return *this;
  }
  virtual void g()
  {
    putMsg("Calling D::g()");
  }
  void f(U& u) 
  { 
    putMsg("D using U"); 
  }
  ~D() 
  { 
    putMsg("D destruction"); 
  }
};

///////////////////////////////////////////////////////////////////////
// global function testFunction
// - Illustrates move construction from temporary objects.

D testFunction()
{
  Utilities::title("Running testFunction");
  D d;
  return d;  // d will be moved, not copied
}

///////////////////////////////////////////////////////////////////////
// Test stub
// - You should experiment with this and with the parts provided
//   for the classes.
// - What do you think will happen if you comment out the move 
//   operations?

int main()
{
  Utilities::title("Demonstrating Operation of Compound Object", '=');
  U u;
  D d;
  d.f(u);
  d = testFunction();

  Utilities::title("starting copy construction");
  D d2 = d;

  Utilities::title("starting move construction");
  D d3 = std::move(d);
  // d is now invalid

  Utilities::title("Demonstrating polymorphism");
  B* ptr = new B();
  ptr->g();
  ptr = &d;
  ptr->g();

  Utilities::title("leaving main's scope");
}
