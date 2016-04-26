Morgan Weaver
Homework 2
CPSC 5052: Systems Principles II

1. Multithreaded Vowel Counter
--------------------------------
This program was compiled for testing with g++ on the school's cs1 server.  It takes one argument: the file directory containing up to 20 text files (text1.txt, text2.txt, etc).  It skips files that are hidden and only begin with a period (.toast, etc.).  Upon finishing, it prints out a count for each of the five standard vowels, a, e, i, o, and u, whether lower or upper case.

Example for compiling and executing:

g++ sysPrincIIHW2.cpp -lpthread
./a.out /home/st/weaverm1/TextFiles

2. Lazy Dental Clinic Semaphore/Thread Program
------------------------------------------------
This program was compiled for testing with g++ on the school's cs1 server.  It takes one argument for easy testing: the number of patient threads desired.  The threads for Dentist and Patient run in an infinite loop til ctrl-c is administered.

Example for compiling and executing with 3 patient threads:

g++ sysPrinclHW2lazyDentist.cpp -lpthread
./a.out 3