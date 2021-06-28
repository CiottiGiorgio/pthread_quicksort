# pthread_quicksort

This project is an implementation of the sorting algorithm "quicksort" with Hoare partition scheme (with and without parallelization).

## Disclaimer
This is not meant to be used anywhere else other than this project because this is just a didactic exercise.
As much as I tested this code please use it with caution. It is not general enough for a serious project in my opinion.

## Description

I wanted to make a comparison between the single-core version and the multi-core version. Of course choosing the number of threads and the threshold for switching to insertion sort is going to be a matter of the machine this algorithms is running on.

Nevertheless on my personal machine (CPU: Ryzen 7 5800X) the sweet spot seems to be around #THREADS=8 and THRESHOLD=256 elements.

Tuning this parameters for each possible machine authomatically is going to be an impossible challenge. There for I encourage anyone that stumble across this project to study the "External distribution sort".

https://en.wikipedia.org/wiki/External_sorting#External_distribution_sort

The RAM model of computation sometimes is much more realistic than the classic model where every memory access is of constant time.