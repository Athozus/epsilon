#ifndef POINCARE_LOGICAL_OPERATOR_H
#define POINCARE_LOGICAL_OPERATOR_H

#include <poincare/expression.h>

namespace Poincare {

class LogicalOperatorNode : public ExpressionNode {
public:
  template<typename T> static bool IsApproximativelyNotZero(T x);
  // not: 2, (and, nand): 1, (or, xor, nor): 0. -1 if not a logical operator type.
  static int LogicalOperatorTypePrecedence(const ExpressionNode * operatorExpression);

#if POINCARE_TREE_LOG
  void logNodeName(std::ostream & stream) const override {
    stream << "LogicalOperator";
  }
void logAttributes(std::ostream & stream) const override{
  stream << " operator=\"" << operatorName();
}
#endif
protected:
  constexpr static int k_sizeOfNameBuffer = 4 + 2 + 1; // "nand" + 2*" " + nullTermination
  virtual const char * operatorName() const = 0;
private:
  LayoutShape leftLayoutShape() const override { assert(false); return LayoutShape::BoundaryPunctuation; };
  bool childAtIndexNeedsUserParentheses(const Expression & child, int childIndex) const override;
};

// Not Operator

class NotOperatorNode : public LogicalOperatorNode {
public:
  constexpr static const char * k_name = "not";
  size_t size() const override { return sizeof(NotOperatorNode); }
  Type type() const override { return Type::NotOperator; }
  int numberOfChildren() const override { return 1; }

private:
  const char * operatorName() const override { return k_name; }

  // Layout
  Layout createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits, Context * context) const override;
  int serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;

  // Evaluation
  Evaluation<float> approximate(SinglePrecision p, const ApproximationContext& approximationContext) const override {
    return templatedApproximate<float>(approximationContext);
  }
  Evaluation<double> approximate(DoublePrecision p, const ApproximationContext& approximationContext) const override {
    return templatedApproximate<double>(approximationContext);
  }
  template<typename T> Evaluation<T> templatedApproximate(const ApproximationContext& approximationContext) const;

  // Reduction
  Expression shallowReduce(const ReductionContext& reductionContext) override;
};

class NotOperator : public ExpressionOneChild<NotOperator, NotOperatorNode> {
public:
  using ExpressionBuilder::ExpressionBuilder;
  Expression shallowReduce(const ExpressionNode::ReductionContext& reductionContext);
};

// Binary Logical Operator

class BinaryLogicalOperatorNode : public LogicalOperatorNode {
public:
  enum class OperatorType : uint8_t {
    And,
    Or,
    Xor,
    Nand,
    Nor,
    LengthOfEnum // Used to compute length of enum.
  };
  static bool IsBinaryLogicalOperator(const char * name, int nameLength, OperatorType * type);

  Type type() const override { return Type::BinaryLogicalOperator; }
  size_t size() const override { return sizeof(BinaryLogicalOperatorNode); }
  int numberOfChildren() const override { return 2; }

  OperatorType operatorType() const { return m_typeOfOperator; }
  void setOperatorType(OperatorType type) { m_typeOfOperator = type; }
  bool evaluate(bool a, bool b) const;

private:
  constexpr static int k_numberOfOperators = static_cast<int>(OperatorType::LengthOfEnum);
  struct OperatorName {
    OperatorType type;
    const char * name;
  };
  constexpr static OperatorName k_operatorNames[] = {
    {OperatorType::And, "and"},
    {OperatorType::Or, "or"},
    {OperatorType::Xor, "xor"},
    {OperatorType::Nand, "nand"},
    {OperatorType::Nor, "nor"}
  };
  static_assert(sizeof(k_operatorNames) / sizeof(OperatorName) == k_numberOfOperators, "Wrong number of binary logical operators");

  const char * operatorName() const override;

  // Layout
  Layout createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits, Context * context) const override;
  int serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;

  // Evaluation
  Evaluation<float> approximate(SinglePrecision p, const ApproximationContext& approximationContext) const override {
    return templatedApproximate<float>(approximationContext);
  }
  Evaluation<double> approximate(DoublePrecision p, const ApproximationContext& approximationContext) const override {
    return templatedApproximate<double>(approximationContext);
  }
  template<typename T> Evaluation<T> templatedApproximate(const ApproximationContext& approximationContext) const;

  // Reduction
  Expression shallowReduce(const ReductionContext& reductionContext) override;

  OperatorType m_typeOfOperator;
};

class BinaryLogicalOperator : public Expression {
public:
  BinaryLogicalOperator(const BinaryLogicalOperatorNode * n) : Expression(n) {}
  static BinaryLogicalOperator Builder(Expression firstChild, Expression secondChild, BinaryLogicalOperatorNode::OperatorType type);
  Expression shallowReduce(const ExpressionNode::ReductionContext& reductionContext);
};

}

#endif