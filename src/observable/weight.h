//
//  observable.h
//  Feynman_Simulator
//
//  Created by Kun Chen on 10/13/14.
//  Copyright (c) 2014 Kun Chen. All rights reserved.
//

#ifndef __Feynman_Simulator__observable__
#define __Feynman_Simulator__observable__

#include "estimate.h"
#include "../utility/array.h"
#include "../lattice/lattice.h"

namespace Weight {

class WeightNoMeasure {
  protected:
    WeightNoMeasure(const Lattice &, real Beta, int Order, int SpinVol, std::string);
    std::string _Name;
    real _Beta;
    real _dBeta; //_Beta/MAX_TAU
    real _dBetaInverse;
    int _Order;
    Lattice _Lat;
    unsigned int _Shape[5];
    Array::array4<Complex> _Weight;

    int SpinIndex(spin SpinIn, spin SpinOut);
    int SpinIndex(spin *TwoSpinIn, spin *TwoSpinOut);
    int TauToBin(real tau);
    real BinToTau(int Bin);
    const int MAX_BIN = 128;
    enum Dim { ORDER,
               SP,
               SUB,
               VOL,
               TAU };

  public:
    void Reset(real Beta);
    void InitializeState();
    void Save(const std::string &FileName, std::string Mode = "a");
    bool Load(const std::string &);
};

class WeightNeedMeasure : public WeightNoMeasure {
  protected:
    real _Norm;
    Array::array5<Complex> _WeightAccu;
    EstimatorBundle<Complex> _Average;

  public:
    WeightNeedMeasure(const Lattice &, real Beta, int order, int Spine, std::string);

    Estimate<Complex> WeightWithError(int order);

    int OrderAcceptable(int StartFromOrder, real ErrorThreshold);
    void UpdateWeight(int UpToOrder);

    void AddStatistics();
    void ClearStatistics();
    void SqueezeStatistics(real factor);
    //    std::string PrettyString();
    void Save(const std::string &FileName, std::string Mode = "a");
    bool Load(const std::string &);
};

//TODO: Add fitting function here
class Sigma : public WeightNeedMeasure {
  public:
    Sigma(const Lattice &, real Beta, int order);
    Complex Weight(const Site &, const Site &, real, real, spin, spin);
    Complex WeightOfDelta(spin, spin);
    void MeasureNorm(real weight);
    void Measure(const Site &, const Site &, real, real, spin, spin, int Order, const Complex &);
};

class Polar : public WeightNeedMeasure {
  public:
    Polar(const Lattice &, real Beta, int order);
    Complex Weight(const Site &, const Site &, real, real, spin *, spin *);
    void Measure(const Site &, const Site &, real, real, spin *, spin *, int Order, const Complex &);
};

class G : public WeightNoMeasure {
  public:
    G(const Lattice &, real Beta, int order);
    Complex Weight(const Site &, const Site &, real, real, spin, spin);
    Complex BareWeight(const Site &, const Site &, real, real, spin, spin);
};

class W : public WeightNoMeasure {
  public:
    W(const Lattice &, real Beta, int order);
    Complex Weight(const Site &, const Site &, real, real, spin *, spin *, bool);
    Complex WeightOfDelta(const Site &, const Site &, spin *, spin *, bool);
    Complex BareWeight(const Site &, const Site &, real, real, spin *, spin *);
};

class Worm {
  public:
    inline real Weight(const Site &, const Site &, real, real)
    {
        return 1.0;
    }
};
}

int TestObservable();
#endif /* defined(__Feynman_Simulator__observable__) */
