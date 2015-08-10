# rentrak code challenge

1. General remarks:
* All functionality described in items 1, 2.1 and 2.2 was implemented. 
* The advanced filtering capability of item 2.3 was not implemented.
* Code must be built with C++11 support. I could only test with gcc 4.8.4 on a Linux PC box.

2. Build instructions:
- Checkout code from git
- Edit the makefile and change the GCC parameter to your g++ path.
- Run 'make'
- An executable "query" should be created.

3. Usage:
- Data can be loaded into the datastore with the -i option:
$ ./query -i data1.txt
4 record(s) read from file
4 record(s) added to datastore

- A database file called "_dataStore.db" is produced. Subsequent stores update the same file.
- The program also supports the -s, -o, -f and -g command-line parameters. Examples:

$ ./query -s TITLE,REV,DATE -o DATE,TITLE
the matrix,4.00,2014-04-01
the hobbit,8.00,2014-04-02
the matrix,4.00,2014-04-02
unbreakable,6.00,2014-04-03

$ ./query -s TITLE,REV,DATE -f DATE=2014-04-01
the matrix,4.00,2014-04-01

$ ./query -s TITLE,REV:sum,STB:collect -g TITLE
the hobbit,8.00,[stb2]
the matrix,8.00,[stb1,stb3]
unbreakable,6.00,[stb1]


