
//
// Created by Aaron Albright on 12/6/22.

#ifndef QUANTLIB_RATES_H
#define QUANTLIB_RATES_H

#include <iostream>
#include "ql/currencies/america.hpp"
#include "ql/settings.hpp"
#include "ql/termstructures/yield/ratehelpers.hpp"
#include "ql/termstructures/yield/piecewiseyieldcurve.hpp"
#include "ql/time/all.hpp"

using namespace QuantLib;


struct Data{
    Integer 	n		;
    TimeUnit 	units	;
    Rate 		rate	;
};


int market(){
    Calendar calendar = UnitedStates(QuantLib::UnitedStates::NYSE);
    const Natural settlementDays = 2;
    const Date today = Date::todaysDate();

    Settings::instance().evaluationDate() = today;
    Date settlement = calendar.advance(today, settlementDays, Days);

    std::vector<Data> depositData ={
            { 1, Months, 0.04581 },
            { 2, Months, 0.04573 },
            { 3, Months, 0.04557 },
            { 6, Months, 0.04496 },
            { 9, Months, 0.04490 }};

    std::vector<Data> swapData ={
            { 1, Years, 0.0454 },
            {  5, Years, 0.0499 },
            { 10, Years, 0.0547 },
            { 20, Years, 0.0589 },
            { 30, Years, 0.0596 }};

    std::vector<boost::shared_ptr<RateHelper>> instruments;

    for(auto & a : depositData){
        instruments.push_back(
            boost::shared_ptr<RateHelper>(
                new DepositRateHelper(
                    a.rate,
                    a.n * a.units,
                    settlementDays, calendar,
                    ModifiedFollowing, true,
                    Actual360())));
    }
    // Needs to be passed as paramter to the rate helper
    boost::shared_ptr<IborIndex> index(
        new IborIndex("US libor 3M",
                      3 * Months,
                      settlementDays,
                      USDCurrency(),
                      calendar,
                      ModifiedFollowing,
                      false,
                      Actual360()));

    index->addFixing(settlement, depositData[2].rate);

    for(auto & b : swapData)
    {
        instruments.push_back(
            boost::shared_ptr<RateHelper>(
                new SwapRateHelper(
                    b.rate,
                    b.n * b.units,
                    calendar, Semiannual,
                    Unadjusted,Actual360(),index)));
    }
    //dates form term
    auto c = boost::shared_ptr < YieldTermStructure>(
        new PiecewiseYieldCurve < ForwardRate,Cubic>(
            settlement, instruments, Actual360()));

    for(int i = 0; i < 30; i++)
    {
        Date e = settlement + Period(i, Years);
        //c is the date of term
        std::cout
            << "On " << c
            << ", zero rate is "
            << c->zeroRate(e, Actual365Fixed(), Continuous)
            <<  std::endl;
    }
    for(int i = 0; i < 25; i++)
    {
        Date e = settlement + Period(i, Months);

        std::cout
            << "On " << c
            << ", zero rate is "
            << c->zeroRate(e, Actual365Fixed(), Continuous)
            <<  std::endl;
    }

    return 0;
}



#endif // QUANTLIB_RATES_H
