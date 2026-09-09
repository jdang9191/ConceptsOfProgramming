
  private void scanToken() {
    char c = advance();
    switch (c) {
      case '(': addToken(LEFT_PAREN); break;
      case ')': addToken(RIGHT_PAREN); break;
      case '{': addToken(LEFT_BRACE); break;
      case '}': addToken(RIGHT_BRACE); break;
      case ',': addToken(COMMA); break;
      case '.': addToken(DOT); break;
      case '-': addToken(MINUS); break;
      case '+': addToken(PLUS); break;
      case ';': addToken(SEMICOLON); break;
      case '*': addToken(STAR); break;
      case '!':
        addToken(match('=') ? BANG_EQUAL : BANG);
        break;
      case '=':
        addToken(match('=') ? EQUAL_EQUAL : EQUAL);
        break;
      case '<':
        addToken(match('=') ? LESS_EQUAL : LESS);
        break;
      case '>':
        addToken(match('=') ? GREATER_EQUAL : GREATER);
        break;
      case '/':
        //check what type of slash this is
        if (match('/')) {
          //line comment: goes until end of line
          while (peek() != '\n' && !isAtEnd()) advance();
        } else if (match('*')) {
          //block comment
          blockComment();
        } else {
          addToken(SLASH);
        }
        break;
      case ' ':
      case '\r':
      case '\t':
        break;
      case '\n':
        line++;
        break;
      case '"': string(); break;
      default:
        if (isDigit(c)) {
          number();
        } else if (isAlpha(c)) {
          identifier();
        } else {
          Lox.error(line, "Unexpected character.");
        }
        break;
    }
  }

  //function to handle block comments
  private void blockComment() {
    while (!isAtEnd()) {
      if (peek() == '*' && peekNext() == '/') {
        //found the closing */ of the block comment
        advance();  //consume *
        advance();  //consume /
        return;
      }
      
      //check for newlines to track line numbers
      if (peek() == '\n') line++;
      advance();
    }

    //the comment was never closed
    Lox.error(line, "Unterminated block comment.");
  }
