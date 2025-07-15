#pragma once

class ExprStatement;
class PrintStatement;
class VarStatement;

class IStatementVisitor {
public:
  virtual void visit(const ExprStatement &statement) = 0;
  virtual void visit(const PrintStatement &statement) = 0;
  virtual void visit(const VarStatement &statement) = 0;
};