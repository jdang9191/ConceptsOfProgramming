-- Challenge 1: The functional counterpart to the Visitor pattern.
--
-- Each type gets one instance containing all operations for that type.
-- Adding a new type means defining another instance, without changing
-- the operations already defined for other types.

class Expression expression where
  evaluate :: expression -> Double
  printExpr :: expression -> String

data Literal = Literal Double

instance Expression Literal where
  evaluate (Literal value) =
    value

  printExpr (Literal value) =
    show value

data Negation = Negation Literal

instance Expression Negation where
  evaluate (Negation expression) =
    -(evaluate expression)

  printExpr (Negation expression) =
    "(-" ++ printExpr expression ++ ")"

data Addition = Addition Literal Literal

instance Expression Addition where
  evaluate (Addition left right) =
    evaluate left + evaluate right

  printExpr (Addition left right) =
    "(" ++ printExpr left ++ " + " ++ printExpr right ++ ")"

main :: IO ()
main = do
  let literal = Literal 5
  let negation = Negation literal
  let addition = Addition (Literal 1) (Literal 2)

  putStrLn ("Literal: " ++ printExpr literal)
  putStrLn ("Negation: " ++ printExpr negation)
  putStrLn ("Addition: " ++ printExpr addition)
  putStrLn ("Addition result: " ++ show (evaluate addition))
