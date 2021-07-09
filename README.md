# pthread_quicksort

This project is an implementation of the sorting algorithm "Quicksort" with Hoare partition scheme in 3 variants (naive, hybrid, multi threading).

The hybrid variant is single core but with some time and space optimizations.

## Disclaimer
This is not meant to be used anywhere else other than this project because this is just a didactic exercise.
As much as I tested this code please use it with caution. It is not general enough for a serious project in my opinion.

## Usage
Use `$ make` to build this project but keep in mind that I'm using `clang-llvm`. You should modify the parameter in the `makefile` to match your compiler.

Parameters for main function:
- {0, 1, 2} for selecting the variant of the algorithm
- Any int as the size of the array to sort
- Any int as the number of repetition when benchmarking the algorithm

## Description

I wanted to make a comparison between the single-core version and the multi-core version. There are some parameters that greatly influence the performance on a given machine of this code.
- THREADS is of course the number of running threads inside the thread pool
- HYBRID_THRESHOLD is the number of elements when switching from quicksort to insertion sort
- THREADED_THRESHOLD is the number of elements when switching from threaded quicksort to hybrid quicksort

Switching from quicksort to insertion sort is helpful when dealing with a small sub-array because, even though the time complexity is worse, it has a small constant factor and still performs less comparison on average in this scenario.

Furthermore in the threaded version each recursive call is substituted by a new task in the queue. There comes a point where the cost of scheduling a new task is more than just progressing inside the same thread. At that point we regress to the hybrid quicksort.

On my personal machine (CPU: Ryzen 7 5800X) the sweet spot seems to be around:
- THREADS=16
- HYBRID_THRESHOLD=2^7 elements
- THREADED_THRESHOLD=2^16 elements

Even though my CPU has only 8 cores, 16 threads still seems to be beneficial. I speculate that, even though this task is memory intensive and never blocks (IO, networking, ...), filling all the threads of my CPU cuts down the costs for context switching.

I did not explore a version without a threadpool because then the problem of balancing the load among threads arises (some threads could finish much faster than others. In a pool the next thread is immediately executed). Keeping the number of threads spawned also becomes challenging.

## External model

Tuning these parameters for each possible machine authomatically is going to be an impossible challenge. There for I encourage anyone that stumbles across this project to study the "External distribution sort".

https://en.wikipedia.org/wiki/External_sorting#External_distribution_sort

The External model of computation sometimes is much more realistic than the classic model where every memory access is of constant time.