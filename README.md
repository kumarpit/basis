```    
                    ___           ___                       ___
     _____         /  /\         /  /\        ___          /  /\                   
    /  /::\       /  /::\       /  /:/_      /  /\        /  /:/_             
   /  /:/\:\     /  /:/\:\     /  /:/ /\    /  /:/       /  /:/ /\  
  /  /:/~/::\   /  /:/~/::\   /  /:/ /::\  /__/::\      /  /:/ /::\ 
 /__/:/ /:/\:| /__/:/ /:/\:\ /__/:/ /:/\:\ \__\/\:\__  /__/:/ /:/\:\            
 \  \:\/:/~/:/ \  \:\/:/__\/ \  \:\/:/~/:/    \  \:\/\ \  \:\/:/~/:/
  \  \::/ /:/   \  \::/       \  \::/ /:/      \__\::/  \  \::/ /:/ 
   \  \:\/:/     \  \:\        \__\/ /:/       /__/:/    \__\/ /:/  
    \  \::/       \  \:\         /__/:/        \__\/       /__/:/   
     \__\/         \__\/         \__\/                     \__\/    
```
Basis is a linear algebra library written in C as an exercise to revise linear algebra and low-level programming concepts.

Here are the operations that this library hopes to accomplish:
- Matrix File IO
    - Read (to read a matrix from a file)
    - Copy (to duplicate an existing matrix)
    - Make (to create an n-by-p matrix of Zeros)
    - Write (to write to a file)
- Simple Matrix Operations
    - Generate Identity Matrices
    - Matrix Trace (Sum of elements along the diagonal)
    - Matrix Transpose (To flip a matrix along the diagonal)
    - Matrix Mean (returns the average of each column in a matrix)
    - Matrix Multiplication
    - Matrix Scaling
    - Matrix Dot Product
- Advanced Matrix Operations
    - Gram-Schmidt (For orthonormalising a set of vectors)
    - Eigenvalues
    - Matrix Determinates
    - Invert
    - Matrix Solver (Solve equations of the form Ax = b)
    - LU Decomposition
    - QR decomposition
    - Least squares solution to Ax = b
    - Polynomial and Cubic Spline interpolation
- Basic Linear Programming
- FFT

## Goals
- think about optimizing cache performance (ref lab 8 cpsc 313)
- parallelize matrix multiply manually using pthreads https://www.eecs.umich.edu/courses/eecs570/discussions/w20/pthreads.html

## References
- https://www.andreinc.net/2021/01/20/writing-your-own-linear-algebra-matrix-library-in-c
