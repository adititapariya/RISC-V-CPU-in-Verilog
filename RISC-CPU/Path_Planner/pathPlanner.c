#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __linux__ // for host pc

#include <stdio.h>

void _put_byte(char c) { putchar(c); }

void _put_str(char *str)
{
    while (*str)
    {
        _put_byte(*str++);
    }
}

void print_output(uint8_t num)
{
    if (num == 0)
    {
        putchar('0'); // if the number is 0, directly print '0'
        _put_byte('\n');
        return;
    }

    if (num < 0)
    {
        putchar('-'); // print the negative sign for negative numbers
        num = -num;   // make the number positive for easier processing
    }

    // convert the integer to a string
    char buffer[20]; // assuming a 32-bit integer, the maximum number of digits is 10 (plus sign and null terminator)
    uint8_t index = 0;

    while (num > 0)
    {
        buffer[index++] = '0' + num % 10; // convert the last digit to its character representation
        num /= 10;                        // move to the next digit
    }

    // print the characters in reverse order (from right to left)
    while (index > 0)
    {
        putchar(buffer[--index]);
    }
    _put_byte('\n');
}

void _put_value(uint8_t val) { print_output(val); }

#else // for the test device

void _put_value(uint8_t val) {}
void _put_str(char *str) {}

#endif

/*
Functions Usage

instead of using printf() function for debugging,
use the below function calls to print a number, string or a newline

for newline: _put_byte('\n');
for string:  _put_str("your string here");
for number:  _put_value(your_number_here);

Examples:
        _put_value(START_POINT);
        _put_value(END_POINT);
        _put_str("Hello World!");
        _put_byte('\n');

*/

// main function
int main(int argc, char const *argv[])
{

#ifdef __linux__

    const uint8_t START_POINT = atoi(argv[1]);
    const uint8_t END_POINT = atoi(argv[2]);
    uint8_t NODE_POINT = 0;
    uint8_t CPU_DONE = 0;

#else
// Address value of variables are updated for RISC-V Implementation
#define START_POINT (*(volatile uint8_t *)0x02000000)
#define END_POINT (*(volatile uint8_t *)0x02000004)
#define NODE_POINT (*(volatile uint8_t *)0x02000008)
#define CPU_DONE (*(volatile uint8_t *)0x0200000c)

#endif

    // array to store the planned path
    uint8_t path_planned[32];
    // index to keep track of the path_planned array
    uint8_t idx = 0;

    int dest = START_POINT;
    int src = END_POINT;
    int i, j, adj;
    int parent[30];
    int inf = 99;
    bool visited[30] = {0};
    int cost[30][30] = {
        {0, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {1, 0, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1},
        {99, 1, 0, 1, 99, 99, 99, 99, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 1, 0, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 1, 0, 1, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 1, 0, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 1, 99, 0, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 1, 0, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 1, 99, 99, 99, 99, 1, 0, 1, 99, 99, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 1, 0, 1, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 0, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 99, 0, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 1, 99, 99, 99, 0, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 0, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 0, 1, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 0, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 99, 0, 1, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 0, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 99, 0, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 0, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 0, 1, 99, 99, 1, 99, 99, 99, 99, 1},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 0, 1, 1, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 0, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 99, 0, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 99, 99, 99, 0, 1, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 0, 1, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 0, 1, 1, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 0, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 99, 0, 1},
        {99, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 1, 99, 99, 99, 99, 99, 99, 99, 1, 0}};
    int dist[30];
    for (i = 0; i < 30; i++)
    {
        parent[i] = i;
        dist[i] = inf;
        dist[src] = 0;
    }
    for (i = 0; i < 30; i++)
    {
        int minvalue = inf;
        int minnode = 0;
        for (j = 0; j < 30; j++)
        {
            if (!visited[j] && dist[j] < minvalue)
            {
                minvalue = dist[j];
                minnode = j;
            }
        }
        int nearest = minnode;
        visited[nearest] = true;

        for (adj = 0; adj < 30; adj++)
        {
            if (cost[nearest][adj] != inf && dist[adj] > dist[nearest] + cost[nearest][adj])
            {
                dist[adj] = dist[nearest] + cost[nearest][adj];
                parent[adj] = nearest;
            }
        }
    }

    int parnode = parent[dest];
    path_planned[0] = dest;
    path_planned[1] = parnode;
    int count = 2;
    while (parnode != src)
    {
        parnode = parent[parnode];
        path_planned[count] = parnode;
        count = count + 1;
    }
    idx = count;
    path_planned[count] = src;
    if (src == dest)
    {
        idx = 1;
    }

    // the node values are written into data memory sequentially
    for (int i = 0; i < idx; ++i)
    {
        NODE_POINT = path_planned[i];
    }

    // Path Planning Computation Done Flag
    CPU_DONE = 1;

#ifdef __linux__ // for host pc

    _put_str("######### Planned Path #########\n");
    for (int i = 0; i < idx; ++i)
    {
        _put_value(path_planned[i]);
    }
    _put_str("################################\n");

#endif

    return 0;
}