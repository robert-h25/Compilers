#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//#include "lexer.h"
#include "parser.h"


// you can declare prototypes of parser functions below

//creating current and new file structures
//FileStruct currentfile, newfile;

//creating global ParserInfo
ParserInfo error;

void parserError(SyntaxErrors message, Token t);
int class();
int memberDeclar();
int classVarDeclar();
int type();
int subroutineDeclar();
int paramList();
int subroutineBody();
int statement();
int varDeclarStatement();
int letStatement();
int ifStatement();
int whileStatement();
int doStatement();
int subroutineCall();
int expressionList();
int returnStatement();
int expression();
int ArithmeticExpression();
int RelationalExpression();
int term();
int factor();
int operand();

//if we encounter a parser error
void parserError(SyntaxErrors message, Token t){
	//update SyntaxErrors er
	error.er = message; 
	//update token
	error.tk.tp = t.tp;
	strcpy(error.tk.lx,t.lx);
	error.tk.ln = t.ln;
	
	//exit(1);
}

// class -> classIdentifier{ {memberDeclar} }
int class(){
	// check first token.lx == class
	Token t = PeekNextToken();	
	if(strcmp(t.lx,"class")==0){
		t = GetNextToken();
		//printf("%s ",t.lx);
		// get classidentifier
		Token t1 = PeekNextToken();
		if(t1.tp!=ID){
			SyntaxErrors message = idExpected;
			parserError(message,t1);
			return 0;
		}
		t1=GetNextToken();
		//printf("%s ",t1.lx);

		// check for {
		t1 = PeekNextToken();		
		if (strcmp(t1.lx,"{")==0){
			t1=GetNextToken();
			//printf("%s ",t1.lx);
			Token t2 = PeekNextToken();
			while(strcmp(t2.lx,"}")!=0){
				// call memberDeclar
				if(memberDeclar()==0){
					return 0;
				}
				t2 = PeekNextToken();

			}
	
		}
		// else return error
		else{
			//printf("----{ expected----");
			SyntaxErrors message = openBraceExpected;
			parserError(message, t1);
			return 0;
		}
	}
	// else return error
	else{
		//printf("----class expected----\n");
		SyntaxErrors message = classExpected;
		parserError(message, t);
		return 0;
	}
	
	t = GetNextToken();
	//printf("%s ",t.lx);
	// check for }
	if (strcmp(t.lx,"}")==0){		
		// call memberDeclar
	}
	// else return error
	else{
		//printf("----} expected----");
		SyntaxErrors message = closeBraceExpected;
		parserError(message, t);
		return 0;
	}
	return 1;
}

// memberDeclar -> classVarDeclar | subroutineDeclar
int memberDeclar(){
	Token t = PeekNextToken();
	// check if we have a classVarDeclar or subroutineDeclar
	if((strcmp(t.lx,"static") == 0|| strcmp(t.lx,"field") == 0)){
		if(classVarDeclar()==0){
			return 0;
		}
	}
	else if((strcmp(t.lx,"constructor")==0||strcmp(t.lx,"function")==0||strcmp(t.lx,"method")==0)){
		if(subroutineDeclar()==0){
			return 0;
		}	
	}
	else{
		//printf("\nmemberDeclarErr\n");
		SyntaxErrors message = memberDeclarErr;
		parserError(message,t);
		return 0;
	}
	return 1;
}

// classVarDeclar -> (static | field) type identifier{,identifier};
int classVarDeclar(){
	// getting (static|field)
	Token t = GetNextToken();
	//printf("%s ",t.lx);
	// finding if we have a type next
	Token t1 = PeekNextToken();
	
	if(strcmp(t1.lx,"int") == 0 || strcmp(t1.lx,"char") == 0 || strcmp(t1.lx,"boolean") == 0 || t1.tp == ID){
		//printf("\n%s",t1.lx);
		type();
	}
	// else error
	else{
		//printf("\ntype expected1\n");
		SyntaxErrors message = illegalType;
		parserError(message,t1);		
		return 0;
	}

	//get identifier
	Token t2;
	t = GetNextToken();
	//printf("%s ",t.lx);
	if(t.tp == ID){
		t1 = PeekNextToken();		
		while(strcmp(t1.lx,",")==0){
			//consume ,
			t1 = GetNextToken();
			//printf("%s ",t1.lx);

			t2 = GetNextToken();
			//printf("%s ",t2.lx);
			if(t2.tp != ID){
				//printf("ID expected");
				SyntaxErrors message = idExpected;
				parserError(message,t2);	
				return 0;
			}
			t1=PeekNextToken();
		}
	}
	//check for ;
	t = GetNextToken();
	//printf("%s \n",t.lx);
	if (strcmp(t.lx,";")!=0){
		//printf("----; expected----");
		SyntaxErrors message = semicolonExpected;
		parserError(message,t);	
		return 0;
	}
	return 1;
}

// type -> int|char|boolean|identifier
int type(){
	Token t = GetNextToken();
	//printf("%s ",t.lx);
}

// subroutineDeclar -> (constructor|function|method) (type|void) idetifier (paramlist)subroutineBody
int subroutineDeclar(){
	// get (constructor|function|method) 
	Token t = GetNextToken();
	//printf("%s ",t.lx);
	// getting type
	Token t1 = PeekNextToken();
	if(strcmp(t1.lx,"int") == 0 || strcmp(t1.lx,"char") == 0 || strcmp(t1.lx,"boolean") == 0 || t1.tp == ID){
		//printf("\n%s",t1.lx);
		type();
	}
	else if (strcmp(t1.lx,"void")==0){
		//consume void
		t1 = GetNextToken();
		//printf("%s ",t1.lx);
	}
	// else error
	else{
		//printf("\ntype expected2\n");
		SyntaxErrors message = illegalType;
		parserError(message,t1);	
		return 0;
	}

	// get identifier
	t = GetNextToken();
	//printf("%s ",t.lx);

	// get paramList
	t = GetNextToken();
	//printf("%s ",t.lx);
	if (strcmp(t.lx,"(")==0){
		if(paramList()==0){
			return 0;
		}
		
	}
	else{
		//printf("\n ----( expected----\n");
		SyntaxErrors message = openParenExpected;
		parserError(message,t);	
		return 0;
	}

	t = GetNextToken();
	//rintf("%s ",t.lx);
	if (strcmp(t.lx,")")==0){
		if(subroutineBody()==0){
			return 0;
		}
	}
	else{
		//printf("\n %s----) expected----\n",t.lx);
		SyntaxErrors message = closeParenExpected;
		parserError(message,t);	
		return 0;
	}
	return 1;
}

// paramList -> type identifier {,type identifier} | e
int paramList(){
	// get type
	Token t1 = PeekNextToken();
	if(strcmp(t1.lx,"int") == 0 || strcmp(t1.lx,"char") == 0 || strcmp(t1.lx,"boolean") == 0 || t1.tp == ID){
		
		type();
	}
	// no parameters
	else if(strcmp(t1.lx,")")==0){
		
		return 1 ;
	}
	// no closeparen
	else if(strcmp(t1.lx,"{")==0){
		//printf("----) expected----");
		SyntaxErrors message = closeParenExpected;
		parserError(message,t1);	
		return 0;
	}
	// else error
	else{

		//printf("\ntype expected\n");
		SyntaxErrors message = illegalType;
		parserError(message,t1);	
		return 0;
	}

	//get identifier
	Token t = GetNextToken();
	//printf("%s ",t.lx);
	Token t2;
	//ensuring token is an ID
	if(t.tp == ID){
		// check if we have more params
		t1 = PeekNextToken();		
		while(strcmp(t1.lx,",")==0){
			//consume ,
			t1 = GetNextToken();
			//printf("%s ",t1.lx);

			//check type
			t1=PeekNextToken();			
			if(strcmp(t1.lx,"int") == 0 || strcmp(t1.lx,"char") == 0 || strcmp(t1.lx,"boolean") == 0 || t1.tp == ID){		
				type();
			}
			// check type is followed by an ID
			t2 = GetNextToken();
			//printf("%s ",t2.lx);
			if(t2.tp != ID){
				//printf("ID expected");
				SyntaxErrors message = idExpected;
				parserError(message,t2);	
				return 0;
			}
			t1=PeekNextToken();
		}
	}
	else{

		//printf("ID expected");
		SyntaxErrors message = idExpected;
		parserError(message,t);	
		return 0;
	}
	return 1;
}

// subroutineBody ->{ {statement} }
int subroutineBody(){
	Token t = PeekNextToken();
	//printf("%s ",t.lx);
	if (strcmp(t.lx,"{")==0){
		// consume {
		t = GetNextToken();
		Token t1 = PeekNextToken();
		while(strcmp(t1.lx,"}")!=0){
			// repeat until token = }			
			if(statement()==0){
				return 0;
			}
			t1=PeekNextToken();		
		}
		//consume }
		t1=GetNextToken();
		//printf("%s ",t1.lx);
	}
	// else return error
	else{
		//printf("----{ expected----");
		SyntaxErrors message = openBraceExpected;
		parserError(message,t);	
		return 0;
	}
	return 1;

}

// statement -> varDeclarStatement|letStatement|ifStatement|whileStatement|doStatement|returnStatement
int statement(){
	//find what token is next
	Token t = PeekNextToken();
	if (strcmp(t.lx,"var")==0){
		if(varDeclarStatement()==0){
			return 0;
		}
	}
	else if(strcmp(t.lx,"let")==0){
		if(letStatement()==0){
			return 0;
		}
	}
	else if(strcmp(t.lx,"if")==0){
		if(ifStatement()==0){
			return 0;
		}
	}
	else if(strcmp(t.lx,"while")==0){
		if(whileStatement()==0){
			return 0;
		}
	}
	else if(strcmp(t.lx,"do")==0){
		if(doStatement()==0){
			return 0;
		}
	}
	else if (strcmp(t.lx,"return")==0){
		if(returnStatement()==0){
			return 0;
		}
	}
	else{
		//error
		//printf("\nStatement expected\n");
		SyntaxErrors message = syntaxError;
		parserError(message,t);	
		return 0;
	}
	return 1;
}

// varDeclarStatement -> var type identifier{,identifier};
int varDeclarStatement(){
	// consume var
	Token t = GetNextToken();
	//printf("%s ",t.lx);
	
	// check for type
	t = GetNextToken();
	//printf("%s ",t.lx);
	if(strcmp(t.lx,"int") == 0 || strcmp(t.lx,"char") == 0 || strcmp(t.lx,"boolean") == 0 || t.tp == ID){
		//printf("\n%s",t1.lx);
		type();
	}

	// check for , then an identifier
	Token t1 = PeekNextToken();

	// if more than one paramaters
	while (strcmp(t1.lx,",")==0){
		// consume ,
		t1 = GetNextToken();
		//printf("%s ",t1.lx);
		//check next token is an ID
		Token t2 = PeekNextToken();
		if(t2.tp != ID){
			//printf("ID expected");
			SyntaxErrors message = idExpected;
			parserError(message,t2);	
			return 0;
		}	
		t2 = GetNextToken();
		//printf("%s ",t2.lx);	
		t1 = PeekNextToken();
	}

	// check for ;
	t = GetNextToken();
	//printf("%s ",t.lx);
	if (strcmp(t.lx,";")!=0){		
		//printf("----; expected----");
		SyntaxErrors message = semicolonExpected;
		parserError(message,t);	
		return 0;
	}
	return 1;
}

// letStatement -> let identifier [ [expression] ] = expression ;
int letStatement(){
	// consume let
	Token t = GetNextToken();
	//printf("%s ",t.lx);
	// get identifier
	t = GetNextToken();
	//printf("%s ",t.lx);
	if( t.tp == ID){
		
	}
	else{
		//printf("\n----ID expected----");
		SyntaxErrors message = idExpected;
		parserError(message,t);	
		return 0;
	}
	
	// check [
	t = PeekNextToken();
	
	if(strcmp(t.lx,"[")==0){
		//consume [
		t=GetNextToken();
		//printf("%s ",t.lx);
		// check if expression 
		expression();
		// get ]
		t = GetNextToken();
		//printf("%s ",t.lx);
		if (strcmp(t.lx,"]")!=0){
			//printf("\n----] expected----");
			SyntaxErrors message = closeBracketExpected;
			parserError(message,t);	
			return 0;
		}
	
	}	
	// get =
	t = PeekNextToken();
	
	if (strcmp(t.lx,"=")!=0){
		//printf("\n---- = expected ----");
		SyntaxErrors message = equalExpected;
		parserError(message,t);	
		return 0;
	}
	t=GetNextToken();
	//printf("%s ",t.lx);
	// get expression
	if(expression()==0){
		return 0;
	}
	
	// get ;
	t = GetNextToken();
	//printf("%s ",t.lx);
	if (strcmp(t.lx,";")!=0){
		//printf("----; expected----");
		SyntaxErrors message = semicolonExpected;
		parserError(message,t);
		return 0;	
	}
	return 1;
}

// ifStatement -> if (expression){ {statement} } [else{ {statement} }]
int ifStatement(){
	// consume if
	Token t = GetNextToken();
	//printf("%s ",t.lx);
	// get (
	t = GetNextToken();
	//printf("%s ",t.lx);
	if(strcmp(t.lx,"(")!=0){
		//printf("\n---- ( expected ----");
		SyntaxErrors message = openParenExpected;
		parserError(message,t);	
		return 0;
	}
	// get expression
	else{
		if(expression()==0){
			return 0;
		}
	}
	// get )
	t = GetNextToken();
	//printf("%s ",t.lx);
	if(strcmp(t.lx,")")!=0){
		//printf("\n---- ) expected ----");
		SyntaxErrors message = closeParenExpected;
		parserError(message,t);	
		return 0;
	}
	
	// get {
	t = GetNextToken();
	//printf("%s ",t.lx);
	if(strcmp(t.lx,"{")!= 0){
		//printf("\n---- { expected ----");
		SyntaxErrors message = openBraceExpected;
		parserError(message,t);	
		return 0;
	}
	else{
		// get statements
		Token t1 = PeekNextToken();
		while(strcmp(t1.lx,"}") !=0){
			if(statement()==0){
				return 0;
			}
			t1 =  PeekNextToken();
		}
			
	}
	// get }
	t = GetNextToken();
	//printf("%s ",t.lx);
	if(strcmp(t.lx,"}")!= 0){
		//printf("\n---- } expected ----");
		SyntaxErrors message = closeBraceExpected;
		parserError(message,t);	
		return 0;
	}
	
	// check for else
	t = PeekNextToken();	
	if(strcmp(t.lx,"else")==0){
		//consume else
		t = GetNextToken();
		//printf("%s ",t.lx);
		// get {
		t=PeekNextToken();
		if(strcmp(t.lx,"{")!= 0){
			//printf("\n---- { expected ----");
			SyntaxErrors message = openBraceExpected;
			parserError(message,t);	
			return 0;
		}
		else{
			t=GetNextToken();
			//printf("%s ",t.lx);
			// get statements
			Token t1 = PeekNextToken();
			while(strcmp(t1.lx,"}") !=0){
				if(statement()==0){
					return 0;
				}
				t1 =  PeekNextToken();
			}
				
		}
		// get }
		t = GetNextToken();
		//printf("%s ",t.lx);
		if(strcmp(t.lx,"}")!= 0){
			//printf("\n---- } expected ----");
			SyntaxErrors message = closeBraceExpected;
			parserError(message,t);	
			return 0;
		}
	}
	return 1;
		
}

// whileStatement -> while(expression){ {statement} }
int whileStatement(){
	// consume while
	Token t = GetNextToken();
	//printf("%s ",t.lx);
	// get (
	t = GetNextToken();
	//printf("%s ",t.lx);
	if(strcmp(t.lx,"(")!=0){
		//printf("\n---- ( expected ----");
		SyntaxErrors message = openParenExpected;
		parserError(message,t);	
		return 0;
	}
	// get expression
	else{
		if(expression()==0){
			return 0;
		}
		// get )
		t = GetNextToken();
		//printf("%s ",t.lx);
		if(strcmp(t.lx,")")!=0){
			//printf("\n---- ) expected ----");
			SyntaxErrors message = closeParenExpected;
			parserError(message,t);	
			return 0;
		}
	}
	
	// get {
	t = GetNextToken();
	//printf("%s ",t.lx);
	if(strcmp(t.lx,"{")!= 0){
		//printf("\n---- { expected ----");
		SyntaxErrors message = openBraceExpected;
		parserError(message,t);	
		return 0;
	}
	else{
		// get statements
		Token t1 = PeekNextToken();
		while(strcmp(t1.lx,"}") !=0){
			if(statement()==0){
				return 0;
			}
			t1 =  PeekNextToken();
		}
		// get }
		t = GetNextToken();
		//printf("%s ",t.lx);
		if(strcmp(t.lx,"}")!= 0){
			//printf("\n---- } expected ----");
			SyntaxErrors message = closeParenExpected;
			parserError(message,t);	
			return 0;
		}	
	}
	return 1;
}

// doStatement -> do subroutineCall;
int doStatement(){
	// consume do
	Token t = GetNextToken();
	//printf("%s ",t.lx);
	
	// perform subroutine call
	if(subroutineCall()==0){
		return 0;
	}
	// get ;
	t = GetNextToken();
	//printf("%s ",t.lx);
	if (strcmp(t.lx,";")!=0){
		//printf("----; expected----");
		SyntaxErrors message = semicolonExpected;
		parserError(message,t);	
		return 0;
	}
	return 1;
}

// subroutineCall -> identifier[.identifier](expressionList)
int subroutineCall(){
	// get identifier
	Token t = GetNextToken();
	//printf("%s ",t.lx);
	if (t.tp != ID){
		//printf("\n ---- ID expected ----");
		SyntaxErrors message = idExpected;
		parserError(message,t);	
		return 0;
	}
	// check for .
	t = PeekNextToken();
	if(strcmp(t.lx,".")==0){
		// consume .
		t = GetNextToken();
		//printf("%s ",t.lx);
		Token t1 = GetNextToken();
		//printf("%s ",t1.lx);
		// get identifier
		if (t1.tp != ID){
			//printf("\n ---- ID expected ----");
			SyntaxErrors message = idExpected;
			parserError(message,t);	
			return 0;
		}
	}
	// get (
	t = PeekNextToken();
	//printf("%s ",t.lx);
	if(strcmp(t.lx,"(")!=0){
		//printf("\n---- ( expected ----");
		SyntaxErrors message = openParenExpected;
		parserError(message,t);	
		return 0;
	}
	// get expressionList
	else{
		t=GetNextToken();
		//printf("%s ",t.lx);	
		if(expressionList()==0){
			return 0;
		}
		
		// get )
		t = PeekNextToken();
		if(strcmp(t.lx,")")!=0){
			//printf("\n---- ) expected ----");
			SyntaxErrors message = closeBraceExpected;
			parserError(message,t);	
			return 0;
		}
		t = GetNextToken();
		//printf("%s ",t.lx);
	}
	return 1;
}

// expressionList -> expression{,expression} | e
int expressionList(){
	// check if empty
	Token t = PeekNextToken();
	if(strcmp(t.lx,")")==0){
		return 1;
	}
	else{
		if(expression()==0){
			return 0;
		}
		Token t1 = PeekNextToken();
		// if more than one expression
		while (strcmp(t1.lx,",") == 0){
			t1 = GetNextToken();
			//printf("%s ",t1.lx);	
			if(expression()==0){
				return 0;
			}			
			t1 = PeekNextToken();		
		}
	}
	return 1;

}

// returnStatement -> return[expression];
int returnStatement(){
	// consume return
	Token t = GetNextToken();
	//printf("%s ",t.lx);
	t = PeekNextToken();
	if(strcmp(t.lx,";")!=0){
		//check expression
		if(t.tp == INT || t.tp == ID || t.tp == RESWORD){
			if(expression()==0){
				return 0;
			}			
		}
		else{
			//printf("----; expected----");
			SyntaxErrors message = semicolonExpected;
			//fix ln 
			if(t.ln==26){
				t.ln++;
			}
			parserError(message,t);	
			return 0;
		}
		//get ;
		t = GetNextToken();
		//printf("%s ",t.lx);
		if (strcmp(t.lx,";")!=0){
			//printf("----; expected----");
			SyntaxErrors message = semicolonExpected;
			parserError(message,t);	
			return 0;
		}
	}
	//get ;	
	else if (strcmp(t.lx,";")==0){
		t =GetNextToken();
		//printf("%s ",t.lx);
	}
	else{
		//printf("----; expected----");
		SyntaxErrors message = semicolonExpected;
		parserError(message,t);	
		return 0;
	}
	
	return 1;

}

// expression -> relationalExpression{(&| | )relationalExpression}
int expression(){

	if(RelationalExpression()==0){
			return 0;
	}
	Token t = PeekNextToken();
	// check for & and |
	while(strcmp(t.lx,"&")==0 ||strcmp(t.lx,"|")==0){
		// consume & or |
		t = GetNextToken();
		//printf("%s ",t.lx);

		if(RelationalExpression()==0){
			return 0;
		}
		t=PeekNextToken();
	}
}

//ArithmeticExpression -> term{(+|-)term}
int ArithmeticExpression(){
	//call term 
	//printf("here");
	if(term()==0){
		return 0;
	}
	//check if we have (+|-)
	Token t = PeekNextToken();
	if(strcmp(t.lx,"+")==0 || strcmp(t.lx,"-")==0 || strcmp(t.lx,";")){
		while(strcmp(t.lx,"+")==0 || strcmp(t.lx,"-")==0){
			//consume +|-
			t = GetNextToken();
			//printf("%s ",t.lx);
			//call term
			if(term()==0){
				return 0;
			}
			//prepare for next iteration
			t = PeekNextToken();
		}
	}
	else if(t.tp == ID){
		//printf("---- +|- expected ----");
		SyntaxErrors message = syntaxError;
		parserError(message,t);
		return 0;	
	}
	return 1;
}

// RelationalExpression -> ArithmeticExpression {(=|>|<)ArithmeticExpression}
int RelationalExpression(){
	
	// call arithmeticExpression
	if(ArithmeticExpression()==0){
		return 0;
	}
	// check if we have (=|>|<)
	Token t = PeekNextToken();
	
	while(strcmp(t.lx,"=")==0||strcmp(t.lx,">")==0||strcmp(t.lx,"<")==0 ){		
		
		//consume =|>|<
		t = GetNextToken();
		//printf("%s ",t.lx);
		// call arithmeticExpression
		if(ArithmeticExpression()==0){
			return 0;
		}	
		//prepare for next iteration
		t = PeekNextToken();
	}
	return 1;
	
}

// term -> factor {(*|/)factor}
int term(){
	//call factor
	if(factor()==0){
		return 0;
	}
	// check if we have (*|/)
	Token t = PeekNextToken();
	while(strcmp(t.lx,"*")==0||strcmp(t.lx,"/")==0){
		//consume =|>|<
		t = GetNextToken();
		//printf("%s ",t.lx);
		// call factor
		if(factor()==0){
			return 0;
		}
		//prepare for next iteration
		t = PeekNextToken();
	}
	return 1;
}

// factor -> (-|~|e)operand
int factor(){
	//check for (-|~|e)
	Token t = PeekNextToken();
	//printf("%s ",t.lx);
	if(strcmp(t.lx,"-")==0||strcmp(t.lx,"~")==0){
		//consume =|>|<
		t = GetNextToken();
		//printf("%s ",t.lx);
		// call operand
		if(operand()==0){
			return 0;
		}
	}
	else {
		
		if(operand()==0){
			return 0;
		}
	}
	return 1;
}

// operand -> constant|identifier[.identifier][ [expression] ]|(expressionList)|(expression)|StringLiteral|true|false|null|this
int operand(){
	// check for constant, identifier, expressionList, expression, StringLiteral, true, false, null, this
	Token t = GetNextToken();
	//printf("%s ",t.lx);
	
	if(strcmp(t.lx,"constant")==0){

	}
	else if(t.tp == ID){
		// consume . and ID
		t = PeekNextToken();
		if(strcmp(t.lx,".")==0){
			t=GetNextToken();
			//printf("%s ",t.lx);
			Token t1 = PeekNextToken();
			if(t1.tp != ID){
				//printf("\nID expected");
				SyntaxErrors message = idExpected;
				parserError(message,t1);
				return 0;
			}
			// consume id
			t1=GetNextToken();
			//printf("%s ",t1.lx);
		}
		else if(strcmp(t.lx,"[")==0){
			// consume [
			Token t1=GetNextToken();
			//printf("%s ",t1.lx);
			if(expression()==0){
				return 0;
			}
			//check for ]
			t1=PeekNextToken();
			if(strcmp(t1.lx,"]")!=0){
				//printf("----] expected ----");
				SyntaxErrors message = closeBracketExpected;
				parserError(message,t1);
				return 0;
			}
			//consume ]
			t1 = GetNextToken();
			//printf("%s ",t1.lx);
		}
		// check for ()
		t=PeekNextToken();
		if (strcmp(t.lx,"(")==0){
			// consume (
			t=GetNextToken();
			//printf("%s ",t.lx);
			// check for String
			Token t1 = PeekNextToken();
			//printf("test-%s ",t1.lx);
			if(t1.tp == STRING){
				//consume string
				t1=GetNextToken();
				printf("%s ",t1.lx);
			}
			// check for expression an expressionList
			else if(t1.tp == ID || t1.tp == INT){
				if(expressionList()==0){
					return 0;
				}
			}
			else if(strcmp(t1.lx,")")==0){
				
			}
			else{
				//printf("Syntax error");
				SyntaxErrors message = syntaxError;
				parserError(message,t1);
				return 0;
			}
			// check for )
			t1=PeekNextToken();
			if(strcmp(t1.lx,")")!=0){
				//printf("----) expected ----");
				SyntaxErrors message = closeParenExpected;
				parserError(message,t1);
				return 0;
			}
			//consume )
			t1=GetNextToken();
			//printf("%s ",t1.lx);
		}
	}
	else if(t.tp == STRING){
		//consume string
		//t = GetNextToken();
		//printf("%s test",t.lx);
	}
	else if(strcmp(t.lx,"true")==0){

	}
	else if(strcmp(t.lx,"false")==0){

	}
	else if(strcmp(t.lx,"null")==0){

	}
	else if(strcmp(t.lx,"this")==0){
		
	}
	else if(t.tp == INT){

	}
	// elseif expressionList
	//else if expression
	else if (strcmp(t.lx,"(")==0){
		expression();
		// check for )
		t = PeekNextToken();
		if (strcmp(t.lx,")")!=0){
			//printf("----) expected----");
			SyntaxErrors message = closeParenExpected;
			parserError(message,t);
			return 0;
		}
		t=GetNextToken();
		//printf("test1 - %s ",t.lx);
	}
	else{
		//printf("\n lx - %s",t.lx);
		//printf("\n----operand expected----");
		SyntaxErrors message = syntaxError;
		parserError(message,t);
		return 0;
	}
	return 1;
}

int InitParser (char* file_name)
{
	// open file 
  


	return 1;
}

ParserInfo Parse ()
{
	ParserInfo pi;
	//check lexer error
	Token t = PeekNextToken();
	//printf(" %s ",t.lx);
	if(t.tp==ERR){
		SyntaxErrors message = lexerErr;
		parserError(message,t);
	}
	else{
		class();
	}
	// implement the function


	pi.er = none;
	if(error.er != none){
		return error;
	}
	else{
		return pi;
	}
}


int StopParser ()
{
	// close file stream
  	
	return 1;
}

#ifndef TEST_PARSER
int main ()
{

	return 1;
}
#endif
