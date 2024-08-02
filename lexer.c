/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
Lexer Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Robert Helps
Student ID: 201497998
Email: sc21rh@leeds.ac.uk
Date Work Commenced: 28/2/23
*************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "lexer.h"


// YOU CAN ADD YOUR OWN FUNCTIONS, DECLARATIONS AND VARIABLES HERE
typedef struct{
  FILE *openfile;
  char filename[128] ;
  int linenumber;
}FileStruct;
//creating current and new file structures
FileStruct currentfile, newfile;

//Error messages
char EofComment[128] = "Error: unexpected eof in comment";
char NewLineInString[128] = "Error: new line in string constant";
char EofInString[128] = "Error: unexpected eof in string constant";
char IllSymbol[128] = "Error: illegal symbol in source file";

// list of symbols
const char symbols[] = {'(',')','[',']','{','}',
                         ',',';','=','.','+','-',
                         '*','/','|','~','<','>',
                         '&',':'};

//list of reserved words
const char *reserved_words[] = {"class","constructor","method", "function",
                                "int","boolean","char","void",
                                "var","static","field",
                                "let","do","if","else","while","return",
                                "true","false","null",
                                "this"};

//function to check if we have a symbol
int CheckSymbol(char word){
  // switch case to check if we have a symbol
  int array_length = sizeof(symbols) / sizeof(symbols[0]);
  for (int i = 0; i < array_length; i++){
    //if word is a symbol
    if (symbols[i] == word){
      return 1;
    }
  }
  return 0;

}

//function to check if we have a reserved word
int CheckReservedWord(char *word){
  int array_length = sizeof(reserved_words) / sizeof(reserved_words[0]);
    for (int i = 0; i < array_length; i++){
    //if word is a symbol
    if (strcmp(reserved_words[i],word)==0){
      return 1;
    }
  }
  return 0;
}

// function to get all the tokens from the source file






// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
int InitLexer (char* file_name)
{
  // open file 
  
  currentfile.openfile = fopen(file_name,"r");
  strcpy(currentfile.filename , file_name);
    //check file opens
  if (currentfile.openfile == NULL){
    printf("ERROR: File did not open");
    //fclose(openfile);
    return 0;
  }
  //comapre newfile name to old file name
  if(currentfile.filename != newfile.filename){
    //we have a new file set ln to 1 and set current filename
    
    currentfile.linenumber = 1;
  }
  strcpy(newfile.filename , currentfile.filename);


  return 1;
}


// Get the next token from the source file
Token GetNextToken ()
{
	Token theToken;
  
  // character we read from the file
  int character;
  // continues until we get a token
  while(1){
    character = getc(currentfile.openfile);
 
    // end of file
    if (character == EOF){
      theToken.tp = EOFile;
      strcpy(theToken.fl, currentfile.filename); 
      theToken.ln = currentfile.linenumber;
      return theToken;
    }

    // newline character means we iterate ln
    while(isspace(character)){
      if (character == '\n'){
        currentfile.linenumber++;
        //break;
      }
      character = getc(currentfile.openfile);

    }

    //check comments
    while (character == '/'){

      int second_character = getc(currentfile.openfile);
      // if we encounter a space or "
      if (second_character == 32 || second_character == '"'){
        break;
      }

      // second / indicates comment line
      else if (second_character == '/'){
        int i=0;
        // until end of line ignore line
        while(second_character != '\n'){
          
          second_character=getc(currentfile.openfile);
          // if EOF we return EOF          
          if(second_character == EOF){
            //printf("eol line: %i",currentfile.linenumber);
            theToken.ln = currentfile.linenumber;
            theToken.tp = EOF;
            //copy error message and filename
            strcpy(theToken.lx , EofComment);
            strcpy(theToken.fl, currentfile.filename); 
            return theToken;
          }
         
        }
        //iterate linenumber
        currentfile.linenumber++;
        
      }
      // /* comment
      else if (second_character == '*'){
        int star = getc(currentfile.openfile);
        // if new line
        if (star == '\n'){
          // iterate linenumber
          currentfile.linenumber++;
        }
        
        // search until we find */
        while (true){
          int backslash = getc(currentfile.openfile);
          
          //if new line
          if (backslash == '\n'){
            // iterate linenumber
            currentfile.linenumber++;
          }

          // when we find */ we want to read after
          if (star == '*' && backslash == '/'){
            break;

            //check if next two character are /*
            int check1 = getc(currentfile.openfile);
            int check2 = getc(currentfile.openfile);
            
            if (check1 == '/' && check2 == '*'){
              
            }
            //end of /* comment
            else{
              break;
            }
          }
          // EOF so we return an ERR
          if (backslash == EOF){
            theToken.ln = currentfile.linenumber;
            theToken.tp = ERR;
            strcpy(theToken.lx , EofComment);
            strcpy(theToken.fl, currentfile.filename); 
            
            return theToken;
          }

          //reset getc
          star = backslash;
        }
        continue;
      }  

      // not a comment     
      else{
        ungetc(second_character, currentfile.openfile);
      }
      
     
      character = getc(currentfile.openfile);
      //consume spaces 
      while(isspace(character) || character == '\n'){
        //if end of line
        if (character == '\n'){
          //iterate linumber
          currentfile.linenumber++;
        }
        character = getc(currentfile.openfile);
        // if EOF
        if (character == EOF){
          theToken.tp = EOFile;
          strcpy(theToken.fl, currentfile.filename); 
          theToken.ln = currentfile.linenumber;
          return theToken;
        }
      }
    
    }

    //set line of the token
    theToken.ln = currentfile.linenumber;
    
    //isalpha() to check if in the alphabet
    if (isalpha(character) || character=='_')
    { 
      // temporary array to store token lx
      char temp[128] = "";
      int i = 0;
      // while alphabet, _ or number we continue to read
      while((isalpha(character) || character=='_' || isdigit(character))){
        //store character
        temp[i++] = character;

        //end of token by symbol
        int check_next = getc(currentfile.openfile); 
        //cast check_next to a char
        char check = (char)check_next;    
        if (CheckSymbol(check) == 1){
          ungetc(check, currentfile.openfile);
          //check reserve words
          if(CheckReservedWord(temp) == 1){
            TokenType type = RESWORD;
            theToken.tp = type;
            strcpy(theToken.lx,temp);
            strcpy(theToken.fl, currentfile.filename);
            return theToken;
          }

          // else we have an ID
          TokenType type = ID;
          theToken.tp = type;
          strcpy(theToken.fl, currentfile.filename);
          strcpy(theToken.lx,temp);          
          return theToken;
        }

        //end of lexemme by space
        else if (isspace(check_next)){
          //check reserved words
          if(CheckReservedWord(temp) == 1){
            TokenType type = RESWORD;
            theToken.tp = type;
            strcpy(theToken.lx,temp);
            strcpy(theToken.fl, currentfile.filename);
            return theToken;
          }
          // else we have an ID
          TokenType type = ID;
          theToken.tp = type;
          strcpy(theToken.lx,temp);
          strcpy(theToken.fl, currentfile.filename);
          return theToken;
        }
        // end of file
        else if (check_next == EOF){
          //check reserved words
          if(CheckReservedWord(temp) == 1){
            TokenType type = RESWORD;
            theToken.tp = type;
            strcpy(theToken.lx,temp);
            strcpy(theToken.fl, currentfile.filename);
            return theToken;
          }
          // else we have an ID
          TokenType type = ID;
          theToken.tp = type;
          strcpy(theToken.lx,temp);
          strcpy(theToken.fl, currentfile.filename);
          return theToken;
        } 
        //send character back to file stream
        ungetc(check_next,currentfile.openfile);
        
        //get next character
        character = getc(currentfile.openfile);
      }
      //cope filename to token struct
      strcpy(theToken.fl, currentfile.filename);
      return theToken;      
    }
      
    //isdigit() to check if digit
    if (isdigit(character)){
      // add to temporary array 
      char temp[128] = "";
      int i = 0;
      
      while(isdigit(character) > 0){
        //save character to temp array
        temp[i++] = character;

        //end of token by symbol
        int check_next = getc(currentfile.openfile); 
        //cast check_next to a char
        char check = (char)check_next;   
        if (CheckSymbol(check) == 1){
          TokenType type = INT;        
          theToken.tp = type;
          strcpy(theToken.lx,temp);
          
          ungetc(check, currentfile.openfile);
          return theToken;
        }
        // end of token by space
        else if (isspace(check_next)){
          TokenType type = INT;        
          theToken.tp = type;
          strcpy(theToken.lx,temp);          
          return theToken;
        }
        //send character back to file stream
        ungetc(check_next,currentfile.openfile);
        
        //get next character
        character = getc(currentfile.openfile);
      }
      return theToken;
    }

    //if we have a symbol
    if (CheckSymbol(character) == 1){
      char temp[32] = "";
      temp[0] = character;
      TokenType type = SYMBOL;
      theToken.tp = type;
      strcpy(theToken.lx, temp );
      strcpy(theToken.fl, currentfile.filename); 
      return theToken;
    }
    //start of string
    else if(character == '"'){
      //temp array to store string
      char temp[128] = "";
      int i=0;
      character = getc(currentfile.openfile);
      // search until we find " indicating end of string
      while (character != '"'){
        // new line in string
        if (character == '\n'){
          theToken.tp = ERR;
          strcpy(theToken.lx, NewLineInString);
          strcpy(theToken.fl, currentfile.filename);
          return theToken;
        }
        // EOF in string
        if(character == EOF){
          theToken.tp = ERR;
          strcpy(theToken.lx, EofInString);
          strcpy(theToken.fl, currentfile.filename);
          return theToken;
        }
        //save character to array and iterate i
        temp[i] = character;
        i++;
        character = getc(currentfile.openfile);
      }
      // end of string
      theToken.tp = STRING;
      strcpy(theToken.lx, temp);
      strcpy(theToken.fl, currentfile.filename);
      return theToken;      
    }
    // end of file
    else if(character == EOF){
      //printf("test1");
      theToken.tp = EOFile;
      strcpy(theToken.fl, currentfile.filename);      
      theToken.ln = currentfile.linenumber;
      return theToken;
    }
    // illegal symbol
    else{
      TokenType type = ERR;
      theToken.tp = type;
      strcpy(theToken.lx, IllSymbol);
      strcpy(theToken.fl, currentfile.filename); 
      return theToken;
    }
  }  

  return theToken;
}

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken ()
{
  //get next token
  Token theToken = GetNextToken();
  // length of token lx
  int length = strlen(theToken.lx);
  char token[128] = "";
  //copy lx, filename and linenumber
  strcpy(token,theToken.lx);
  strcpy(theToken.fl , currentfile.filename);
  theToken.ln = currentfile.linenumber;
  // end of file we return token
  if(theToken.tp == EOFile){
    return theToken;
  }
  // if string we return "
  else if(theToken.tp == STRING){
    ungetc('"',currentfile.openfile);
  }
  // else return space character
  else{
    ungetc(' ',currentfile.openfile);
  }
  // push back character byte by byte
  for (int i = (length-1);i>=0; i--){
    ungetc(token[i],currentfile.openfile);
  }
  // return 1st "
  if(theToken.tp == STRING){
    ungetc('"',currentfile.openfile);
  }
  return theToken;
}

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer ()
{
  // close file stream
  fclose(currentfile.openfile);

	return 0;
}

// do not remove the next line
#ifndef TEST
int main (int argc, char **argv)
{
	// implement your main function here
  // NOTE: the autograder will not use your main function
  if (argc > 2){
    printf("Too many arguments supplied");
  }
  else if (argc == 1){
    printf("Not enough arguments supplied");
  }
  // get file name from command line
  else{
    
 
  }
  char* file_name = argv[1];
  // call function to initialise the lexer
  
  int check = InitLexer(file_name);
  if (check == 0){
    return 0;
  }

  // call function to get tokens from the file
  //int test = 0;
  Token t = GetNextToken();
  while(t.tp != EOFile){
    //printf("test");
    if(t.tp == EOFile){
      return 0;
    }
    
    Token t = GetNextToken ();
    //printf ("%i\n",t.ln);
  }
  // return 1 if all correct else 0
  // call function to stop the lexer
  StopLexer();
  
	return 0;
}
// do not remove the next line
#endif
