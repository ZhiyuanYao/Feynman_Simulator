//
//  weight_inheritance.h
//  Feynman_Simulator
//
//  Created by Kun Chen on 11/7/14.
//  Copyright (c) 2014 Kun Chen. All rights reserved.
//

#ifndef Feynman_Simulator_weight_inheritance_h
#define Feynman_Simulator_weight_inheritance_h
#include "weight_estimator.h"

namespace weight {

//TODO: Add fitting function here
class Sigma : public WeightNeedMeasure {
  public:
    Sigma(const Lattice &, real Beta, int order, bool IsTauSymmetric = false);
    //Monte Carlo interface
    Complex Weight(const Site &, const Site &, real, real, spin, spin);
    Complex WeightOfDelta(spin, spin);
    void Measure(const Site &, const Site &, real, real, spin, spin, int Order, const Complex &);
    //Dyson interface
    void FFT(fft::Dir, Mode);
};

class Polar : public WeightNeedMeasure {
  public:
    Polar(const Lattice &, real Beta, int order);
    //Monte Carlo interface
    Complex Weight(const Site &, const Site &, real, real, spin *, spin *);
    void Measure(const Site &, const Site &, real, real, spin *, spin *, int Order, const Complex &);
    //Dyson interface
    void FFT(fft::Dir, Mode);
};
class G : public WeightNoMeasure {
  public:
    G(const Lattice &, real Beta, int order, bool IsTauSymmetric = false);
    Array::array4<Complex> BareWeight;
    //Monte Carlo interface
    Complex Weight(const Site &, const Site &, real, real, spin, spin, bool);
    Complex Weight(int, const Site &, const Site &, real, real, spin, spin, bool);
    void StartWithBare();
    //Dyson interface
    void FFT(fft::Dir, Mode);

  protected:
    void _InitialBare();
};

/**
*  W is the interaction. An assumption is made here: translational and \emp{MIRROR} symmetry of the lattice (constructed by unit cells) are imposed on interaction.
    The mirror symmetry is only required on the level of the whole lattice, not within a unit cell.
*/
class W : public WeightNoMeasure {
  public:
    W(const Lattice &, real Beta, int order);
    Array::array3<Complex> BareWeight;
    //Monte Carlo interface
    Complex Weight(const Site &, const Site &, real, real, spin *, spin *, bool, bool, bool);
    Complex Weight(int, const Site &, const Site &, real, real, spin *, spin *, bool, bool, bool);
    void StartWithBare();
    //Dyson interface
    void FFT(fft::Dir, Mode);
    //DEBUG interface
    void WriteBareToASCII();

  protected:
    void _InitialBare();
};

class Worm {
  public:
    static real Weight(const Site &, const Site &, real, real)
    {
        return 1.0;
    }
};

class Norm {
  public:
    static real Weight()
    {
        return 1.0;
    }
};
}

#endif
