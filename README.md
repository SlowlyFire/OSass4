# OSass4
ass4 in operation systems bar ilan. 
this files are the one that i submit.
there is also a makefile and a test we could use
to use the test: python3 ex4_test.py 
to use the 2 files only:
1. gcc ex4_client.c -o client.out 
2. gcc ex4_srv.c -o srv.out
3. srv.out & (to make in run in bg (background))
4. ./client.out 333 70 2 40
this one is running the client proccess ( i can run lots of them)
first arg - server pid (we could get from ps in command line, or from ./srv.out &)
second arg - first number to calculate
third arg - operator : 1 is +, 2 is -, 3 is *, and 4 is /
fourth arg - second number to calculate 

---
there is also a string we print for division with 0, which is forbidden.
