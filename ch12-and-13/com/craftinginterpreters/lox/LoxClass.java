//> Classes lox-class
package com.craftinginterpreters.lox;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;

/* Classes lox-class < Classes lox-class-callable
class LoxClass {
*/
//> lox-class-callable
class LoxClass extends LoxInstance implements LoxCallable {
//< lox-class-callable
  final String name;
//> Inheritance lox-class-superclass-field
  final LoxClass superclass;
//< Inheritance lox-class-superclass-field
/* Classes lox-class < Classes lox-class-methods

  LoxClass(String name) {
    this.name = name;
  }
*/
//> lox-class-methods
  private final Map<String, LoxFunction> methods;

/* Classes lox-class-methods < Inheritance lox-class-constructor
  LoxClass(String name, Map<String, LoxFunction> methods) {
*/
//> Inheritance lox-class-constructor
  LoxClass(LoxClass metaclass, String name, LoxClass superclass,
           Map<String, LoxFunction> methods) {
    super(metaclass);
    this.superclass = superclass;
//< Inheritance lox-class-constructor
    this.name = name;
    this.methods = methods;
  }
//< lox-class-methods
//> lox-class-find-method
  LoxFunction findMethod(LoxInstance instance, String name) {
    List<LoxFunction> chain = new ArrayList<>();
    LoxClass current = this;

    // Keep every override so each bound method can point at the next one.
    while (current != null) {
      if (current.methods.containsKey(name)) {
        chain.add(current.methods.get(name));
      }
      current = current.superclass;
    }

    LoxFunction next = null;
    for (LoxFunction method : chain) {
      next = method.bind(instance, next);
    }

    return next;
  }
//< lox-class-find-method

  @Override
  public String toString() {
    return name;
  }
//> lox-class-call-arity
  @Override
  public Object call(Interpreter interpreter,
                     List<Object> arguments) {
    LoxInstance instance = new LoxInstance(this);
//> lox-class-call-initializer
    LoxFunction initializer = findMethod(instance, "init");
    if (initializer != null) {
      initializer.bind(instance).call(interpreter, arguments);
    }

//< lox-class-call-initializer
    return instance;
  }

  @Override
  public int arity() {
/* Classes lox-class-call-arity < Classes lox-initializer-arity
    return 0;
*/
//> lox-initializer-arity
    LoxFunction initializer = findMethod(new LoxInstance(this), "init");
    if (initializer == null) return 0;
    return initializer.arity();
//< lox-initializer-arity
  }
//< lox-class-call-arity
}
