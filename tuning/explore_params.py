"""
This file is a way of automating benchmarking with variable parameters such as:
dimension of unsorted array, number of repetitions, kind of algorithm,
various tresholds.

Inside this code we basically produce a fixed N and K. Then we produce the triple
(N_THREADS, HYBRID_THRESHOLD, THREADED_THRESHOLD) such that HYBRID_THRESHOLD < THREADED_THRESHOLD.

Then we substitute these parameters in the source code of "src/quicksort.c" in the "#define". We compile and execute.
The output of the benchmark is captured and appended in the "tuning/results.csv" along with the parameters.

The user should only modify the parameters:
shift_N,
K,
range_*     (range parameters are then expanded in the range function inside the for loops)

Be sure to have a functioning make file otherwise the compilation process will not happen.
"""


import subprocess
import itertools
from sys import stdout


_algorithms = {"quicksort": 0, "Hybrid Quicksort": 1, "Threaded Quicksort": 2}
algorithm = _algorithms["Threaded Quicksort"]
shift_N = 26
N = 2**shift_N
K = 5

range_n_threads = (8, 16+1)
range_shift_hybrid_threshold = (6, 8+1)
range_shift_threaded_threshold = (14, 17+1, 2)

source_file = "./src/quicksort.c"
results_file = "./tuning/results.csv"


def main():
    with open(results_file, "w") as f:
        f.write("Algorithm,N,N_THREADS,HYBRID_THRESHOLD,THREADED_THRESHOLD,Time Elapsed\n")
    
    for n_threads, shift_hybrid_threshold in itertools.product(range(*range_n_threads), range(*range_shift_hybrid_threshold)):
        for shift_threaded_threshold in filter(lambda x: shift_hybrid_threshold < x, range(*range_shift_threaded_threshold)):
            with open(source_file, "r") as f:
                input_file = f.readlines()

            for i, line in enumerate(input_file):
                if "#define N_THREADS" in line:
                    input_file[i] = f"#define N_THREADS\t\t\t({n_threads})\n"
                elif "#define HYBRID_THRESHOLD" in line:
                    input_file[i] = f"#define HYBRID_THRESHOLD\t(1<<{shift_hybrid_threshold})\n"
                elif "#define THREADED_THRESHOLD" in line:
                    input_file[i] = f"#define THREADED_THRESHOLD\t(1<<{shift_threaded_threshold})\n"

            with open(source_file, "w") as f:
                f.writelines(input_file)

            with open(results_file, "a") as f:
                f.write(f"{algorithm},2^{shift_N},{n_threads},2^{shift_hybrid_threshold},2^{shift_threaded_threshold},")

                subprocess.run(["make"], stdout=subprocess.DEVNULL)
                benchmark_resutls = subprocess.run(["bin/main", f"{algorithm}", f"{N}", f"{K}"], capture_output=True)

                f.write(f"{benchmark_resutls.stdout.decode()} s\n")


if __name__ == "__main__":
    main()
