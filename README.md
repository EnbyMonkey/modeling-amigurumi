# Installation
Clone the repository. The project is built using CMake >= 3.15. You will need to install some dependencies on your own, such as [Eigen3](https://eigen.tuxfamily.org/index.php?title=Main_Page). For Ceres Solver to work properly, you should install [SuiteSparse](https://people.engr.tamu.edu/davis/suitesparse.html).

# Testing patterns
There is a pattern.txt file in the include folder. Type in your pattern here then run the program. Each line corresponds to a single row, and the first line must be 'mc'. Valid stitches are 'sc', 'inc' and 'dec', and 'fo' can be used at the end of a pattern to represent sewing the final row closed. A number in front of a stitch indicates how many times to do that stitch, and a number in front of square brakets [...] indicates how many times to repeat what is in the square brakets. For example, '3[2sc,inc]' means 'Do "2 single crochet stitches followed by an increase" 3 times.

# Limitations
The graph layout does not work well for small patterns. It it quite slow for larger patterns. It doesn't support joining patterns, as described [here](https://www.crochet365knittoo.com/joining-amigurumi-limbs-an-easy-technique/).
