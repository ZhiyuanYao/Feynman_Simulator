//
//  measure.cpp
//  Feynman_Simulator
//
//  Created by Kun Chen on 10/19/14.
//  Copyright (c) 2014 Kun Chen. All rights reserved.
//

#include "markov_monitor.h"
#include "module/diagram/diagram.h"
#include "module/parameter/parameter.h"
#include "module/weight/weight.h"
#include "module/weight/component.h"
#include "utility/dictionary.h"

using namespace std;
using namespace diag;
using namespace para;
using namespace mc;

MarkovMonitor::MarkovMonitor()
{
    cEstimator.AddEstimator("1");
    rEstimator.AddEstimator("1");
}

bool MarkovMonitor::BuildNew(ParaMC& para, Diagram& diag, weight::Weight& weight)
{
    Para = &para;
    Diag = &diag;
    Weight = &weight;
    cEstimator.ClearStatistics();
    rEstimator.ClearStatistics();
    //TODO: more observables
    return true;
}

void MarkovMonitor::ReWeight()
{
    //TODO: reweight Estimators here
}

bool MarkovMonitor::FromDict(const Dictionary& dict, ParaMC& para, Diagram& diag, weight::Weight& weight)
{
    Para = &para;
    Diag = &diag;
    Weight = &weight;
    //TODO: more observables
    return cEstimator.FromDict(dict.Get<Dictionary>("cEstimator"))
           || rEstimator.FromDict(dict.Get<Dictionary>("rEstimator"));
}
Dictionary MarkovMonitor::ToDict()
{
    Dictionary dict;
    dict["cEstimator"] = cEstimator.ToDict();
    dict["rEstimator"] = rEstimator.ToDict();
    //TODO: more observables
    return dict;
}

void MarkovMonitor::Annealing()
{
    SqueezeStatistics();
}

void MarkovMonitor::SqueezeStatistics()
{
}

void MarkovMonitor::ReWeightEachOrder()
{
}

void MarkovMonitor::Measure()
{
    //    cEstimator[0].Measure(<#const Complex &#>);
    //    cEstimator["1"].Measure(<#const Complex &#>);
    //    Weight->Sigma->Measure(
}

void MarkovMonitor::AddStatistics()
{
}