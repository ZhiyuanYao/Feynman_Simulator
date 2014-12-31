//
//  state.cpp
//  Feynman_Simulator
//
//  Created by Kun Chen on 11/2/14.
//  Copyright (c) 2014 Kun Chen. All rights reserved.
//

#include "parameter.h"
#include "utility/utility.h"
#include "utility/dictionary.h"
using namespace para;


Message Parameter::GenerateMessage()
{
    Message Message_;
    Message_.Version = Version;
    Message_.Beta = Beta;
    return Message_;
}

void Parameter::UpdateWithMessage(const Message& Message_)
{
    Version = Message_.Version;
    Beta = Message_.Beta;
    T = 1.0 / Beta;
}

bool Parameter::_FromDict(const Dictionary& _para)
{
    GET(_para, L);
    GET(_para, InitialBeta);
    GET(_para, DeltaBeta);
    GET(_para, FinalBeta);
    GET(_para, Order);
    GET(_para, NSublat);
    _para.HasKey("Beta") ? GET(_para, Beta) : Beta = InitialBeta;
    _para.HasKey("Version") ? GET(_para, Version) : Version = 0;

    Lat.Initialize(L, NSublat);
    T = 1.0 / Beta;

    ASSERT_ALLWAYS(Beta >= InitialBeta && Beta <= FinalBeta, "Beta should be between Initial and Final Beta");
    ASSERT_ALLWAYS(Order < MAX_ORDER, "Order can not be bigger than " << MAX_ORDER);
    return true;
}
Dictionary Parameter::_ToDict()
{
    Dictionary _para;
    SET(_para, L);
    SET(_para, InitialBeta);
    SET(_para, DeltaBeta);
    SET(_para, FinalBeta);
    SET(_para, Order);
    SET(_para, NSublat);
    SET(_para, Beta);
    SET(_para, Version);
    return _para;
}

bool Parameter::_BuildNew(const std::string& InputFile)
{
    Dictionary _para;
    _para.Load(InputFile);
    GET(_para, L);
    GET(_para, InitialBeta);
    GET(_para, DeltaBeta);
    GET(_para, FinalBeta);
    GET(_para, Order);
    GET(_para, NSublat);

    Lat.Initialize(L, NSublat);
    Version = 0;
    Beta = InitialBeta;
    T = 1.0 / Beta;
    if (Order >= MAX_ORDER)
        ABORT("Order can not be bigger than " << MAX_ORDER);
    return true;
}

bool ParaMC::BuildNew(const std::string& InputFile)
{
    Parameter::_BuildNew(InputFile);
    Dictionary _para;
    _para.Load(InputFile);
    GET(_para, Toss);
    GET(_para, Sample);
    GET(_para, Sweep);
    GET(_para, Seed);
    GET(_para, WormSpaceReweight);
    GET(_para, OrderReWeight);
    GET(_para, MaxTauBin);

    ASSERT_ALLWAYS(OrderReWeight.size() == Order + 1, "OrderReWeight should have Order+1 elementes!");
    Counter = 0;
    this->RNG.Reset(Seed);
    return true;
}

bool ParaMC::FromDict(const Dictionary& _para)
{
    Parameter::_FromDict(_para);
    GET(_para, Toss);
    GET(_para, Sample);
    GET(_para, Sweep);
    GET(_para, WormSpaceReweight);
    GET(_para, OrderReWeight);
    GET(_para, MaxTauBin);
    _para.HasKey("Counter") ? GET(_para, Counter) : Counter = 0;
    _para.HasKey("Seed") ? GET(_para, Seed) : Seed = 0;
    if (_para.HasKey("RNG"))
        GET(_para, RNG);
    else
        RNG.Reset(Seed);
    ASSERT_ALLWAYS(OrderReWeight.size() == Order + 1, "OrderReWeight should have Order+1 elementes!");
    return true;
}
Dictionary ParaMC::ToDict()
{
    Dictionary _para;
    SET(_para, Toss);
    SET(_para, Sample);
    SET(_para, Sweep);
    SET(_para, WormSpaceReweight);
    SET(_para, OrderReWeight);
    SET(_para, MaxTauBin);
    SET(_para, Counter);
    SET(_para, RNG);
    _para.Update(Parameter::_ToDict());
    return _para;
}

void ParaMC::SetTest()
{
    Version = 0;
    int size[2] = { 8, 8 };
    NSublat = 2;
    L = Vec<int>(size);
    Lat = Lattice(L, NSublat);
    InitialBeta = 1.0;
    DeltaBeta = 0.0;
    FinalBeta = 1.0;
    Beta = 1.0;
    Order = 1;
    OrderReWeight = { 1, 1};
    Toss = 10000;
    Sample = 5000000;
    Seed = 519180543;
    WormSpaceReweight = 0.1;
    T = 1.0 / Beta;
    Counter = 0;
    MaxTauBin = 32;
}
