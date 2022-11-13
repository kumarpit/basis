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
    - Matrix Read (to read a matrix from a file)
    - Matrix Copy (to duplicate an existing matrix)
    - Matrix Make (to create an n-by-p matrix of Zeros)
    - Matrix Free (to release memory)
    - Matrix Write (to write to a file)
    - Matrix Print (to display a matrix on the screen)
- Simple Matrix Operations
    - Identity Matrices
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
    - Matrix Inverts
    - Matrix Solver

## Ideas
- think about optimizing cache performance (ref lab 8 cpsc 313)

## References
- https://www.andreinc.net/2021/01/20/writing-your-own-linear-algebra-matrix-library-in-c
