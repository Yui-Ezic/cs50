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
    for(int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if(!(isalpha(argv[1][i])))
        {
            printf("Keyword must only contain letters A-Z and a-z\n");
            return 1;
        }
    }
    string s = GetString();
    for(int i = 0, n = strlen(s), j = 0, l = strlen(argv[1]); i < n; i++)
    {
        if(isalpha(s[i]))
        {
            int k;
            if(isupper(argv[1][j]))
                k = argv[1][j] - 65;
            else
                k = argv[1][j] - 97;
            if(isupper(s[i]))
                printf("%c", (s[i] - 65 + k) % 26 + 65);
            else
                printf("%c", (s[i] - 97 + k) % 26 + 97);
            j = (j+1)%l;
        }
        else
            printf("%c", s[i]);        
    }
    printf("\n");
    return 0;
}
