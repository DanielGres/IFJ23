.IFJcode23
DEFVAR GF@mylet
MOVE GF@mylet somevar
EXPRESSION GF@var
JUMPIFNEQ IF0 GF@var bool@true
DEFVAR GF@mylet
MOVE GF@mylet somevar
JUMP END0
LABEL IF0
DEFVAR GF@mylet
MOVE GF@mylet somevar
LABEL END0
EXIT int@0
