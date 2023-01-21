<h2>przetwarzanie_rozproszone</h2>

commands:

compilation: `mpicc hello_world.c -o hello`

running: `mpirun ./hello`

running with number of processes specified: `mpirun -np 4 ./hello`