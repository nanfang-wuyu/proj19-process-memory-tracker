# Project Design Document



#### Group Members:



## Project Background and Description:

> According to the project description in the github repository, consult relevant materials to improve the project background.





There are several tools for showing memory usage in Linux by process and user:

- `ps`: The very basic way to show memory usage in Linux is us <u>ps</u> and it is available to check all the processes on Linux.
- `top`: One extremely easy way to see what processes are using the most memory is to start <u>top</u>.
- `pmap`: With the PID of the processes need to check memory usage, run <u>pmap</u> adding with PID to show not only the total memory used in process but also the memory libraries and other files required to run this particular process.

The memory leak is a type of resource leak happens when computer programs manage the memory defectively. There are 2 ways to cause memory lead problem in OS level: the one is the memory is not freed after the data it hold will never be used again, and the another is due to miss delete of pointer, which leads the memory never usable. And it is a particularly serious issue for large scale programs.

To detect the memory leak, <u>memprof</u> is a tool for profiling memory usage and finding memory leaks. It can generate a profile how much memory was allocated by each function in your program. Also, it can scan memory and find blocks that you’ve allocated but are no longer referenced anywhere as. And there are some other tools for detecting memory leak as well: 



# Implementation

> According to the project background and expected goals, clarify the technical route, list the main technologies used in the project, and the main innovation points.

### Technical Route



## Main Technologies

### Main Innovation Points



## Expected Goals

> The expected goals can be the expected goals listed in the repository, or based on the project background, put forward your own ideas and propose suitable project goals.

Expected goal 1:

Expected goal 2:

Expected goal 3:



## Division of Labor

> List the main division of labor for each team member

