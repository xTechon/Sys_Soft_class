#include "headers.h"
#define TEST 0
#define DEBUG 0
#define ELMSG(L, V)                                                            \
  printf("\nERROR %2d: LOCATION %x SURPASSES SIC MEMORY\n", L, V);

int TestMode();
int Directives(int, int *, int *, char *, char *, char *, SYMBOL, int);
// int checkOverflow(int count);
int main(int argc, char *argv[]) {
  //program start
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
  //Opcode hash table
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
    // TODO fix not triggering in blank lines
    // quit if it finds a blank line
    if (line[0] == '\n') {
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
      if (locCount != 0) {
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
  PrintTree();
  printf("\n");

  printf("\nFinish Pass 1");
  // Pass 2
  rewind(fp);
  int progLen = locCount - getTitleNode()->node.Address;
  lCount = 0;
  locCount = 0;
  OPCODES *hashtemp = 0;
  // address entries in the object code are 6 characters long
  char *address = malloc(7 * sizeof(char));
  memset(address, '\0', 7 * sizeof(char));

  // keep track of the linked list record
  int recSize = 0;

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
  //HEAD->record = RetrieveREC(HEAD);

  // Terminate Header record
  TAIL = PushLinkREC(TAIL, "\n");

  printf("\nH Record is:\n");
  PrintList(HEAD);

  while (fgets(line, 1024, fp) != NULL) {
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
      printf("\nnext token after symbol is: %s", nextToken);
    } else {
      nextToken = strtok(line, " \t\n");
    }
    strcpy(opcode, nextToken);
    KillWhiteChar(opcode);
    hashtemp = FindHash(OpcodeTable, 29, opcode);
    dirTrack = CmprDir(nextToken);
    // itterate through dirTrack
    if (dirTrack < 0) {
      int test = Directives(dirTrack, &lCount, &locCount, nextToken, operand, argument, sym, 0);
      // printf("\nTest is: %d", test);
      if (test == 1) {
        printf("\nclosing program");
        fclose(fp);
        exit(0);
      } else if (test == -1) {
        printf("\nstarting word");
        // case for a word
        // operand = strtok(nextToken, " \t\n");
        operand = strtok(NULL, " \t");
        // KillWhiteChar(operand);
        printf("\nOperand is: %s", operand);
        if (rHEAD == NULL) {
          Relative(&rHEAD, &TAIL, locCount, &recSize);
          printf("\nrHead is at %s", rHEAD->record);
        }
        if (recSize <= 27) {
          char i[7];
          sprintf(i, "%06X", atoi(operand));
          TAIL = PushLinkREC(TAIL, i);
          recSize += 3;
        } else if (recSize > 27) {
          // create a temp to hold the record size
          RECLIST temp;
          temp.record = malloc(3 * sizeof(char));
          memset(temp.record, '\0', 3 * sizeof(char));
          // insert the temp into the list after record start address
          temp.next = rHEAD->next;
          rHEAD->next = &temp;
          // enter in the record size
          sprintf(temp.record, "%02X", recSize);
          // combine and clear space
          //rHEAD->record = RetrieveREC(rHEAD);
          // create a new tail
          // TAIL = PushLinkREC(rHEAD, "\n");
          TAIL = rHEAD;
          // reset the rHEAD
          rHEAD = NULL;
        }
      }
      // BYTE case
      else if (test == -2) {
        if (rHEAD == NULL) {
          Relative(&rHEAD, &TAIL, locCount, &recSize);
        }
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
            int i = 0;
            while (argument[i] != '\0') {
              i++;
            }
            if (i % 2) {
              i++;
            }
            i /= 2;
            if (recSize < 27) {
              char x[7];
              sprintf(x, "%X", j);
              TAIL = PushLinkREC(TAIL, x);
            } else if (recSize >= 27) {
              // create a temp to hold the record size
              RECLIST temp;
              temp.record = malloc(3 * sizeof(char));
              memset(temp.record, '\0', 3 * sizeof(char));
              // insert the temp into the list after record start address
              temp.next = rHEAD->next;
              rHEAD->next = &temp;
              // enter in the record size
              sprintf(temp.record, "%02X", recSize);
              // combine and clear space
              //rHEAD->record = RetrieveREC(rHEAD);
              // create a new tail
              // TAIL = PushLinkREC(rHEAD, "\n");
              TAIL = rHEAD;
              // reset the rHEAD
              rHEAD = NULL;
            }
            recSize += i;
            locCount += i; // increment by the number of bytes required to store
            // constant
          }
        } else if (operand[0] == 'C') {
          strtok(operand, "'");
          argument = strtok(NULL, "'");
          // printf("\nArgument is: %s", argument);
          int i = 0;
#if DEBUG
          printf("\nCHARACTER CONSTANT: %s", argument);
#endif
          while (argument[i] != '\0') {
            // just in case the constant overflows
            if (rHEAD == NULL) {
              Relative(&rHEAD, &TAIL, (locCount + i), &recSize);
            }
            // put the character into the list
            if (recSize < 28) {
              char c[3]; // to store character as hex
              sprintf(c, "%2X", argument[i]); // convert character value into hex value
              TAIL = PushLinkREC(TAIL, c);
              recSize += 1;
            } else if (recSize >= 28) {
              printf("\nactivate wrap");
              // create a temp to hold the record size
              RECLIST *temp = malloc(sizeof(RECLIST));
              temp->record = malloc(3 * sizeof(char));
              memset(temp->record, '\0', 3 * sizeof(char));
              // insert the temp into the list after record start address
              temp->next = rHEAD->next;
              rHEAD->next = temp;
              // enter in the record size
              sprintf(temp->record, "%2X", recSize);
              // combine and clear space
              //rHEAD->record = RetrieveREC(rHEAD);
              // create a new tail
              // TAIL = PushLinkREC(rHEAD, "\n");
              TAIL = rHEAD;
              // reset the rHEAD
              rHEAD = NULL;
            }
            i++;
          }
          locCount += i; // characters are stored in one byte
          if (checkOverflow(locCount)) {
            ELMSG(lCount, locCount) return 1;
          }
        }
      }
    }
    // Where the records get created
    else if (hashtemp != NULL) {
      //#if DEBUG
      // printf("\n\"%s\" is an OPCODE", nextToken);
      //#endif
      operand = strtok(NULL, " ,\t");
      KillWhiteChar(operand);
      // get the symbol
      sym = FindSymbol(operand);
      //#if DEBUG
      // printf("\nOPCODE OPERAND: %s", operand);
      //#endif
      if (rHEAD == NULL) {
        Relative(&rHEAD, &TAIL, locCount, &recSize);
        //printf("\nrHEAD in out of funct: %s", rHEAD->record);
        //printf("\nTAIL in out of funct: %s", TAIL->record);
        PrintList(HEAD);
      } else if (recSize < 27) {
        char instruct[7];
        sprintf(instruct, "%02X%04X", hashtemp->OpCode, sym.Address);
        TAIL = PushLinkREC(TAIL, instruct);
        recSize += 3;
      } else if (recSize >= 27) {
        InsertLength(&rHEAD, &TAIL, recSize);
        PrintList(HEAD);
        /*
        // create a temp to hold the record size
        RECLIST temp;
        temp.record = malloc(3 * sizeof(char));
        memset(temp.record, '\0', 3 * sizeof(char));
        // insert the temp into the list after record start address
        printf("\nrHEAD in out of funct: %s", rHEAD->record);
        printf("\nTAIL in out of funct: %s", TAIL->record);
        printf("\nrHEAD->next is: %s", rHEAD->next->record);
        // enter in the record size
        sprintf(temp.record, "%02X", recSize);
        printf("\ntemp.record is now: %s", temp.record);
        temp.next = rHEAD->next;
        printf("\ntemp.next is now: %s", temp.next->record);
        rHEAD->next = &temp;
        printf("\nrHEAD-> is now: %s", rHEAD->next->record);
        // combine and clear space
        //rHEAD->record = RetrieveREC(rHEAD);
        // create a new tail
        TAIL = PushLinkREC(TAIL, "\n");
        PrintList(HEAD);
        // reset the rHEAD
        rHEAD = NULL;
        */
      }
      locCount += 3;
    } else {
      printf("\nERROR %2d: \"%s\" IS NOT A VALID OPCODE\n", lCount, nextToken);
      fclose(fp);
      exit(0);
    }
  }

  printf("\nrecSize is %d", recSize);
  if (rHEAD != NULL) {
    RECLIST temp;
    temp.record = malloc(3 * sizeof(char));
    memset(temp.record, '\0', 3 * sizeof(char));
    // insert the temp into the list after record start address
    temp.next = rHEAD->next;
    rHEAD->next = &temp;
    // enter in the record size
    sprintf(temp.record, "%02X", recSize);
    // combine and clear space
    //rHEAD->record = RetrieveREC(rHEAD);
    // create a new tail
    // TAIL = PushLinkREC(rHEAD, "\n");
    TAIL = rHEAD;
    // reset the rHEAD
    rHEAD = NULL;
  }
  PrintList(HEAD);
  fclose(fp);
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
int Directives(int dirTrack, int *lCount, int *locCount, char *nextToken, char *operand, char *argument, SYMBOL sym, int flag) {
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
    return 0;
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
    return 0;
  case -5: // RESR
    operand = strtok(NULL, "#\n");
    *locCount += 3;
    if (checkOverflow(*locCount)) {
      ELMSG(*lCount, *locCount) return 1;
    }
    return 0;
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
      *locCount += 3;
      return -1;
    }
    return 0;
  }
  return 0;
}
