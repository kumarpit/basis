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
- Basic Vector Operations
- Basic Matrix Operations
    - Generate Identity Matrices
    - Trace
    - Transpose
    - Mean (returns the average of each column in a matrix)
    - Matrix Multiplication
    - Scaling
    - Solve Equations of the form `Ax = b`
- Advanced Matrix Operations
    - Gram-Schmidt (For orthonormalising a set of vectors)
    - Determining Eigenvalues and Eigenvectors
    - Determinants
    - Matrix Inverse
    - LU (with Partial Pivoting), QR, SVD Decomposition
    - Least Squares Solution to `Ax = b`
    - Polynomial and Cubic Spline interpolation
- Linear Programming
    - Simplex Method
    - Matrix Games
    - Optimal Transport
- FFT
- Quarternions
- Python Bindings
  - Call program from Python
  - Call Python for image compression using SVD

## References
- https://www.andreinc.net/2021/01/20/writing-your-own-linear-algebra-matrix-library-in-c
- https://www.eecs.umich.edu/courses/eecs570/discussions/w20/pthreads.html
