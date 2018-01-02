#ifndef __Q3_H__
#define __Q3_H__

_Coroutine StringLiteral {
    // YOU ADD MEMBERS HERE
    char ch;
    string readHistory;
    bool isQuote();
    void prefixCheck();
    bool isCharSimpleEscapeSequence();
    bool isCharOct();
    bool isCharHex();
    void escapeSequenceCheck();
    void sCharCheck();
    void main();                        // coroutine main
  public:
    StringLiteral();
    ~StringLiteral();
    _Event Match {};                    // last character match
    _Event Error {};                    // last character invalid
    void next( char c );
};

#endif
