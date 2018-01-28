#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage ./caesar <key>\n");
        return 1;
    }
    int k = atoi(argv[1]);
    string s = GetString();
    for(int i = 0, n = strlen(s); i < n; i++)
    {
        if(isalpha(s[i]))
        {
            if(isupper(s[i]))
            {
                printf("%c", (s[i] - 65 + k) % 26 + 65);
            }
            else
            {
                printf("%c", (s[i] - 97 + k) % 26 + 97);
            }
        }
        else
        {
            printf("%c", s[i]);
        }         
    }
    printf("\n");
    return 0;
}
