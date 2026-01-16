Input: Your program should be non-interactive (that is, the program should not interact with
the user by asking them explicit questions) and take the following command-line arguments:
<F> <K> <I> <T> <R>, where
 F: name of the data file
 K: number of clusters (positive integer greater than one)
 I: maximum number of iterations (positive integer)
 T: convergence threshold (non-negative real)
 R: number of runs (positive integer)

Warning #1: Do not hard-code any of these parameters (e.g., using a statement such as “int R =
100;”) into your program (you should also not use meaningless variable names such as R). The
values of these parameters should be given by the user at run-time through the command prompt.
See below for an example.




The first line of F contains the number of points (N) and the dimensionality of each point (D).
Each of the subsequent lines contains one data point in blank separated format. In your program,
you should represent the attributes of each point using a double-precision floating-point data
type (e.g., “double” data type in C/C++/Java). In other words, you should not use an integral
data type (byte, short, char, int, long, etc.) to represent an attribute. You should also not use a
string data type (char * in C/C++, std::string in C++, or String in Java) to store the data points in
memory. However, you may use non-integral data types (e.g., string’s) for temporary storage
(e.g., to store a line you read from the input file).



Warning #2: Do not confuse uniform random selection and uniform random generation. The
former involves selecting centers from the existing/real data points uniformly at random, that is,
each data point has a (roughly) equal chance to be selected. The latter involves generating 
© 2026 M. Emre Celebi
artificial centers within the data space uniformly at random. You are supposed to select
randomly, not generate randomly. In other words, the selected centers must be real data points



## Output: 
Display the initial cluster centers selected from the data set. Note that the output of your
program may be different in each execution due to randomization.
Sample Input/Output:
% F = iris_bezdek.txt (name of data file)
% K = 3 (number of clusters)
% I = 100 (maximum number of iterations in a run)
% T = 0.000001 (convergence threshold)
% R = 100 (number of runs)
% test is the name of the executable file
% “>” indicates command-prompt, which is not part of the output
> test iris_bezdek.txt 3 100 0.000001 100
5.1 3.4 1.5 0.2
7.2 3.2 6 1.8
4.6 3.1 1.5 0.2
Note that the output above consists of only three lines of information because K = 3 and that
each line of the output contains four values because iris_bezdek is a 4-dimensional data set.
Testing: Test your program on the 10 data sets given on Blackboard.
