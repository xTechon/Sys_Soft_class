#include "headers.h"
#define TEST 0
#define DEBUG 0
#define DEBUGP2 0
#define TABLES 0
#define ELMSG(L, V)                                                            \
  printf("\nERROR %2d: LOCATION %x SURPASSES SIC MEMORY\n", L, V);

int TestMode();
int Directives(int, int *, int *, char *, char *, char *, SYMBOL, int);
// int checkOverflow(int count);
int main(int argc, char *argv[]) {
  // program start
#if TEST
  TestMode();
  exit(0); // uncomment for testing mode
#endif
  // File Checking Section of code
  if (argc != 2) { // check to see if correct amnt of arguments entered
    printf("ERROR: Usage: %s filename\n", argv[0]);
    exit(0);
  }
  FILE *fp;
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    printf("ERROR: %s could not be opened for reading.\n", argv[1]);
    exit(0);
  }
#if DEBUG
  printf("\nFile Opened successfully.");
#endif
  // Opcode hash table
  memset(OpcodeTable, 0, 29 * sizeof(OPLIST *));
  ReadOpCodeFile(); // import opcodes
  char line[1024];
  char fline[1024];
  int errC = 0;
  char *newsym;
  newsym = malloc(1024 * sizeof(char));
  memset(newsym, '\0', 1024 * sizeof(char));
  char *nextToken;
  nextToken = malloc(1024 * sizeof(char));
  memset(nextToken, '\0', 1024 * sizeof(char));
  char *operand;
  operand = malloc(1024 * sizeof(char));
  memset(operand, '\0', 1024 * sizeof(char));
  char *argument;
  argument = malloc(1024 * sizeof(char));
  memset(argument, '\0', 1024 * sizeof(char));
  char *opcode;
  opcode = malloc(1024 * sizeof(char));
  memset(opcode, '\0', 1024 * sizeof(char));
  int lCount = 0;   // keeps track of the line number
  int locCount = 0; // location in BYTES not words
  SYMBOL sym;
  memset(&sym, 0, sizeof(sym));
  int dirTrack = 0;
  // int hexCounter = 0;
  // Pass 1
  while (fgets(line, 1024, fp) != NULL) {
    strcpy(fline, line);
    lCount++; // Keep track of lines
    if (line[0] == 35) {
#if DEBUG
      printf("\nComment: %s", line); // remove or comment out before submission
#endif
      continue;
    }
    // quit if it finds a blank line
    if (line[0] == 32 || line[0] == '\n' || line[0] == '\0' || line[0] == 13) {
      printf("\nERROR %2d: FILE HAS BLANK LINES\n", lCount);
      exit(0);
    }
    // check for symbols that start with capitals
    if ((line[0] >= 65) && (line[0] <= 90)) {
      newsym = strtok(line, " \t\n");
#if DEBUG
      printf("\nNEW SYMBOL ON LINE: %d", lCount);
      printf("\nNEW SYMBOL IS: %s", newsym);
#endif
      errC = IsAValidSymbol(newsym);
      if (errC != 1) {
        printf("\nERROR %2d: INVALID SYMBOL \"%s\" WITH CODE: %d\n", lCount,
               newsym, errC);
        fclose(fp);
        exit(0);
      }
      // set some of the values of the new Symbol
      strcpy(sym.Name, newsym);
      sym.SourceLine = lCount;
      if (getTitleNode() != NULL) {
        sym.Address = locCount;
        if (PushLeaf(sym)) {
          fclose(fp);
          printf("\nERROR %2d: DUPLICATE SYMBOL \"%s\"\n", lCount, sym.Name);
          exit(0);
        }
#if DEBUG
        printf("\nPUSHED LEAF");
#endif
      }
      nextToken = strtok(NULL, " \t\n");
      // get the new value for newsym
    } else {
      nextToken = strtok(line, " \t\n");
    }
#if DEBUG
    printf("\nCurrent token is: %s", nextToken);
#endif
    strcpy(opcode, nextToken);
    KillWhiteChar(opcode);
    dirTrack = CmprDir(nextToken);
    // case if newsym is a directive
    // directive behavior
    if (dirTrack < 0) {
      if (Directives(dirTrack, &lCount, &locCount, nextToken, operand, argument,
                     sym, 1)) {
        fclose(fp);
        exit(0);
      }
    }
    // case if newsym is an opcode
    else if (FindHash(OpcodeTable, 29, opcode) != NULL) {
#if DEBUG
      printf("\n\"%s\" is an OPCODE", nextToken);
#endif
      operand = strtok(NULL, " \t");
#if DEBUG
      printf("\n%s OPCODE OPERAND", operand);
#endif
      locCount += 3;
    } else {
      printf("\nERROR %2d: \"%s\" IS NOT A VALID OPCODE\n", lCount, nextToken);
      fclose(fp);
      exit(0);
    }
    // max word size is 2^23, check programmer's ref
    // printf("\nLocation is: %x\n", locCount);
  }
  // Print out the Symbol Table
#if TABLES
  PrintTree();
  printf("\n");
#endif
  // printf("\nFinish Pass 1");
  if (getTitleNode() == NULL) {
    printf("\nERROR   : FILE MISSING START DIRECTIVE\n");
    fclose(fp);
    exit(0);
  }
  // Pass 2
  rewind(fp);
  int progLen = locCount - getTitleNode()->node.Address;
  lCount = 0;
  locCount = 0;
  int IndexMode = 0;
  OPCODES *hashtemp = 0;
  SYMBOL *symvalid;
  // address entries in the object code are 6 characters long
  char *address = malloc(7 * sizeof(char));
  memset(address, '\0', 7 * sizeof(char));

  // keep track of the linked list record
  int recSize = 0;

  // records for Mods
  RECLIST *MHEAD = (RECLIST *)malloc(sizeof(RECLIST));
  memset(MHEAD, 0, sizeof(RECLIST));
  // tail for Mods
  RECLIST *MTAIL = (RECLIST *)malloc(sizeof(RECLIST));
  memset(MTAIL, 0, sizeof(RECLIST));
  // link mod head and tail together
  MTAIL = MHEAD;
  // flag for initial mod case
  int modExist = 0;
  // Main obj file head
  RECLIST *HEAD = (RECLIST *)malloc(sizeof(RECLIST));
  HEAD->record = malloc(20 * sizeof(char));
  memset(HEAD->record, '\0', 20 * sizeof(char));

  // Relative head for record list (starts at new lines)
  RECLIST *rHEAD = NULL; //= (RECLIST *)malloc(sizeof(RECLIST));
  // memset(rHEAD, 0, sizeof(RECLIST));

  // end of the linked list
  RECLIST *TAIL = (RECLIST *)malloc(sizeof(RECLIST));
  memset(TAIL, 0, sizeof(RECLIST));

  // put H to start the header record
  strcpy(HEAD->record, "H");
  recSize++;

  // using newsym to hold the title of the program
  memset(newsym, '\0', 1024 * sizeof(char)); // clear newsym
  sprintf(newsym, "%-6s", getTitleNode()->node.Name);

  // put the title in the record and set the tail
  TAIL = PushLinkREC(HEAD, newsym);

  // increment recSize by the length of the title
  recSize += 6;

  // add the starting address to the header
  sprintf(address, "%06X", getTitleNode()->node.Address);
  TAIL = PushLinkREC(TAIL, address);
  recSize += 6;

  // add prog length to the header
  memset(address, '\0', 7 * sizeof(char)); // clear address field
  sprintf(address, "%06X", progLen);
  TAIL = PushLinkREC(TAIL, address);
  recSize += 6;

  // Combine header record into head
  // HEAD->record = RetrieveREC(HEAD);

  // Terminate Header record
  char newline[2] = {'\n', '\0'};
  TAIL = PushLinkREC(TAIL, newline);
#if DEBUGP2
  printf("\nH Record is:\n");
  PrintList(HEAD);
#endif

  // Pass2 start
  while (fgets(line, 1024, fp) != NULL) {
#if DEBUGP2
    printf("\n\nNEWLINE: %s\nlocCount is: %X\n", line, locCount);
#endif
    strcpy(fline, line);
    // printf("\n%s", line);
    lCount++;
    // Take comments out of the output
    if (line[0] == 35) {
#if DEBUG
      printf("\nComment: %s", line);
#endif
      continue;
    }
    // symbol behavior
    if ((line[0] >= 65) && (line[0] <= 90)) {
      strtok(line, " \t\n");
      nextToken = strtok(NULL, " \t\n");
#if DEBUGP2
      printf("\nnext token after symbol is: %s", nextToken);
#endif
    } else {
      nextToken = strtok(line, " \t\n");
    }
    strcpy(opcode, nextToken);
    KillWhiteChar(opcode);
    hashtemp = FindHash(OpcodeTable, 29, opcode);
    dirTrack = CmprDir(nextToken);
    // itterate through dirTrack
    if (dirTrack < 0) {
      int test = Directives(dirTrack, &lCount, &locCount, nextToken, operand,
                            argument, sym, 0);
      // printf("\nTest is: %d", test);
      if (test == 1) {
        printf("\nclosing program");
        fclose(fp);
        exit(0);
      }
      // WORD case
      else if (test == -1) {
#if DEBUGP2
        printf("\nstarting word");
#endif
        // case for a word
        // operand = strtok(nextToken, " \t\n");
        operand = strtok(NULL, " \t");
        // KillWhiteChar(operand);
#if DEBUGP2
        printf("\nOperand is: %s", operand);
#endif
        if (rHEAD == NULL) {
#if DEBUGP2
          printf("\nWord Head null");
          printf("\nlocCount here is: %X", locCount);
#endif
          Relative(&rHEAD, &TAIL, locCount, &recSize);
#if DEBUGP2
          printf("\nrHead is at %s", rHEAD->record);
          PrintList(HEAD);
#endif
        }
        if (recSize <= 27) {
#if DEBUGP2
          printf("\nWord record creation");
#endif
          char i[7];
          sprintf(i, "%06X", atoi(operand));
          TAIL = PushLinkREC(TAIL, i);
          recSize += 3;
#if DEBUGP2
          PrintList(HEAD);
#endif
        } else if (recSize > 27) {
#if DEBUGP2
          printf("\nWord record wrapping");
#endif
          InsertLength(&rHEAD, &TAIL, recSize);
#if DEBUGP2
          PrintList(HEAD);
#endif
        }
        locCount += 3;
      }
      // BYTE case
      else if (test == -2) {
        if (rHEAD == NULL) {
#if DEBUGP2
          printf("\nByte constant null");
#endif
          Relative(&rHEAD, &TAIL, locCount, &recSize);
#if DEBUGP2
          PrintList(HEAD);
#endif
        }
        operand = strtok(NULL, "#\n");
        // Hexadecimal case
        if (operand[0] == 'X') {
          strtok(operand, "'");
          argument = strtok(NULL, "'");
          if (ValHEX(argument)) {
            int j = 0;
            j += (int)strtol(argument, NULL, 16); // convert char in hex to int
            if (j > 8388608) {
              printf("\nERROR %2d: HEXADECIMAL CONSTANT OVER INTEGER LIMIT ON "
                     "LINE\n",
                     lCount);
              exit(0);
            }
            int i = 0;
            while (argument[i] != '\0') {
              i++;
            }
            if (i % 2) {
              i++;
            }
            i /= 2;
            if (recSize <= 27) {
#if DEBUGP2
              printf("\nHex constant record");
#endif
              char x[7];
              sprintf(x, "%X", j);
              TAIL = PushLinkREC(TAIL, argument);
#if DEBUGP2
              PrintList(HEAD);
#endif
            } else if (recSize > 27) {
#if DEBUGP2
              printf("\nHex constant wrapping");
#endif
              InsertLength(&rHEAD, &TAIL, recSize);
#if DEBUGP2
              PrintList(HEAD);
#endif
            }
            recSize += i;
            locCount += i; // increment by the number of bytes required to store
            // constant
          } else {
            printf("\nERROR %2d: \"%s\" IS NOT A VALID HEXADECIMAL CONSTANT\n",
                   lCount, argument);
            exit(0);
          }
        }
        // Character case
        else if (operand[0] == 'C') {
          strtok(operand, "'");
          argument = strtok(NULL, "'");
          int i = 0;
          while (argument[i] != '\0') {
            if (rHEAD == NULL) {
#if DEBUGP2
              printf("\nCharacter constant record null");
#endif
              Relative(&rHEAD, &TAIL, (locCount + i), &recSize);
              char c[3]; // to store character as hex
              sprintf(c, "%X",
                      argument[i]); // convert character value into hex value
              TAIL = PushLinkREC(TAIL, c);
              recSize += 1;
#if DEBUGP2
              PrintList(HEAD);
#endif
            }
            if (recSize <= 29) {
#if DEBUGP2
              printf("\nCharacter constant record");
#endif
              char c[3]; // to store character as hex
              sprintf(c, "%X",
                      argument[i]); // convert character value into hex value
              TAIL = PushLinkREC(TAIL, c);
              recSize += 1;
#if DEBUGP2
              PrintList(HEAD);
#endif
            } else if (recSize > 29) {
#if DEBUGP2
              printf("\nCharacter constant record wrapping");
#endif
              InsertLength(&rHEAD, &TAIL, recSize);
              Relative(&rHEAD, &TAIL, (locCount + i), &recSize);
              char c[3]; // to store character as hex
              sprintf(c, "%X",
                      argument[i]); // convert character value into hex value
              TAIL = PushLinkREC(TAIL, c);
              recSize += 1;
#if DEBUGP2
              PrintList(HEAD);
#endif
            }
            i++;
          }
          locCount += i; // characters are stored in one byte
        }
        if (checkOverflow(locCount)) {
          ELMSG(lCount, locCount) return 1;
        }
      }
      // END, RESB, and RESW case
      else if (test <= -3) {
#if DEBUGP2
        printf("\nDirective END, RESB, or RESW read");
#endif
        InsertLength(&rHEAD, &TAIL, recSize);
        // create END record
        if (test == -3) {
#if DEBUGP2
          printf("\ncreating end record");
          printf("\nLine is: %s", fline);
          printf("\nnextToken is: %s", nextToken);
          printf("\nopcode is: %s", opcode);
          printf("\noperand is: %s", operand);
          printf("\nretrieving operand");
#endif
          operand = strtok(fline, " \t\n");
          if ((line[0] >= 65) && (line[0] <= 90)) {
            operand = strtok(NULL, " \t\n");
          }
          operand = strtok(NULL, " \t\n");
#if DEBUGP2
          printf("\noperand: %s", operand);
#endif
          TAIL->next = MHEAD;
          MTAIL = PushLinkREC(MTAIL, "E");
          KillWhiteChar(operand);
          symvalid = FindSymbol(operand);
          if (symvalid == NULL) {
            printf("\nERROR %2d: \"%s\" IS AN UNDEFINED SYMBOL\n", lCount,
                   operand);
            fclose(fp);
            exit(0);
          }
          char add[8];
          sprintf(add, "%06X\n", (*symvalid).Address);
          MTAIL = PushLinkREC(MTAIL, add);
        }
#if DEBUGP2
        PrintList(HEAD);
        PrintList(rHEAD);
#endif
      }
    }
    // Where the records get created
    else if (hashtemp != NULL) {
#if DEBUGP2
      printf("\n\"%s\" is an OPCODE", nextToken);
#endif
      operand = strtok(NULL, " ,\t#\n");
#if DEBUGP2
      printf("\noperand is: %s", operand);
#endif
      if (operand != NULL && (operand[0] != 13)) {
#if DEBUGP2
        printf("\nEntered conditinoal");
#endif
        KillWhiteChar(operand);
#if DEBUGP2
        printf("\noperand is: %s", operand);
#endif
        // get the symbol
        symvalid = FindSymbol(operand);
        if (symvalid == NULL) {
          printf("\nERROR %2d: \"%s\" IS AN UNDEFINED SYMBOL\n", lCount,
                 operand);
          fclose(fp);
          exit(0);
        }
        IndexMode = (*symvalid).Address;
#if DEBUGP2
        printf("\nOPCODE OPERAND: %s", operand);
#endif
        argument = strtok(NULL, " ,\t#");
        if ((argument != NULL) && (argument[0] == 'X')) {
#if DEBUGP2
          printf("\nSymbol has index, %s", argument);
#endif
          IndexMode += 0x8000;
        }
      }
#if DEBUGP2
      printf("\noperand failed test");
#endif
      if (rHEAD == NULL) {
#if DEBUGP2
        printf("\nOpcode head null");
#endif
        Relative(&rHEAD, &TAIL, locCount, &recSize);
        char instruct[7];
        char mod[18];
        if (operand != NULL && operand[0] != 13) {
          sprintf(instruct, "%02X%04X", hashtemp->OpCode, IndexMode);
          sprintf(mod, "M%06X04+%-6s\n", locCount + 1,
                  getTitleNode()->node.Name);
          if (!modExist) {
            CreateModHEAD(&MHEAD, mod, 18);
            modExist = 1;
          } else {
            MTAIL = PushLinkREC(MTAIL, mod);
          }
        } else
          sprintf(instruct, "%02X0000", hashtemp->OpCode);
        TAIL = PushLinkREC(TAIL, instruct);
        recSize += 3;
#if DEBUGP2
        PrintList(HEAD);
        PrintList(MHEAD);
#endif
      } else if (recSize <= 27) {
#if DEBUGP2
        printf("\nOpcode record creation");
#endif
        char instruct[7];
        char mod[18];
        if (operand != NULL && operand[0] != 13) {
          sprintf(instruct, "%02X%04X", hashtemp->OpCode, IndexMode);
          sprintf(mod, "M%06X04+%-6s\n", locCount + 1,
                  getTitleNode()->node.Name);
          if (!modExist) {
            CreateModHEAD(&MHEAD, mod, 18);
            modExist = 1;
          } else {
            MTAIL = PushLinkREC(MTAIL, mod);
          }
        } else {
          sprintf(instruct, "%02X0000", hashtemp->OpCode);
        }
        TAIL = PushLinkREC(TAIL, instruct);
        recSize += 3;
#if DEBUGP2
        PrintList(HEAD);
        PrintList(MHEAD);
#endif
      } else if (recSize > 27) {
#if DEBUGP2
        printf("\nOpcode wrapping");
#endif
        InsertLength(&rHEAD, &TAIL, recSize);
        Relative(&rHEAD, &TAIL, locCount, &recSize);
        char instruct[7];
        char mod[18];
        if (operand != NULL && operand[0] != 13) {
          sprintf(instruct, "%02X%04X", hashtemp->OpCode, IndexMode);
          sprintf(mod, "M%06X04+%-6s\n", locCount + 1,
                  getTitleNode()->node.Name);
          if (!modExist) {
            CreateModHEAD(&MHEAD, mod, 18);
            modExist = 1;
          } else {
            MTAIL = PushLinkREC(MTAIL, mod);
          }
        } else
          sprintf(instruct, "%02X0000", hashtemp->OpCode);
        TAIL = PushLinkREC(TAIL, instruct);
        recSize += 3;
#if DEBUGP2
        PrintList(HEAD);
        PrintList(MHEAD);
#endif
      }
      locCount += 3;
    } else {
      printf("\nERROR %2d: \"%s\" IS NOT A VALID OPCODE\n", lCount, nextToken);
      fclose(fp);
      exit(0);
    }
  }
#if TABLES
  PrintList(HEAD);
#endif

  // File Writing
  char obj[] = ".obj";
  int fileNameLength = strlen(argv[1]) + strlen(obj) + 1;
  char fileName[fileNameLength];
  strcpy(fileName, argv[1]);
  strcat(fileName, obj);
  // output stream
  FILE *op = fopen(fileName, "w");

  // write list to file
  outputList(HEAD, op);
  fclose(op);
  fclose(fp);
  // free(newsym);
  // free(opcode);
  // free(argument);
  // free(nextToken);
  exit(0);
}

// testing mode to skip normal program runtime behavior
int TestMode() {
  /*
   int j = IsAValidSymbol("BYTE");
   printf("\ninput is: %s\nj is: %i", "BYTE",j);
   printf("\nTesting Binary Tree");


   OPCODES opTest;
   strcpy(opTest.Name, "ADD");
   opTest.OpCode = 0x18;
   ReadOpCodeFile();
   int result;
   result = FindHash(OpcodeTable, 29, &opTest);
   printf("\nFindHash Result: %d", result);
   */
  exit(0);
}
int Directives(int dirTrack, int *lCount, int *locCount, char *nextToken,
               char *operand, char *argument, SYMBOL sym, int flag) {
#if DEBUG
  printf("\n\"%s\" is a DIRECTIVE", nextToken);
#endif
  switch (dirTrack) {
  case -1: // BYTE
    if (flag == 1) {
      operand = strtok(NULL, "#\n");
#if DEBUG
      printf("\nBYTE OPERAND: %s", operand);
#endif
      if (operand[0] == 'X') {
        strtok(operand, "'");
        argument = strtok(NULL, "'");
#if DEBUG
        printf("\nHEXADECIMAL CONSTANT: %s", argument);
#endif
        if (ValHEX(argument)) {
          int j = 0;
          j += (int)strtol(argument, NULL, 16); // convert char in hex to int
          if (j > 8388608) {
            printf("\nERROR %2d: HEXADECIMAL CONSTANT OVER INTEGER LIMIT ON "
                   "LINE\n",
                   *lCount);
            return 1;
          }
          int i = 0;
          while (argument[i] != '\0') {
            i++;
          }
          if (i % 2) {
            i++;
          }
          i /= 2;
          *locCount += i; // increment by the number of bytes required to store
          // constant
        } else {
          printf("\nERROR %2d: \"%s\" IS NOT A VALID HEXADECIMAL CONSTANT\n",
                 *lCount, argument);
          return 1;
        }
      } else if (operand[0] == 'C') {
        strtok(operand, "'");
        argument = strtok(NULL, "'");
        int i = 0;
#if DEBUG
        printf("\nCHARACTER CONSTANT: %s", argument);
#endif
        while (argument[i] != '\0') {
          i++;
        }
        *locCount += i; // characters are stored in one byte
      }
      if (checkOverflow(*locCount)) {
        ELMSG(*lCount, *locCount) return 1;
      }
      return 0;
    } else
      return -2;
  case -2: // END
    operand = strtok(NULL, "#\n");
    if (checkOverflow(*locCount)) {
      ELMSG(*lCount, *locCount) return 1;
    }
    if (flag == 1)
      return 0;
    else
      return -3;
  case -3: // EXPORT
    operand = strtok(NULL, "#\n");
    *locCount += 3;
    if (checkOverflow(*locCount)) {
      ELMSG(*lCount, *locCount) return 1;
    }
    return 0;
  case -4: // RESB
    operand = strtok(NULL, "#\n");
    *locCount += atoi(operand);
    if (checkOverflow(*locCount)) {
      ELMSG(*lCount, *locCount) return 1;
    }
    if (flag == 1)
      return 0;
    else
      return -4;
  case -5: // RESR
    operand = strtok(NULL, "#\n");
    *locCount += 3;
    if (checkOverflow(*locCount)) {
      ELMSG(*lCount, *locCount) return 1;
    }
    if (flag == 1)
      return 0;
    else
      return -5;
  case -6: // RESW
    operand = strtok(NULL, "#\n");
    *locCount += atoi(operand) * 3;
    if (checkOverflow(*locCount)) {
      ELMSG(*lCount, *locCount) return 1;
    }
    return 0;
  case -7: // START
    operand = strtok(NULL, "#\n");
    *locCount = (int)strtol(operand, NULL, 16); // convert char in hex to int
#if DEBUG
    printf("\n%x\n", *locCount);
#endif
    if (checkOverflow(*locCount)) {
      ELMSG(*lCount, *locCount) return 1;
    }
    if (flag == 1) {
      sym.Address = *locCount;
      sym.SourceLine = *lCount;
      PushLeaf(sym);
    }
    return 0;
  case -8: // WORD
    if (flag == 1) {
      operand = strtok(NULL, "#\n");
      int i = atoi(operand);
      if (i > 8388608 || i < -8388608) {
        printf("\nERROR %2d: INTEGER CONSTANT %d EXCEEDS LIMIT\n", *lCount, i);
      }
      *locCount += 3;
      if (checkOverflow(*locCount)) {
        ELMSG(*lCount, *locCount) exit(0);
      }
      return 0;
    } else if (flag == 0) {
      return -1;
    }
    return 0;
  }
  return 0;
}
