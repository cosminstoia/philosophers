## Overview
The Philosophers project is part of the 42 School curriculum, designed to introduce students to the concepts of concurrency, parallelism, and synchronization. The project simulates the classic Dining Philosophers problem, where multiple philosophers sit at a table and alternate between thinking, eating, and sleeping. The challenge lies in managing the shared resources (forks) without causing deadlocks or race conditions.

## Features
- Simulates the Dining Philosophers problem with configurable parameters.
- Uses threads and mutexes to manage concurrency and synchronization.
- Prevents deadlocks and handles starvation scenarios.
- Provides detailed logging of the philosophers' actions for debugging and analysis.

## Usage
### Parameters
- number_of_philosophers: The number of philosophers and forks.
- time_to_die: Time (in milliseconds) a philosopher can go without eating before dying.
- time_to_eat: Time (in milliseconds) a philosopher spends eating.
- time_to_sleep: Time (in milliseconds) a philosopher spends sleeping.
- number_of_times_each_philosopher_must_eat (optional): The number of times each philosopher must eat before the simulation ends. If not specified, the simulation runs indefinitely.
