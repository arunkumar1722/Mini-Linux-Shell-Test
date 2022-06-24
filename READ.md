Mini Linux Shell

Console based application which implements a mini Linux
Shell command interpreter with following features:

     1. mycat      Prints the whole file.
  
     2. myhead     Prints first 10 lines of the file.
  
     3. mytail     Prints last 10 lines of the file.
  
     4. myhistory  Prints last 50 commands. 
  
     5. help       Prints the manual.
  
     6. exit       Exit the shell. 

Run the shell using following

    gcc shell.c
    ./a.out

Sample Output

    Welcome to My Shell 
    The following commands are available:
      mycat      Prints the whole file. 
      myhead     Prints first 10 lines of the file. 
      mytail     Prints last 10 lines of the file. 
      myhistory  Prints last 50 commands. 
      help       Prints the manual.
      exit       Exit the shell.
    
    > mycat sample.txt
    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    > myhead sample.txt
    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    
    > mytail sample.txt
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    > myhistory
    mycat sample.txt
    
    myhead sample.txt
    
    mytail sample.txt
    
    myhistory
    
    > help
    The following commands are available:
      mycat      Prints the whole file. 
      myhead     Prints first 10 lines of the file. 
      mytail     Prints last 10 lines of the file. 
      myhistory  Prints last 50 commands. 
      help       Prints the manual.
      exit       Exit the shell.
    
    > exit
     