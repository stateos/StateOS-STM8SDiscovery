StateOS
=======

Free, extremely simple, amazingly tiny and very fast real-time operating system (RTOS) designed for deeply embedded applications.
It was inspired by the concept of a state machine.
TEMPLATE

Features
--------

- kernel works in preemptive or cooperative mode
- kernel can operate in tick-less mode (32-bit timer required)
- signals (auto clearing, protected)
- events
- flags (one, all, accept, ignore)
- barriers
- semaphores (binary, limited, counting)
- mutexes (recursive and priority inheritance)
- fast mutexes (non-recursive and non-priority-inheritance)
- condition variables
- memory pools
- message queues
- mailbox queues
- timers (one-shot, periodic)
- all documentation is contained within the source files

Target
-------

STM8S-Discovery board.

License
-------

This project is licensed under the terms of [GNU GENERAL PUBLIC LICENSE Version 3](http://www.gnu.org/philosophy/why-not-lgpl.html).
