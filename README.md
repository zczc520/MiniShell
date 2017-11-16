# MiniShell
### Resume
A mini shell implemented by C in Ubuntu-16.04
### Describe
We have three models:
* myerror (distinguish different with errcode)
* command (execute specific command)
* process (parse input line to get commands)
### Dependence
myerror.h/.c -> command.h/.c -> process.h/.c -> main.c
