#ifndef PROBABILITY_MODELS_STATISTIC_ONE_MEAN_Z_INTERVAL_H
#define PROBABILITY_MODELS_STATISTIC_ONE_MEAN_Z_INTERVAL_H

#include "interfaces/distributions.h"
#include "interfaces/significance_tests.h"
#include "interval.h"

namespace Probability {

class OneMeanZInterval : public Interval {
public:
  SignificanceTestType significanceTestType() const override { return SignificanceTestType::OneMean; }
  DistributionType distributionType() const override { return DistributionType::Z; }
  I18n::Message title() const override { return OneMean::ZTitle(); }
  // Significance Test: OneMean
  bool initializeDistribution(DistributionType distributionType) override { return OneMean::IntervalInitializeDistribution(this, distributionType); }
  int numberOfAvailableDistributions() const override { return OneMean::NumberOfAvailableDistributions(); }
  I18n::Message distributionTitle() const override { return OneMean::DistributionTitle(); }
  I18n::Message distributionDescription() const override { return OneMean::IntervalDistributionDescription(); }
  void initParameters() override { OneMean::InitZIntervalParameters(this); }
  bool authorizedParameterAtIndex(double p, int i) const override { return OneMean::ZAuthorizedParameterAtIndex(i, p); }
  void setParameterAtIndex(double p, int index) override {
    p = OneMean::ProcessParamaterForIndex(p, index);
    Interval::setParameterAtIndex(p, index);
  }
  void computeInterval() override { OneMean::ComputeZInterval(this); }
  const char * estimateSymbol() const override { return OneMean::EstimateSymbol(); }
  Poincare::Layout testCriticalValueSymbol(const KDFont * font = KDFont::LargeFont) override { return DistributionZ::TestCriticalValueSymbol(font); }

  // Distribution: z
  float canonicalDensityFunction(float x) const override { return DistributionZ::CanonicalDensityFunction(x, m_degreesOfFreedom); }
  double cumulativeDistributiveFunctionAtAbscissa(double x) const override { return DistributionZ::CumulativeNormalizedDistributionFunction(x, m_degreesOfFreedom); }
  double cumulativeDistributiveInverseForProbability(double * p) override { return DistributionZ::CumulativeNormalizedInverseDistributionFunction(*p, m_degreesOfFreedom); }

private:
  // Significance Test::OneMean
  int numberOfStatisticParameters() const override { return OneMean::NumberOfParameters(); }
  ParameterRepresentation paramRepresentationAtIndex(int i) const override { return OneMean::ParameterRepresentationAtIndex(i); }
  double * parametersArray() override { return m_params; }
  // Distribution: z
  float computeYMax() const override { return DistributionZ::YMax(m_degreesOfFreedom); }

  double m_params[OneMean::k_numberOfParams];
};

}  // namespace Probability

#endif /* PROBABILITY_MODELS_STATISTIC_ONE_MEAN_Z_INTERVAL_H */