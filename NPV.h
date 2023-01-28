
#ifndef QUANTLIB_NPV_H
#define QUANTLIB_NPV_H
#include "ql/instruments/vanillaoption.hpp"
#include "ql/math/integrals/tanhsinhintegral.hpp"
#include "ql/pricingengines/vanilla/analyticeuropeanengine.hpp"
#include "ql/pricingengines/vanilla/baroneadesiwhaleyengine.hpp"
#include "ql/pricingengines/vanilla/binomialengine.hpp"
#include "ql/pricingengines/vanilla/bjerksundstenslandengine.hpp"
#include "ql/pricingengines/vanilla/fdblackscholesvanillaengine.hpp"
#include "ql/pricingengines/vanilla/mcamericanengine.hpp"
#include "ql/pricingengines/vanilla/mceuropeanengine.hpp"
#include "ql/pricingengines/vanilla/qdfpamericanengine.hpp"
#include "ql/time/calendars/target.hpp"
#include "ql/utilities/dataformatters.hpp"

#include <iostream>
#include <iomanip>
#include <string>


using namespace QuantLib;




int NPV() {

        Calendar calendar = TARGET();
        Date todaysDate(7, December, 2022);
        Date settlementDate(9, December, 2022);
        Settings::instance().evaluationDate() = todaysDate;

        // option details
        Option::Type type(Option::Call);
        Real underlying = 16.50;
        Real strike = 17;
        Spread dividendYield = 0.00;
        Rate riskFreeRate = 0.06;
        Volatility volatility = 0.3672;
        Date maturity(16, December, 2023);
        DayCounter dayCounter = Actual365Fixed();
        std::string method;

        std::cout << "Option type = " << type << std::endl;
        std::cout << "Maturity = " << maturity << std::endl;
        std::cout << "Underlying price = " << underlying << std::endl;
        std::cout << "Strike = " << strike << std::endl;
        std::cout << "Risk-free interest rate = " << io::rate(riskFreeRate) << std::endl;
        std::cout << "Dividend yield = " << io::rate(dividendYield) << std::endl;
        std::cout << "Volatility = " << io::volatility(volatility) << std::endl;



        ext::shared_ptr<Exercise> americanExercise(new AmericanExercise(settlementDate, maturity));

        Handle<Quote> underlyingH(ext::shared_ptr<Quote>(new SimpleQuote(underlying)));

        Handle<YieldTermStructure> flatTermStructure(ext::shared_ptr<YieldTermStructure>(
            new FlatForward(settlementDate, riskFreeRate, dayCounter)));
        Handle<YieldTermStructure> flatDividendTS(ext::shared_ptr<YieldTermStructure>(
            new FlatForward(settlementDate, dividendYield, dayCounter)));
        Handle<BlackVolTermStructure> flatVolTS(ext::shared_ptr<BlackVolTermStructure>(
            new BlackConstantVol(settlementDate, calendar, volatility, dayCounter)));
        ext::shared_ptr<StrikedTypePayoff> payoff(new PlainVanillaPayoff(type, strike));
        ext::shared_ptr<BlackScholesMertonProcess> bsmProcess(new BlackScholesMertonProcess(
            underlyingH, flatDividendTS, flatTermStructure, flatVolTS));

        VanillaOption americanOption(payoff, americanExercise);

        method = "Barone-Adesi/Whaley";
        americanOption.setPricingEngine(
            ext::shared_ptr<PricingEngine>(new BaroneAdesiWhaleyApproximationEngine(bsmProcess)));
        std::cout << method << americanOption.NPV() << std::endl;


        method = "Bjerksund/Stensland";
        americanOption.setPricingEngine(
            ext::shared_ptr<PricingEngine>(new BjerksundStenslandApproximationEngine(bsmProcess)));
        std::cout << method << americanOption.NPV() << std::endl;


        Size timeSteps = 900;

        method = "Binomial Jarrow-Rudd";

        americanOption.setPricingEngine(ext::shared_ptr<PricingEngine>(
            new BinomialVanillaEngine<JarrowRudd>(bsmProcess, timeSteps)));
        std::cout << method << americanOption.NPV() << std::endl;
        method = "Binomial Cox-Ross-Rubinstein";

        americanOption.setPricingEngine(ext::shared_ptr<PricingEngine>(
            new BinomialVanillaEngine<CoxRossRubinstein>(bsmProcess, timeSteps)));
        std::cout << method << americanOption.NPV() << std::endl;


        method = "Additive equiprobabilities";
        americanOption.setPricingEngine(ext::shared_ptr<PricingEngine>(
            new BinomialVanillaEngine<AdditiveEQPBinomialTree>(bsmProcess, timeSteps)));
        std::cout << method << americanOption.NPV() << std::endl;


        method = "Binomial Trigeorgis";
        americanOption.setPricingEngine(ext::shared_ptr<PricingEngine>(
            new BinomialVanillaEngine<Trigeorgis>(bsmProcess, timeSteps)));
        std::cout << method << americanOption.NPV() << std::endl;


        method = "Binomial Tian";
        americanOption.setPricingEngine(
            ext::shared_ptr<PricingEngine>(new BinomialVanillaEngine<Tian>(bsmProcess, timeSteps)));
        std::cout << method

                  << americanOption.NPV() << std::endl;


        method = "Binomial Leisen-Reimer";
        americanOption.setPricingEngine(ext::shared_ptr<PricingEngine>(
            new BinomialVanillaEngine<LeisenReimer>(bsmProcess, timeSteps)));
        std::cout << method << americanOption.NPV() << std::endl;


        method = "Binomial Joshi";
        americanOption.setPricingEngine(ext::shared_ptr<PricingEngine>(
            new BinomialVanillaEngine<Joshi4>(bsmProcess, timeSteps)));
        std::cout << method << americanOption.NPV() << std::endl;


   return 0;

#endif // QUANTLIB_NPV_H
}