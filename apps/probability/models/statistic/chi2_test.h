#ifndef PROBABILITY_MODELS_STATISTIC_CHI2_TEST_H
#define PROBABILITY_MODELS_STATISTIC_CHI2_TEST_H

#include <poincare/horizontal_layout.h>

#include <cmath>

#include "interfaces/distributions.h"
#include "test.h"

namespace Probability {

class Chi2Test : public Test {
public:
  Chi2Test();

  SignificanceTestType significanceTestType() const override { return SignificanceTestType::Categorical; }
  DistributionType distributionType() const override { return DistributionType::Chi2; }
  bool initializeCategoricalType(CategoricalType type);

  Poincare::Layout testCriticalValueSymbol(const KDFont * font = KDFont::LargeFont) override {
    return DistributionChi2::TestCriticalValueSymbol(font);
  }

  float canonicalDensityFunction(float x) const override {
    return DistributionChi2::CanonicalDensityFunction(x, m_degreesOfFreedom);
  }
  double cumulativeDistributiveFunctionAtAbscissa(double x) const override {
    return DistributionChi2::CumulativeNormalizedDistributionFunction(x, m_degreesOfFreedom);
  }
  double cumulativeDistributiveInverseForProbability(double * proba) override {
    return DistributionChi2::CumulativeNormalizedInverseDistributionFunction(*proba, m_degreesOfFreedom);
  }
  int indexOfDegreeOfFreedom() const { return indexOfThreshold() + 1; }

  // Matrix special functions
  virtual void setParameterAtPosition(double value, int row, int column) = 0;
  void setParameterAtIndex(double p, int index) override;  // Hidden
  virtual double parameterAtPosition(int row, int column) const = 0;
  virtual bool authorizedParameterAtPosition(double p, int row, int column) const = 0;
  bool authorizedParameterAtIndex(double p, int i) const override;
  /* Delete parameter at location, return true if the deleted param was the last
   * non-deleted value of its row or column. */
  virtual bool deleteParameterAtPosition(int row, int column) = 0;
  virtual void recomputeData() = 0;
  virtual int maxNumberOfColumns() const = 0;
  virtual int maxNumberOfRows() const = 0;

  constexpr static float k_undefinedValue = NAN;

protected:
  using Test::parameterAtIndex;  // Hidden

  // Instanciate unused abstract classs
  void initParameters() override {}
  ParameterRepresentation paramRepresentationAtIndex(int i) const override {
    return ParameterRepresentation{Poincare::HorizontalLayout::Builder(), I18n::Message::Default};
  }

  // Chi2 specific
  virtual double expectedValue(int index) const = 0;
  virtual double observedValue(int index) const = 0;
  virtual int numberOfValuePairs() const = 0;
  double computeChi2();

private:
  float computeXMin() const override { return DistributionChi2::XMin(m_degreesOfFreedom); }
  float computeXMax() const override { return DistributionChi2::XMax(m_degreesOfFreedom); }
  float computeYMax() const override {
    return DistributionChi2::YMax(m_degreesOfFreedom);
  }
};

}  // namespace Probability

#endif /* PROBABILITY_MODELS_STATISTIC_CHI2_TEST_H */