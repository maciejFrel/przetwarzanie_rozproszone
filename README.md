# przetwarzanie_rozproszone

commands:
- compilation: `mpicc hello_world.c -o hello`
- running: `mpirun ./hello`
- running with number of processes specified: `mpirun -np 4 ./hello`
- combined `mpicc x.c -o x && mpirun x`