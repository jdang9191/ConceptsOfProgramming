// Challenge 2: Define a visitor that converts an expression to RPN.

interface Expr {
  //each expression sends the visitor to its matching method.
  <R> R accept(Visitor<R> visitor);
}

interface Visitor<R> {
  //visitor method for each expression type.
  R visitLiteralExpr(Literal expr);
  R visitUnaryExpr(Unary expr);
  R visitBinaryExpr(Binary expr);
}

enum TokenType {
  PLUS,
  MINUS,
  STAR
}

class Token {
  final TokenType type;
  final String lexeme;
  final Object literal;
  final int line;

  Token(TokenType type, String lexeme, Object literal, int line) {
    this.type = type;
    this.lexeme = lexeme;
    this.literal = literal;
    this.line = line;
  }
}

class Literal implements Expr {
  final double value;

  Literal(double value) {
    this.value = value;
  }

  @Override
  public <R> R accept(Visitor<R> visitor) {
    // A literal dispatches to the literal visitor method.
    return visitor.visitLiteralExpr(this);
  }
}

class Unary implements Expr {
  final Token operator;
  final Expr right;

  Unary(Token operator, Expr right) {
    this.operator = operator;
    this.right = right;
  }

  @Override
  public <R> R accept(Visitor<R> visitor) {
    //unary expression dispatches to the unary visitor method.
    return visitor.visitUnaryExpr(this);
  }
}

class Binary implements Expr {
  final Expr left;
  final Token operator;
  final Expr right;

  Binary(Expr left, Token operator, Expr right) {
    this.left = left;
    this.operator = operator;
    this.right = right;
  }

  @Override
  public <R> R accept(Visitor<R> visitor) {
    // A binary expression dispatches to the binary visitor method.
    return visitor.visitBinaryExpr(this);
  }
}

class RpnPrinter implements Visitor<String> {
  @Override
  public String visitLiteralExpr(Literal expr) {
    //literals stay in the same position in RPN.
    return String.valueOf(expr.value);
  }

  @Override
  public String visitUnaryExpr(Unary expr) {
    // RPN places a unary operator after its operand.
    return expr.right.accept(this) + " " + expr.operator.lexeme;
  }

  @Override
  public String visitBinaryExpr(Binary expr) {
    //visit both operands before placing the binary operator.
    return expr.left.accept(this) + " "
        + expr.right.accept(this) + " "
        + expr.operator.lexeme;
  }
}

class Main {
  public static void main(String[] args) {
    //build the expression: (1 + 2) * (4 - 3).
    Expr expression = new Binary(
        new Binary(
            new Literal(1),
            new Token(TokenType.PLUS, "+", null, 1),
            new Literal(2)),
        new Token(TokenType.STAR, "*", null, 1),
        new Binary(
            new Literal(4),
            new Token(TokenType.MINUS, "-", null, 1),
            new Literal(3)));

    //recursively converts the whole tree to one string.
    RpnPrinter printer = new RpnPrinter();
    System.out.println(expression.accept(printer));
    //output: 1.0 2.0 + 4.0 3.0 - *
  }
}