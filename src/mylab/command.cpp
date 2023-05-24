#include <cstdio>
#include <cstdlib>
#include <map>
#include "command.h"

using namespace std;

extern map<string, Variable>   g_var;
extern const char *PI;
extern const char *ANS;
extern const char *EULER;


void command(Statement *stmt)
{
    const char *cmd = stmt->m_command;
    if (!strcmp(cmd, "exit"))
        do_exit();
    else if (!strcmp(cmd, "who"))
        do_who();
    else if (!strcmp(cmd, "whos"))
        do_whos();
    else if (!strcmp(cmd, "clc"))
        system("clear");
    else if (!strcmp(cmd, "clear"))
        do_clear(stmt);
}

void do_exit()
{
    printf("good-bye\n");
    exit(0);
}

void do_who()
{
    printf("Your variables are:\n");
    for (map<string, Variable>::const_iterator it = g_var.begin(); it != g_var.end(); ++it)
        printf("%s  ", it->first.c_str());
    printf("\n");
}

void do_whos()
{
    int w[4] = {4, 10, 10, 10};
    map<string, Variable>::const_iterator it;
    char size[10];

    for (it = g_var.begin(); it != g_var.end(); ++it)
        if (it->first.length() > w[0])
            w[0] = it->first.length();
    w[0] += 2;
    printf("\033[4m%-*s%-*s%-*s%-*s\033[0m\n", w[0], "Name", w[1], "Class", w[2], "Type", w[3], "Size");
    for (it = g_var.begin(); it != g_var.end(); ++it)
    {
        size[0] = 0;
        const Variable &v = it->second;
        if (v.m_type == ET_VECTOR)
            sprintf(size, "%d", v.m_vector->get_size());
        else if (v.m_type == ET_MATRIX)
        {
            Matrix_Dim dim = v.m_matrix->get_size();
            sprintf(size, "%d×%d", dim.row, dim.col);
        }
        printf("%-*s%-*s%-*s%-*s\n", w[0], it->first.c_str(), w[1], v.category(), w[2], v.type(), w[3], size);
    }
}

void do_clear(Statement *stmt)
{
    if (stmt->m_namelist->size())
    {
        for (string n : *stmt->m_namelist)
            if (n != PI && n != ANS)
                g_var.erase(n);
    }
    else
    {
        for (auto it = g_var.begin(); it != g_var.end();)
            if (it->first != PI && it->first != ANS && it->first != EULER)
                it = g_var.erase(it);
            else
                ++it;
    }
}
