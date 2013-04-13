#ifndef PARSE_CMD_LINE_H
#define PARSE_CMD_LINE_H

struct CmdLineOpts
{
    int num_producers;
    int num_consumers;
    int time_to_run;
    bool use_large_test;
    bool verbose;

    CmdLineOpts();
    void describe();
};

int parseCmdLineOpts(int, char **, CmdLineOpts &);

#endif
